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
}

Location::Location()
{
	threatLevel = maxThreatLow = maxThreatMed = maxThreatHigh = 0;
	threatLevelName = maxThreatLowName = maxThreatMedName = maxThreatHighName = "";

	LoadDefinitions();
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
}

void Location::UpdateType()
{
	if (threatLevel <= maxThreatLow)
		threatLevelName = maxThreatLowName;
	else if (threatLevel <= maxThreatMed)
		threatLevelName = maxThreatMedName;
	else if (threatLevel <= maxThreatHigh)
		threatLevelName = maxThreatHighName;
	else
		sysLog.Log("Invalid City population");
}

void Location::SetThreatLevel()
{
	// Randomly generate a number between min and max start threat values.
	threatLevel = MathUtil::RandomIntInRange(minStartThreat, maxStartThreat);

	// Update type name accordingly.
	UpdateType();
}