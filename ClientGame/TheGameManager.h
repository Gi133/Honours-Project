#pragma once

#include "stdafx.h"
#include "NameManager.h"
#include "UIManager.h"
#include "LocationGenerator.h"
#include "City.h"
#include "Location.h"
#include "NPC.h"
#include "ResourceManager.h"
#include "TimeManager.h"

#include <thread>
#include <mutex>

// TODO: Add thread number check. There is a limit on how many you can run!

#define theGameManger TheGameManager::getInstance()

class TheGameManager :
	public GameManager
{
private:
	std::unique_ptr<UIManager> uiManager;
	std::shared_ptr<TimeManager> timeManager;
	std::unique_ptr<LocationGenerator> locationGenerator;
	std::vector<std::shared_ptr<City>> cityContainer;
	std::vector<std::shared_ptr<Location>> locationContainer;
	std::vector<std::shared_ptr<NPC>> npcContainer;

	Vector2 mapSize, halfMapSize;

	std::vector<std::unique_ptr<std::thread>> threadContainer;
	std::mutex cityContainerMutex, locationContainerMutex, positionMutex;

	void GenerateMap();
	void GenerateCities(int cityNumber); // Function that generates cities.
	void GenerateLocations(int locationNumber); // Function that generate locations.

	// Thread functions.
	void GenerateCity();
	void GenerateLocation();

protected:
	TheGameManager();
	~TheGameManager(){ _instance = nullptr; }
	static TheGameManager* _instance;

public:
	static TheGameManager& getInstance();
	virtual void Render() override;
	virtual void Update(float dt) override;
};
