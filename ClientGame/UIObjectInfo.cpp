#include "stdafx.h"
#include "UIObjectInfo.h"

UIObjectInfo::UIObjectInfo()
{
	InitializeElements();
}

UIObjectInfo::~UIObjectInfo()
{
}

void UIObjectInfo::InitializeElements()
{
	// Name, Current City, Task, Task Progress, Inventory.
	AddElement(5);

	elementContainer.at(0)->SetTitleText("Name");
	elementContainer.at(1)->SetTitleText("Current City");
	elementContainer.at(2)->SetTitleText("Task");
	elementContainer.at(3)->SetTitleText("Task Progress");
	elementContainer.at(4)->SetTitleText("Inventory");

	AdjustCurrentElements();
}

void UIObjectInfo::AdjustCurrentElements()
{
	if (!currentNPC.expired())
	{
		// Name
		elementContainer.at(0)->SetContentText(currentNPC.lock()->GetName());

		// Current City
		elementContainer.at(1)->SetContentText(currentNPC.lock()->GetCityName());

		// Task
		elementContainer.at(2)->SetContentText(currentNPC.lock()->GetCurrentGoal());

		// Task Progress
		elementContainer.at(3)->SetContentText(currentNPC.lock()->GetCurrentGoalProgress());

		// Inventory
		elementContainer.at(4)->SetContentText(currentNPC.lock()->GetInventory().lock()->GetInventoryString());
	}
	else
	{
		// Name
		elementContainer.at(0)->SetContentText("N/A");

		// Current City
		elementContainer.at(1)->SetContentText("N/A");

		// Task
		elementContainer.at(2)->SetContentText("N/A");

		// Task Progress
		elementContainer.at(3)->SetContentText("N/A");

		// Inventory
		elementContainer.at(4)->SetContentText("N/A");
	}
}

void UIObjectInfo::Update()
{
	AdjustCurrentElements();
}