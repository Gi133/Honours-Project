#include "stdafx.h"
#include "TheGameManager.h"

TheGameManager* TheGameManager::_instance = nullptr;

TheGameManager& TheGameManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new TheGameManager;
	return *_instance;
}

TheGameManager::TheGameManager()
{
	theSwitchboard.SubscribeTo(this, "SpaceBar");

	mapSize = halfMapSize = Vector2(0.0f, 0.0f);

	uiManager.reset(new UIManager);
	timeManager.reset(new TimeManager);

	uiManager->AssignTimeManager(timeManager);

	theMap.GenerateMap();

	uiManager->SetupMapActors();

	theWorld.AppendToWindowName("Game Timer Paused, Press SPACE to Start.");
}

void TheGameManager::Render()
{
}

void TheGameManager::Update(float dt)
{
	

	uiManager->Update();
	timeManager->Update();
}

void TheGameManager::ReceiveMessage(Message *message)
{
	if ((timeManager->GetStarted() == false) && (message->GetMessageName() == "SpaceBar"))
	{
		theWorld.RemoveAppendToWindowName();
		timeManager->StartTime();
	}
}

