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
		_instance = new TheGameManager;
	return *_instance;
}

TheGameManager::TheGameManager()
{
	mapSize = halfMapSize = Vector2(0.0f, 0.0f);

	uiManager.reset(new UIManager);
	timeManager.reset(new TimeManager);
	locationGenerator.reset(new LocationGenerator);

	uiManager->AssignTimeManager(timeManager);

	GenerateMap();

	uiManager->SetupMapActors(cityContainer, locationContainer);
}

void TheGameManager::Render()
{
}

void TheGameManager::Update(float dt)
{
	uiManager->Update();
	timeManager->Update();
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
		GenerateCity();
		// 		std::unique_ptr<std::thread> newThread;
		// 		newThread.reset(new std::thread(&TheGameManager::GenerateCity, this));
		// 		threadContainer.push_back(std::move(newThread));
	}

	// 	while (!threadContainer.empty())
	// 	{
	// 		for (std::vector<std::unique_ptr<std::thread>>::iterator i = threadContainer.begin(); i != threadContainer.end(); ++i)
	// 		{
	// 			if (i->get()->joinable())
	// 			{
	// 				i->get()->join();
	// 				threadContainer.erase(i);
	// 				threadContainer.shrink_to_fit();
	// 				break;
	// 			}
	// 		}
	// 	}
}

void TheGameManager::GenerateLocations(int locationNum)
{
	for (auto i = 0; i < locationNum; i++)
	{
		GenerateLocation();
		// 		std::unique_ptr<std::thread> newThread;
		// 		newThread.reset(new std::thread(&TheGameManager::GenerateLocation, this));
		// 		threadContainer.push_back(std::move(newThread));
	}
}

void TheGameManager::GenerateCity()
{
	// Crate a new City.
	std::shared_ptr<City> newCity;
	newCity.reset(new City());

	newCity->SetPosition(locationGenerator->GeneratePosition(1, cityContainer, locationContainer));

	newCity->SetName(theNameManger.GenerateName(3, true));
	newCity->SetPopulation();
	newCity->SetResources();

	// Move the city into the container.
	cityContainer.push_back(newCity);
}

void TheGameManager::GenerateLocation()
{
	// Crate a new location.
	std::unique_ptr<Location> newLocation;
	newLocation.reset(new Location());

	newLocation->SetPosition(locationGenerator->GeneratePosition(2, cityContainer, locationContainer));

	newLocation->SetName(theNameManger.GenerateName(4));
	newLocation->SetThreatLevel();

	// Move the location into the container.
	locationContainer.push_back(std::move(newLocation));
}