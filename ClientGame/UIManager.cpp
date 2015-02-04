// UI Manager Class
// This will manage all the UI elements of the application.

#include "stdafx.h"
#include "UIManager.h"

UIManager::UIManager()
{
	uiMap.reset(new UIMap());
	uiNPCInfo.reset(new UINPCInfo());
	uiWorldInfo.reset(new UIWorldInfo());
}

UIManager::~UIManager()
{
}

void UIManager::Update()
{
	// Update function for UI.
	uiNPCInfo->Update();
	uiWorldInfo->Update();
}