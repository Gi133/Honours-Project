#pragma once
#include "stdafx.h"
#include "Utils.h"

#define theResourceManager ResourceManager::getInstance()

class ResourceManager
{
private:
	ResourceManager();
	~ResourceManager(){ _instance = nullptr; }
	static ResourceManager* _instance;

	void LoadPrefs();
	void LoadResourceTable();

	std::map<std::string, int> resourceBasePriceTable;
	std::string resourceBasePriceLocation;

public:
	static ResourceManager& getInstance();
};