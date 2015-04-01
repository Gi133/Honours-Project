#pragma once
#include "AIGoal.h"
class AIGoal_UpgradeBag :
	public AIGoal
{
private:
	void ReceiveMessage(Message *message);

public:
	AIGoal_UpgradeBag(std::weak_ptr<NPC> _owner);
	~AIGoal_UpgradeBag();

	void Activate();
	int Process();
	void Terminate();

	std::string GetGoalString();
	std::string GetGoalProgressString();
};

