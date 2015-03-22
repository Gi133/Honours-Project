#pragma once
#include "AIGoal.h"
class AIGoal_MoveToCity :
	public AIGoal
{
private:
	std::weak_ptr<City> destination;
	float distance, progress;

	unsigned int ConvertProgresstoPercentage();

	void LoadDefaults();
	void SetupMessageSubscription();

	void ReceiveMessage(Message *message);

public:
	AIGoal_MoveToCity(std::weak_ptr<NPC> _owner, std::weak_ptr<City> _destination);
	~AIGoal_MoveToCity();

	void Activate();
	int Process();
	void Terminate();

	std::string GetGoalString();
	std::string GetGoalProgressString();
};
