#pragma once

#include "Brain.h"
#include "Inventory.h"
#include "NameManager.h"

class NPC
{
private:
	std::string name;

	std::unique_ptr<Brain> aiBrain;
	std::unique_ptr<Inventory> inventory;

	void LoadDefaults();
	void InitializeInventory(const int startingBagNumber, const int startingGoldNumber);

public:
	NPC();
	~NPC();

	void SetGold(const int newGold, const bool checkLimit){ inventory->SetGold(newGold, checkLimit); }
	void AddGold(const int goldToAdd, const bool checkLimit) { inventory->AddGold(goldToAdd, checkLimit); } // Dual purpose, can be used to add or subtract gold.
	void SetGoldLimit(const int newGoldLimit) { inventory->SetGoldLimit(newGoldLimit); }
	int GetGold(){ return inventory->GetGold(); }
	int GetGoldLimit(){ return inventory->GetGoldLimit(); }

	void SetBagNumber(const int bagNumber) { throw std::logic_error("The method or operation is not implemented."); }
	
	void GenerateName();
	std::string GetName(){ return name; }
};
