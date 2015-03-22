#pragma once
#include "stdafx.h"
#include "AIGoal_Think.h"

class NPC :
	public std::enable_shared_from_this<NPC>
{
private:
	std::string name;

	std::unique_ptr<AIGoal_Think> aiBrain;
	std::shared_ptr<Inventory> inventory;
	std::weak_ptr<City> currentCity;

	bool aiControlled;

	float biasProfitability; // Value showing how profitable the trade should be for the AI to think about taking it.
	float biasPatience; // Value to show the distance the NPC is willing to travel for trades.
	float biasTrade; // Value showing NPC bias to trade.

	float baseMoveSpeed, calculatedMoveSpeed;
	void LoadDefaults();
	void InitializeInventory(const int startingBagNumber, const int startingGoldNumber);

	virtual void CalculateMoveSpeed();

	void GenerateBias();

public:
	NPC();
	~NPC();

	void Update();

	void SetupBrain();
	void SetAIControlled(const bool _aiControlled) { aiControlled = _aiControlled; }
	bool GetAIControlled() { return aiControlled; }

	std::weak_ptr<Inventory> GetInventory() { return inventory; }
	std::weak_ptr<City> GetCurrentCityPtr() { return currentCity; }

	void GenerateName();
	void GenerateStartingCity();

	void SetCity(const int cityIterator) { currentCity = theMap.GetCityContainerRef().get().at(cityIterator); }
	void SetCity(const std::weak_ptr<City> destination);
	void SetStartingCity(const std::string cityName);

	std::string GetName() { return name; }
	std::string GetCityName() { return currentCity.lock()->GetName(); }

	float GetMoveSpeed()const;
	std::string GetCurrentGoal()const;
	std::string GetCurrentGoalProgress()const;

	float GetBiasProfitability() const { return biasProfitability; }
	float GetBiasPatience() const { return biasPatience; }
	float GetBiasTrade() const { return biasTrade; }

	void QueueRandomMoveToCity(); // Function to have the merchant move to a random city after current queued goals are done.
	void QueueMoveToCity(std::weak_ptr<City> _city);
	void QueueTrade();
};
