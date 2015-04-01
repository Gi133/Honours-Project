#pragma once
class InventoryPurse
{
private:
	int gold, goldLimit, upgradeLevel, baseUpgradePrice, upgradeCapIncrease, upgradePriceIncrease, maxUpgradeLevel;

	void UpdateGold(); // Function to check gold limits and other variables related to gold.
	void LoadDefaults();

public:
	InventoryPurse();
	InventoryPurse(const int startingGold, const unsigned int startingUpgradeLevel = 1); // Bypasses gold checks.
	~InventoryPurse();

	unsigned int GetUpgradeLevel() { return upgradeLevel; }
	unsigned int GetMaxUpgradeLevel() { return maxUpgradeLevel; }
	unsigned int GetUpgradePrice();
	void Upgrade();

	// Gold Management
	// If checkLimit is set to false then gold limit will be ignored.
	void SetGold(const int newGold, const bool checkLimit = true) { gold = newGold; if (checkLimit)UpdateGold(); }
	void AdjustGold(const int goldToAdd, const bool checkLimit = true) { gold += goldToAdd; if (checkLimit)UpdateGold(); } // Dual Purpose, can add and subtract.
	void SetGoldLimit(const int newGoldLimit) { goldLimit = newGoldLimit; }
	void AddGoldLimit(const int goldLimitAdd) { goldLimit += goldLimitAdd; }
	int GetGold() { return gold; }
	int GetGoldLimit() { return goldLimit; }
};
