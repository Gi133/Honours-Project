#include "stdafx.h"
#include "AIGoal_MoveToCity.h"

AIGoal_MoveToCity::AIGoal_MoveToCity(std::weak_ptr<NPC> _owner, std::weak_ptr<City> _destination) :AIGoal(_owner, GoalMoveToCity)
{
	destination = _destination;

	distance = progress = 0.0f;

	LoadDefaults();
	SetupMessageSubscription();
}

AIGoal_MoveToCity::~AIGoal_MoveToCity()
{
}

void AIGoal_MoveToCity::Activate()
{
	goalStatus = ACTIVE;

	// Calculate distance.
	distance = theMap.GetDistanceBetween(owner.lock()->GetCurrentCityPtr(), destination);

	// Fail check
	if (distance < 0)
		goalStatus = FAILED;
}

int AIGoal_MoveToCity::Process()
{
	ReactivateInactive(); // Activate if inactive.

	// Check if tick has passed, if so then add to progress and flip tick to false.
	if (tick)
	{
		progress += owner.lock()->GetMoveSpeed();
		tick = false;
	}

	if (progress >= distance)
	{
		// What are you doing here? You should be completed!
		owner.lock()->SetCity(destination);
		goalStatus = COMPLETED;
	}

	return goalStatus;
}

void AIGoal_MoveToCity::Terminate()
{
	goalStatus = COMPLETED;
}

unsigned int AIGoal_MoveToCity::ConvertProgresstoPercentage()
{
	return (progress / distance * 100.0f);
}

void AIGoal_MoveToCity::LoadDefaults()
{
}

void AIGoal_MoveToCity::SetupMessageSubscription()
{
}

void AIGoal_MoveToCity::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == tickMessageName)
		tick = true;
}

std::string AIGoal_MoveToCity::GetGoalString()
{
	return "Moving to " + destination.lock()->GetName();
}

std::string AIGoal_MoveToCity::GetGoalProgressString()
{
	return IntToString(ConvertProgresstoPercentage()) + "%";
}

unsigned int AIGoal_MoveToCity::GetGoalProgress()
{
	return ConvertProgresstoPercentage();
}
