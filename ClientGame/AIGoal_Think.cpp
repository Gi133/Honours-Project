#include "stdafx.h"
#include "AIGoal_Think.h"


AIGoal_Think::AIGoal_Think(std::weak_ptr<NPC> _owner):AIGoalComposite(_owner, GoalThink)
{

}


AIGoal_Think::~AIGoal_Think()
{
	for (GoalEvaluators::iterator i = evaluators.begin(); i != evaluators.end(); ++i)
		delete i->get();

	evaluators.clear();
}

void AIGoal_Think::CalculateGoal()
{
	auto bestScore = 0.0f;
	std::weak_ptr<AIGoalEvaluator> bestScoreGoal;

	for (auto i : evaluators)
	{
		auto score = i->CalculateDesirability(owner);

		if (score >= bestScore)
		{
			bestScore = score;
			bestScoreGoal = i;
		}
	}

	bestScoreGoal.lock()->SetGoal(owner);
}

bool AIGoal_Think::isNotFront(unsigned int _goalType) const
{
	if (!subgoals.empty())
		return subgoals.front()->GetType() != goalType;

	return true;
}

int AIGoal_Think::Process()
{
	ActivateInactive();

	int status = ProcessSubgoals();

	if (status == COMPLETED || status == FAILED)
	{
		if (!owner.lock()->GetAIControlled())
			goalStatus = INACTIVE;
	}

	return goalStatus;
}

void AIGoal_Think::Activate()
{
	if (owner.lock()->GetAIControlled())
		CalculateGoal();

	goalStatus = ACTIVE;
}

void AIGoal_Think::Terminate()
{
}
