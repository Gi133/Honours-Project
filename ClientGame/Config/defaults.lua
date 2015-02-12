-- Any tables that you put in here will be loaded into Preferences
-- NOTE: Majority of these values have been hardcoded into the code incase this file
-- is missing or values assigned are not valid.

WindowSettings = 
{
	name = "NPC AI with Personality",
	width = 1024,
	height = 768,
	antiAliasing = 1,
	fullScreen = 0,
	resizable = 0,
}

LayerNames = 
{
	UI = "UILayer",
	UIText = "UITextLayer",
}

TimerSettings = 
{
	tickTime = 10,
	tickMessageName = "Tick",
	maxTicksDay = 1,
	dayMessageName = "Day",
	maxDays = 30,
	monthMessageName = "Month",
	maxMonths = 10,
	yearNameMessage = "Year",
}

NameGeneratorSettings = 
{
	cityNameLocation = "Resources/Files/city_names.txt",
	npcNameLocation = "Resources/Files/npc_names.txt",
	npcSurnameLocation = "Resources/Files/npc_surnames.txt",
	dungeonNameLocation = "Resources/Files/dungeon_names.txt",
}

MapGeneratorSettings = 
{
	cities = 10,
	locations = 10,
}

CityGeneratorSettings =
{
	maxAttempts = 10,
	radius = 100.0,
	minStartPop = 80,
	maxStartPop = 2500,
}

-- The maximum population for each stage before advancing.
-- Other setting variables required for cities.
-- Consuption, Population and Resource change values in percentages of current.
CitySettings = 
{
	maxPopVillage = 140,
	nameVillage = "Village",
	maxPopTown = 500,
	nameTown = "Town",
	maxPopSmallCity = 1500,
	nameSmallCity = "Small City",
	maxPopLargeCity = 3000,
	nameLargeCity = "Large City",
	maxStartingResources = 100000,
	minStartingResources = 10000,
	minConsuptionPerDay = 0,
	maxConsuptionPerDay = 2,
	minPopulationChange = 0,
	maxPopulationChange = 5,
	minResourceChange = 0,
	maxResourceChange = 25,
}

LocationGeneratorSettings =
{
	maxAttempts = 10,
	radius = 50.0,
}

-- Dungeon Generation Settings
LocationSettings = 
{
	minStartThreat = 0,
	maxStartThreat = 100,
	maxThreatLow = 33,
	maxThreatLowName = "Weak",
	maxThreatMed = 66,
	maxThreatMedName = "Dangerous",
	maxThreatHigh = 100,
	maxThreatHighName = "Deadly",
}

-- Resource Settings
ResourceFileSettings = 
{
	resourceBasePriceLocation = "Resources/Files/resource_base_price.config",
}

-- The data below is in percentages.
UINPCInfoSettings = 
{
	width = 0.20,
	height = 1.00,
	textStartX = 0.1,
	textStartY = 0.18,
	textOffsetY = 0.8,
	contentOffset = 0.1,
}

UIWorldInfoSettings =
{
	width = 0.80,
	height = 0.15,
	textStartX = 0.15,
	textStartY = 0.5,
	textOffsetX = 1.2,
	contentOffset = 0.1,
}

UIMapSettings = 
{
	width = 0.80,
	height = 0.85,
}

-- Bag and Inventory Settings
BagSettings = 
{
	bagMaxSpaceBase = 100,
	bagMaxUpgradeLevel = 10,
	bagUpgradeBasePrice = 50,
	bagUpgradeLevelSpace = 20,
}

-- City Inventory Settings
CityInventorySettings =
{
	minResourceQuantity = 1000,
}

-- NPC Inventory Settings
NPCInventorySettings = 
{
	
}