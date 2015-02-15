#pragma once
#include "City.h"
#include "Location.h"

// Variable name verbosity for the sake of not having to pass around values too much or needlessly re-reading files.

class LocationGenerator
{
private:
	Vec2i mapSize, mapHalfSize;
	int maxCityAttempts, maxLocationAttempts;
	float cityRadius, locationRadius;

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
	Vector2 GeneratePosition(int mode, std::vector<std::unique_ptr<City>>& refCityContainer, std::vector<std::unique_ptr<Location>>& refLocationContainer);
};
