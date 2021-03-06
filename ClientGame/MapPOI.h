// Base class for all the map entities.
#pragma once

class MapPOI
{
protected:
	std::unique_ptr<HUDActor> iconActor;
	std::unique_ptr<TextActor> nameActor;

	std::string name;

	Vector2 mapCoordinates;
	virtual void LoadDefinitions() = 0;

public:
	MapPOI();
	~MapPOI();

	void SetActorPosition(Vec2i positionCenter);

	void SetPosition(Vector2 newPosition) { mapCoordinates = newPosition; }
	void SetPosition(int posX, int posY) { mapCoordinates.X = posX; mapCoordinates.Y = posY; }
	Vector2 GetPosition() { return mapCoordinates; }

	void SetName(std::string newName) { name = newName; }
	std::string GetName() { return name; }
};
