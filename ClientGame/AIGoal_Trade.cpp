#include "stdafx.h"
#include "AIGoal_Trade.h"
#include "AIGoal_BuyResources.h"
#include "AIGoal_SellResources.h"

AIGoal_Trade::AIGoal_Trade(std::weak_ptr<NPC> _owner) :AIGoalComposite(_owner, GoalTrade)
{
	resourceName = cityNameFinish = cityNameStart = "";
	quantity = 0;
	totalProgress = 0;
}

AIGoal_Trade::~AIGoal_Trade()
{
}

int AIGoal_Trade::Process()
{
	ReactivateInactive();

	goalStatus = ProcessSubgoals();

	ReactivateFailed();

	return goalStatus;
}

void AIGoal_Trade::Activate()
{
	// Clear the subgoal list.
	RemoveAllSubgoals();

	// Pick a trade.
	goalStatus = FindTrade();
}

void AIGoal_Trade::Terminate()
{
	goalStatus = COMPLETED;
}

std::string AIGoal_Trade::GetGoalString()
{
	return "Trading " + IntToString(quantity) + " units of " + resourceName + "\n" + cityNameStart + " -> " +
		cityNameFinish + "\n" + subgoals.front()->GetGoalString() + "(" +
		subgoals.front()->GetGoalProgressString() + ")";
}

std::string AIGoal_Trade::GetGoalProgressString()
{
	return "Total: " + IntToString(ConvertProgressToPercentage()) + "%\nSubgoal: "
		+ subgoals.front()->GetGoalProgressString();
}

int AIGoal_Trade::FindTrade()
{
	unsigned int quantity = 0;
	std::vector<ResourceManager::TradingStruct> tradeRoutes;

	// Get trade routes from current city.
	tradeRoutes = theResourceManager.GetTradeRoutesFromCity(owner.lock()->GetCurrentCityPtr(), owner.lock()->GetBiasProfitability(), owner.lock()->GetBiasPatience());

	if (!tradeRoutes.empty())
	{
		// Pick random trade route and queue trade.
		auto trade = tradeRoutes.at(MathUtil::RandomIntInRange(0, tradeRoutes.size()));

		// Calculate quantity.
		unsigned int maxTradeGold = owner.lock()->GetInventory().lock()->GetGold() / theResourceManager.GetResourceSellingPriceAtCity(trade.start, trade.resource);
		unsigned int maxTradeInventory = owner.lock()->GetInventory().lock()->GetAvailableSpace();

		if (maxTradeGold > maxTradeInventory)
			quantity = maxTradeInventory;
		else
			quantity = maxTradeGold;

		QueueTrade(trade.start, trade.finish, trade.resource, quantity);

		return ACTIVE;
	}
	else
	{
		// Could not find trade, populate neighbors, queue a move goal to a random neighbor
		PopulateNeighbors();

		for (auto city : neighbors)
		{
			auto newTradeRoutes = theResourceManager.GetTradeRoutesFromCity(city, owner.lock()->GetBiasProfitability(), owner.lock()->GetBiasPatience());
			tradeRoutes.reserve(tradeRoutes.size() + newTradeRoutes.size());
			std::move(std::begin(newTradeRoutes), std::end(newTradeRoutes), std::back_inserter(tradeRoutes));
		}

		if (tradeRoutes.empty())
			return FAILED;
		else
		{
			// Pick random trade route and queue trade.
			auto trade = tradeRoutes.at(MathUtil::RandomIntInRange(0, tradeRoutes.size()));

			// Calculate quantity.
			unsigned int maxTradeGold = owner.lock()->GetInventory().lock()->GetGold() / theResourceManager.GetResourceSellingPriceAtCity(trade.start, trade.resource);
			unsigned int maxTradeInventory = owner.lock()->GetInventory().lock()->GetAvailableSpace();

			if (maxTradeGold > maxTradeInventory)
				quantity = maxTradeInventory;
			else
				quantity = maxTradeGold;

			QueueTrade(trade.start, trade.finish, trade.resource, quantity);

			return ACTIVE;
		}
	}
}

void AIGoal_Trade::QueueTrade(std::weak_ptr<City> _start, std::weak_ptr<City> _finish, std::string _resourceName, unsigned int _quantity, const bool moveBack /* = true */)
{
	std::unique_ptr<AIGoal_MoveToCity> goalMove;
	std::unique_ptr<AIGoal_BuyResources> goalBuy;
	std::unique_ptr<AIGoal_SellResources> goalSell;

	// Save variables.
	resourceName = _resourceName;
	quantity = _quantity;
	cityNameStart = _start.lock()->GetName();
	cityNameFinish = _finish.lock()->GetName();

	if (owner.lock()->GetCityName() != _start.lock()->GetName()) // If start city isn't the current city, queue move to start.
	{
		goalMove.reset(new AIGoal_MoveToCity(owner, _start));
		QueueSubgoal(std::move(goalMove));
		totalProgress++;
	}

	// Queue resource buy.
	goalBuy.reset(new AIGoal_BuyResources(owner, resourceName, quantity));
	QueueSubgoal(std::move(goalBuy));
	totalProgress++;

	// Queue move to finish.
	goalMove.reset(new AIGoal_MoveToCity(owner, _finish));
	QueueSubgoal(std::move(goalMove));
	totalProgress++;

	// Queue Sell Resources.
	goalSell.reset(new AIGoal_SellResources(owner, resourceName, quantity));
	QueueSubgoal(std::move(goalSell));
	totalProgress++;

	if (moveBack)
	{
		goalMove.reset(new AIGoal_MoveToCity(owner, _start));
		QueueSubgoal(std::move(goalMove));
		totalProgress++;

		// Potentially include code to have the AI trade something on the way back also.
	}
}

void AIGoal_Trade::PopulateNeighbors()
{
	theMap.GetSortedNeighbors(neighbors, owner.lock()->GetCurrentCityPtr(), owner.lock()->GetBiasPatience());
}

int AIGoal_Trade::ConvertProgressToPercentage()
{
	return (totalProgress - subgoals.size()) / totalProgress * 100;
}