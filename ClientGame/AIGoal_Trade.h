#pragma once
#include "AIGoalComposite.h"

class AIGoal_Trade :
	public AIGoalComposite
{
private:
	std::vector<std::weak_ptr<City>> neighbors;

	std::string resourceName, cityNameStart, cityNameFinish;
	unsigned int quantity, buyPrice;

	unsigned int totalProgress;

	int FindTrade();
	int FindTradeForResourcesInBag();
	void QueueTrade(const ResourceManager::TradingStruct _trade, const unsigned int _quantity);

	void PopulateNeighbors();

	int ConvertProgressToPercentage();

public:
	AIGoal_Trade(std::weak_ptr<NPC> _owner);
	~AIGoal_Trade();

	int Process();
	void Activate();
	void Terminate();

	std::string GetGoalString();
	std::string GetGoalProgressString();
};
