#pragma once
#include "InventoryBag.h"

class Inventory
{
private:
	bool city; // Bool to store whether the inventory belongs to a city or not.

	std::vector<std::shared_ptr<InventoryBag>> bagContainer;

public:
	Inventory(bool isCity = false, int startingBagNumber = 1);
	~Inventory();

	// Add bags.
	void AddBag(int numBag = 1);

	// Resource Management
	void AddToResource(std::string resourceName, int quantity);
	void SetResource(std::string resourceName, int quantity);

	// DO NOT use this for anything but generating city starting resources. UNSAFE.
	// Params:
	// bagNumber: The bag you want to work on.
	// iter: The position of the resource you are changing.
	// quantity: The amount you are setting the resource to.
	void SetResource(const int bagNumber, const int iter, const int quantity);
	int GetTotalResourceAmount(std::string resourceName);
};
