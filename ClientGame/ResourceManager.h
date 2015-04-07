#pragma once
#include "Utils.h"
#include <array>

#define theResourceManager ResourceManager::getInstance()

class ResourceManager : public MessageListener
{
private:
	ResourceManager();
	~ResourceManager() { _instance = nullptr; }
	static ResourceManager* _instance;

	unsigned int priceMultiplier;

	std::string tickMessageName;
	bool averagePricesUpdated;

	void InitializeAveragePriceVector();
	void UpdateAveragePrices();

	void LoadPrefs();
	void LoadResourceTable();
	void LoadResourceNames();

	std::map<std::string, int> resourceBasePriceTable;
	std::string resourceBasePriceLocation;

	std::vector<std::string> resourceNames;
	std::vector<float> averagePrice;

	void ReceiveMessage(Message *message);

public:
	static ResourceManager& getInstance();

	struct TradingStruct
	{
		std::weak_ptr<City> start;
		std::weak_ptr<City> finish;
		std::string resource;
		unsigned int expectedBuyPrice;
		unsigned int expectedSellPrice;
	};

	// Reload Values into maps.
	void ReloadTables() { LoadResourceTable(); LoadResourceNames(); }

	// Return base price based on name.
	int GetBasePrice(const std::string resourceName);

	// Return average price on a resource.
	int GetAveragePrice(const std::string resourceName);
	int GetAveragePrice(const unsigned int resourceIt);

	// Returns all resource names in a string vector.
	std::vector<std::string> GetResourceNames() { return resourceNames; }

	// Return the number of all resources in the game.
	int GetTotalResources() { return resourceNames.size(); }

	std::string GetResourceName(unsigned int resourceIterator); // Return the name of the resource at iterator.
	std::string GetResourceName(std::string _resourceName); // In case the name passed was misspelled.
	unsigned int GetResourceIterator(std::string resourceName);

	// Resource Prices at Cities
	unsigned int GetResourceSellingPriceAtCity(std::weak_ptr<City> _city, std::string _resource);
	unsigned int GetResourceSellingPriceAtCity(std::weak_ptr<City> _city, const int _resourceIt);

	int GetTradeProfitabilityBetween(std::weak_ptr<City> _cityStart, std::weak_ptr<City> _cityFinish, std::string _resourceName);
	int GetTradeProfitabilityBetween(std::weak_ptr<City> _cityStart, std::weak_ptr<City> _cityFinish, const unsigned int _resourceIt);

	std::vector<TradingStruct> GetSpecificTradeRoutesFromCity(const std::weak_ptr<City> _city, std::string _resourceName, const unsigned int _moneyAvailable, const int _profitabilityBias = 0, const float _maxDistance = 0.0f, float _maxThreat = -1.0f);
	std::vector<TradingStruct> GetSpecificTradeRoutesFromCity(const std::weak_ptr<City> _city, const unsigned int _resourceIt, const unsigned int _moneyAvailable, const int _profitabilityBias = 0, const float _maxDistance = 0.0f, float _maxThreat = -1.0f);
	std::vector<TradingStruct> GetSpecificTradeRoutesFromCity(const std::weak_ptr<City> _city, std::map<std::string, int> inventory, const unsigned int _moneyAvailable, const int _profitabilityBias = 0, const float _maxDistance = 0.0f, float _maxThreat = -1.0f);

	std::vector<TradingStruct> GetTradeRoutesFromCity(const std::weak_ptr<City> _city, const unsigned int _moneyAvailable, const int _profitabilityBias = 0, const float _maxDistance = 0.0f, float _maxThreat = -1.0f);
	std::vector<TradingStruct> GetTradeRoutesBetween(const std::weak_ptr<City> _cityBegin, const std::weak_ptr<City> _cityFinish, const unsigned int _moneyAvailable, const int _profitabilityBias = 0, const float _maxDistance = 0.0f, float _maxThreat = -1.0f);
};