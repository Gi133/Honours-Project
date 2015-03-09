#pragma once
#include "UIWindow.h"

class UIMap :
	public UIWindow
{
private:
	void LoadMapSettings();

	Vector2 mapSize, mapSizeRatio;

public:
	UIMap();
	~UIMap();

	void CalculateMapRatio();
	void SetupMapActors(std::reference_wrapper<std::vector<std::shared_ptr<City>>> refCityContainer, std::reference_wrapper<std::vector<std::shared_ptr<Location>>> refLocationContainer);
};
