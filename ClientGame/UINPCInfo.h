// UI Section to show all info on the "active" NPC.

#pragma once
#include "UIBase.h"

class UINPCInfo :
	public UIBase
{
private:
	std::unique_ptr<TextActor> textName, textNameContent;
	std::unique_ptr<TextActor> textClass, textClassContent;

	std::string nameString, classString;

	void Setup() override;
	void SetupTextActors() override;

public:
	UINPCInfo();
	~UINPCInfo();

	// Getters and Setters
	std::string GetNameString(){ return nameString; }
	std::string GetClassString(){ return classString; }
	void SetNameString(std::string newNameString){ nameString = newNameString; }
	void SetClassString(std::string newClassString){ classString = newClassString; }

	void Update() override;
};
