#include "stdafx.h"
#include "UIManager.h"

namespace
{
	const auto windowWorldInfoSizeXFallBack = 180.0f;
	const auto windowWorldInfoSizeYFallBack = 768.0f;
	const auto windowWorldInfoAnchorFallBack = "topRight";
	const auto windowWorldInfoColorFallBack = "#7C4712";

	const auto windowMapSizeXFallBack = 800;
	const auto windowMapSizeYFallBack = 800;
	const auto windowMapAnchorFallBack = "bottomRight";
	const auto windowMapColorFallBack = "#33CC33";
	const auto windowMapLayerNameFallBack = "UIMapLayer";

	const auto windowObjectInfoSizeXFallBack = 224;
	const auto windowObjectInfoSizeYFallBack = 768;
	const auto windowObjectInfoAnchorFallBack = "left";
	const auto windowObjectInfoColorFallBack = "#7C4712";
}

UIManager::UIManager()
{
	LoadConfig();

	windowMap.reset(new UIMap());
	windowMap->SetWindowSize(windowMapSize);
	windowMap->SetWindowAnchor(windowMapAnchor);
	windowMap->SetWindowColorHex(windowMapColor);
	windowMap->SetLayer(windowMapLayerName);

	windowWorldInfo.reset(new UIWorldInfo());
	windowWorldInfo->SetWindowSize(windowWorldInfoSize);
	windowWorldInfo->SetWindowAnchor(windowWorldInfoAnchor);
	windowWorldInfo->SetWindowColorHex(windowWorldInfoColor);
	windowWorldInfo->InitializeElements();

	windowObjectInfo.reset(new UIObjectInfo());
	windowObjectInfo->SetWindowSize(windowObjectInfoSize);
	windowObjectInfo->SetWindowAnchor(windowObjectInfoAnchor);
	windowObjectInfo->SetWindowColorHex(windowObjectInfoColor);
}

UIManager::~UIManager()
{
}

void UIManager::Update()
{
	windowWorldInfo->Update();
}

void UIManager::LoadConfig()
{
	auto windowWorldInfoSizeX = thePrefs.GetFloat("UIManagerSettings", "windowWorldInfoSizeX");
	if (!windowWorldInfoSizeX)
		windowWorldInfoSizeX = windowWorldInfoSizeXFallBack;

	auto windowWorldInfoSizeY = thePrefs.GetFloat("UIManagerSettings", "windowWorldInfoSizeY");
	if (!windowWorldInfoSizeY)
		windowWorldInfoSizeY = windowWorldInfoSizeYFallBack;

	windowWorldInfoAnchor = thePrefs.GetString("UIManagerSettings", "windowWorldInfoAnchor");
	if (windowWorldInfoAnchor.empty())
		windowWorldInfoAnchor = windowWorldInfoAnchorFallBack;

	windowWorldInfoColor = thePrefs.GetString("UIManagerSettings", "windowWorldInfoColor");
	if (windowWorldInfoColor.empty())
		windowWorldInfoColor = windowWorldInfoColorFallBack;

	auto windowMapSizeX = thePrefs.GetFloat("UIManagerSettings", "windowMapSizeX");
	if (!windowMapSizeX)
		windowMapSizeX = windowMapSizeXFallBack;

	auto windowMapSizeY = thePrefs.GetFloat("UIManagerSettings", "windowMapSizeY");
	if (!windowMapSizeY)
		windowMapSizeY = windowMapSizeYFallBack;

	windowMapAnchor = thePrefs.GetString("UIManagerSettings", "windowMapAnchor");
	if (windowMapAnchor.empty())
		windowMapAnchor = windowMapAnchorFallBack;

	windowMapColor = thePrefs.GetString("UIManagerSettings", "windowMapColor");
	if (windowMapColor.empty())
		windowMapColor = windowMapColorFallBack;

	windowMapLayerName = thePrefs.GetString("LayerNames", "UIMap");
	if (windowMapLayerName.empty())
		windowMapLayerName = windowMapLayerNameFallBack;

	auto windowObjectInfoSizeX = thePrefs.GetFloat("UIManagerSettings", "windowObjectInfoSizeX");
	if (!windowObjectInfoSizeX)
		windowObjectInfoSizeX = windowObjectInfoSizeXFallBack;

	auto windowObjectInfoSizeY = thePrefs.GetFloat("UIManagerSettings", "windowObjectInfoSizeY");
	if (!windowObjectInfoSizeY)
		windowObjectInfoSizeY = windowObjectInfoSizeYFallBack;

	windowObjectInfoAnchor = thePrefs.GetString("UIManagerSettings", "windowObjectInfoAnchor");
	if (windowObjectInfoAnchor.empty())
		windowObjectInfoAnchor = windowObjectInfoAnchorFallBack;

	windowObjectInfoColor = thePrefs.GetString("UIManagerSettings", "windowObjectInfoColor");
	if (windowObjectInfoColor.empty())
		windowObjectInfoColor = windowObjectInfoColorFallBack;

	// Convert the fetched data and assign it where necessary.
	windowWorldInfoSize = Vector2(windowWorldInfoSizeX, windowWorldInfoSizeY);
	windowMapSize = Vector2(windowMapSizeX, windowMapSizeY);
	windowObjectInfoSize = Vector2(windowObjectInfoSizeX, windowObjectInfoSizeY);
}