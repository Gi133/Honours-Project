#include "stdafx.h"
#include "AIGoalComposite.h"

AIGoalComposite::AIGoalComposite(std::weak_ptr<NPC> _owner, int _type) :AIGoal(_owner, _type)
{
}

AIGoalComposite::~AIGoalComposite()
{
}

void AIGoalComposite::AddSubgoal(std::unique_ptr<AIGoal> newGoal)
{
	subgoals.push_front(std::move(newGoal));
}

void AIGoalComposite::QueueSubgoal(std::unique_ptr<AIGoal> newGoal)
{
	subgoals.push_back(std::move(newGoal));
}

void AIGoalComposite::RemoveAllSubgoals()
{
	subgoals.clear();
}

int AIGoalComposite::ProcessSubgoals()
{
	while (!subgoals.empty() && (subgoals.front()->isComplete() || subgoals.front()->hasFailed()))
	{
		subgoals.front()->Terminate();
		subgoals.pop_front();
	}

	if (!subgoals.empty())
	{
		int status = subgoals.front()->Process();

		if (status == COMPLETED && subgoals.size() > 1)
			return ACTIVE;

		return status;
	}
	else
		return COMPLETED;
}