#pragma once
#include "AIGoalComposite.h"

class AIGoal_Trade :
	public AIGoalComposite
{
private:
	std::vector<std::weak_ptr<City>> neighbors;

	std::string resourceName, cityNameStart, cityNameFinish;
	unsigned int quantity;

	unsigned int totalProgress;

	int FindTrade();
	void QueueTrade(std::weak_ptr<City> _start, std::weak_ptr<City> _finish, std::string _resourceName, unsigned int _quantity, const bool moveBack = true);

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
