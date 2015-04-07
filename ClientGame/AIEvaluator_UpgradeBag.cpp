#include "stdafx.h"
#include "AIEvaluator_UpgradeBag.h"

AIEvaluator_UpgradeBag::AIEvaluator_UpgradeBag(const float _bias) :AIGoalEvaluator(_bias)
{
}


AIEvaluator_UpgradeBag::~AIEvaluator_UpgradeBag()
{
}

float AIEvaluator_UpgradeBag::CalculateDesirability(std::weak_ptr<NPC> _owner)
{
	auto desirability = 0.0f;

	// If there are still upgrades to be done.
	if (_owner.lock()->GetInventory().lock()->GetBagUpgradeAvailable())
	{
		if (static_cast<unsigned int>(_owner.lock()->GetInventory().lock()->GetGold()) >= _owner.lock()->GetInventory().lock()->GetBagUpgradeCost())
		{
			float upgradeCost = _owner.lock()->GetInventory().lock()->GetBagUpgradeCost();
			float remainingMoney = _owner.lock()->GetInventory().lock()->GetGold() - upgradeCost;
			
			desirability = remainingMoney / upgradeCost;
		}
	}

	return (desirability*bias);
}

void AIEvaluator_UpgradeBag::SetGoal(std::weak_ptr<NPC> _owner)
{
	_owner.lock()->AddGoal_UpgradeBag();
}

void AIEvaluator_UpgradeBag::LoadDefaults()
{

}
