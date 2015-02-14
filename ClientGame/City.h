#pragma once
#include "MapPOI.h"
#include "Inventory.h"

// Try to keep these the same as in settings file.
enum CityType
{
	Village,
	Town,
	SmallCity,
	LargeCity,
};

class City :
	public MapPOI, public MessageListener
{
private:
	int population, minStartPop, maxStartPop; // Town Population.
	int maxPopVillage, maxPopTown, maxPopSmallCity, maxPopLargeCity;
	int minStartingResources, maxStartingResources, minConsumptionPerDay,
		maxConsumptionPerDay, minPopulationChange, maxPopulationChange, minResourceChange, maxResourceChange;
	std::string nameVillage, nameTown, nameSmallCity, nameLargeCity, tickMessageName, dayMessageName, monthMessageName;

	CityType cityType;
	std::string cityTypeName;
	std::unique_ptr<Inventory> inventory;

	void UpdateCityType();
	void LoadDefinitions() override;

	virtual void ReceiveMessage(Message *message) override;

public:
	City();
	~City();

	void SetPopulation(); // Generate population based on settings files.
	void SetPopulation(const int newPopulation){ population = newPopulation; UpdateCityType(); } // Specify the population of the city.
	void SetResources(); // Generate resource quantities based on settings file options.
	void SetResources(std::string resourceName, int quantity);

	int GetPopulation(){ return population; }
	std::string GetCityType(){ return cityTypeName; }
};
