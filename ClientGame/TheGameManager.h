#pragma once

#include "stdafx.h"
#include "UIManager.h"

#define theGameManger TheGameManager::getInstance()

class TheGameManager :
	public GameManager
{
public:
	static TheGameManager& getInstance();
	virtual void Render();
	virtual void Update(float dt);

protected:
	TheGameManager();
	static TheGameManager* _instance;

private:
	std::unique_ptr<UIManager> uiManager;
};
