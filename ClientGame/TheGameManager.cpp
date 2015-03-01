#include "stdafx.h"
#include "TheGameManager.h"

namespace
{
	const auto startingNPCNumberFallBack = 1;
}

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
	theSwitchboard.SubscribeTo(this, "E");
	theSwitchboard.SubscribeTo(this, "Q");

	mapSize = halfMapSize = Vector2(0.0f, 0.0f);
	activeNPCIterator = 0;

	uiManager.reset(new UIManager);
	timeManager.reset(new TimeManager);

	uiManager->AssignTimeManager(timeManager);

	theMap.GenerateMap();

	uiManager->SetupMapActors();

	InitializeNPC();

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

void TheGameManager::InitializeNPC()
{
	auto startingNPCNumber = thePrefs.GetInt("GameSettings", "startingNPCNumber");
	if (!startingNPCNumber)
		startingNPCNumber = startingNPCNumberFallBack;

	AddNPC(startingNPCNumber);
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
		if (npcContainer.size() > 1)
		{
			if (activeNPCIterator < (npcContainer.size() - 1))
				activeNPCIterator++;
			else
				activeNPCIterator = 0;

			uiManager->SetCurrentNPC(npcContainer.at(activeNPCIterator));
		}
	}

	if (message->GetMessageName() == "Q")
	{
		if (npcContainer.size() > 1)
		{
			if (activeNPCIterator > 0)
				activeNPCIterator--;
			else
				activeNPCIterator = npcContainer.size() - 1;

			uiManager->SetCurrentNPC(npcContainer.at(activeNPCIterator));
		}
	}
}