#pragma once
#include "AIGoal.h"

class AIGoal_BuyResources :
	public AIGoal
{
private:
	std::string resourceName;
	int resourceIterator, quantity;
	unsigned int expectedPrice;

	void ReceiveMessage(Message *message);

public:
	AIGoal_BuyResources(std::weak_ptr<NPC> _owner, const std::string _resourceName, const unsigned int _quantity, const unsigned int _expectedPrice);
	~AIGoal_BuyResources();

	void Activate();
	int Process();
	void Terminate();

	std::string GetGoalString();
	std::string GetGoalProgressString();
};
