#pragma once
#include "AIGoalComposite.h"
#include "AIGoalEvaluator.h"

#include "AIGoal_MoveToCity.h"

class AIGoalEvaluator;

class AIGoal_Think :
	public AIGoalComposite
{
private:
	typedef std::vector<std::shared_ptr<AIGoalEvaluator>> GoalEvaluators;

	GoalEvaluators evaluators;

	void SetupEvaluators();

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

	std::string GetGoalString();
	std::string GetGoalProgressString();

	// Top Level goal functions.
	void AddTradeGoal();
	void Add_UpgradePurse();
	void Add_UpgradeBag();
	void Add_PurchaseBag();

	// Goal queue functions.
	void Queue_MoveToCity(std::weak_ptr<City> _destination);
	void Add_MoveToCity(std::weak_ptr<City> _destination);
};
