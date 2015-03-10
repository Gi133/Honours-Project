#pragma once
#include "UIManager.h"

#define theGameManger TheGameManager::getInstance()

class TheGameManager :
	public GameManager
{
private:
	bool paused;

	std::unique_ptr<UIManager> uiManager;
	std::shared_ptr<TimeManager> timeManager;
	std::vector<std::shared_ptr<NPC>> npcContainer;

	Vector2 mapSize, halfMapSize;

	unsigned int activeNPCIterator;

	void AddNPC(const int numberToAdd);
	void InitializeNPC();

	virtual void ReceiveMessage(Message *message) override;

	void onPause();
	void onUnpause();

protected:
	TheGameManager();
	~TheGameManager() { _instance = nullptr; }
	static TheGameManager* _instance;

public:
	static TheGameManager& getInstance();
	virtual void Render() override;
	virtual void Update(float dt) override;
};
