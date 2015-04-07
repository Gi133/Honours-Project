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
	if (!subgoals.empty())
	{
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
	}
	else
		goalStatus = COMPLETED;

	ReactivateFailed();

	return goalStatus;
}

void AIGoal_Trade::Activate()
{
	// Clear the subgoal list.
	RemoveAllSubgoals();

	// Check if the inventory is empty.
	// This also means that NPCs will prioritize emptying their inventory before getting another trade.
	if (!owner.lock()->GetInventory().lock()->GetEmpty())
	{
		// Inventory was not empty so assume that one of the previous goals failed.
		// Find a trade for the resources in inventory.
		goalStatus = FindTradeForResourcesInBag();
	}
	else
	{
		// Pick a trade.
		goalStatus = FindTrade();
	}
}

void AIGoal_Trade::Terminate()
{
	goalStatus = COMPLETED;
}

std::string AIGoal_Trade::GetGoalString()
{
	auto output = "Trading " + IntToString(quantity) + " units of " + resourceName +
		"\n" + cityNameStart + " -> " +	cityNameFinish + "\n";

	if (!subgoals.empty())
		output += subgoals.front()->GetGoalString();

	return output;
}

std::string AIGoal_Trade::GetGoalProgressString()
{
	auto output = "Total: " + IntToString(ConvertProgressToPercentage());

	if (!subgoals.empty())
		output += "%\nSubgoal: " + subgoals.front()->GetGoalProgressString();

	return output;
}

int AIGoal_Trade::FindTrade()
{	
	auto currentGold = owner.lock()->GetInventory().lock()->GetGold();

	// Get trade routes from current city.
	auto tradeRoutes = theResourceManager.GetTradeRoutesFromCity(owner.lock()->GetCurrentCityPtr(), 
		currentGold, owner.lock()->GetBiasProfitability(),
		owner.lock()->GetBiasPatience(), owner.lock()->GetBiasFear());

	if (!tradeRoutes.empty())
	{
		// Pick random trade route and queue trade.
		auto trade = tradeRoutes.at(MathUtil::RandomIntInRange(0, tradeRoutes.size()));

		if (CalculateQuantity(trade) <= 0)
			return FAILED;

		QueueTrade(trade, quantity);
	}
	else
	{
		// Could not find trade, populate neighbors, queue a move goal to a random neighbor
		PopulateNeighbors();

		for (auto city : neighbors)
		{
			auto newTradeRoutes = theResourceManager.GetTradeRoutesFromCity(city,
				currentGold, owner.lock()->GetBiasProfitability(),
				owner.lock()->GetBiasPatience(), owner.lock()->GetBiasFear());

			tradeRoutes.reserve(tradeRoutes.size() + newTradeRoutes.size());
			std::move(std::begin(newTradeRoutes), std::end(newTradeRoutes), std::back_inserter(tradeRoutes));
		}

		if (tradeRoutes.empty())
			return FAILED;
		else
		{
			// Pick random trade route and queue trade.
			auto trade = tradeRoutes.at(MathUtil::RandomIntInRange(0, tradeRoutes.size()));

			if (CalculateQuantity(trade) <= 0)
				return FAILED;

			QueueTrade(trade, quantity);
		}
	}

	return ACTIVE;
}

int AIGoal_Trade::CalculateQuantity(const std::reference_wrapper<ResourceManager::TradingStruct> _trade)
{
	quantity = 0;

	// Calculate quantity.
	int currentGold = owner.lock()->GetInventory().lock()->GetGold();
	int priceAtCity = theResourceManager.GetResourceSellingPriceAtCity(_trade.get().start, _trade.get().resource);
	int availableSpace = owner.lock()->GetInventory().lock()->GetAvailableSpace();
	int cityInventoryAmount = owner.lock()->GetCurrentCityPtr().lock()->GetInventory().lock()->GetTotalResourceAmount(_trade.get().resource);
	int maxTradeGold = currentGold / priceAtCity;

	if (maxTradeGold > availableSpace)
		quantity = availableSpace;
	else if (cityInventoryAmount < quantity)
		quantity = cityInventoryAmount;
	else
		quantity = maxTradeGold;

	return quantity;
}

int AIGoal_Trade::FindTradeForResourcesInBag()
{
	bool tradeFound = false;

	quantity = 0;

	// Get a map of all the resources in bag.
	auto resourcesInBag = owner.lock()->GetInventory().lock()->GetAllResourcesFiltered();

	// Check if you can sell resources in current city.
	for (std::map<std::string, int>::iterator i = resourcesInBag.begin(); i != resourcesInBag.end(); ++i)
	{
		// Calculate profitability.
		auto priceAtCity = theResourceManager.GetResourceSellingPriceAtCity(owner.lock()->GetCurrentCityPtr(), i->first);
		int profitability = priceAtCity - owner.lock()->GetBuyPrice();
		if (profitability >= owner.lock()->GetBiasProfitability())
		{
			std::unique_ptr<AIGoal_SellResources> newGoal;
			newGoal.reset(new AIGoal_SellResources(owner, i->first, i->second, priceAtCity));
			subgoals.push_back(std::move(newGoal));
			tradeFound = true;
		}
	}

	if (!tradeFound)
	{
		// If no trade was found
		auto tradeRoutes = theResourceManager.GetSpecificTradeRoutesFromCity(owner.lock()->GetCurrentCityPtr(), resourcesInBag,
			owner.lock()->GetInventory().lock()->GetGold(), owner.lock()->GetBiasProfitability(), owner.lock()->GetBiasPatience(), owner.lock()->GetBiasFear());

		if (!tradeRoutes.empty())
		{
			// Pick random trade route and queue trade.
			auto trade = tradeRoutes.at(MathUtil::RandomIntInRange(0, tradeRoutes.size()));

			if (CalculateQuantity(trade) <= 0)
				return FAILED;

			QueueTrade(trade, quantity);
		}
		else // No trades found
		{
			// Check if there is space in bags.
			if (owner.lock()->GetInventory().lock()->GetAvailableSpace() > 0)
			{
				if (FindTrade() == FAILED)
				{
					// If no trade routes found at all, active "Holy Shit" mode and try to sell inventory at whatever price.
					for (std::map<std::string, int>::iterator i = resourcesInBag.begin(); i != resourcesInBag.end(); ++i)
					{
						// Calculate profitability.
						auto priceAtCity = theResourceManager.GetResourceSellingPriceAtCity(owner.lock()->GetCurrentCityPtr(), i->first);

						std::unique_ptr<AIGoal_SellResources> newGoal;
						newGoal.reset(new AIGoal_SellResources(owner, i->first, i->second, priceAtCity));
						subgoals.push_back(std::move(newGoal));
					}
				}
			}
		}
	}
	return ACTIVE;
}

void AIGoal_Trade::QueueTrade(const ResourceManager::TradingStruct _trade, const unsigned int _quantity)
{
	std::unique_ptr<AIGoal_MoveToCity> goalMove;
	std::unique_ptr<AIGoal_BuyResources> goalBuy;
	std::unique_ptr<AIGoal_SellResources> goalSell;

	totalProgress = 0;

	// Save variables for goal strings
	cityNameStart = _trade.start.lock()->GetName();
	cityNameFinish = _trade.finish.lock()->GetName();
	quantity = _quantity;
	resourceName = _trade.resource;

	if (owner.lock()->GetCityName() != cityNameStart) // If start city isn't the current city, queue move to start.
	{
		goalMove.reset(new AIGoal_MoveToCity(owner, _trade.start));
		QueueSubgoal(std::move(goalMove));
		totalProgress++;
	}

	// Queue resource buy.
	goalBuy.reset(new AIGoal_BuyResources(owner, _trade.resource, _quantity, _trade.expectedBuyPrice));
	QueueSubgoal(std::move(goalBuy));
	totalProgress++;

	// Queue move to finish.
	goalMove.reset(new AIGoal_MoveToCity(owner, _trade.finish));
	QueueSubgoal(std::move(goalMove));
	totalProgress++;

	// Queue Sell Resources.
	goalSell.reset(new AIGoal_SellResources(owner, _trade.resource, _quantity, _trade.expectedSellPrice));
	QueueSubgoal(std::move(goalSell));
	totalProgress++;
}

void AIGoal_Trade::PopulateNeighbors()
{
	theMap.GetSortedNeighbors(neighbors, owner.lock()->GetCurrentCityPtr(), owner.lock()->GetBiasPatience());
}

int AIGoal_Trade::ConvertProgressToPercentage()
{
	if (!subgoals.empty())
	{
		float totalGoals = (totalProgress - subgoals.size()) / totalProgress * 100;
		float currentGoalProgress = subgoals.front()->GetGoalProgress() / totalProgress;
		return totalGoals + currentGoalProgress;
	}
	
	return 0;
}