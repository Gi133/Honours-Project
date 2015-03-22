#pragma once
#include "InventoryBag.h"
#include "InventoryPurse.h"

class Inventory
{
private:
	bool city; // Bool to store whether the inventory belongs to a city or not.

	int numPerLine; // Used to structure the inventory string.
	unsigned int maxBagNumber;

	std::vector<std::shared_ptr<InventoryBag>> bagContainer;
	std::unique_ptr<InventoryPurse> purse;

	void LoadDefauts();

public:
	Inventory(const bool isCity = false, int startingBagNumber = 1, int StartingGold = 0);
	~Inventory();

	// Add bags.
	void AddBag(const int numBag = 1);

	// Bag Limit
	unsigned int GetBagNumber()const { return bagContainer.size(); }
	unsigned int GetBagLimit()const { return maxBagNumber; }
	void SetBagLimit(unsigned int _newBagLimit) { maxBagNumber = _newBagLimit; }

	// Bag Upgrading.
	unsigned int GetBagUpgradeCost(); // Return the lowest price required to upgrade a bag.

	// Resource Management
	void AdjustResource(const std::string resourceName, const int quantity);
	void SetResource(const std::string resourceName, const int quantity);
	int GetTotalResourceAmount(const std::string resourceName);
	std::string GetInventoryString();

	// Resource Management via Iterator.
	// Mostly used in loops.
	// Params:
	// * bagNumber: The bag you want to work on.
	// * iter: The position of the resource you are changing.
	// * quantity: The amount you are setting the resource to.
	void SetResource(const int bagNumber, const int iter, const int quantity);
	void AdjustResource(const int bagNumber, const int iter, const int quantity);
	int GetResourceInBag(const int bagNumber, const int iter);
	int GetTotalResourceAmount(const int iter);

	// Gold Managing.
	void SetGold(const int newGold, const bool checkLimit = true) { purse->SetGold(newGold, checkLimit); }
	void AdjustGold(const int goldToAdd, const bool checkLimit = true) { purse->AdjustGold(goldToAdd, checkLimit); } // Dual Purpose, can add and subtract.
	void SetGoldLimit(const int newGoldLimit) { SetGoldLimit(newGoldLimit); }
	int GetGold() { return purse->GetGold(); }
	int GetGoldLimit() { return purse->GetGoldLimit(); }
	int GetAvailableSpace() const;
};
