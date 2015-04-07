#include "stdafx.h"
#include "AIGoal_BuyResources.h"

AIGoal_BuyResources::AIGoal_BuyResources(std::weak_ptr<NPC> _owner, const std::string _resourceName, 
	const unsigned int _quantity, const unsigned int _expectedPrice) :AIGoal(_owner, GoalBuyResource)
{
	resourceName = _resourceName;
	resourceIterator = theResourceManager.GetResourceIterator(_resourceName);
	quantity = _quantity;
	expectedPrice = _expectedPrice;
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

// 	if (owner.lock()->GetCityName() != targetCityPtr.lock()->GetName())
// 	{
// 		// Add move goal to owner.
// 		owner.lock()->AddMoveToCity(targetCityPtr);
// 	}
// 	else
	if (tick)
	{
		auto availableSpace = owner.lock()->GetInventory().lock()->GetAvailableSpace();

		// Check if NPC has enough space.
		if (availableSpace >= quantity)
		{
			// Get resource price at location.
			auto resourcePrice = theResourceManager.GetResourceSellingPriceAtCity(owner.lock()->GetCurrentCityPtr(), resourceName);

			// Check Prices, if buy price is less or same as expected.
			if (resourcePrice <= expectedPrice)
			{
				// Add quantity to NPC.
				owner.lock()->GetInventory().lock()->AdjustResource(resourceName, quantity);

				// Subtract Resources from city.
				owner.lock()->GetCurrentCityPtr().lock()->GetInventory().lock()->AdjustResource(resourceName, -quantity);

				// Subtract money to NPC.
				owner.lock()->GetInventory().lock()->AdjustGold(-quantity*resourcePrice);

				// Save the price.
				owner.lock()->SetBuyPrice(resourcePrice);

				goalStatus = COMPLETED;
			}
			else
				goalStatus = FAILED;
		}
		else
			goalStatus = FAILED;

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