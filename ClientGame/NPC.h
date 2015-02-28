#pragma once

#include "Brain.h"
#include "Inventory.h"
#include "NameManager.h"
#include "Map.h"
#include <regex>

class NPC
{
private:
	std::string name;

	std::unique_ptr<Brain> aiBrain;
	std::shared_ptr<Inventory> inventory;

	std::weak_ptr<City> currentCity;

	void LoadDefaults();
	void InitializeInventory(const int startingBagNumber, const int startingGoldNumber);

public:
	NPC();
	~NPC();

	std::weak_ptr<Inventory> GetInventory() { return inventory; }

	void GenerateName();
	void GenerateStartingCity();
	void SetCity(const int cityIterator) { currentCity = theMap.GetCityContainerRef().get().at(cityIterator); }
	void SetStartingCity(const std::string cityName);

	std::string GetName() { return name; }
	std::string GetCityName() { return currentCity.lock()->GetName(); }
};
