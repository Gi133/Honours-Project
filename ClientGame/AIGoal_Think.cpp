#include "stdafx.h"
#include "AIGoal_Think.h"
#include "AIGoal_Trade.h"
#include "AIEvaluator_Trade.h"
#include "AIGoal_UpgradePurse.h"
#include "AIGoal_UpgradeBag.h"
#include "AIEvaluator_UpgradeBag.h"
#include "AIEvaluator_UpgradePurse.h"

AIGoal_Think::AIGoal_Think(std::weak_ptr<NPC> _owner) :AIGoalComposite(_owner, GoalThink)
{
	SetupEvaluators();
}

AIGoal_Think::~AIGoal_Think()
{
	evaluators.clear();
}

void AIGoal_Think::SetupEvaluators()
{
	std::shared_ptr<AIEvaluator_Trade> evaluatorTrade;
	evaluatorTrade.reset(new AIEvaluator_Trade(owner.lock()->GetBiasTrade()));
	evaluators.push_back(std::move(evaluatorTrade));

	std::shared_ptr<AIEvaluator_UpgradeBag> evaluatorBagUpgrade;
	evaluatorBagUpgrade.reset(new AIEvaluator_UpgradeBag(owner.lock()->GetBiasBagUpgrade()));
	evaluators.push_back(std::move(evaluatorBagUpgrade));

	std::shared_ptr<AIEvaluator_UpgradePurse> evaluatorPurseUpgrade;
	evaluatorPurseUpgrade.reset(new AIEvaluator_UpgradePurse(owner.lock()->GetBiasPurseUpgrade()));
	evaluators.push_back(std::move(evaluatorPurseUpgrade));
}

void AIGoal_Think::CalculateGoal()
{
#ifdef _DEBUG
	auto timeStart = std::chrono::high_resolution_clock::now();
#endif // _DEBUG

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

#ifdef _DEBUG
	auto timeDifference = std::chrono::high_resolution_clock::now() - timeStart;
	sysLog.Log("Time " + owner.lock()->GetName() + "spent thinking: " + IntToString(std::chrono::duration_cast<std::chrono::microseconds>(timeDifference).count()));
#endif // _DEBUG
}

bool AIGoal_Think::isNotFront(unsigned int _goalType) const
{
	if (!subgoals.empty())
		return subgoals.front()->GetType() != goalType;

	return true;
}

int AIGoal_Think::Process()
{
	ReactivateInactive();

	int status = ProcessSubgoals();

	if (status == COMPLETED || status == FAILED)
		if (owner.lock()->GetAIControlled())
			goalStatus = INACTIVE;

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

std::string AIGoal_Think::GetGoalString()
{
	// Check if there are any goals left.
	if (!subgoals.empty())
		return subgoals.front()->GetGoalString();

	return "No Task Set.";
}

std::string AIGoal_Think::GetGoalProgressString()
{
	// Check if there is a subgoal and query it.
	if (!subgoals.empty())
		return subgoals.front()->GetGoalProgressString();

	return "N/A";
}

void AIGoal_Think::AddTradeGoal()
{
	std::unique_ptr<AIGoal_Trade> newGoal;
	newGoal.reset(new AIGoal_Trade(owner));
	subgoals.push_front(std::move(newGoal));
}

void AIGoal_Think::Queue_MoveToCity(std::weak_ptr<City> _destination)
{
	std::unique_ptr<AIGoal_MoveToCity> newGoal;
	newGoal.reset(new AIGoal_MoveToCity(owner, _destination));
	subgoals.push_back(std::move(newGoal));
}

void AIGoal_Think::Add_MoveToCity(std::weak_ptr<City> _destination)
{
	std::unique_ptr<AIGoal_MoveToCity> newGoal;
	newGoal.reset(new AIGoal_MoveToCity(owner, _destination));
	subgoals.push_front(std::move(newGoal));
}

void AIGoal_Think::Add_UpgradePurse()
{
	std::unique_ptr<AIGoal_UpgradePurse> newGoal;
	newGoal.reset(new AIGoal_UpgradePurse(owner));
	subgoals.push_front(std::move(newGoal));
}

void AIGoal_Think::Add_UpgradeBag()
{
	std::unique_ptr<AIGoal_UpgradeBag> newGoal;
	newGoal.reset(new AIGoal_UpgradeBag(owner));
	subgoals.push_front(std::move(newGoal));
}