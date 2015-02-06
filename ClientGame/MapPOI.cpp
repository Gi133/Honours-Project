#include "stdafx.h"
#include "MapPOI.h"

MapPOI::MapPOI()
{
	iconActor.reset(new Actor());
	nameActor.reset(new TextActor());

	name = "NULL";

	mapCoordinates = Vector2(0.0f, 0.0f);
}

MapPOI::~MapPOI()
{
}