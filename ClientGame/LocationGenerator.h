#pragma once
#include "stdafx.h"
class LocationGenerator
{
private:
	Vec2i mapSize, mapHalfSize;
	int maxCityAttempts, maxLocationAttempts;
	float cityRadius, locationRadius;

	std::mutex randomGenMutex;

	void GetMapSize();
	void GetPreferenceData();
	Vector2 RandomPosInMap();

public:
	LocationGenerator();
	~LocationGenerator();

	// Generate the Position on the map.
	// Params:
	//	* Mode : 1 City, 2 Dungeon
	//	* refCityContainer : Reference to the City Container.
	//	* refLocationContainer : Reference to the Location Container.
	Vector2 GeneratePosition(int mode, std::vector<std::shared_ptr<City>> &refCityContainer, std::vector<std::shared_ptr<Location>> &refLocationContainer);
};
