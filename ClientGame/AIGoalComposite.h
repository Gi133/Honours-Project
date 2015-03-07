/*!
 * \file AIGoalComposite.h
 * \date 2015/03/07 18:22
 *
 * \author Andreas Xirotyris
 * Contact: evilflem@gmail.com
 *
 * \brief Composite AI Goal class
 *
 * TODO: long description
 *
 * \note Based on code by Mat Buckland (www.ai-junkie.com). Updated to use features of C++11 and modified to use Angel2D framework where necessary.
*/

#pragma once
#include "stdafx.h"
#include "AIGoal.h"

template <class actor_type>
class AIGoalComposite :
	public AIGoal<actor_type>
{
private:
	using std::list<std::unique_ptr<AIGoal<actor_type>>> = subgoalList;

protected:
	subgoalList subgoals;

	// Remove any completed subgoals from the beginning of the list then process next goal.
	int ProcessSubgoals();

	void ForwardMessage(const Message* _message); // Forwards a message to first subgoal.

public:
	AIGoalComposite(std::weak_ptr<actor_type> _owner, AIGOALTYPE _type) :AIGoal<actor_type>(_owner, _type) {}
	AIGoalComposite(std::weak_ptr<actor_type> _owner, int _type) :AIGoal<actor_type>(_owner, _type) {}
	virtual ~AIGoalComposite(RemoveAllSubgoals(););

	virtual void Activate() = 0;
	virtual int Process() = 0;
	virtual void Terminate() = 0;

	void AddSubgoal(std::unique_ptr<AIGoal<actor_type>> goal) { subgoals.push_front(std::move(goal)); }

	// Remove all subgoals from the list.
	void RemoveAllSubgoals();

	// Default Message Handing
	virtual void ReceiveMessage(Message *message) { ForwardMessage(message); }
};

template <class actor_type>
void AIGoalComposite<actor_type>::RemoveAllSubgoals()
{
	for (subgoalList::iterator i = subgoals.begin(); i != subgoals.end(); ++i)
	{
		i->Terminate;
		i.reset();
	}

	subgoals.clear();
}

template<class actor_type>
inline int AIGoalComposite<actor_type>::ProcessSubgoals()
{
	// Remove goals that fit the criteria.
	while (!subgoals.empty() && (subgoals.front()->isComplete() || subgoals.front() == hasFailed()))
	{
		subgoals.front()->Terminate();
		subgoals.front().reset();
		subgoals.pop_front();
	}

	// Check if there are any subgoals left
	if (!subgoals.empty())
	{
		// Process topmost subgoal.
		auto subgoalStatus = subgoals.front()->Process();

		// Special case check
		if (subgoalStatus == COMPLETED && subgoals.size() > 1)
			return ACTIVE;

		return subgoalStatus;
	}
	else
		return COMPLETED; // No more subgoals left.
}

template<class actor_type>
inline void AIGoalComposite<actor_type>::ForwardMessage(const Message * _message)
{
	if (!subgoals.empty())
		subgoals.front()->ReceiveMessage(_message);
}
