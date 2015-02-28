#include "stdafx.h"
#include "NPC.h"

namespace
{
	const auto startingGoldNumberFallBack = 1000;
	const auto startingBagNumberFallBack = 1;
}

NPC::NPC()
{
	LoadDefaults();
	GenerateName();
	GenerateStartingCity();
}

NPC::~NPC()
{
}

void NPC::GenerateName()
{
	name = theNameManager.GenerateName(1, true) + " " + theNameManager.GenerateName(2, true);
}

void NPC::GenerateStartingCity()
{
	// Ugly as all hell, but it will randomly generate a starting location by picking a random index number.
	currentCity = theMap.GetCityContainerRef().get().at(MathUtil::RandomIntInRange(0, theMap.GetCityContainerRef().get().size()));
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