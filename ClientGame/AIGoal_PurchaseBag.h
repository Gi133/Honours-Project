#pragma once
#include "AIGoal.h"
class AIGoal_PurchaseBag :
	public AIGoal
{
private:
	void ReceiveMessage(Message *message);

public:
	AIGoal_PurchaseBag(std::weak_ptr<NPC> _owner);
	~AIGoal_PurchaseBag();

	void Activate();
	int Process();
	void Terminate();

	std::string GetGoalString();
	std::string GetGoalProgressString();
};

