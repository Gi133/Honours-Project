#include "stdafx.h"
#include "ResourceManager.h"

namespace
{
	const auto resourceBasePriceLocationFallBack = "Resources/Files/resource_base_price.config";
}

ResourceManager* ResourceManager::_instance = nullptr;

ResourceManager& ResourceManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new ResourceManager;
	return *_instance;
}

ResourceManager::ResourceManager()
{
	LoadPrefs();
	LoadResourceTable();
	LoadResourceNames();
}

void ResourceManager::LoadPrefs()
{
	resourceBasePriceLocation = thePrefs.GetString("ResourceFileSettings", "resourceBasePriceLocation");
	if (resourceBasePriceLocation.empty())
		resourceBasePriceLocation = resourceBasePriceLocationFallBack;
}

void ResourceManager::LoadResourceTable()
{
	Utils::loadConfigFile(resourceBasePriceLocation, resourceBasePriceTable);
}

void ResourceManager::LoadResourceNames()
{
	// Load resource names into the vector.
	for (auto resource : resourceBasePriceTable)
		resourceNames.push_back(resource.first);
}

int ResourceManager::GetBasePrice(const std::string resourceName)
{
	return resourceBasePriceTable.at(resourceName);
}