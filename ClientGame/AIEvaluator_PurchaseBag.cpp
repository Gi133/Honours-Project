#include "stdafx.h"
#include "AIEvaluator_PurchaseBag.h"


AIEvaluator_PurchaseBag::AIEvaluator_PurchaseBag(const float _bias) :AIGoalEvaluator(_bias)
{
}


AIEvaluator_PurchaseBag::~AIEvaluator_PurchaseBag()
{
}

float AIEvaluator_PurchaseBag::CalculateDesirability(std::weak_ptr<NPC> _owner)
{
	auto desirability = 0.0f;

	// If there are still bag slots empty.
	if (_owner.lock()->GetInventory().lock()->GetBagNumberUpgradeAvailable())
	{
		if (static_cast<unsigned int>(_owner.lock()->GetInventory().lock()->GetGold()) >= _owner.lock()->GetInventory().lock()->GetBagUpgradeCost())
		{
			float purchaseCost = _owner.lock()->GetInventory().lock()->GetBagPurchaseCost();
			float remainingMoney = _owner.lock()->GetInventory().lock()->GetGold() - purchaseCost;

			desirability = remainingMoney / purchaseCost;
		}
	}

	return (desirability*bias);
}

void AIEvaluator_PurchaseBag::SetGoal(std::weak_ptr<NPC> _owner)
{
	_owner.lock()->AddGoal_PurchaseBag();
}

void AIEvaluator_PurchaseBag::LoadDefaults()
{

}