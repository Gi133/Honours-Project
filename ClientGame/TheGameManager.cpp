#include "stdafx.h"
#include "TheGameManager.h"

namespace
{
	const int cityNumFallBack = 10;
	const int locationNumFallBack = 10;
}

TheGameManager* TheGameManager::_instance = NULL;

TheGameManager& TheGameManager::getInstance()
{
	if (_instance == NULL)
		_instance = new TheGameManager();
	return *_instance;
}

TheGameManager::TheGameManager()
{
	uiManager.reset(new UIManager());
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
	// Get Map Size.
	Vector2 mapSize(thePrefs.GetFloat("Map", "width"), thePrefs.GetFloat("Map", "height"));

	// Get number of cities to be generated.
	int cityNum = thePrefs.GetInt("MapGeneratorSettings", "cities");
	if (!cityNum)
		cityNum = cityNumFallBack;

	// Get number of locations to be generated.
	int locationNumber = thePrefs.GetInt("MapGeneratorSettings", "locations");
}

void TheGameManager::GenerateCities(int cityNumber)
{
	for (auto i = 0; i < cityNumber; i++)
	{
		// Crate a new City.
		std::unique_ptr<City> newCity;

		// Generate city info.
		

		// Move the city into the container.
		cityContainer.push_back(std::move(newCity));
	}
}

void TheGameManager::GenerateLocations()
{

}
