#pragma once
class AIGoalEvaluator
{
protected:
	// Multiply desirability score with this value.
	float bias;

	virtual void LoadDefaults() = 0;

public:
	AIGoalEvaluator(const float _bias);
	virtual ~AIGoalEvaluator();

	virtual float CalculateDesirability(std::weak_ptr<NPC> _owner) = 0;
	virtual void SetGoal(std::weak_ptr<NPC> _owner) = 0;
};
