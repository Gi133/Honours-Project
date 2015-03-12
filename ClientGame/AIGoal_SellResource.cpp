#include "stdafx.h"
#include "AIGoal_SellResource.h"

AIGoal_SellResource::AIGoal_SellResource(std::weak_ptr<NPC> _owner, const std::string _resourceName, const unsigned int _quantity) :AIGoal(_owner, GoalSellResource)
{
	resourceName = _resourceName;
	quantity = _quantity;
}

AIGoal_SellResource::~AIGoal_SellResource()
{
}

void AIGoal_SellResource::Activate()
{
	goalStatus = ACTIVE;
}

int AIGoal_SellResource::Process()
{
	ActivateInactive(); // Activate if inactive.

	if (tick)
	{
		// Check if NPC has resource quantity.
		if (owner.lock()->GetInventory().lock()->GetTotalResourceAmount(resourceName) >= quantity)
		{
			// Get resource price at location.
			auto resourcePrice = theResourceManager.GetResourceSellingPriceAtCity(owner.lock()->GetCurrentCityPtr(), resourceName);

			// Subtract quantity from NPC.
			owner.lock()->GetInventory().lock()->AdjustResource(resourceName, -quantity);

			// Add Resources to city.
			owner.lock()->GetCurrentCityPtr().lock()->GetInventory().lock()->AdjustResource(resourceName, quantity);

			// Add money to NPC.
			owner.lock()->GetInventory().lock()->AddGold(quantity);


			goalStatus = COMPLETED;
		}
		else
			goalStatus = FAILED;

		tick = false;
	}

	return goalStatus;
}

void AIGoal_SellResource::Terminate()
{
	goalStatus = COMPLETED;
}

std::string AIGoal_SellResource::GetGoalString()
{
	return "Selling: " + IntToString(quantity) + " x " + theResourceManager.GetResourceName(resourceName);
}

std::string AIGoal_SellResource::GetGoalProgressString()
{
	return "";
}

void AIGoal_SellResource::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == tickMessageName)
		tick = true;
}