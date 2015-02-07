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
}

City::City()
{
	population = minStartPop = maxStartPop = 0;

	cityType = Village; // Initialize city as village.
	cityTypeName = "Village";

	LoadDefinitions();
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
		cityTypeName = "Village";
	}
	else if (population <= maxPopTown)
	{
		cityType = Town;
		cityTypeName = "Town";
	}
	else if (population <= maxPopSmallCity)
	{
		cityType = SmallCity;
		cityTypeName = "Small City";
	}
	else if (population <= maxPopLargeCity)
	{
		cityType = LargeCity;
		cityTypeName = "Large City";
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
}
