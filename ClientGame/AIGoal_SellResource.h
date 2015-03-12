#pragma once
#include "AIGoal.h"

class AIGoal_SellResource :
	public AIGoal
{
private:
	std::string resourceName;
	int resourceIterator, quantity;

	void ReceiveMessage(Message *message);

public:
	AIGoal_SellResource(std::weak_ptr<NPC> _owner, const std::string _resourceName, const unsigned int _quantity);
	~AIGoal_SellResource();

	void Activate();
	int Process();
	void Terminate();

	std::string GetGoalString();
	std::string GetGoalProgressString();
};

