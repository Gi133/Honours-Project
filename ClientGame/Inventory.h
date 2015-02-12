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
	void SubtractFromResource(std::string resourceName, int quantity);
	void SetResource(std::string resourceName, int quantity);
	int GetTotalResourceAmount(std::string resourceName);

	// Resource Management via Iterator.
	// Mostly used in loops.
	// Params:
	// * bagNumber: The bag you want to work on.
	// * iter: The position of the resource you are changing.
	// * quantity: The amount you are setting the resource to.
	void SetResource(const int bagNumber, const int iter, const int quantity);
	void AddToResource(const int bagNumber, const int iter, const int quantity);
	void SubtractFromResource(const int bagNumber, const int iter, const int quantity);
	int GetResourceInBag(const int bagNumber, const int iter);
	int GetTotalResourceAmount(const int iter);
};
