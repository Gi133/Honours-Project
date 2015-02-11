#include "stdafx.h"
#include "InventoryBag.h"

namespace
{
	const auto bagMaxSpaceBaseFallBack = 100;
	const auto bagMaxUpgradeLevelFallBack = 10;
	const auto bagUpgradeBasePriceFallBack = 50;
	const auto bagUpgradeLevelSpaceFallBack = 20;
}

InventoryBag::InventoryBag(const bool isCity /* = false */)
{
	bagUpgradeLevel = 1;

	LoadConfig(isCity);
	Setup();
	UpdateBagSpace();
}

InventoryBag::~InventoryBag()
{
}

void InventoryBag::Setup()
{
	// Initialize an empty bag.

	auto resourceName = theResourceManager.GetResourceNames();
	for (std::string i : resourceName)
		bagContents.insert(std::pair<std::string, int>(i, 0));
}

void InventoryBag::LoadConfig(const bool isCity)
{
	// Check whether the inventory being created is for a city.
	if (isCity)
	{
		// Cities do not really care about upgrades and max space.
		bagMaxSpaceBase = 0;
		bagMaxUpgradeLevel = 0;
		bagUpgradeBasePrice = 0;
		bagUpgradeLevelSpace = 0;
		bagMaxSpace = 0;
		bagAvailableSpace = 0;
	}
	else
	{
		bagMaxSpaceBase = thePrefs.GetInt("InventoryBagSettings", "bagMaxSpaceBase");
		if (!bagMaxSpaceBase)
			bagMaxSpaceBase = bagMaxSpaceBaseFallBack;

		bagMaxUpgradeLevel = thePrefs.GetInt("InventoryBagSettings", "bagMaxUpgradeLevel");
		if (!bagMaxUpgradeLevel)
			bagMaxUpgradeLevel = bagMaxUpgradeLevelFallBack;

		bagUpgradeBasePrice = thePrefs.GetInt("InventoryBagSettings", "bagUpgradeBasePrice");
		if (!bagUpgradeBasePrice)
			bagUpgradeBasePrice = bagUpgradeBasePriceFallBack;

		bagUpgradeLevelSpace = thePrefs.GetInt("InventoryBagSettings", "bagUpgradeLevelSpace");
		if (!bagUpgradeLevelSpace)
			bagUpgradeLevelSpace = bagUpgradeLevelSpaceFallBack;
	}
}

void InventoryBag::UpdateBagSpace()
{
	bagUsedSpace = 0;
	for (auto i : bagContents)
		bagUsedSpace += i.second;

	if (bagMaxSpace > 0) // If 0 then bag is for a city, no need to update available space.
	{
		bagMaxSpace = bagMaxSpaceBase + (bagUpgradeLevelSpace * bagUpgradeLevel);
		bagAvailableSpace = bagMaxSpace - bagUsedSpace;
	}
}

bool InventoryBag::CheckBagSpace(const int quantity)
{
	if (bagAvailableSpace >= quantity)
		return true;
	else
		return false;
}

void InventoryBag::EmptyBag()
{
	// Set all bag contents to 0.
	for (auto i : bagContents)
		i.second = 0;

	UpdateBagSpace();
}

void InventoryBag::AddResource(const std::string resourceName, int quantity)
{
	for (auto i : bagContents)
		if (std::regex_search(i.first, std::regex(resourceName, std::regex_constants::ECMAScript | std::regex_constants::icase)))
		{
			i.second += quantity;
			break;
		}
}

void InventoryBag::SubtractResource(const std::string resourceName, int quantity)
{
	for (auto i : bagContents)
		if (std::regex_search(i.first, std::regex(resourceName, std::regex_constants::ECMAScript | std::regex_constants::icase)))
		{
			i.second -= quantity;
			break;
		}
}

void InventoryBag::SetResource(const std::string resourceName, int quantity)
{
	for (auto i : bagContents)
		if (std::regex_search(i.first, std::regex(resourceName, std::regex_constants::ECMAScript | std::regex_constants::icase)))
		{
			i.second = quantity;
			break;
		}
}

void InventoryBag::SetResource(const unsigned int iterator, int quantity)
{
	if (iterator <= bagContents.size())
	{
		// Convert the integer into an iterator position
		auto it = bagContents.begin();
		std::advance(it, iterator);
		it->second = quantity;
	}
}

std::string InventoryBag::OutputResourceQuantity(const std::string resourceName)
{
	for (auto i : bagContents)
		if (std::regex_search(i.first, std::regex(resourceName, std::regex_constants::ECMAScript | std::regex_constants::icase)))
		{
			auto output = i.first + ": " + std::to_string(i.second);
			sysLog.Log(output);
			return output;
		}

	return 0;
}

int InventoryBag::GetResourceQuantity(const std::string resourceName)
{
	for (auto i : bagContents)
		if (std::regex_search(i.first, std::regex(resourceName, std::regex_constants::ECMAScript | std::regex_constants::icase)))
			return i.second;

	return 0;
}

int InventoryBag::GetBagUpradeCost()
{
	if (CheckBagUpgradeable())
	{
		return (bagUpgradeLevel * bagUpgradeBasePrice);
	}
	else
		return 0;
}

void InventoryBag::UpgradeBag()
{
	if (CheckBagUpgradeable())
		bagUpgradeLevel++;

	UpdateBagSpace();
}