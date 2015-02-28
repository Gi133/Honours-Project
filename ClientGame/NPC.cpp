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
}

NPC::~NPC()
{
}

void NPC::GenerateName()
{
	name = theNameManger.GenerateName(1, true);
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
