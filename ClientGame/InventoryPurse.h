#pragma once
class InventoryPurse
{
private:
	int gold, goldLimit;

	void UpdateGold(); // Function to check gold limits and other variables related to gold.

public:
	InventoryPurse();
	InventoryPurse(const int startingGold); // Bypasses gold checks.
	~InventoryPurse();

	// Gold Management
	// If checkLimit is set to false then gold limit will be ignored.
	void SetGold(const int newGold, const bool checkLimit = true) { gold = newGold; if (checkLimit)UpdateGold(); }
	void AdjustGold(const int goldToAdd, const bool checkLimit = true) { gold += goldToAdd; if (checkLimit)UpdateGold(); } // Dual Purpose, can add and subtract.
	void SetGoldLimit(const int newGoldLimit) { goldLimit = newGoldLimit; }
	void AddGoldLimit(const int goldLimitAdd) { goldLimit += goldLimitAdd; }
	int GetGold() { return gold; }
	int GetGoldLimit() { return goldLimit; }
};
