#include "stdafx.h"
#include "UIManager.h"

namespace
{
	const auto windowWorldInfoSizeXFallBack = 180.0f;
	const auto windowWorldInfoSizeYFallBack = 768.0f;
	const auto windowWorldInfoAnchorFallBack = "topRight";
	const auto windowWorldInfoColorFallBack = "0x000000";
}

UIManager::UIManager()
{
	LoadConfig();

	windowWorldInfo.reset(new UIWorldInfo());

	windowWorldInfo->SetWindowSize(windowWorldInfoSize);
	windowWorldInfo->SetWindowAnchor(windowWorldInfoAnchor);
	windowWorldInfo->SetWindowColorHex(windowWorldInfoColor);
}

UIManager::~UIManager()
{
}

void UIManager::Update()
{
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

	// Convert the fetched data and assign it where necessary.
	windowWorldInfoSize = Vector2(windowWorldInfoSizeX, windowWorldInfoSizeY);
}
