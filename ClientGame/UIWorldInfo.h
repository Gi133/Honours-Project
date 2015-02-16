#pragma once
#include "stdafx.h"
#include "UIWindow.h"
#include "TimeManager.h"

class UIWorldInfo :
	public UIWindow
{
private:
	std::weak_ptr<TimeManager> timeManager;
	std::string dayMessageName, monthMessageName, yearMessageName;

	void LoadUIWorldInfoPreferences();
	virtual void ReceiveMessage(Message *message) override;
public:
	UIWorldInfo();
	~UIWorldInfo();

	void InitializeElements();
	void AssignTimeManager(std::weak_ptr<TimeManager> _timeManager){ timeManager = _timeManager; GrabTimeValues(); }

	void GrabTimeValues();

	void Update();
};
