#pragma once
#include "AIGoal.h"

class AIGoalComposite :
	public AIGoal
{
private:
	typedef std::list<std::unique_ptr<AIGoal>> subgoalList;

protected:
	subgoalList subgoals;

	int ProcessSubgoals();

public:
	AIGoalComposite(std::weak_ptr<NPC> _owner, int _type);
	virtual ~AIGoalComposite();

	virtual void Activate() = 0;
	virtual int Process() = 0;
	virtual void Terminate() = 0;

	virtual std::string GetGoalString() = 0;

	void AddSubgoal(std::unique_ptr<AIGoal> newGoal);
	void RemoveAllSubgoals();
};

