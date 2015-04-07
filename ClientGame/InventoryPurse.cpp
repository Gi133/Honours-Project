#include "stdafx.h"
#include "InventoryPurse.h"

namespace
{
	const auto baseUpgradePriceFallBack = 500;
	const auto upgradeCapIncreaseFallBack = 5000;
	const auto upgradePriceIncreaseFallBack = 1000;
	const auto maxUpgradeLevelFallBack = 10;
}

InventoryPurse::InventoryPurse()
{
	upgradeLevel = 1;
	LoadDefaults();
}

InventoryPurse::InventoryPurse(const int startingGold, const unsigned int startingUpgradeLevel /* = 1*/)
{
	if (startingUpgradeLevel > 0)
		upgradeLevel = startingUpgradeLevel;
	else
		upgradeLevel = 1;

	gold = startingGold;
	LoadDefaults();
}

InventoryPurse::~InventoryPurse()
{
}

unsigned int InventoryPurse::GetUpgradePrice()
{
	return baseUpgradePrice + (upgradePriceIncrease * (upgradeLevel - 1));
}

void InventoryPurse::Upgrade()
{
	upgradeLevel++;
	goldLimit += upgradeCapIncrease;
}

void InventoryPurse::UpdateGold()
{
	// Check if Gold is within the gold limits, if not then only add the allowed gold.
	auto remainingGold = goldLimit - gold;

	if (remainingGold < 0)
	{
		gold = goldLimit;
		sysLog.Log("Could not add gold, amount to add exceeded purse limit.");
	}
}

void InventoryPurse::LoadDefaults()
{
	baseUpgradePrice = thePrefs.GetInt("InventoryPurseSettings", "baseUpgradePrice");
	if (!baseUpgradePrice)
		baseUpgradePrice = baseUpgradePriceFallBack;

	upgradeCapIncrease = thePrefs.GetInt("InventoryPurseSettings", "upgradeCapIncrease");
	if (!upgradeCapIncrease)
		upgradeCapIncrease = upgradeCapIncreaseFallBack;

	upgradePriceIncrease = thePrefs.GetInt("InventoryPurseSettings", "upgradePriceIncrease");
	if (!upgradePriceIncrease)
		upgradePriceIncrease = upgradePriceIncreaseFallBack;

	maxUpgradeLevel = thePrefs.GetInt("InventoryPurseSettings", "maxUpgradeLevel");
	if (!maxUpgradeLevel)
		maxUpgradeLevel = maxUpgradeLevelFallBack;
}