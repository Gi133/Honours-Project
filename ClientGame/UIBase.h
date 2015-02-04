#pragma once
#include "stdafx.h"
class UIBase
{
public:
	UIBase();
	~UIBase();

	// Update call.
	virtual void Update();

protected:
	std::unique_ptr<HUDActor> backgroundUI;

	// Position of the actor (middle point).
	Vector2 centerPoint;

	// Size of the actor.
	float width, height;

	// Top Left and Bottom Right location
	Vector2 topLeft;
	Vector2 bottomRight;

	void UpdateMinMax(); // Update the 2d coordinates for top left and bottom right vectors.

private:
	virtual void Setup(); // Override in children.
	virtual void SetupTextActors(); // Override in children.
};
