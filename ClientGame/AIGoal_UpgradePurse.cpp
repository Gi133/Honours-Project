#include "stdafx.h"
#include "AIGoal_UpgradePurse.h"

AIGoal_UpgradePurse::AIGoal_UpgradePurse(std::weak_ptr<NPC> _owner) :AIGoal(_owner, GoalUpgradePurse)
{
}

AIGoal_UpgradePurse::~AIGoal_UpgradePurse()
{
}

void AIGoal_UpgradePurse::Activate()
{
	goalStatus = ACTIVE;
}

int AIGoal_UpgradePurse::Process()
{
	ReactivateInactive();

	if (tick)
	{
		owner.lock()->GetInventory().lock()->UpgradePurse();
		goalStatus = COMPLETED;
	}

	return goalStatus;
}

void AIGoal_UpgradePurse::Terminate()
{
	goalStatus = COMPLETED;
}

std::string AIGoal_UpgradePurse::GetGoalString()
{
	return "Upgrading purse";
}

std::string AIGoal_UpgradePurse::GetGoalProgressString()
{
	return "";
}

void AIGoal_UpgradePurse::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == tickMessageName)
		tick = true;
}