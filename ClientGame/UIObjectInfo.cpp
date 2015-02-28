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
		elementContainer.at(0)->SetContentText(currentNPC.lock()->GetName());
		elementContainer.at(1)->SetContentText(currentNPC.lock()->GetCityName());

		elementContainer.at(4)->SetContentText(currentNPC.lock()->GetInventory().lock()->GetInventoryString());
	}
}

void UIObjectInfo::Update()
{
	AdjustCurrentElements();
}