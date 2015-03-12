#include "stdafx.h"
#include "ResourceManager.h"

namespace
{
	const auto resourceBasePriceLocationFallBack = "Resources/Files/resource_base_price.config";
	const unsigned int priceMultiplierFallBack = 10;
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

	priceMultiplier = thePrefs.GetInt("ResourceFileSettings", "priceMultiplier");
	if (!priceMultiplier)
		priceMultiplier = priceMultiplierFallBack;
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

std::string ResourceManager::GetResourceName(unsigned int resourceIterator)
{
	return resourceNames.at(resourceIterator);
}

std::string ResourceManager::GetResourceName(std::string _resourceName)
{
	for (auto i : resourceNames)
		if (std::regex_search(i, std::regex(_resourceName, std::regex_constants::ECMAScript | std::regex_constants::icase)))
			return i;

	return "";
}

unsigned int ResourceManager::GetResourceIterator(std::string resourceName)
{
	for (unsigned int i = 0; i < resourceNames.size(); i++)
		if (std::regex_search(resourceNames.at(i), std::regex(resourceName, std::regex_constants::ECMAScript | std::regex_constants::icase)))
			return i;

	return 0;
}

unsigned int ResourceManager::GetResourceSellingPriceAtCity(std::weak_ptr<City> _city, std::string _resource)
{
	auto resourceBasePrice = resourceBasePriceTable.at(GetResourceName(_resource));
	auto city = _city.lock();

	// Formula:
	// Resource Price =  BasePrice + ((Population / CurrentStock) / CurrentStock) * BasePrice * MULTIPLIER_CONSTANT(10);
	return (resourceBasePrice + (city->GetPopulation() / city->GetInventory().lock()->GetTotalResourceAmount(_resource)) * resourceBasePrice * priceMultiplier);
}
