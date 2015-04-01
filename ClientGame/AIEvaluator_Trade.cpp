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
	float goldLimit = npcInventory.lock()->GetGoldLimit();
	float remainingMoneyToCap = goldLimit - npcInventory.lock()->GetGold();

	// Calculate desirability based on the amount of money the NPC has.
	// For not, this desirability is set to a constant.
	desirability = remainingMoneyToCap/goldLimit;

	return (desirability*bias);
}

void AIEvaluator_Trade::SetGoal(std::weak_ptr<NPC> _owner)
{
	_owner.lock()->QueueTrade();
}