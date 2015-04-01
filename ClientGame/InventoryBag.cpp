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
	bagMaxSpaceBase = 0;
	bagMaxUpgradeLevel = 0;
	bagUpgradeBasePrice = 0;
	bagUpgradeLevelSpace = 0;
	bagMaxSpace = 0;
	bagAvailableSpace = 0;

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

		// Initialise the max bag space value.
		bagMaxSpace = bagMaxSpaceBase;
	}
}

void InventoryBag::UpdateBagSpace()
{
	bagUsedSpace = 0;

	for (auto resourceName : theResourceManager.GetResourceNames())
		bagUsedSpace += bagContents.at(resourceName);

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
	for (auto resourceName : theResourceManager.GetResourceNames())
		bagContents.at(resourceName) = 0;

	UpdateBagSpace();
}

void InventoryBag::AdjustResource(std::string resourceName, int quantity)
{
	resourceName = theResourceManager.GetResourceName(resourceName);

	if (resourceName != "")
		bagContents.at(resourceName) += quantity;
	else
		sysLog.Log("ERROR: Wrong resource name used!");
}

void InventoryBag::AdjustResource(const unsigned int iterator, int quantity)
{
	auto resourceName = theResourceManager.GetResourceName(iterator);

	if (resourceName != "")
		bagContents.at(resourceName) += quantity;
	else
		sysLog.Log("ERROR: Wrong resource name used!");
}

void InventoryBag::SetResource(std::string resourceName, int quantity)
{
	resourceName = theResourceManager.GetResourceName(resourceName);

	if (resourceName != "")
		bagContents.at(resourceName) = quantity;
	else
		sysLog.Log("ERROR: Wrong resource name used!");
}

void InventoryBag::SetResource(const unsigned int iterator, int quantity)
{
	auto resourceName = theResourceManager.GetResourceName(iterator);

	if (resourceName != "")
		bagContents.at(resourceName) = quantity;
	else
		sysLog.Log("ERROR: Wrong resource name used!");
}

std::string InventoryBag::OutputResourceQuantity(std::string resourceName)
{
	resourceName = theResourceManager.GetResourceName(resourceName);

	if (resourceName != "")
	{
		auto output = resourceName + ": " + std::to_string(bagContents.at(resourceName));
		sysLog.Log(output);
		return output;
	}
	else
		sysLog.Log("ERROR: Wrong resource name used!");

	return 0;
}

int InventoryBag::GetResourceQuantity(std::string resourceName)
{
	resourceName = theResourceManager.GetResourceName(resourceName);

	if (resourceName != "")
		return bagContents.at(resourceName);
	else
		sysLog.Log("ERROR: Wrong resource name used!");

	return 0;
}

int InventoryBag::GetResourceQuantity(const int iterator)
{
	auto resourceName = theResourceManager.GetResourceName(iterator);

	if (resourceName != "")
		return bagContents.at(resourceName);
	else
		sysLog.Log("ERROR: Wrong resource name used!");

	return 0;
}

bool InventoryBag::CheckBagUpgradeable()
{
	return (bagUpgradeLevel < bagMaxUpgradeLevel);
}

int InventoryBag::GetBagUpradeCost()
{
	return (bagUpgradeLevel * bagUpgradeBasePrice);
}

void InventoryBag::UpgradeBag()
{
	if (CheckBagUpgradeable())
		bagUpgradeLevel++;

	UpdateBagSpace();
}