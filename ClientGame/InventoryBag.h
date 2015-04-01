// Inventory system Base Class
// Map<string, int> : Map<ResourceName, InventoryQuantity>

#pragma once
class InventoryBag
{
private:
	int bagMaxSpace, bagMaxSpaceBase, bagUsedSpace, bagAvailableSpace;
	int bagUpgradeLevel, bagMaxUpgradeLevel, bagUpgradeBasePrice, bagUpgradeLevelSpace;

	std::map<std::string, int> bagContents;

	// Setup the map based on the known resources from the resource manager.
	void Setup();
	void LoadConfig(const bool isCity);

	void UpdateBagSpace(); // Function to update bag space values.

public:
	// Create a new Bag for the inventory.
	// Params:
	// * isCity : True/False if the bag being created will have unlimited space or not.
	InventoryBag(const bool isCity = false);
	~InventoryBag();

	unsigned int getBagLevel() { return bagUpgradeLevel; }
	unsigned int getBagMaxLevel() { return bagMaxUpgradeLevel; }

	std::string OutputResourceQuantity(std::string resourceName);

	bool CheckBagSpace(const int quantity); // Function to check if there is enough space available.

	// Resource Quantity controls.
	void AdjustResource(std::string resourceName, int quantity);
	void AdjustResource(const unsigned int iterator, int quantity);
	void SetResource(std::string resourceName, int quantity);
	void SetResource(const unsigned int iterator, int quantity);
	int GetResourceQuantity(std::string resourceName);
	int GetResourceQuantity(const int iterator);

	// Set Contents of the bag to 0.
	void EmptyBag();

	// BagUpgrade Functions.
	int GetBagLevel() { return bagUpgradeLevel; }
	bool CheckBagUpgradeable(); // If upgrade level is lower than max upgrade level, return true.
	int GetBagUpradeCost();
	void UpgradeBag();
	void SetUpgradeLevel(const int upgradeLevel) { bagUpgradeLevel = upgradeLevel; UpdateBagSpace(); } // Debug Function, should not be used in code.

	int GetAvailableBagSpace() { UpdateBagSpace(); return bagAvailableSpace; }
	int GetMaxSpace() { UpdateBagSpace(); return bagMaxSpace; }
	int GetUsedSpace() { UpdateBagSpace(); return bagUsedSpace; }
};
