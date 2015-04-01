#pragma once
#include "AIGoal.h"
class AIGoal_UpgradePurse :
	public AIGoal
{
private:
	void ReceiveMessage(Message *message);

public:
	AIGoal_UpgradePurse(std::weak_ptr<NPC> _owner);
	~AIGoal_UpgradePurse();

	void Activate();
	int Process();
	void Terminate();

	std::string GetGoalString();
	std::string GetGoalProgressString();
};

