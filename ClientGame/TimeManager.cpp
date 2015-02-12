#include "stdafx.h"
#include "TimeManager.h"

namespace
{
	const auto tickTimeFallBack = 10;
	const auto tickMessageNameFallBack = "Tick";

	const auto maxTicksDayFallBack = 1;
	const auto maxDaysFallBack = 30;
	const auto maxMonthsFallBack = 10;
}

TimeManager::TimeManager()
{
	ticks = 0;
	day = month = year = 1;
	tickMessageName = "";

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

	tickMessageName = thePrefs.GetString("TimeManagerSettings", "tickMessageName");
	if (tickMessageName.empty())
		tickMessageName = tickMessageNameFallBack;

	maxTicksDay = thePrefs.GetInt("TimeManagerSettings", "maxTicksDay");
	if (!maxTicksDay)
		maxTicksDay = maxTicksDayFallBack;

	maxDays = thePrefs.GetInt("TimeManagerSettings", "maxDays");
	if (!maxDays)
		maxDays = maxDaysFallBack;

	maxMonths = thePrefs.GetInt("TimeManagerSettings", "maxMonths");
	if (!maxMonths)
		maxMonths = maxMonthsFallBack;
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
	}

	if (day >= maxDays)
	{
		month++;
		day = 1;
	}

	if (month >= maxMonths)
	{
		year++;
		month = 1;
	}
}
