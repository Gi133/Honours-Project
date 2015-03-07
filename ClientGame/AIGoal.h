/*!
* \file AIGoal.h
* \date 2015/03/07 17:48
*
* \author Andreas Xirotyris
* Contact: evilflem@gmail.com
*
* \brief AIGoal class, also acting as Atomic Goal.
*
* TODO: long description
*
* \note Based on code by Mat Buckland (www.ai-junkie.com). Updated to use features of C++11 and modified to use Angel2D framework where necessary.
*/

#pragma once
#include "stdafx.h"
#include "GoalTypeEnum.h"

template<class actor_type>
class AIGoal :
	public MessageListener
{
protected:
	std::weak_ptr<actor_type> owner; // Pointer to the owner of the goal.

	AIGOALSTATUS goalStatus; // Value showing the current status of the goal.
	AIGOALTYPE goalType; // Value showing the type of goal.

	virtual void ReceiveMessage(Message *message) { return; } // Default behavior for message handing.

	// Common Functions.
	void ActivateInactive() { if (goalStatus == INACTIVE) Activate(); } // Activate if inactive.
	void Reactivate() { if (goalStatus == FAILED) Activate(); } // Reactive if failed.

public:
	/*
	*	INACTIVE : The Goal has not been activated yet.
	*	ACTIVE : The goal has been activated and will be processed each update.
	*	COMPLETED : The goal has been completed and will be removed next update.
	*	FAILED : The goal has failed and will be either preplanned on removed on update.
	*/
	enum AIGOALSTATUS { INACTIVE, ACTIVE, COMPLETED, FAILED };

	/*
	 *	_owner: pointer to the owner of the goal
	 *	_type: type of goal
	 *	NOTE: Starts as INACTIVE
	 */
	AIGoal(weak_ptr<actor_type> _owner, AIGOALTYPE _type);
	AIGoal(weak_ptr<actor_type> _owner, int _type);
	virtual ~AIGoal();

	virtual void Activate() = 0;
	virtual int Process() = 0;
	virtual void Terminate() = 0;

	virtual void AddSubgoal(std::unique_ptr<AIGoal<actor_type>> goal) { throw std::runtime_error("Attempted to add subgoal into an atomic goal.") }

	// Status Checking
	AIGOALSTATUS GetStatus()const { return goalStatus; }
	bool isComplete()const { return goalStatus == COMPLETED; }
	bool isActive()const { return goalStatus == ACTIVE; }
	bool hasFailed()const { return goalStatus == FAILED; }
	bool isInactive()const { return goalStatus == INACTIVE; }

	// Type Checking
	AIGOALTYPE GetType()const { return goalType; }
	int GetTypeAsInt()const { return static_cast<int>(goalType); }
};

template<class actor_type>
inline AIGoal<actor_type>::AIGoal(weak_ptr<actor_type> _owner, AIGOALTYPE _type)
{
	owner = _owner;
	goalType = _type;
	goalStatus = INACTIVE;
}

template<class actor_type>
inline AIGoal<actor_type>::AIGoal(weak_ptr<actor_type> _owner, int _type)
{
	owner = _owner;
	goalType = static_cast<AIGOALTYPE>(_type);
	goalStatus = INACTIVE;
}

template<class actor_type>
inline AIGoal<actor_type>::~AIGoal()
{
}