#include "stdafx.h"
#include "AIGoal_PurchaseBag.h"


AIGoal_PurchaseBag::AIGoal_PurchaseBag(std::weak_ptr<NPC> _owner) :AIGoal(_owner, GoalPurchaseBag)
{
}


AIGoal_PurchaseBag::~AIGoal_PurchaseBag()
{
}

void AIGoal_PurchaseBag::Activate()
{
	goalStatus = ACTIVE;
}

int AIGoal_PurchaseBag::Process()
{
	ReactivateInactive();

	if (tick)
	{
		int bagCost = owner.lock()->GetInventory().lock()->GetBagPurchaseCost();
		int ownerGold = owner.lock()->GetInventory().lock()->GetGold();

		if (ownerGold > bagCost)
		{
			// Remove gold from inventory.
			owner.lock()->GetInventory().lock()->AdjustGold(-bagCost);

			// Purchase a new bag.
			owner.lock()->GetInventory().lock()->AddBag(1);
			goalStatus = COMPLETED;
		}
		else
			goalStatus = FAILED;
	}

	return goalStatus;
}

void AIGoal_PurchaseBag::Terminate()
{
	goalStatus = COMPLETED;
}

std::string AIGoal_PurchaseBag::GetGoalString()
{
	return "Purchasing Bag";
}

std::string AIGoal_PurchaseBag::GetGoalProgressString()
{
	return "";
}

void AIGoal_PurchaseBag::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == tickMessageName)
		tick = true;
}