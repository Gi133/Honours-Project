#include "stdafx.h"
#include "UIWorldInfo.h"

namespace
{
}

UIWorldInfo::UIWorldInfo()
{
}

UIWorldInfo::~UIWorldInfo()
{
}

void UIWorldInfo::LoadUIWorldInfoPreferences()
{
}

void UIWorldInfo::Update(std::weak_ptr<TimeManager> timeManager)
{
	elementContainer.at(0)->SetContentText(std::to_string(timeManager.lock()->getDay()));
	elementContainer.at(1)->SetContentText(std::to_string(timeManager.lock()->getMonth()));
	elementContainer.at(2)->SetContentText(std::to_string(timeManager.lock()->getYear()));
}

void UIWorldInfo::InitializeElements()
{
	// Day, Month, Year.
	AddElement(3);

	elementContainer.at(0)->SetTitleText("Day");
	elementContainer.at(1)->SetTitleText("Month");
	elementContainer.at(2)->SetTitleText("Year");
}
