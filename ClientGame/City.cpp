#include "stdafx.h"
#include "City.h"

namespace
{
	const auto minStartPopFallBack = 8;
	const auto maxStartPopFallBack = 2500;

	const auto maxPopVillageFallBack = 140;
	const auto maxPopTownFallBack = 500;
	const auto maxPopSmallCityFallBack = 1500;
	const auto maxPopLargeCityFallBack = 3000;

	const auto nameVillageFallBack = "Village";
	const auto nameTownFallBack = "Town";
	const auto nameSmallCityFallBack = "Small City";
	const auto nameLargeCityFallBack = "Large City";

	const auto maxStartingResourcesFallBack = 100000;
	const auto minStartingResourcesFallBack = 10000;
}

City::City()
{
	population = minStartPop = maxStartPop = 0;

	cityType = Village; // Initialize city as village.
	cityTypeName = "";

	LoadDefinitions();

	inventory.reset(new Inventory(true));
}

City::~City()
{
}

void City::SetPopulation()
{
	// Randomly generate population based on min and max numbers loaded.
	population = MathUtil::RandomIntInRange(minStartPop, maxStartPop);

	// Update city type accordingly.
	UpdateCityType();
}

void City::UpdateCityType()
{
	// TODO: Fix this up so it is not nearly as messy and ugly.

	if (population <= maxPopVillage)
	{
		cityType = Village;
		cityTypeName = nameVillage;
	}
	else if (population <= maxPopTown)
	{
		cityType = Town;
		cityTypeName = nameTown;
	}
	else if (population <= maxPopSmallCity)
	{
		cityType = SmallCity;
		cityTypeName = nameSmallCity;
	}
	else if (population <= maxPopLargeCity)
	{
		cityType = LargeCity;
		cityTypeName = nameLargeCity;
	}
	else
		sysLog.Log("Invalid City population");
}

void City::LoadDefinitions()
{
	minStartPop = thePrefs.GetInt("CitySettings", "minStartPop");
	if (!minStartPop)
		minStartPop = minStartPopFallBack;

	maxStartPop = thePrefs.GetInt("CitySettings", "maxStartPop");
	if (!maxStartPop)
		maxStartPop = maxStartPopFallBack;

	maxPopVillage = thePrefs.GetInt("CitySettings", "maxPopVillage");
	if (!maxPopVillage)
		maxPopVillage = maxPopVillageFallBack;

	maxPopTown = thePrefs.GetInt("CitySettings", "maxPopTown");
	if (!maxPopTown)
		maxPopTown = maxPopTownFallBack;

	maxPopSmallCity = thePrefs.GetInt("CitySettings", "maxPopSmallCity");
	if (!maxPopSmallCity)
		maxPopSmallCity = maxPopSmallCityFallBack;

	maxPopLargeCity = thePrefs.GetInt("CitySettings", "maxPopLargeCity");
	if (!maxPopLargeCity)
		maxPopLargeCity = maxPopLargeCityFallBack;

	nameVillage = thePrefs.GetString("CitySettings", "nameVillage");
	if (nameVillage.empty())
		nameVillage = nameVillageFallBack;

	nameTown = thePrefs.GetString("CitySettings", "nameTown");
	if (nameTown.empty())
		nameTown = nameTownFallBack;

	nameSmallCity = thePrefs.GetString("CitySettings", "nameSmallCity");
	if (nameSmallCity.empty())
		nameSmallCity = nameSmallCityFallBack;

	nameLargeCity = thePrefs.GetString("CitySettings", "nameLargeCity");
	if (nameLargeCity.empty())
		nameLargeCity = nameLargeCityFallBack;

	maxStartingResources = thePrefs.GetInt("CitySettings", "maxStartingResources");
	if (!maxStartingResources)
		maxStartingResources = maxStartingResourcesFallBack;

	minStartingResources = thePrefs.GetInt("CitySettings", "minStartingResources");
	if (!minStartingResources)
		minStartingResources = minStartingResourcesFallBack;
}

void City::SetResources()
{
	for (auto i = 0; i < theResourceManager.GetTotalResources(); i++)
		inventory->SetResource(0, i, MathUtil::RandomIntInRange(minStartingResources, maxStartingResources));
}

void City::SetResources(std::string resourceName, int quantity)
{
	inventory->SetResource(resourceName, quantity);
}