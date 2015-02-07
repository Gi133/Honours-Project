#pragma once
#include "MapPOI.h"

class Location :
	public MapPOI
{
private:
	std::string threatLevelName, maxThreatLowName, maxThreatMedName, maxThreatHighName;
	int threatLevel, maxThreatLow, maxThreatMed, maxThreatHigh, minStartThreat, maxStartThreat;

	void LoadDefinitions() override;
	void UpdateType();

public:
	Location();
	~Location();

	// Function to set threat level, if no parameter is given then it will randomly generate it.
	void SetThreatLevel();
	void SetThreatLevel(int newThreatLevel){ threatLevel = newThreatLevel; UpdateType(); }

	int getThreatLevel(){ return threatLevel; }
};
