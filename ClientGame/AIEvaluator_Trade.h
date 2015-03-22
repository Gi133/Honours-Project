#pragma once
#include "AIGoalEvaluator.h"

class AIEvaluator_Trade :
	public AIGoalEvaluator
{
private:
	void LoadDefaults();

public:
	AIEvaluator_Trade(const float _bias);
	~AIEvaluator_Trade();

	float CalculateDesirability(std::weak_ptr<NPC> _owner);
	void SetGoal(std::weak_ptr<NPC> _owner);
};
