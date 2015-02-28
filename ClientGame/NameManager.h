#pragma once
#include <fstream>
#include "stdafx.h"

#define theNameManger NameManager::getInstance()

class NameManager
{
private:
	NameManager();
	~NameManager();
	static NameManager* _instance;

	std::string cityNameLocation, npcNameLocation, npcSurnameLocation, dungeonNameLocation;

	std::vector<std::string> cityNameContainer, npcNameContainer, npcSurnameContainer, dungeonNameContainer;
	std::vector<std::string> uniqueNames;

	// TODO: Attempt to make it more in-line with C++11 standard.
	std::vector<std::string>* nameContainer;

	unsigned int duplicateNumber;

	void LoadFileLocations(); // Function to load all the file locations.

	// Function to load names from files into a vector of strings.
	// Takes in a text file and outputs each line into a vector of strings
	// Params:
	// * fileLocation : Pass by reference as string.
	// * outVector : Pass by reference the vector to output into.
	void LoadNames(const std::string &fileLocation, std::vector<std::string> &outVector);

public:
	static NameManager& getInstance();

	// Functions to return names that have been loaded in.
	// Params:
	// * mode : 1 NPC Name, 2 NPC Surname, 3 City Name, 4 Dungeon Name.
	// * isUnique : Default True. If true then name is taken out of the name pool.
	std::string GenerateName(const int mode, bool isUnique = true);
};
