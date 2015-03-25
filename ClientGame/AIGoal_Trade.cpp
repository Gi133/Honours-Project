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

	// Error checking, if the NPC goals were overridden and the top goal is to buy or sell resources but not at the target city,
	// If that is the case then queue an atomic goal to move it back to where it was supposed to be, else keep processing subgoals.
	if ((GetFrontType() == GoalBuyResource) && (owner.lock()->GetCurrentCityPtr().lock()->GetName() != cityNameStart))
	{
		std::unique_ptr<AIGoal_MoveToCity> moveToBuyGoal;
		moveToBuyGoal.reset(new AIGoal_MoveToCity(owner, theMap.CityNameToPtr(cityNameStart)));
		AddSubgoal(std::move(moveToBuyGoal));
	}
	else if ((GetFrontType() == GoalSellResource) && (owner.lock()->GetCurrentCityPtr().lock()->GetName() != cityNameFinish))
	{
		std::unique_ptr<AIGoal_MoveToCity> moveToSellGoal;
		moveToSellGoal.reset(new AIGoal_MoveToCity(owner, theMap.CityNameToPtr(cityNameFinish)));
		AddSubgoal(std::move(moveToSellGoal));
	}
	else
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
	tradeRoutes = theResourceManager.GetTradeRoutesFromCity(owner.lock()->GetCurrentCityPtr(), owner.lock()->GetBiasProfitability(), owner.lock()->GetBiasPatience(), owner.lock()->GetBiasFear());

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
			auto newTradeRoutes = theResourceManager.GetTradeRoutesFromCity(city, owner.lock()->GetBiasProfitability(), owner.lock()->GetBiasPatience(), owner.lock()->GetBiasFear());
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

void AIGoal_Trade::QueueTrade(std::weak_ptr<City> _start, std::weak_ptr<City> _finish, std::string _resourceName, unsigned int _quantity)
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
}

void AIGoal_Trade::PopulateNeighbors()
{
	theMap.GetSortedNeighbors(neighbors, owner.lock()->GetCurrentCityPtr(), owner.lock()->GetBiasPatience());
}

int AIGoal_Trade::ConvertProgressToPercentage()
{
	return (totalProgress - subgoals.size()) / totalProgress * 100;
}