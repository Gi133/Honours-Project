#pragma once
#include "AIGoalEvaluator.h"
class AIEvaluator_UpgradeBag :
	public AIGoalEvaluator
{
private:
	void LoadDefaults();

public:
	AIEvaluator_UpgradeBag(const float _bias);
	~AIEvaluator_UpgradeBag();

	float CalculateDesirability(std::weak_ptr<NPC> _owner);
	void SetGoal(std::weak_ptr<NPC> _owner);
};

