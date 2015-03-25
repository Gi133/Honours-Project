#include "stdafx.h"
#include "Location.h"

namespace
{
	const auto minStartThreatFallBack = 0;
	const auto maxStartThreatFallBack = 100;

	const auto maxThreatLowFallBack = 33;
	const auto maxThreatMedFallBack = 66;
	const auto maxThreatHighFallBack = 100;

	const auto maxThreatLowNameFallBack = "Weak";
	const auto maxThreatMedNameFallBack = "Dangerous";
	const auto maxThreatHighNameFallBack = "Deadly";

	const auto colorThreatLowFallBack = "#C68C8C";
	const auto colorThreatMedFallBack = "#851818";
	const auto colorThreatHighFallBack = "#EE2C2C";

	const auto radiusThreatLowFallBack = 10.0f;
	const auto radiusThreatMedFallBack = 20.0f;
	const auto radiusThreatHighFallBack = 30.0f;
}

Location::Location()
{
	threatLevel = maxThreatLow = maxThreatMed = maxThreatHigh = 0;
	threatLevelName = maxThreatLowName = maxThreatMedName = maxThreatHighName = "";

	LoadDefinitions();

	iconActor->SetColor(0.0f, 0.0f, 0.0f);
	iconActor->SetDrawShape(ADS_Square);
}

Location::~Location()
{
}

void Location::LoadDefinitions()
{
	minStartThreat = thePrefs.GetInt("LocationSettings", "minStartThreat");
	if (!minStartThreat)
		minStartThreat = minStartThreatFallBack;

	maxStartThreat = thePrefs.GetInt("LocationSettings", "maxStartThreat");
	if (!maxStartThreat)
		maxStartThreat = maxStartThreatFallBack;

	maxThreatLow = thePrefs.GetInt("LocationSettings", "maxThreatLow");
	if (!maxThreatLow)
		maxThreatLow = maxThreatLowFallBack;

	maxThreatMed = thePrefs.GetInt("LocationSettings", "maxThreatMed");
	if (!maxThreatMed)
		maxThreatMed = maxThreatMedFallBack;

	maxThreatHigh = thePrefs.GetInt("LocationSettings", "maxThreatHigh");
	if (!maxThreatHigh)
		maxThreatHigh = maxThreatHighFallBack;

	maxThreatLowName = thePrefs.GetString("LocationSettings", "maxThreatLowName");
	if (maxThreatLowName.empty())
		maxThreatLowName = maxThreatLowNameFallBack;

	maxThreatMedName = thePrefs.GetString("LocationSettings", "maxThreatMedName");
	if (maxThreatMedName.empty())
		maxThreatMedName = maxThreatMedNameFallBack;

	maxThreatHighName = thePrefs.GetString("LocationSettings", "maxThreatHighName");
	if (maxThreatHighName.empty())
		maxThreatHighName = maxThreatHighNameFallBack;

	colorThreatLow = thePrefs.GetString("LocationSettings", "colorThreatLow");
	if (colorThreatLow.empty())
		colorThreatLow = colorThreatLowFallBack;

	colorThreatMed = thePrefs.GetString("LocationSettings", "colorThreatMed");
	if (colorThreatMed.empty())
		colorThreatMed = colorThreatMedFallBack;

	colorThreatHigh = thePrefs.GetString("LocationSettings", "colorThreatHigh");
	if (colorThreatHigh.empty())
		colorThreatHigh = colorThreatHighFallBack;

	radiusThreatLow = thePrefs.GetFloat("LocationSettings", "radiusThreatLow");
	if (!radiusThreatLow)
		radiusThreatLow = radiusThreatLowFallBack;

	radiusThreatMed = thePrefs.GetFloat("LocationSettings", "radiusThreatMed");
	if (!radiusThreatMed)
		radiusThreatMed = radiusThreatMedFallBack;

	radiusThreatHigh = thePrefs.GetFloat("LocationSettings", "radiusThreatHigh");
	if (!radiusThreatHigh)
		radiusThreatHigh = radiusThreatHighFallBack;
}

void Location::UpdateType()
{
	if (threatLevel <= maxThreatLow)
	{
		threatLevelName = maxThreatLowName;
		iconActor->SetColor(Color::FromHexString(colorThreatLow));
		radius = radiusThreatLow;
	}
	else if (threatLevel <= maxThreatMed)
	{
		threatLevelName = maxThreatMedName;
		iconActor->SetColor(Color::FromHexString(colorThreatMed));
		radius = radiusThreatMed;
	}
	else if (threatLevel <= maxThreatHigh)
	{
		threatLevelName = maxThreatHighName;
		iconActor->SetColor(Color::FromHexString(colorThreatHigh));
		radius = radiusThreatHigh;
	}
	else
		sysLog.Log("Invalid location threat level.");
}

void Location::SetThreatLevel()
{
	// Randomly generate a number between min and max start threat values.
	threatLevel = MathUtil::RandomIntInRange(minStartThreat, maxStartThreat);

	// Update type name accordingly.
	UpdateType();
}