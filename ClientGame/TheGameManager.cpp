#include "stdafx.h"
#include "TheGameManager.h"

TheGameManager* TheGameManager::_instance = NULL;

TheGameManager& TheGameManager::getInstance()
{
	if (_instance == NULL)
		_instance = new TheGameManager();
	return *_instance;
}

TheGameManager::TheGameManager()
{
	theUIManager.reset(new UIManager());
}

void TheGameManager::Render()
{
}

void TheGameManager::Update(float dt)
{
	
}