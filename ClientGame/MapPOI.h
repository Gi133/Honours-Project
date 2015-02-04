// Base class for all the map entities.

#pragma once
#include "stdafx.h"

class MapPOI
{
protected:
	std::unique_ptr<Actor> iconActor;
	std::unique_ptr<TextActor> nameActor;

	std::string name;

	Vector2 mapCoordinates;

public:
	MapPOI();
	~MapPOI();
};

