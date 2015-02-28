#include "stdafx.h"
#include "InventoryPurse.h"


InventoryPurse::InventoryPurse()
{
}


InventoryPurse::InventoryPurse(const int startingGold)
{
	gold = startingGold;
}

InventoryPurse::~InventoryPurse()
{
}

void InventoryPurse::UpdateGold()
{
	// Check if Gold is within the gold limits, if not then only add the allowed gold.
	auto remainingGold = goldLimit - gold;

	if (remainingGold < 0)
	{
		gold = goldLimit;
		sysLog.Log("Could not add gold, amount to add exceeded purse limit.");
	}
}