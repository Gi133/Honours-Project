#include "stdafx.h"
#include "UIMap.h"

namespace
{
	const auto mapSizeXFallBack = 100;
	const auto mapSizeYFallBack = 100;
}

UIMap::UIMap()
{
	LoadMapSettings();

	SetLayer(thePrefs.GetString("Settings", "UIMapLayer"));
}

UIMap::~UIMap()
{
}

void UIMap::LoadMapSettings()
{
	auto mapSizeX = thePrefs.GetInt("LocationGeneratorSettings", "mapSizeX");
	if (!mapSizeX)
		mapSizeX = mapSizeXFallBack;

	auto mapSizeY = thePrefs.GetInt("LocationGeneratorSettings", "mapSizeY");
	if (!mapSizeY)
		mapSizeY = mapSizeYFallBack;

	mapSize = Vec2i(mapSizeX, mapSizeY);
}

void UIMap::SetupMapActors(std::reference_wrapper<std::vector<std::shared_ptr<City>>> refCityContainer, std::reference_wrapper<std::vector<std::shared_ptr<Location>>> refLocationContainer)
{
	CalculateMapRatio();
	Vector2 mapUICoords;

	for (std::weak_ptr<City> city : refCityContainer.get())
	{
		// Take the map coordinates and multiply them with the ratio.
		// This will give us a new Vector denoting the city position from the middle of the UI map.
		mapUICoords.X = city.lock()->GetPosition().X * mapSizeRatio.X;
		mapUICoords.Y = city.lock()->GetPosition().Y * mapSizeRatio.Y;

		// The coordinates are not finished yet, what we got is the coordinates from center of the map, but we need to first pass in the map center.
		Vec2i locationPositionUI;
		locationPositionUI.X = mapUICoords.X + GetCenterPosition().X;
		locationPositionUI.Y = mapUICoords.Y + GetCenterPosition().Y;
		city.lock()->SetActorPosition(locationPositionUI);
	}

	for (std::weak_ptr<Location> location : refLocationContainer.get())
	{
		// Take the map coordinates and multiply them with the ratio.
		// This will give us a new Vector denoting the city position from the middle of the UI map.
		mapUICoords.X = location.lock()->GetPosition().X * mapSizeRatio.X;
		mapUICoords.Y = location.lock()->GetPosition().Y * mapSizeRatio.Y;

		// The coordinates are not finished yet, what we got is the coordinates from center of the map, but we need to first pass in the map center.
		Vec2i locationPositionUI;
		locationPositionUI.X = mapUICoords.X + GetCenterPosition().X;
		locationPositionUI.Y = mapUICoords.Y + GetCenterPosition().Y;
		location.lock()->SetActorPosition(locationPositionUI);
	}
}

void UIMap::CalculateMapRatio()
{
	// Calculate the ratio between map units and UI pixel units.
	mapSizeRatio.X = GetBackgroundSize().X / mapSize.X;
	mapSizeRatio.Y = GetBackgroundSize().Y / mapSize.Y;

	thePrefs.SetFloat("LocationGeneratorSettings", "MapRatioX", mapSizeRatio.X);
	thePrefs.SetFloat("LocationGeneratorSettings", "MapRatioY", mapSizeRatio.Y);
}