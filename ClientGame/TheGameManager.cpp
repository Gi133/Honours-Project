#include "stdafx.h"
#include "TheGameManager.h"

namespace
{
	const int cityNumFallBack = 10;
	const int locationNumFallBack = 10;
}

TheGameManager* TheGameManager::_instance = nullptr;

TheGameManager& TheGameManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new TheGameManager();
	return *_instance;
}

TheGameManager::TheGameManager()
{
	mapSize = halfMapSize = Vector2(0.0f, 0.0f);

	uiManager.reset(new UIManager);
	locationGenerator.reset(new LocationGenerator);

	GenerateMap();
}

void TheGameManager::Render()
{
}

void TheGameManager::Update(float dt)
{
	uiManager->Update();
}

void TheGameManager::GenerateMap()
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

void TheGameManager::GenerateCities(int cityNumber)
{
	for (auto i = 0; i < cityNumber; i++)
	{
		// Crate a new City.
		std::unique_ptr<City> newCity;
		newCity.reset(new City());

		newCity->SetPosition(locationGenerator->GeneratePosition(1, cityContainer, locationContainer));
		newCity->SetName(theNameManger.GenerateName(3, true));

		// Move the city into the container.
		cityContainer.push_back(std::move(newCity));
	}
}

void TheGameManager::GenerateLocations(int locationNum)
{
	for (auto i = 0; i < locationNum; i++)
	{
		// Crate a new location.
		std::unique_ptr<Location> newLocation;
		newLocation.reset(new Location());

		newLocation->SetPosition(locationGenerator->GeneratePosition(2, cityContainer, locationContainer));

		// Move the location into the container.
		locationContainer.push_back(std::move(newLocation));
	}
}