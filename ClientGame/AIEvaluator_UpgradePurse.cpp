#include "stdafx.h"
#include "AIEvaluator_UpgradePurse.h"

AIEvaluator_UpgradePurse::AIEvaluator_UpgradePurse(const float _bias) :AIGoalEvaluator(_bias)
{
}

AIEvaluator_UpgradePurse::~AIEvaluator_UpgradePurse()
{
}

float AIEvaluator_UpgradePurse::CalculateDesirability(std::weak_ptr<NPC> _owner)
{
	auto desirability = 0.0f;

	// If there are still upgrades to be done.
	if (_owner.lock()->GetInventory().lock()->GetPurseUpgradeLevel() < _owner.lock()->GetInventory().lock()->GetPurseMaxUpgradeLevel())
	{
		if (static_cast<unsigned int>(_owner.lock()->GetInventory().lock()->GetGold()) >= _owner.lock()->GetInventory().lock()->GetPurseUpgradeCost())
		{
			float upgradeCost = _owner.lock()->GetInventory().lock()->GetPurseUpgradeCost();
			int remainingMoney = _owner.lock()->GetInventory().lock()->GetGold() - upgradeCost;

			desirability = remainingMoney / static_cast<float>(upgradeCost);
		}
	}
	
	return (desirability*bias);
}

void AIEvaluator_UpgradePurse::SetGoal(std::weak_ptr<NPC> _owner)
{
	_owner.lock()->AddGoal_UpgradePurse();
}

void AIEvaluator_UpgradePurse::LoadDefaults()
{
}
