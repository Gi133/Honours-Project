#include "stdafx.h"
#include "AIEvaluator_Trade.h"

AIEvaluator_Trade::AIEvaluator_Trade(const float _bias) :AIGoalEvaluator(_bias)
{
	LoadDefaults();
}

AIEvaluator_Trade::~AIEvaluator_Trade()
{
}

void AIEvaluator_Trade::LoadDefaults()
{
	// Add any settings that should be loaded here.
}

float AIEvaluator_Trade::CalculateDesirability(std::weak_ptr<NPC> _owner)
{
	// Calculate the desirability value.
	auto desirability = 0.0f;

	auto npcInventory = _owner.lock()->GetInventory();

	// Check if you can upgrade bags.
	if (npcInventory.lock()->GetBagLimit() > npcInventory.lock()->GetBagNumber())
	{
		// Get the price for the next bag upgrade.
		auto priceBagUpgrade = _owner.lock()->GetInventory().lock()->GetBagUpgradeCost();

		// Set desirability to trade.
		desirability = 0.8f;
	}
	else
	{
		// Set desirability to some number, change this if more features are added for the npc.
		desirability = 0.5f;
	}

	return (desirability*bias);
}

void AIEvaluator_Trade::SetGoal(std::weak_ptr<NPC> _owner)
{
	_owner.lock()->QueueTrade();
}