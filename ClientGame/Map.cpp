#include "stdafx.h"
#include "Map.h"

namespace
{
	const int cityNumFallBack = 10;
	const int locationNumFallBack = 10;
}

Map* Map::_instance = nullptr;

Map& Map::getInstance()
{
	if (_instance == nullptr)
		_instance = new Map;
	return *_instance;
}

Map::Map()
{
	locationGenerator.reset(new LocationGenerator);
}

void Map::GenerateMap()
{
	// Get number of cities to be generated.
	int cityNum = thePrefs.GetInt("MapGeneratorSettings", "cities");
	if (!cityNum)
		cityNum = cityNumFallBack;

	// Get number of locations to be generated.
	int locationNum = thePrefs.GetInt("MapGeneratorSettings", "locations");
	if (!locationNum)
		locationNum = locationNumFallBack;

	GenerateCities(cityNum);
	GenerateLocations(locationNum);
}

void Map::GenerateCities(int cityNumber)
{
	for (auto i = 0; i < cityNumber; i++)
		GenerateCity();
}

void Map::GenerateLocations(int locationNum)
{
	for (auto i = 0; i < locationNum; i++)
		GenerateLocation();
}

void Map::GenerateCity()
{
	auto generatedPosition = locationGenerator->GeneratePosition(1, cityContainer, locationContainer);

	if (generatedPosition != NULL)
	{
		// Crate a new City.
		std::shared_ptr<City> newCity;
		newCity.reset(new City());

		newCity->SetPosition(generatedPosition);

		newCity->SetName(theNameManager.GenerateName(3, true));
		newCity->SetPopulation();
		newCity->SetResources();

		// Move the city into the container.
		cityContainer.push_back(newCity);
	}
}

void Map::GenerateLocation()
{
	auto generatedPosition = locationGenerator->GeneratePosition(2, cityContainer, locationContainer);

	if (generatedPosition != NULL)
	{
		// Crate a new location.
		std::shared_ptr<Location> newLocation;
		newLocation.reset(new Location());

		newLocation->SetPosition(generatedPosition);

		newLocation->SetName(theNameManager.GenerateName(4));
		newLocation->SetThreatLevel();

		// Move the location into the container.
		locationContainer.push_back(newLocation);
	}
}