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
	activeNPCIterator = 0;

	uiManager.reset(new UIManager);
	timeManager.reset(new TimeManager);

	uiManager->AssignTimeManager(timeManager);

	theMap.GenerateMap();

	uiManager->SetupMapActors();

	AddNPC(1);

	uiManager->SetCurrentNPC(npcContainer.at(0));

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

void TheGameManager::AddNPC(const int numberToAdd)
{
	for (int i = 0; i < numberToAdd; i++)
	{
		std::shared_ptr<NPC> newNPC;
		newNPC.reset(new NPC);
		npcContainer.push_back(std::move(newNPC));
	}
}

void TheGameManager::ReceiveMessage(Message *message)
{
	if ((timeManager->GetStarted() == false) && (message->GetMessageName() == "SpaceBar"))
	{
		theWorld.RemoveAppendToWindowName();
		timeManager->StartTime();
	}

	if (message->GetMessageName() == "E")
	{
		if (activeNPCIterator < npcContainer.size())
			activeNPCIterator++;
		else
			activeNPCIterator = 0;

		uiManager->SetCurrentNPC(npcContainer.at(activeNPCIterator));
	}

	if (message->GetMessageName() == "Q")
	{
		if (activeNPCIterator > 0)
			activeNPCIterator--;
		else
			activeNPCIterator = npcContainer.size();

		uiManager->SetCurrentNPC(npcContainer.at(activeNPCIterator));
	}
}