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

	const auto minConsumptionPerDayFallBack = 0;
	const auto maxConsumptionPerDayFallBack = 2;

	const auto minPopulationChangeFallBack = 0;
	const auto maxPopulationChangeFallBack = 5;

	const auto minResourceChangeFallBack = 0;
	const auto maxResourceChangeFallBack = 25;

	const auto tickMessageNameFallBack = "Tick";
	const auto dayMessageNameFallBack = "Day";
	const auto monthMessageNameFallBack = "Month";
}

City::City()
{
	population = minStartPop = maxStartPop = 0;

	cityType = Village; // Initialize city as village.
	cityTypeName = "";

	LoadDefinitions();

	inventory.reset(new Inventory(true));

	// Hook up messaging system.
	theSwitchboard.SubscribeTo(this, tickMessageName);
	theSwitchboard.SubscribeTo(this, dayMessageName);
	theSwitchboard.SubscribeTo(this, monthMessageName);
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

	tickMessageName = thePrefs.GetString("CitySettings", "tickMessageName");
	if (tickMessageName.empty())
		tickMessageName = tickMessageNameFallBack;

	dayMessageName = thePrefs.GetString("CitySettings", "dayMessageName");
	if (dayMessageName.empty())
		dayMessageName = dayMessageNameFallBack;

	monthMessageName = thePrefs.GetString("CitySettings", "monthMessageName");
	if (monthMessageName.empty())
		monthMessageName = monthMessageNameFallBack;

	minConsumptionPerDay = thePrefs.GetInt("CitySettings", "minConsumptionPerDay");
	if (!minConsumptionPerDay)
		minConsumptionPerDay = minConsumptionPerDayFallBack;

	maxConsumptionPerDay = thePrefs.GetInt("Settings", "maxConsumptionPerDay");
	if (!maxConsumptionPerDay)
		maxConsumptionPerDay = maxConsumptionPerDayFallBack;

	minPopulationChange = thePrefs.GetInt("CitySettings", "minPopulationChange");
	if (!minPopulationChange)
		minPopulationChange = minPopulationChangeFallBack;

	maxPopulationChange = thePrefs.GetInt("CitySettings", "maxPopulationChange");
	if (!maxPopulationChange)
		maxPopulationChange = maxPopulationChangeFallBack;

	minResourceChange = thePrefs.GetInt("CitySettings", "minResourceChange");
	if (!minResourceChange)
		minResourceChange = minResourceChangeFallBack;

	maxResourceChange = thePrefs.GetInt("CitySettings", "maxResourceChange");
	if (!maxResourceChange)
		maxResourceChange = maxResourceChangeFallBack;
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

void City::ReceiveMessage(Message *message)
{
	auto current = 0;
	auto change = 0;
	auto percentage = 0;

	if (message->GetMessageName() == dayMessageName)
	{
		// Randomly take from resource stock.
		for (auto i = 0; i < theResourceManager.GetTotalResources(); i++)
		{
			current = inventory->GetTotalResourceAmount(i);
			percentage = current / 100;
			change = MathUtil::RandomIntInRange((percentage * minConsumptionPerDay), (percentage * maxConsumptionPerDay));
			inventory->SubtractFromResource(0, i, change);
		}
	}

	if (message->GetMessageName() == monthMessageName)
	{
		// For population.
		percentage = population / 100;
		change = MathUtil::RandomIntInRange((percentage * minPopulationChange), (percentage * maxPopulationChange));

		// Randomly add to resource stock and adjust population.
		if (MathUtil::RandomBool())
			population += change;
		else
			population -= change;

		// For resources.
		for (auto i = 0; i < theResourceManager.GetTotalResources(); i++)
		{
			current = inventory->GetTotalResourceAmount(i);
			percentage = current / 100;
			change = MathUtil::RandomIntInRange((percentage * minResourceChange), (percentage * maxResourceChange));
			inventory->SubtractFromResource(0, i, change);
		}

		// Check if city type needs to change.
		UpdateCityType();
	}
}
