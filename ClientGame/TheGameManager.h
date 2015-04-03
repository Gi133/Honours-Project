#pragma once
#include "UIManager.h"

#define theGameManger TheGameManager::getInstance()

class TheGameManager :
	public GameManager
{
private:
#ifdef _DEBUG
	unsigned int loopCounter, maxCounter;
	std::chrono::time_point<std::chrono::high_resolution_clock> totalTimerStart;
#endif // _DEBUG

	bool paused;

	std::unique_ptr<UIManager> uiManager;
	std::shared_ptr<TimeManager> timeManager;
	std::vector<std::shared_ptr<NPC>> npcContainer;

	Vector2 mapSize, halfMapSize;

	unsigned int activeNPCIterator;

	void LoadDefaults();

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
