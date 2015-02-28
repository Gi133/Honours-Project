#pragma once
#include "stdafx.h"
#include "UIWorldInfo.h"
#include "UIMap.h"
#include "UIObjectInfo.h"
#include "TimeManager.h"
#include "Map.h"

class UIManager
{
private:
	std::unique_ptr<UIWorldInfo> windowWorldInfo;
	std::unique_ptr<UIMap> windowMap;
	std::unique_ptr<UIObjectInfo> windowObjectInfo;

	std::weak_ptr<NPC> currentNPC;

	Vector2 windowWorldInfoSize, windowMapSize, windowObjectInfoSize;

	std::string windowWorldInfoAnchor, windowWorldInfoColor, windowMapAnchor, windowMapColor,
		windowMapLayerName, windowObjectInfoAnchor, windowObjectInfoColor;

	void LoadConfig();

public:
	UIManager();
	~UIManager();

	void Update();

	void SetCurrentNPC(const std::weak_ptr<NPC> newCurrentNPC) { windowObjectInfo->SetCurrentNPC(newCurrentNPC); }

	void AssignTimeManager(std::weak_ptr<TimeManager> _timeManager) { if (windowWorldInfo.get()) { windowWorldInfo->AssignTimeManager(_timeManager); } }
	void SetupMapActors();
};