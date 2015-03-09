#pragma once

// Don't worry compiler,
// you will see this at some point 
// but for now heres a forward declaration 
// so you don't flip your shit.
class NPC; 

class AIGoal
{
protected:
	NPC* owner;
	int goalType, goalStatus;

	void ActivateInactive() { if (isInactive()) goalStatus = INACTIVE; } // Reactive if inactive.
	void ReActivate() { if (hasFailed())Activate(); }

public:
	enum AIGOALTYPE
	{
		// High Level
		GoalThink,

		// Atomic
		GoalMoveToCity,
	};

	enum AIGOALSTATUS
	{
		ACTIVE,
		INACTIVE,
		COMPLETED,
		FAILED
	};

	AIGoal(NPC* _owner, int _type)
	{
		goalType = _type;
		owner = _owner;
		goalStatus = INACTIVE;
	}
	virtual ~AIGoal() {}

	virtual void Activate() = 0;
	virtual int Process() = 0;
	virtual void Terminate() = 0;

	virtual void AddSubgoal(std::unique_ptr<AIGoal>) { throw(std::runtime_error("Attempted to add subgoal to an atomic goal.")); }

	bool isComplete()const { return goalStatus == COMPLETED; }
	bool isActive()const { return goalStatus == ACTIVE; }
	bool isInactive()const { return goalStatus == INACTIVE; }
	bool hasFailed()const { return goalStatus == FAILED; }
	int GetType()const { return goalType; }
};
