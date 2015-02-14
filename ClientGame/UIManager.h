#pragma once
#include "stdafx.h"
#include "UIWorldInfo.h"

class UIManager
{
private:
	std::unique_ptr<UIWorldInfo> windowWorldInfo;

	Vector2 windowWorldInfoSize;

	std::string windowWorldInfoAnchor, windowWorldInfoColor;

	void LoadConfig();

public:
	UIManager();
	~UIManager();

	void Update();
};