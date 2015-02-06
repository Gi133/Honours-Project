#include "stdafx.h"
#include "NameManager.h"

namespace
{
	std::string cityNameLocationFallBack = "Resources/Files/city_names.txt";
	std::string npcNameLocationFallBack = "Resources/Files/npc_names.txt";
	std::string npcSurnameLocationFallBack = "Resources/Files/npc_surnames.txt";
	std::string dungeonNameLocationFallBack = "Resources/Files/dungeon_names.txt";
}

NameManager* NameManager::_instance = NULL;

NameManager& NameManager::getInstance()
{
	if (_instance == NULL)
		_instance = new NameManager();
	return *_instance;
}

NameManager::NameManager()
{
	nameContainer = nullptr;
	cityNameLocation = npcNameLocation = npcSurnameLocation = fileLine = "";

	LoadFileLocations();

	LoadNames(cityNameLocation, cityNameContainer);
	LoadNames(npcNameLocation, npcNameContainer);
	LoadNames(npcSurnameLocation, npcSurnameContainer);
	LoadNames(dungeonNameLocation, dungeonNameContainer);
}

NameManager::~NameManager()
{
	delete nameContainer;
	nameContainer = nullptr;
}

void NameManager::LoadFileLocations()
{
	cityNameLocation = thePrefs.GetString("NameGeneratorSettings", "cityNameLocation");
	if (cityNameLocation.empty())
		cityNameLocation = cityNameLocationFallBack;

	npcNameLocation = thePrefs.GetString("NameGeneratorSettings", "npcNameLocation");
	if (npcNameLocation.empty())
		npcNameLocation = npcSurnameLocationFallBack;

	npcSurnameLocation = thePrefs.GetString("NameGeneratorSettings", "npcSurnameLocation");
	if (npcSurnameLocation.empty())
		npcSurnameLocation = npcSurnameLocationFallBack;

	dungeonNameLocation = thePrefs.GetString("NameGeneratorSettings", "dungeonNameLocation");
	if (dungeonNameLocation.empty())
		dungeonNameLocation = dungeonNameLocationFallBack;
}

void NameManager::LoadNames(const std::string &fileLocation, std::vector<std::string> &outVector)
{
	file.open(fileLocation);

	for (fileLine; std::getline(file, fileLine);)
		outVector.push_back(fileLine); // Add each line into the outVector.
}

std::string NameManager::GenerateName(const int mode, bool isUnique /* = true */)
{
	// Mode Cheat Sheet
	// mode : 1 NPC Name, 2 NPC Surname, 3 City Name.

	bool isUniqueFail = false;
	int num = 0;

	// Check mode and adjust targets appropriately.
	switch (mode)
	{
	case 1: // NPC Name
		nameContainer = &npcNameContainer;
		break;

	case 2: // NPC Surname
		nameContainer = &npcSurnameContainer;
		break;

	case 3: // City Name
		nameContainer = &cityNameContainer;
		break;

	case 4: // Dungeon Name
		nameContainer = &dungeonNameContainer;
		break;

	default:
		sysLog.Log("Inappropriate mode parameter.");
		MessageBox(NULL, L"Something went horribly wrong.\nLog file saved.\nThe application will now exit.", L"ERROR!", MB_OK); // Windows message box code.
		theWorld.StopGame();
		break;
	}

	do
	{
		// Generate a random number between 0 and the container size.
		num = MathUtil::RandomIntInRange(0, nameContainer->size());

		// Check against unique name vector to see if name is valid.
		for (auto i : uniqueNames)
			if (i == nameContainer->at(num)) // If name matches an already existing name
			{
				isUniqueFail = true;
				break;
			}
	} while (isUniqueFail);

	// Catch the unlikely scenario of not finding a name.
	if (isUniqueFail)
	{
		sysLog.Log("NAME GENERATION FAILED!");
		sysLog.Log("Name: " + nameContainer->at(num));

		//Display a Message Box for the user.
		MessageBox(NULL, L"Something went horribly wrong.\nLog file saved.\nThe application will now exit.", L"ERROR!", MB_OK); // Windows message box code.
		theWorld.StopGame();
	}

	return nameContainer->at(num);
}