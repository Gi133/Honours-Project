#include "stdafx.h"
#include "Map.h"
#include <cmath>

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

float Map::GetDistanceBetween(std::weak_ptr<City> cityA, std::weak_ptr<City> cityB)
{
	Vector2 cityAPos, cityBPos;
	bool cityAFound = false;
	bool cityBFound = false;

	// Iterate through the container and find cityA.
	for (auto city : cityContainer)
	{
		if (!cityAFound)
		{
			if (cityA.lock()->GetName() == city->GetName())
			{
				cityAPos = city->GetPosition();
				cityAFound = true;
			}
		}

		if (!cityBFound)
		{
			if (cityB.lock()->GetName() == city->GetName())
			{
				cityBPos = city->GetPosition();
				cityBFound = true;
			}
		}

		if (cityAFound && cityBFound)
			break;
	}

	// Exception Catch
	if (!cityAFound || !cityBFound)
		return -1.0f;

	// Knowing the position of the cities, find the distance between them.
	float distanceX = std::abs(cityAPos.X - cityBPos.X);
	float distanceY = std::abs(cityAPos.Y - cityBPos.Y);

	// Pythagoras (a^2 + b^2 = c^2)
	return std::sqrtf(distanceX + distanceY);
}

float Map::GetDistanceBetween(std::string cityNameA, std::string cityNameB)
{
	Vector2 cityAPos, cityBPos;
	bool cityAFound = false;
	bool cityBFound = false;

	// Iterate through the container and find cityA.
	for (auto city : cityContainer)
	{
		if (!cityAFound)
		{
			if (cityNameA == city->GetName())
				cityAPos = city->GetPosition();
		}

		if (!cityBFound)
		{
			if (cityNameB == city->GetName())
				cityBPos = city->GetPosition();
		}

		// If you found both, then you can break.
		if (cityAFound && cityBFound)
			break;
	}

	// Exception Catch
	if (!cityAFound || !cityBFound)
		throw(std::runtime_error("Could not find one or more cities when trying to calculate the distance between them."));

	// Knowing the position of the cities, find the distance between them.
	float distanceX = std::abs(cityAPos.X - cityBPos.X);
	float distanceY = std::abs(cityAPos.Y - cityBPos.Y);

	// Pythagoras (a^2 + b^2 = c^2)
	return std::sqrtf(distanceX + distanceY);
}

std::weak_ptr<City> Map::GetRandomCity()
{
	return cityContainer.at(MathUtil::RandomIntInRange(0, theMap.GetCityContainerRef().get().size()));
}

std::weak_ptr<City> Map::CityNameToPtr(std::string cityName)
{
	for (auto city : cityContainer)
		if (city->GetName() == cityName)
			return city;

	throw(std::runtime_error("Could not find city name in city container."));
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

void Map::GetSortedNeighbors(std::reference_wrapper<std::vector<std::weak_ptr<City>>> _neighborContainer, const std::weak_ptr<City> _city, float _distanceCutOff /*= 0.0f*/)
{
	float distance = 0.0f;
	std::map<float, std::weak_ptr<City>> cityDistanceMap;

	// Empty the container.
	_neighborContainer.get().clear();

	// In case of a negative value passed in, convert it to positive.
	if (_distanceCutOff < 0.0f)
		_distanceCutOff = abs(_distanceCutOff);

	// Add all cities that are within the cutoff distance into the map.
	for (auto city : cityContainer)
	{
		if (city->GetName() != _city.lock()->GetName())
		{
			distance = GetDistanceBetween(city, _city);
			if ((distance < _distanceCutOff) || (_distanceCutOff == 0.0f))
				cityDistanceMap.insert(std::pair<float, std::weak_ptr<City>>(distance, city)); // Add city to container.
		}
	}

	// Sort
	//std::sort(cityDistanceMap.begin(), cityDistanceMap.end());

	for (auto city : cityDistanceMap)
		_neighborContainer.get().push_back(std::move(city.second));
}

std::weak_ptr<City> Map::GetClosestNeighbor(std::weak_ptr<City> _city, float _maxDistance /* = 0.0f*/)
{
	float distance = 0.0f;

	if (_maxDistance < 0.0f)
		_maxDistance = abs(_maxDistance);

	std::map<float, std::weak_ptr<City>> cityDistanceMap;

	for (auto city : cityContainer)
	{
		if (city->GetName() != _city.lock()->GetName())
		{
			distance = GetDistanceBetween(city, _city);
			if ((distance < _maxDistance) || (_maxDistance == 0.0f))
				cityDistanceMap.insert(std::pair<float, std::weak_ptr<City>>(distance, city)); // Add city to container.
		}
	}

	// Sort
	//std::sort(cityDistanceMap.begin(), cityDistanceMap.end());

	return cityDistanceMap.begin()->second;
}