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
	if (!threadContainer.empty())
	{
		for (auto i : threadContainer)
			i->join();
	}
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
	std::shared_ptr<std::thread> newThread;

	for (auto i = 0; i < cityNumber; i++)
	{
		newThread.reset(new std::thread(&TheGameManager::GenerateCity, this));
		threadContainer.push_back(newThread);
	}
}

void TheGameManager::GenerateLocations(int locationNum)
{
	std::shared_ptr<std::thread> newThread;

	for (auto i = 0; i < locationNum; i++)
	{
		newThread.reset(new std::thread(&TheGameManager::GenerateLocation, this));
		threadContainer.push_back(newThread);
	}
}

void TheGameManager::GenerateCity()
{
	// Crate a new City.
	std::unique_ptr<City> newCity;
	newCity.reset(new City());

	std::lock_guard<std::mutex> guard(positionMutex); // Not ideal.
	newCity->SetPosition(locationGenerator->GeneratePosition(1, cityContainer, locationContainer));

	newCity->SetName(theNameManger.GenerateName(3, true));
	newCity->SetPopulation();
	newCity->SetResources();

	// Move the city into the container.
	cityContainerMutex.lock();
	cityContainer.push_back(std::move(newCity));
	cityContainerMutex.unlock();
}

void TheGameManager::GenerateLocation()
{
	// Crate a new location.
	std::unique_ptr<Location> newLocation;
	newLocation.reset(new Location());

	std::lock_guard<std::mutex> guard(positionMutex); // Not ideal.
	newLocation->SetPosition(locationGenerator->GeneratePosition(2, cityContainer, locationContainer));

	newLocation->SetName(theNameManger.GenerateName(4));
	newLocation->SetThreatLevel();

	// Move the location into the container.
	locationContainerMutex.lock();
	locationContainer.push_back(std::move(newLocation));
	locationContainerMutex.unlock();
}