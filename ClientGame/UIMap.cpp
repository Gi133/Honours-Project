#include "stdafx.h"
#include "UIMap.h"

namespace
{
	// Fall back value namespace.
	const float widthFallBack = 0.80f;
	const float heightFallBack = 0.85f;

	// Font Color
	const Color textColour = Color(0.0f, 0.0f, 0.0f, 1.0f);
}

UIMap::UIMap()
{
	// Setup the basics.
	Setup();
}

UIMap::~UIMap()
{
}

void UIMap::Setup()
{
	// Get the window size.
	int windowHeight = theCamera.GetWindowHeight();
	int windowWidth = theCamera.GetWindowWidth();

	// Get settings from file.
	float settingsHeight = thePrefs.GetFloat("UIMapSettings", "height");
	float settingsWidth = thePrefs.GetFloat("UIMapSettings", "width");

	// Check Preferences for size, or use fallback values if those do not exist.
	if (settingsHeight)
		height = windowHeight * settingsHeight;
	else
		height = windowHeight * heightFallBack;

	if (settingsWidth)
		width = windowWidth * settingsWidth;
	else
		width = windowWidth * widthFallBack;

	// Calculate the location of the center point.
	centerPoint.X = width / 2.0f;
	centerPoint.Y = height / 2.0f;

	// Setup the background HUD actor.
	backgroundUI->SetName("uiMapBackground");
	backgroundUI->SetSize(width, height);
	backgroundUI->SetPosition(theCamera.GetWindowWidth() - centerPoint.X, theCamera.GetWindowHeight() - centerPoint.Y);
	backgroundUI->SetColor(0.0f, 0.8f, 0.0f, 1.0f);
	theWorld.Add(backgroundUI.get(), "UILayer");

	thePrefs.SetFloat("Map", "width", width);
	thePrefs.SetFloat("Map", "height", height);

	UpdateMinMax();
}

void UIMap::SetupTextActors()
{
	// Setup the various text actors.

}

void UIMap::Update()
{
}