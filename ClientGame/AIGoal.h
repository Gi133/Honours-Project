#pragma once

// Don't worry compiler,
// you will see this at some point 
// but for now heres a forward declaration 
// so you don't flip your shit.
class NPC; 

class AIGoal:
	public MessageListener
{
protected:
	std::weak_ptr<NPC> owner;
	int goalType, goalStatus;

	void ActivateInactive() { if (isInactive()) Activate(); } // Reactive if inactive.
	void ReActivate() { if (hasFailed()) goalStatus = INACTIVE; }

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

	AIGoal(std::weak_ptr<NPC> _owner, int _type)
	{
		goalType = _type;
		owner = _owner;
		goalStatus = INACTIVE;
	}
	virtual ~AIGoal() {}

	virtual void Activate() = 0;
	virtual int Process() = 0;
	virtual void Terminate() = 0;

	virtual std::string GetGoalString() = 0;
	virtual std::string GetGoalProgressString() { return "Not Applicable."; } // Default behavior.

	virtual void AddSubgoal(std::unique_ptr<AIGoal>) { throw(std::runtime_error("Attempted to add subgoal to an atomic goal.")); }

	virtual void ReceiveMessage(Message *message) {} // Default behavior.

	bool isComplete()const { return goalStatus == COMPLETED; }
	bool isActive()const { return goalStatus == ACTIVE; }
	bool isInactive()const { return goalStatus == INACTIVE; }
	bool hasFailed()const { return goalStatus == FAILED; }
	int GetType()const { return goalType; }
};
