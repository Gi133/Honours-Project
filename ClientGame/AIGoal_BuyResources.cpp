#include "stdafx.h"
#include "AIGoal_BuyResources.h"

AIGoal_BuyResources::AIGoal_BuyResources(std::weak_ptr<NPC> _owner, const std::string _resourceName, const unsigned int _quantity) :
	AIGoal(_owner, GoalBuyResource)
{
	resourceName = _resourceName;
	resourceIterator = theResourceManager.GetResourceIterator(_resourceName);
	quantity = _quantity;
}

AIGoal_BuyResources::~AIGoal_BuyResources()
{
}

void AIGoal_BuyResources::Activate()
{
	goalStatus = ACTIVE;
}

int AIGoal_BuyResources::Process()
{
	ReactivateInactive(); // Activate if inactive.

	if (tick)
	{
		// Check if NPC has enough space.
		if (owner.lock()->GetInventory().lock()->GetAvailableSpace() >= quantity)
		{
			// Get resource price at location.
			auto resourcePrice = theResourceManager.GetResourceSellingPriceAtCity(owner.lock()->GetCurrentCityPtr(), resourceName);

			// Add quantity to NPC.
			owner.lock()->GetInventory().lock()->AdjustResource(resourceName, quantity);

			// Subtract Resources from city.
			owner.lock()->GetCurrentCityPtr().lock()->GetInventory().lock()->AdjustResource(resourceName, -quantity);

			// Subtract money to NPC.
			owner.lock()->GetInventory().lock()->AdjustGold(-quantity*resourcePrice);

			goalStatus = COMPLETED;
		}
		else
			goalStatus = ACTIVE;

		tick = false;
	}

	return goalStatus;
}

void AIGoal_BuyResources::Terminate()
{
	goalStatus = COMPLETED;
}

std::string AIGoal_BuyResources::GetGoalString()
{
	return "Buying " + IntToString(quantity) + " units of " + theResourceManager.GetResourceName(resourceName);
}

std::string AIGoal_BuyResources::GetGoalProgressString()
{
	return "";
}

void AIGoal_BuyResources::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == tickMessageName)
		tick = true;
}