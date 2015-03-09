#pragma once
#include "AIGoalComposite.h"
#include "AIGoalEvaluator.h"

class AIGoalEvaluator;

class AIGoal_Think :
	public AIGoalComposite
{
private:
	typedef std::vector<std::shared_ptr<AIGoalEvaluator>> GoalEvaluators;

	GoalEvaluators evaluators;

public:
	AIGoal_Think(std::weak_ptr<NPC> _owner);
	~AIGoal_Think();

	// Set highest scored evaluator as the current goal.
	void CalculateGoal();

	// Return true if goal not at front of subgoal list.
	bool isNotFront(unsigned int _goalType)const;

	int Process();
	void Activate();
	void Terminate();

	// Top Level goal functions.

	// Goal queue functions.

};

