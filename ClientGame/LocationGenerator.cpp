#include "stdafx.h"
#include "LocationGenerator.h"

namespace
{
	const int maxCityAttemptsFallBack = 10; 
	const int maxLocationAttemptsFallBack = 10;
	const float cityRadiusFallBack = 50.0f;
	const float locationRadiusFallBack = 50.0f;
}

LocationGenerator::LocationGenerator()
{
	GetMapSize();
	GetPreferenceData();
}


LocationGenerator::~LocationGenerator()
{
}

void LocationGenerator::GetMapSize()
{
	// Get Map Size.
	mapSize = Vector2(thePrefs.GetFloat("Map", "width"), thePrefs.GetFloat("Map", "height"));
	mapHalfSize = mapSize / 2;
}

void LocationGenerator::GetPreferenceData()
{
	// Grab max attempt number for cities.
	maxCityAttempts = thePrefs.GetInt("CityGeneratorSettings", "maxAttempts");
	if (!maxCityAttempts) // If 0 then use fallback value.
		maxCityAttempts = maxCityAttemptsFallBack;

	// Grab city radius
	auto cityRadius = thePrefs.GetFloat("CityGeneratorSettings", "radius");
	if (!cityRadius) // If 0 then use fallback value.
		cityRadius = cityRadiusFallBack;

	// Grab max attempt number.
	maxLocationAttempts = thePrefs.GetInt("LocationGeneratorSettings", "maxAttempts");
	if (!maxLocationAttempts) // If 0 then use fallback value.
		maxLocationAttempts = maxLocationAttemptsFallBack;

	// Grab city radius
	locationRadius = thePrefs.GetFloat("LocationGeneratorSettings", "radius");
	if (!locationRadius) // If 0 then use fallback value.
		locationRadius = locationRadiusFallBack;
}

Vector2 LocationGenerator::RandomPosInMap()
{
	// Generate location
	// Positions are generated by offsetting from the middle of the map as to avoid corner clutter.
	return Vector2(static_cast<int>(MathUtil::RandomIntInRange(-mapHalfSize.X, mapHalfSize.X)),
		static_cast<int>(MathUtil::RandomIntInRange(-mapHalfSize.Y, mapHalfSize.Y)));
}

Vector2 LocationGenerator::GeneratePosition(int mode, std::vector<std::unique_ptr<City>>& refCityContainer, std::vector<std::unique_ptr<Location>>& refLocationContainer)
{
	Vector2 position = Vector2(0.0f, 0.0f);

	bool ruleFail = false; // Boolean value to show if any of the generation rules failed.
	auto maxAttempts = 0;
	auto radius = 0.0f;

	switch (mode)
	{
	case 1:
		maxAttempts = maxCityAttempts;
		radius = cityRadius;
		break;

	case 2:
		maxAttempts = maxLocationAttempts;
		radius = locationRadius;
		break;

	default:
		maxAttempts = maxCityAttempts;
		radius = cityRadius;
		break;
	}

	// Generate city info.
	for (auto i = 0; i < maxAttempts; i++)
	{
		position = RandomPosInMap();

		// Check if location is valid.
		for (std::vector<std::unique_ptr<City>>::iterator i = refCityContainer.begin(); i != refCityContainer.end(); ++i)
		{
			auto distance = sqrt(pow(position.X - i->get()->GetPosition().X, 2) + pow(position.Y - i->get()->GetPosition().Y, 2));
			if (distance < radius)
			{
				ruleFail = true;
				break;
			}
		}

		for (std::vector<std::unique_ptr<Location>>::iterator i = refLocationContainer.begin(); i != refLocationContainer.end(); ++i)
		{
			auto distance = sqrt(pow(position.X - i->get()->GetPosition().X, 2) + pow(position.Y - i->get()->GetPosition().Y, 2));
			if (distance < radius)
			{
				ruleFail = true;
				break;
			}
		}

		// If it is valid then return the position.
		if (!ruleFail)
			break;
		else // Else regenerate position and check again.
			sysLog.Log("Failed to Generate location at position: " + Vector2ToString(position));
	}

	return position;
}