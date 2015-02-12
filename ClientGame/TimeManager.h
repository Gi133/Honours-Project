#pragma once
#include <chrono>
#include "stdafx.h"

class TimeManager : public MessageListener
{
private:
	int ticks, day, month, year, tickTime;
	int maxTicksDay, maxDays, maxMonths;

	std::chrono::time_point<std::chrono::system_clock> timerStart; // Used to check time.
	std::string tickMessageName, dayMessageName, monthMessageName, yearMessageName;

	void CheckTime();
	void UpdateDate();
	void LoadConfig();

	virtual void ReceiveMessage(Message *message) override;

public:
	TimeManager();
	~TimeManager();

	void Update();

	int getDay(){ return day; }
	int getMonth(){ return month; }
	int getYear(){ return year; }
	int getTicks(){ return ticks; }
	int getTicksPerDay(){ return maxTicksDay; }
};
