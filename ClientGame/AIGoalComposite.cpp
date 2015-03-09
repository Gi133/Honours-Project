#include "stdafx.h"
#include "AIGoalComposite.h"

AIGoalComposite::AIGoalComposite(NPC* _owner, int _type) :AIGoal(_owner, _type)
{

}

AIGoalComposite::~AIGoalComposite()
{
}

void AIGoalComposite::AddSubgoal(std::unique_ptr<AIGoal> newGoal)
{
	subgoals.push_front(std::move(newGoal));
}

void AIGoalComposite::RemoveAllSubgoals()
{
	for (subgoalList::iterator i = subgoals.begin(); i != subgoals.end(); ++i)
	{
		i->get()->Terminate();

		delete i->get();
	}

	subgoals.clear();
}

int AIGoalComposite::ProcessSubgoals()
{
	while (!subgoals.empty() && (subgoals.front()->isComplete() || subgoals.front()->hasFailed()))
	{
		subgoals.front()->Terminate();
		delete subgoals.front().get();
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


