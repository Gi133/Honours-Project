#pragma once
#include "Utils.h"

#define theResourceManager ResourceManager::getInstance()

class ResourceManager
{
private:
	ResourceManager();
	~ResourceManager() { _instance = nullptr; }
	static ResourceManager* _instance;

	unsigned int priceMultiplier;

	void LoadPrefs();
	void LoadResourceTable();
	void LoadResourceNames();

	std::map<std::string, int> resourceBasePriceTable;
	std::string resourceBasePriceLocation;

	std::vector<std::string> resourceNames;

public:
	static ResourceManager& getInstance();

	// Reload Values into maps.
	void ReloadTables() { LoadResourceTable(); LoadResourceNames(); }

	// Return base price based on name.
	int GetBasePrice(const std::string resourceName);

	// Returns all resource names in a string vector.
	std::vector<std::string> GetResourceNames() { return resourceNames; }

	// Return the number of all resources in the game.
	int GetTotalResources() { return resourceNames.size(); }

	std::string GetResourceName(unsigned int resourceIterator); // Return the name of the resource at iterator.
	std::string GetResourceName(std::string _resourceName); // In case the name passed was misspelled.
	unsigned int GetResourceIterator(std::string resourceName);

	unsigned int GetResourceSellingPriceAtCity(std::weak_ptr<City> _city, std::string _resource);
};