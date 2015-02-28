#pragma once
#include "stdafx.h"
#include "Utils.h"

#define theResourceManager ResourceManager::getInstance()

class ResourceManager
{
private:
	ResourceManager();
	~ResourceManager() { _instance = nullptr; }
	static ResourceManager* _instance;

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
};