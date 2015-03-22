#pragma once
#include "LocationGenerator.h"

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

	float GetDistanceBetween(std::weak_ptr<City> cityA, std::weak_ptr<City> cityB);
	float GetDistanceBetween(std::string cityNameA, std::string cityNameB);

	std::weak_ptr<City> GetRandomCity();
	std::weak_ptr<City> CityNameToPtr(std::string cityName);

	std::reference_wrapper<std::vector<std::shared_ptr<City>>> GetCityContainerRef() { return std::ref(cityContainer); }
	std::reference_wrapper<std::vector<std::shared_ptr<Location>>> GetLocationContainerRed() { return std::ref(locationContainer); }

	// Function that takes in the reference to a vector of city pointers, the city in question and a variable.
	// Adds neighbors into the container based on the distance cutoff value passed in, if this value is set to 0
	// then the container will include all cities in the map, else a check will be done and cities who are further away
	// than the value will be ignored.
	// The cities will be sorted based on distance, from closest to furthest.
	// Variables:
	// *	_neighbourContainer : Reference of a Vector of Weak Pointers to Cities, passed in empty, if not empty then it will be emptied out.
	// *	_city : The city whose neighbors will be found and sorted into the container.
	// *	_distanceCutOff : The maximum distance of the neighbors (if value is 0 then the distance is ignored).
	void GetSortedNeighbors(std::reference_wrapper<std::vector<std::weak_ptr<City>>> _neighborContainer, const std::weak_ptr<City> _city, float _distanceCutOff = 0.0f);

	std::weak_ptr<City> GetClosestNeighbor(std::weak_ptr<City> _city, float _maxDistance = 0.0f);
};