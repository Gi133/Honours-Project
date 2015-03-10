#include "stdafx.h"
#include "NPC.h"

namespace
{
	const auto startingGoldNumberFallBack = 1000;
	const auto startingBagNumberFallBack = 1;
	const auto baseMoveSpeedFallBack = 1.0f;
}

NPC::NPC()
{
	baseMoveSpeed = 0;
	calculatedMoveSpeed = 0;
	aiControlled = true;

	LoadDefaults();
	GenerateName();
	GenerateStartingCity();

	CalculateMoveSpeed();
}

NPC::~NPC()
{
}

void NPC::Update()
{
	// Process AI brain.
	aiBrain->Process();
}

void NPC::GenerateName()
{
	name = theNameManager.GenerateName(1, true) + " " + theNameManager.GenerateName(2, true);
}

void NPC::GenerateStartingCity()
{
	// Ugly as all hell, but it will randomly generate a starting location by picking a random index number.
	currentCity = theMap.GetRandomCity();
}

void NPC::SetCity(const std::weak_ptr<City> destination)
{
	currentCity = destination;
}

void NPC::SetStartingCity(const std::string cityName)
{
	for (auto i : theMap.GetCityContainerRef().get())
		if (std::regex_search(i->GetName(), std::regex(cityName, std::regex_constants::ECMAScript | std::regex_constants::icase)))
		{
			currentCity = i;
			break;
		}
}

void NPC::LoadDefaults()
{
	baseMoveSpeed = thePrefs.GetFloat("NPCSettings", "baseMoveSpeed");
	if (!baseMoveSpeed)
		baseMoveSpeed = baseMoveSpeedFallBack;

	auto startingGoldNumber = thePrefs.GetInt("NPCSettings", "startingGoldNumber");
	if (!startingGoldNumber)
		startingGoldNumber = startingGoldNumberFallBack;

	auto startingBagNumber = thePrefs.GetInt("NPCSettings", "startingBagNumber");
	if (!startingBagNumber)
		startingBagNumber = startingBagNumberFallBack;

	// Initialise Inventory and Gold
	InitializeInventory(startingBagNumber, startingGoldNumber);
}

void NPC::InitializeInventory(const int startingBagNumber, const int startingGoldNumber)
{
	inventory.reset(new Inventory(false, startingBagNumber, startingGoldNumber));
}

void NPC::CalculateMoveSpeed()
{
	// Change this if required.
	calculatedMoveSpeed = baseMoveSpeed;
}

float NPC::GetMoveSpeed()const
{
	return calculatedMoveSpeed;
}

void NPC::SetupBrain()
{
	aiBrain.reset(new AIGoal_Think(shared_from_this()));
}

void NPC::QueueRandomMoveToCity()
{
	aiBrain->Queue_MoveToCity(theMap.GetRandomCity());
}

std::string NPC::GetCurrentGoal() const
{
	return aiBrain->GetGoalString();
}

std::string NPC::GetCurrentGoalProgress() const
{
	return aiBrain->GetGoalProgressString();
}
