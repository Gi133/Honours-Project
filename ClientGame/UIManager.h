#pragma once
#include "stdafx.h"
#include "UIWorldInfo.h"
#include "TimeManager.h"

class UIManager
{
private:
	std::unique_ptr<UIWorldInfo> windowWorldInfo;
	std::weak_ptr<TimeManager> timeManager;

	Vector2 windowWorldInfoSize;

	std::string windowWorldInfoAnchor, windowWorldInfoColor;

	void LoadConfig();

public:
	UIManager();
	~UIManager();

	void Update();

	void AssignTimeManager(std::weak_ptr<TimeManager> _timerManager){ timeManager = _timerManager; }
};