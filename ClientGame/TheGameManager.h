#pragma once

#include "stdafx.h"
#include "NameManager.h"
#include "UIManager.h"
#include "LocationGenerator.h"
#include "City.h"
#include "Location.h"

#define theGameManger TheGameManager::getInstance()

class TheGameManager :
	public GameManager
{
public:
	static TheGameManager& getInstance();
	virtual void Render() override;
	virtual void Update(float dt) override;

protected:
	TheGameManager();
	static TheGameManager* _instance;

private:
	std::unique_ptr<UIManager> uiManager;
	std::unique_ptr<LocationGenerator> locationGenerator;
	std::vector<std::unique_ptr<City>> cityContainer;
	std::vector<std::unique_ptr<Location>> locationContainer;

	Vector2 mapSize, halfMapSize;

	void GenerateMap();
	void GenerateCities(int cityNumber); // Function that generates cities.
	void GenerateLocations(int locationNumber); // Function that generate locations.
};
