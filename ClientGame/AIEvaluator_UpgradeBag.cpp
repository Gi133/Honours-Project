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
	auto inventoryPtr = _owner.lock()->GetInventory();

	// If there are still upgrades to be done.
	if (inventoryPtr.lock()->GetBagUpgradeAvailable())
	{
		if (static_cast<unsigned int>(inventoryPtr.lock()->GetGold()) >= inventoryPtr.lock()->GetBagUpgradeCost()) 
		{
			float remainingMoney = inventoryPtr.lock()->GetGold() - inventoryPtr.lock()->GetBagUpgradeCost();

			desirability = remainingMoney / inventoryPtr.lock()->GetBagUpgradeCost();
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
