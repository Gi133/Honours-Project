#include "stdafx.h"
#include "UIWorldInfo.h"

namespace
{
	// Fall back value namespace.
	const float widthFallBack = 0.68f;
	const float heightFallBack = 0.30f;
	const float textOffsetXFallBack = 1.2f;

	// Font Color
	const Color textColour = Color(0.0f, 0.0f, 0.0f, 1.0f);
}

UIWorldInfo::UIWorldInfo()
{
	// Setup the basics.
	Setup();

	// Initialize strings.
	seasonString = "UNKNOWN";
	yearString = "UNKNOWN";
	monthString = "UNKNOWN";
	dayString = "UNKNOWN";

	// Reset unique pointers.
	seasonContent.reset(new TextActor());
	yearContent.reset(new TextActor());
	monthContent.reset(new TextActor());
	dayContent.reset(new TextActor());

	// Setup text actors.
	SetupTextActors();
}

UIWorldInfo::~UIWorldInfo()
{
}

void UIWorldInfo::Setup()
{
	// Get the window size.
	int windowHeight = theCamera.GetWindowHeight();
	int windowWidth = theCamera.GetWindowWidth();

	// Get settings from file.
	float settingsHeight = thePrefs.GetFloat("UIWorldInfoSettings", "height");
	float settingsWidth = thePrefs.GetFloat("UIWorldInfoSettings", "width");

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
	centerPoint.X = theCamera.GetWindowWidth() - (width / 2.0f);
	centerPoint.Y = height / 2.0f;

	// Setup the background HUD actor.
	backgroundUI->SetName("uiWorldInfoBackground");
	backgroundUI->SetSize(width, height);
	backgroundUI->SetPosition(centerPoint.X, centerPoint.Y);
	backgroundUI->SetColor(0.0f, 0.0f, 0.0f, 0.5f);
	theWorld.Add(backgroundUI.get(), "UILayer");

	UpdateMinMax();
}

void UIWorldInfo::SetupTextActors()
{
	// Variable Declarations
	// offset to move the starting position of the text.
	Vector2 offset = Vector2(width * thePrefs.GetFloat("UIWorldInfoSettings", "textStartX"), height * thePrefs.GetFloat("UIWorldInfoSettings", "textStartY"));
	float textOffsetX = thePrefs.GetFloat("UIWorldInfoSettings", "textOffsetX");

	// Check to make sure offsets are non-zero.
	if (!textOffsetX)
		textOffsetX = textOffsetXFallBack;

	// Convert the UI text coordinates to world coordinates.
	Vector2 worldCoords = MathUtil::ScreenToWorld(topLeft.X + offset.X, topLeft.Y + offset.Y);

	// Season TextActor
	seasonContent->SetColor(textColour);
	seasonContent->SetDisplayString(seasonString);
	seasonContent->SetFont("UIContentFont");
	seasonContent->SetPosition(worldCoords);
	theWorld.Add(seasonContent.get(), "UITextLayer");

	// Year TextActor
	yearContent->SetColor(textColour);
	yearContent->SetDisplayString(yearString);
	yearContent->SetFont("UIContentFont");
	yearContent->SetPosition(seasonContent->GetBoundingBox().Max.X + textOffsetX, worldCoords.Y);
	theWorld.Add(yearContent.get(), "UITextLayer");

	// Month TextActor
	monthContent->SetColor(textColour);
	monthContent->SetDisplayString(monthString);
	monthContent->SetFont("UIContentFont");
	monthContent->SetPosition(yearContent->GetBoundingBox().Max.X + textOffsetX, worldCoords.Y);
	theWorld.Add(monthContent.get(), "UITextLayer");

	// Year TextActor
	dayContent->SetColor(textColour);
	dayContent->SetDisplayString(dayString);
	dayContent->SetFont("UIContentFont");
	dayContent->SetPosition(monthContent->GetBoundingBox().Max.X + textOffsetX, worldCoords.Y);
	theWorld.Add(dayContent.get(), "UITextLayer");
}

void UIWorldInfo::Update()
{
}