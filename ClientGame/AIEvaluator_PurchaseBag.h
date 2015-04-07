#pragma once
#include "AIGoalEvaluator.h"
class AIEvaluator_PurchaseBag :
	public AIGoalEvaluator
{
private:
	void LoadDefaults();

public:
	AIEvaluator_PurchaseBag(const float _bias);
	~AIEvaluator_PurchaseBag();

	float CalculateDesirability(std::weak_ptr<NPC> _owner);
	void SetGoal(std::weak_ptr<NPC> _owner);
};

