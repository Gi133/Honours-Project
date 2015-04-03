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

void TheGameManager::Render()
{
}

TheGameManager::TheGameManager()
{
	LoadDefaults();

	theSwitchboard.SubscribeTo(this, "SpaceBar");
	theSwitchboard.SubscribeTo(this, "E");
	theSwitchboard.SubscribeTo(this, "Q");
	theSwitchboard.SubscribeTo(this, "Z");

	mapSize = halfMapSize = Vector2(0.0f, 0.0f);
	activeNPCIterator = 0;

	uiManager.reset(new UIManager);
	timeManager.reset(new TimeManager);

	uiManager->AssignTimeManager(timeManager);

	theMap.GenerateMap();

	uiManager->SetupMapActors();

	InitializeNPC();

	uiManager->SetCurrentNPC(npcContainer.at(0));

	paused = false;
	onPause();
}

void TheGameManager::onPause()
{
	if (!paused)
	{
		theWorld.AppendToWindowName("Game Timer Paused, Press SPACE to Start.");
		timeManager->StopTime();
		paused = true;
	}
}

void TheGameManager::onUnpause()
{
	if (paused)
	{
		theWorld.RemoveAppendToWindowName();
		timeManager->StartTime();
		paused = false;
	}
}

void TheGameManager::Update(float dt)
{
	uiManager->Update();
	timeManager->Update();

	if (!paused)
	{
#ifdef _DEBUG
		totalTimerStart = std::chrono::high_resolution_clock::now();
		loopCounter = 0;
#endif
		// Update all NPC AI.
		for (auto i : npcContainer)
			i->Update();

#ifdef _DEBUG
		loopCounter++;
		auto timeDifference = std::chrono::high_resolution_clock::now() - totalTimerStart;
		auto timeCount = std::chrono::duration_cast<std::chrono::milliseconds>(timeDifference).count();
		sysLog.Log("Total AI Time: " + IntToString(timeCount));
#endif
	}
}

void TheGameManager::InitializeNPC()
{
	auto startingNPCNumber = thePrefs.GetInt("GameSettings", "startingNPCNumber");
	if (!startingNPCNumber)
		startingNPCNumber = startingNPCNumberFallBack;

	AddNPC(startingNPCNumber);

	// Enable NPC Brain.
	for (auto i : npcContainer)
	{
		i->SetupBrain();
		i->SetAIControlled(true);
	}
}

void TheGameManager::LoadDefaults()
{
#ifdef _DEBUG
	maxCounter = thePrefs.GetInt("DebugSettings", "maxCounter");
#endif // _DEBUG
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
	if (message->GetMessageName() == "SpaceBar")
	{
		if (paused)
			onUnpause();
		else
			onPause();
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

	if (message->GetMessageName() == "Z")
	{
		sysLog.Log("Queuing random move to city.");
		npcContainer.at(activeNPCIterator)->QueueRandomMoveToCity();
	}
}