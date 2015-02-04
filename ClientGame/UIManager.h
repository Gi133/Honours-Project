#pragma once

#include "UIMap.h"
#include "UINPCInfo.h"
#include "UIWorldInfo.h"

class UIManager
{
private:
	std::unique_ptr<UIMap> uiMap;
	std::unique_ptr<UINPCInfo> uiNPCInfo;
	std::unique_ptr<UIWorldInfo> uiWorldInfo;

public:
	UIManager();
	~UIManager();

	void Update();
};
