#pragma once
#include "LocationGenerator.h"
#include "City.h"
#include "Location.h"
#include "NameManager.h"

#define theMap Map::getInstance()

class Map
{
private:
	std::unique_ptr<LocationGenerator> locationGenerator;
	std::vector<std::shared_ptr<City>> cityContainer;
	std::vector<std::shared_ptr<Location>> locationContainer;

	void GenerateCities(int cityNumber); // Function that generates cities.
	void GenerateLocations(int locationNumber); // Function that generate locations.

	// Map functions.
	void GenerateCity();
	void GenerateLocation();

protected:
	Map();
	~Map() { _instance = nullptr; }
	static Map* _instance;

public:
	static Map& getInstance();

	void GenerateMap();

	std::reference_wrapper<std::vector<std::shared_ptr<City>>> GetCityContainerRef() { return std::ref(cityContainer); }
	std::reference_wrapper<std::vector<std::shared_ptr<Location>>> GetLocationContainerRed() { return std::ref(locationContainer); }
};