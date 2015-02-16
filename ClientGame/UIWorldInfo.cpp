#include "stdafx.h"
#include "UIWorldInfo.h"

namespace
{
	const auto dayMessageNameFallBack = "Day";
	const auto monthMessageNameFallBack = "Month";
	const auto yearMessageNameFallBack = "Year";
}

UIWorldInfo::UIWorldInfo()
{
	LoadUIWorldInfoPreferences();

	theSwitchboard.SubscribeTo(this, dayMessageName);
	theSwitchboard.SubscribeTo(this, monthMessageName);
	theSwitchboard.SubscribeTo(this, yearMessageName);
}

UIWorldInfo::~UIWorldInfo()
{
}

void UIWorldInfo::LoadUIWorldInfoPreferences()
{
	dayMessageName = thePrefs.GetString("TimeManagerSettings", "dayMessageName");
	if (dayMessageName.empty())
		dayMessageName = dayMessageNameFallBack;

	monthMessageName = thePrefs.GetString("TimeManagerSettings", "monthMessageName");
	if (monthMessageName.empty())
		monthMessageName = monthMessageNameFallBack;

	yearMessageName = thePrefs.GetString("TimeManagerSettings", "yearMessageName");
	if (yearMessageName.empty())
		yearMessageName = yearMessageNameFallBack;
}

void UIWorldInfo::Update()
{
}

void UIWorldInfo::InitializeElements()
{
	// Day, Month, Year.
	AddElement(3);

	elementContainer.at(0)->SetTitleText("Day");
	elementContainer.at(1)->SetTitleText("Month");
	elementContainer.at(2)->SetTitleText("Year");

	AdjustCurrentElements();
}

void UIWorldInfo::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == "CameraChange")
	{
		SetWindowAnchor(windowAnchor);
		AdjustCurrentElements();
	}

	if (message->GetMessageName() == dayMessageName)
		elementContainer.at(0)->SetContentText(std::to_string(timeManager.lock()->getDay()));

	if (message->GetMessageName() == monthMessageName)
		elementContainer.at(1)->SetContentText(std::to_string(timeManager.lock()->getMonth()));

	if (message->GetMessageName() == yearMessageName)
		elementContainer.at(2)->SetContentText(std::to_string(timeManager.lock()->getYear()));
}

void UIWorldInfo::GrabTimeValues()
{
	elementContainer.at(0)->SetContentText(std::to_string(timeManager.lock()->getDay()));
	elementContainer.at(1)->SetContentText(std::to_string(timeManager.lock()->getMonth()));
	elementContainer.at(2)->SetContentText(std::to_string(timeManager.lock()->getYear()));
}