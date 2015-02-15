#include "stdafx.h"
#include "TimeManager.h"

namespace
{
	const auto tickTimeFallBack = 10;

	const auto tickMessageNameFallBack = "Tick";
	const auto dayMessageNameFallBack = "Day";
	const auto monthMessageNameFallBack = "Month";
	const auto yearMessageNameFallBack = "Year";

	const auto maxTicksDayFallBack = 1;
	const auto maxDaysFallBack = 30;
	const auto maxMonthsFallBack = 10;
}

TimeManager::TimeManager()
{
	ticks = 0;
	day = month = year = 1;
	tickMessageName = dayMessageName = monthMessageName = yearMessageName = "";

	LoadConfig();
	theSwitchboard.SubscribeTo(this, tickMessageName);
}

TimeManager::~TimeManager()
{
}

void TimeManager::Update()
{
	CheckTime();
}

void TimeManager::CheckTime()
{
	auto timerNow = std::chrono::steady_clock::now();
	auto timerPassed = timerNow - timerStart;

	if (std::chrono::duration_cast<std::chrono::seconds>(timerPassed).count() >= tickTime)
		theSwitchboard.Broadcast(new Message(tickMessageName));
}

void TimeManager::LoadConfig()
{
	tickTime = thePrefs.GetInt("TimeManagerSettings", "tickTime");
	if (!tickTime)
		tickTime = tickTimeFallBack;

	maxTicksDay = thePrefs.GetInt("TimeManagerSettings", "maxTicksDay");
	if (!maxTicksDay)
		maxTicksDay = maxTicksDayFallBack;

	maxDays = thePrefs.GetInt("TimeManagerSettings", "maxDays");
	if (!maxDays)
		maxDays = maxDaysFallBack;

	maxMonths = thePrefs.GetInt("TimeManagerSettings", "maxMonths");
	if (!maxMonths)
		maxMonths = maxMonthsFallBack;

	tickMessageName = thePrefs.GetString("TimeManagerSettings", "tickMessageName");
	if (tickMessageName.empty())
		tickMessageName = tickMessageNameFallBack;

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

void TimeManager::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == tickMessageName)
	{
		timerStart = std::chrono::steady_clock::now();
		ticks++;
		UpdateDate();
	}
}

void TimeManager::UpdateDate()
{
	// Check ticks, adjust day accordingly.
	if (ticks >= maxTicksDay)
	{
		day++;
		ticks = 0;
		theSwitchboard.Broadcast(new Message(dayMessageName));
	}

	if (day >= maxDays)
	{
		month++;
		day = 1;
		theSwitchboard.Broadcast(new Message(monthMessageName));
	}

	if (month >= maxMonths)
	{
		year++;
		month = 1;
		theSwitchboard.Broadcast(new Message(yearMessageName));
	}
}