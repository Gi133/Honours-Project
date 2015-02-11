#pragma once
#include "MapPOI.h"

// Try to keep these the same as in settings file.
enum CityType
{
	Village,
	Town,
	SmallCity,
	LargeCity,
};

class City :
	public MapPOI
{
private:
	int population, minStartPop, maxStartPop; // Town Population.
	int maxPopVillage, maxPopTown, maxPopSmallCity, maxPopLargeCity;
	std::string nameVillage, nameTown, nameSmallCity, nameLargeCity;

	CityType cityType;
	std::string cityTypeName;

	void UpdateCityType();
	void LoadDefinitions() override;

public:
	City();
	~City();

	void SetPopulation(); // Generate population based on settings files.
	void SetPopulation(const int newPopulation){ population = newPopulation; UpdateCityType(); } // Specify the population of the city.

	int GetPopulation(){ return population; }
	std::string GetCityType(){ return cityTypeName; }
};
