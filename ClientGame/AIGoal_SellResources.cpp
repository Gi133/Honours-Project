#include "stdafx.h"
#include "AIGoal_SellResources.h"

AIGoal_SellResources::AIGoal_SellResources(std::weak_ptr<NPC> _owner, const std::string _resourceName,
	const unsigned int _quantity) :AIGoal(_owner, GoalSellResource)
{
	resourceName = _resourceName;
	resourceIterator = theResourceManager.GetResourceIterator(_resourceName);
	quantity = _quantity;
}

AIGoal_SellResources::~AIGoal_SellResources()
{
}

void AIGoal_SellResources::Activate()
{
	goalStatus = ACTIVE;
}

int AIGoal_SellResources::Process()
{
	ReactivateInactive(); // Activate if inactive.

// 	if (owner.lock()->GetCityName() != targetCityPtr.lock()->GetName())
// 	{
// 		// Add move goal to owner.
// 		owner.lock()->AddMoveToCity(targetCityPtr);
// 	}
// 	else
	if (tick)
	{
		// Get resource price at location.
		auto resourcePrice = theResourceManager.GetResourceSellingPriceAtCity(owner.lock()->GetCurrentCityPtr(), resourceName);

		// Subtract quantity from NPC.
		owner.lock()->GetInventory().lock()->AdjustResource(resourceName, -quantity);

		// Add Resources to city.
		owner.lock()->GetCurrentCityPtr().lock()->GetInventory().lock()->AdjustResource(resourceName, quantity);

		// Add money to NPC.
		owner.lock()->GetInventory().lock()->AdjustGold(quantity*resourcePrice);

		goalStatus = COMPLETED;

		tick = false;
	}

	return goalStatus;
}

void AIGoal_SellResources::Terminate()
{
	goalStatus = COMPLETED;
}

std::string AIGoal_SellResources::GetGoalString()
{
	return "Selling " + IntToString(quantity) + " units of " + theResourceManager.GetResourceName(resourceName);
}

std::string AIGoal_SellResources::GetGoalProgressString()
{
	return "";
}

void AIGoal_SellResources::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == tickMessageName)
		tick = true;
}