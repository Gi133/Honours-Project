#pragma once
#include "UIBase.h"
class UIWorldInfo :
	public UIBase
{
private:
	std::unique_ptr<TextActor> seasonContent, yearContent, monthContent, dayContent;
	std::string seasonString, yearString, monthString, dayString;

	void Setup() override;
	void SetupTextActors() override;

public:
	UIWorldInfo();
	~UIWorldInfo();

	void Update() override;
};
