#include "stdafx.h"
#include "UINPCInfo.h"

namespace
{
	// Fallback Values if the settings file is deleted or misplaced.
	const float heightFallBack = 1.0f;
	const float widthFallBack = 0.33f;
	const float contentOffsetXFallBack = 0.1f;
	const float textOffsetYFallBack = 1.2f;
	const Vector2 namePositionFallBack = Vector2(0.1f, 0.1f);
	const Color textColour = Color(0.0f, 0.0f, 0.0f, 1.0f);
}

UINPCInfo::UINPCInfo()
{
	// Set up the basics.
	Setup();

	// Initialize Strings.
	nameString = "UNKNOWN";
	classString = "UNKNOWN";

	// Reset text actor pointers.
	textName.reset(new TextActor());
	textNameContent.reset(new TextActor());
	textClass.reset(new TextActor());
	textClassContent.reset(new TextActor());

	// Setup text actors.
	SetupTextActors();
}

UINPCInfo::~UINPCInfo()
{
}

void UINPCInfo::Setup()
{
	// Get the window size.
	int windowHeight = theCamera.GetWindowHeight();
	int windowWidth = theCamera.GetWindowWidth();

	// Get settings from file.
	float settingsHeight = thePrefs.GetFloat("UINPCInfoSettings", "height");
	float settingsWidth = thePrefs.GetFloat("UINPCInfoSettings", "width");

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
	backgroundUI->SetName("uiNPCInfoBackground");
	backgroundUI->SetSize(width, height);
	backgroundUI->SetPosition(centerPoint);
	backgroundUI->SetColor(0.0f, 0.0f, 0.0f, 0.5f);
	theWorld.Add(backgroundUI.get(), "UILayer");

	UpdateMinMax();
}

void UINPCInfo::Update()
{
	// Update
}

void UINPCInfo::SetupTextActors()
{
	// Variable Declarations
	// offset to move the starting position of the text.
	Vector2 offset = Vector2(width * thePrefs.GetFloat("UINPCInfoSettings", "textStartX"), height * thePrefs.GetFloat("UINPCInfoSettings", "textStartY"));
	float contentOffsetX = thePrefs.GetFloat("UINPCInfoSettings", "contentOffset");
	float textOffsetY = thePrefs.GetFloat("UINPCInfoSettings", "textOffsetY");

	// Check to make sure offsets are non-zero.
	if (!contentOffsetX)
		contentOffsetX = contentOffsetXFallBack;
	if (!textOffsetY)
		textOffsetY = textOffsetYFallBack;

	// Convert the UI text coordinates to world coordinates.
	Vector2 worldCoords = MathUtil::ScreenToWorld(topLeft.X + offset.X, topLeft.Y + offset.Y);

	// Name TextActor
	textName->SetColor(textColour);
	textName->SetDisplayString("NAME: ");
	textName->SetFont("UITitleFont");
	textName->SetPosition(worldCoords);
	theWorld.Add(textName.get(), "UITextLayer");

	// Name Content TextActor
	textNameContent->SetColor(textColour);
	textNameContent->SetDisplayString(nameString);
	textNameContent->SetFont("UIContentFont");
	textNameContent->SetPosition(textName->GetBoundingBox().Max.X + contentOffsetX, textName->GetBoundingBox().Min.Y);
	theWorld.Add(textNameContent.get(), "UITextLayer");

	// Class TextActor
	textClass->SetColor(textColour);
	textClass->SetDisplayString("CLASS: ");
	textClass->SetFont("UITitleFont");
	textClass->SetPosition(worldCoords.X, textName->GetBoundingBox().Min.Y - (textOffsetY * 1));
	theWorld.Add(textClass.get());

	// Class Content Text Actor
	textClassContent->SetColor(textColour);
	textClassContent->SetDisplayString(classString);
	textClassContent->SetFont("UIContentFont");
	textClassContent->SetPosition(textClass->GetBoundingBox().Max.X + contentOffsetX, textClass->GetBoundingBox().Min.Y);
	theWorld.Add(textClassContent.get(), "UITextLayer");

	// I AM A CHANGE!!
}