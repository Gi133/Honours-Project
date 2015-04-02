#include "stdafx.h"
#include "ResourceManager.h"

namespace
{
	const auto resourceBasePriceLocationFallBack = "Resources/Files/resource_base_price.config";
	const unsigned int priceMultiplierFallBack = 10;
	const auto tickMessageNameFallBack = "Tick";
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
	InitializeAveragePriceVector();

	averagePricesUpdated = false;

	theSwitchboard.SubscribeTo(this, tickMessageName);
}

void ResourceManager::InitializeAveragePriceVector()
{
	for (unsigned int i = 0; i < resourceNames.size(); i++)
		averagePrice.push_back(0.0f);
}

void ResourceManager::UpdateAveragePrices()
{
	auto cities = theMap.GetCityContainerRef().get();

	for (unsigned int resourceIt = 0; resourceIt < resourceNames.size(); resourceIt++)
	{
		// For every City, add price of a product into the average price vector.
		for (unsigned int i = 0; i < cities.size(); i++)
			averagePrice.at(resourceIt) += GetResourceSellingPriceAtCity(cities.at(i), resourceIt);

		// Calculate average.
		averagePrice.at(resourceIt) /= cities.size();
	}

	averagePricesUpdated = true;
}

void ResourceManager::LoadPrefs()
{
	resourceBasePriceLocation = thePrefs.GetString("ResourceFileSettings", "resourceBasePriceLocation");
	if (resourceBasePriceLocation.empty())
		resourceBasePriceLocation = resourceBasePriceLocationFallBack;

	priceMultiplier = thePrefs.GetInt("ResourceFileSettings", "priceMultiplier");
	if (!priceMultiplier)
		priceMultiplier = priceMultiplierFallBack;

	tickMessageName = thePrefs.GetString("CitySettings", "tickMessageName");
	if (tickMessageName.empty())
		tickMessageName = tickMessageNameFallBack;
}

void ResourceManager::LoadResourceTable()
{
	Utils::loadConfigFile(resourceBasePriceLocation, resourceBasePriceTable);
}

void ResourceManager::LoadResourceNames()
{
	// Load resource names into the vector.
	for (std::map<std::string, int>::iterator i = resourceBasePriceTable.begin(); i != resourceBasePriceTable.end(); ++i)
		resourceNames.push_back(i->first);
}

void ResourceManager::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == tickMessageName)
		UpdateAveragePrices();
}

int ResourceManager::GetBasePrice(const std::string resourceName)
{
	return resourceBasePriceTable.at(resourceName);
}

int ResourceManager::GetAveragePrice(const std::string resourceName)
{
	return averagePrice.at(GetResourceIterator(resourceName));
}

int ResourceManager::GetAveragePrice(const unsigned int resourceIt)
{
	return averagePrice.at(resourceIt);
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
	auto deviation = static_cast<float>(city->GetPopulation()) / static_cast<float>(city->GetInventory().lock()->GetTotalResourceAmount(_resource))* resourceBasePrice * priceMultiplier;
	return resourceBasePrice + deviation;
}

unsigned int ResourceManager::GetResourceSellingPriceAtCity(std::weak_ptr<City> _city, const int _resourceIt)
{
	auto resourceBasePrice = resourceBasePriceTable.at(GetResourceName(_resourceIt));
	auto city = _city.lock();

	// Formula:
	// Resource Price =  BasePrice + ((Population / CurrentStock) / CurrentStock) * BasePrice * MULTIPLIER_CONSTANT(10);
	auto deviation = static_cast<float>(city->GetPopulation()) / static_cast<float>(city->GetInventory().lock()->GetTotalResourceAmount(_resourceIt))* resourceBasePrice * priceMultiplier;
	return resourceBasePrice + deviation;
}

int ResourceManager::GetTradeProfitabilityBetween(std::weak_ptr<City> _cityStart, std::weak_ptr<City> _cityFinish, std::string _resourceName)
{
	// Correct name.
	_resourceName = GetResourceName(_resourceName);

	return GetResourceSellingPriceAtCity(_cityFinish, _resourceName) - GetResourceSellingPriceAtCity(_cityStart, _resourceName);;
}

int ResourceManager::GetTradeProfitabilityBetween(std::weak_ptr<City> _cityStart, std::weak_ptr<City> _cityFinish, const unsigned int _resourceIt)
{
	if (_resourceIt < resourceNames.size())
		return GetResourceSellingPriceAtCity(_cityFinish, _resourceIt) - GetResourceSellingPriceAtCity(_cityStart, _resourceIt);

	throw(std::runtime_error("Resource Iterator out of range when checking for profitability"));
}

std::vector<ResourceManager::TradingStruct> ResourceManager::GetSpecificTradeRoutesFromCity(const std::weak_ptr<City> _city, std::string _resourceName,
	const int _profitabilityBias /* = 0*/, const float _maxDistance /*= 0.0f*/, float _maxThreat /* = -1.0f */)
{
	if (!averagePricesUpdated)
		UpdateAveragePrices();

	std::vector<TradingStruct> output;

	// Neighbor variables.
	std::vector<std::weak_ptr<City>> neighbors;
	unsigned int neighborPrice = 0;

	unsigned int tradeProfitability = 0;

	// Check name.
	_resourceName = GetResourceName(_resourceName);

	// Get price of item in city.
	auto buyingPrice = GetResourceSellingPriceAtCity(_city, _resourceName);

	// If buying price is below average then consider it a good sale.
	if (buyingPrice <= averagePrice.at(GetResourceIterator(_resourceName)))
	{
		// Check for trades by looking at neighbors starting with closest.
		theMap.GetSortedNeighbors(std::ref(neighbors), _city, _maxDistance);
		for (auto neighbor : neighbors)
		{
			auto profitability = GetTradeProfitabilityBetween(_city, neighbor, _resourceName);
			if (profitability >= _profitabilityBias)
			{
				// Check Threat.
				auto threat = theMap.GetDangerRatingBetween(_city, neighbor);
				if ((threat <= _maxThreat) || (_maxThreat <= -1.0f))
				{
					TradingStruct tradeRoute;

					// Configure the trade route.
					tradeRoute.start = _city;
					tradeRoute.finish = neighbor;
					tradeRoute.resource = _resourceName;
					tradeRoute.expectedBuyPrice = GetResourceSellingPriceAtCity(_city, _resourceName);
					tradeRoute.expectedSellPrice = GetResourceSellingPriceAtCity(neighbor, _resourceName);

					// Add to possible trade routes.
					output.push_back(tradeRoute);
				}
			}
		}
	}

	averagePricesUpdated = false;

	return output;
}

std::vector<ResourceManager::TradingStruct> ResourceManager::GetSpecificTradeRoutesFromCity(const std::weak_ptr<City> _city, const unsigned int _resourceIt,
	const int _profitabilityBias /*= 0*/, const float _maxDistance /*= 0.0f*/, float _maxThreat /*= -1.0f*/)
{
	if (!averagePricesUpdated)
		UpdateAveragePrices();

	std::vector<TradingStruct> output;

	// Neighbor variables.
	std::vector<std::weak_ptr<City>> neighbors;
	unsigned int neighborPrice = 0;

	unsigned int tradeProfitability = 0;

	// Check resource iterator.
	if (_resourceIt > resourceNames.size())
		throw(std::runtime_error("Resource iterator out of bounds"));

	// Get price of item in city.
	auto buyingPrice = GetResourceSellingPriceAtCity(_city, _resourceIt);

	// If buying price is below average then consider it a good sale.
	if (buyingPrice <= averagePrice.at(_resourceIt))
	{
		// Check for trades by looking at neighbors starting with closest.
		theMap.GetSortedNeighbors(std::ref(neighbors), _city, _maxDistance);
		for (auto neighbor : neighbors)
		{
			auto profitability = GetTradeProfitabilityBetween(_city, neighbor, _resourceIt);
			if (profitability >= _profitabilityBias)
			{
				// Check Threat.
				auto threat = theMap.GetDangerRatingBetween(_city, neighbor);
				if ((threat <= _maxThreat) || (_maxThreat <= -1.0f))
				{
					TradingStruct tradeRoute;

					// Configure the trade route.
					tradeRoute.start = _city;
					tradeRoute.finish = neighbor;
					tradeRoute.resource = resourceNames.at(_resourceIt);
					tradeRoute.expectedBuyPrice = GetResourceSellingPriceAtCity(_city, _resourceIt);
					tradeRoute.expectedSellPrice = GetResourceSellingPriceAtCity(neighbor, _resourceIt);

					// Add to possible trade routes.
					output.push_back(tradeRoute);
				}
			}
		}
	}

	averagePricesUpdated = false;

	return output;
}

std::vector<ResourceManager::TradingStruct> ResourceManager::GetSpecificTradeRoutesFromCity(const std::weak_ptr<City> _city, std::map<std::string, int> inventory, const int _profitabilityBias /*= 0*/, const float _maxDistance /*= 0.0f*/, float _maxThreat /*= -1.0f*/)
{
	if (!averagePricesUpdated)
		UpdateAveragePrices();

	std::vector<TradingStruct> output;

	// Neighbor variables.
	std::vector<std::weak_ptr<City>> neighbors;
	unsigned int neighborPrice = 0;

	unsigned int tradeProfitability = 0;

	// For all the resources.
	for (std::map<std::string, int>::iterator i = inventory.begin(); i != inventory.end(); ++i)
	{
		// Get Resource iterator to make things easy.
		unsigned int resourceIt = GetResourceIterator(i->first);

		// Get price of item in city.
		auto buyingPrice = GetResourceSellingPriceAtCity(_city, resourceIt);

		// If buying price is below average then consider it a good sale.
		if (buyingPrice <= averagePrice.at(resourceIt))
		{
			// Check for trades by looking at neighbors starting with closest.
			theMap.GetSortedNeighbors(std::ref(neighbors), _city, _maxDistance);
			for (auto neighbor : neighbors)
			{
				auto profitability = GetTradeProfitabilityBetween(_city, neighbor, resourceIt);
				if (profitability >= _profitabilityBias)
				{
					// Check Threat.
					auto threat = theMap.GetDangerRatingBetween(_city, neighbor);
					if ((threat <= _maxThreat) || (_maxThreat <= -1.0f))
					{
						TradingStruct tradeRoute;

						// Configure the trade route.
						tradeRoute.start = _city;
						tradeRoute.finish = neighbor;
						tradeRoute.resource = resourceNames.at(resourceIt);
						tradeRoute.expectedBuyPrice = GetResourceSellingPriceAtCity(_city, resourceIt);
						tradeRoute.expectedSellPrice = GetResourceSellingPriceAtCity(neighbor, resourceIt);

						// Add to possible trade routes.
						output.push_back(tradeRoute);
					}
				}
			}
		}
	}

	averagePricesUpdated = false;

	return output;
}

std::vector<ResourceManager::TradingStruct> ResourceManager::GetTradeRoutesFromCity(const std::weak_ptr<City> _city, const int _profitabilityBias /*= 0*/,
	const float _maxDistance /*= 0.0f*/, float _maxThreat /*= -1.0f*/)
{
	std::vector<TradingStruct> output;

	for (unsigned int i = 0; i < resourceNames.size(); i++)
	{
		// For each resource, find potential trade routes.
		auto potentialTrade = GetSpecificTradeRoutesFromCity(_city, i, _profitabilityBias, _maxDistance);

		// Add the potential trade routes into the output vector.
		if (output.empty())
			output = std::move(potentialTrade);
		else
		{
			output.reserve(output.size() + potentialTrade.size());
			std::move(std::begin(potentialTrade), std::end(potentialTrade), std::back_inserter(output));
		}
	}

	return output;
}

std::vector<ResourceManager::TradingStruct> ResourceManager::GetTradeRoutesBetween(const std::weak_ptr<City> _cityBegin, const std::weak_ptr<City> _cityFinish,
	const int _profitabilityBias /*= 0*/, const float _maxDistance /*= 0.0f*/, float _maxThreat /*= -1.0f*/)
{
	std::vector<TradingStruct> output;

	for (unsigned int resourceIt = 0; resourceIt < resourceNames.size(); resourceIt++)
	{
		auto profitability = GetTradeProfitabilityBetween(_cityBegin, _cityFinish, resourceIt);
		if (profitability >= _profitabilityBias)
		{
			TradingStruct tradeRoute;

			auto threat = theMap.GetDangerRatingBetween(_cityBegin, _cityFinish);
			if ((_maxThreat <= -1.0f) || (threat <= _maxThreat))
			{
				// Ignore threat and add everything to the output.
				// Configure the trade route.
				tradeRoute.start = _cityBegin;
				tradeRoute.finish = _cityFinish;
				tradeRoute.resource = resourceNames.at(resourceIt);
				tradeRoute.expectedBuyPrice = GetResourceSellingPriceAtCity(_cityBegin, resourceIt);
				tradeRoute.expectedSellPrice = GetResourceSellingPriceAtCity(_cityFinish, resourceIt);

				// Add to possible trade routes.
				output.push_back(tradeRoute);
			}
		}
	}

	return output;
}