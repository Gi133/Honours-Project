#pragma once

#include "stdafx.h"
#include "NameManager.h"
#include "UIManager.h"
#include "NPC.h"
#include "ResourceManager.h"
#include "TimeManager.h"

#define theGameManger TheGameManager::getInstance()

class TheGameManager :
	public GameManager, public MessageListener
{
private:
	std::unique_ptr<UIManager> uiManager;
	std::shared_ptr<TimeManager> timeManager;
	std::vector<std::shared_ptr<NPC>> npcContainer;

	Vector2 mapSize, halfMapSize;

	virtual void ReceiveMessage(Message *message) override;
protected:
	TheGameManager();
	~TheGameManager(){ _instance = nullptr; }
	static TheGameManager* _instance;

public:
	static TheGameManager& getInstance();
	virtual void Render() override;
	virtual void Update(float dt) override;
};
