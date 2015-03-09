#pragma once
class AIGoalEvaluator
{
protected:
	// Multiply desirability score with this value.
	float bias;

public:
	AIGoalEvaluator(const float _bias);
	virtual ~AIGoalEvaluator();

	virtual float CalculateDesirability(NPC* _owner) = 0;
	virtual void SetGoal(NPC* _owner) = 0;
};

