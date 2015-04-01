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
	auto inventoryPtr = _owner.lock()->GetInventory();

	// If there are still upgrades to be done.
	if (inventoryPtr.lock()->GetPurseUpgradeLevel() < inventoryPtr.lock()->GetPurseMaxUpgradeLevel())
	{
		if (static_cast<unsigned int>(inventoryPtr.lock()->GetGold()) >= inventoryPtr.lock()->GetPurseUpgradeCost())
		{
			float remainingMoney = inventoryPtr.lock()->GetGold() - inventoryPtr.lock()->GetPurseUpgradeCost();

			desirability = remainingMoney / inventoryPtr.lock()->GetPurseUpgradeCost();
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
