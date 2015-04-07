#include "stdafx.h"
#include "AIGoal_UpgradeBag.h"

AIGoal_UpgradeBag::AIGoal_UpgradeBag(std::weak_ptr<NPC> _owner) :AIGoal(_owner, GoalUpgradeBag)
{
}

AIGoal_UpgradeBag::~AIGoal_UpgradeBag()
{
}

void AIGoal_UpgradeBag::Activate()
{
	goalStatus = ACTIVE;
}

int AIGoal_UpgradeBag::Process()
{
	ReactivateInactive();

	if (tick)
	{
		int ownerGold = owner.lock()->GetInventory().lock()->GetGold();
		int upgradeCost = owner.lock()->GetInventory().lock()->GetBagUpgradeCost();

		if (ownerGold > upgradeCost)
		{
			// Remove money.
			owner.lock()->GetInventory().lock()->AdjustGold(-upgradeCost);

			// Upgrade bag.
			owner.lock()->GetInventory().lock()->UpgradeBag();
			goalStatus = COMPLETED;
		}
		else
		{
			sysLog.Log("AI ERROR : Could not upgrade bag.");
			goalStatus = FAILED;
		}
	}
		
	return goalStatus;
}

void AIGoal_UpgradeBag::Terminate()
{
	goalStatus = COMPLETED;
}

std::string AIGoal_UpgradeBag::GetGoalString()
{
	return "Upgrading bag";
}

std::string AIGoal_UpgradeBag::GetGoalProgressString()
{
	return "";
}

void AIGoal_UpgradeBag::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == tickMessageName)
		tick = true;
}
