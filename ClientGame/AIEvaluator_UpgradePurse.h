#pragma once
#include "AIGoalEvaluator.h"
class AIEvaluator_UpgradePurse :
	public AIGoalEvaluator
{
private:
	void LoadDefaults();

public:
	AIEvaluator_UpgradePurse(const float _bias);
	~AIEvaluator_UpgradePurse();

	float CalculateDesirability(std::weak_ptr<NPC> _owner);
	void SetGoal(std::weak_ptr<NPC> _owner);
};

