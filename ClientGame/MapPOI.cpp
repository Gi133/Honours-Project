#include "stdafx.h"
#include "MapPOI.h"

MapPOI::MapPOI()
{
	iconActor.reset(new HUDActor());
	nameActor.reset(new TextActor());
	iconActor->SetLayer(4);

	iconActor->SetColor(0.0f, 0.0f, 0.0f);
	iconActor->SetDrawShape(ADS_Circle);
	iconActor->SetSize(10.0f);

	name = "NULL";

	mapCoordinates = Vector2(0.0f, 0.0f);
}

MapPOI::~MapPOI()
{
}

void MapPOI::SetActorPosition(Vec2i positionCenter)
{
	iconActor->SetPosition(positionCenter);

	sysLog.Log("Registering actor at: " + Vector2ToString(positionCenter));
	theWorld.Add(iconActor.get());
}