#pragma once
#include "stdafx.h"
#include "UIWindow.h"
#include "TimeManager.h"

class UIWorldInfo :
	public UIWindow
{
private:
	std::string dayMessageName, monthMessageName, yearMessageName, tickMessageName;

	void LoadUIWorldInfoPreferences();

	virtual void ReceiveMessage(Message *message) override;
public:
	UIWorldInfo();
	~UIWorldInfo();

	void InitializeElements();

	void Update(std::weak_ptr<TimeManager> timeManager);
};
