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
	resizable = 1,
	vsync = 1,
	enableBackground = 1,
	enableDebugGrid = 1,
}

LayerNames = 
{
	UI = "UILayer",
	UIText = "UITextLayer",
	UIMap = "UIMapLayer",
}

FontSettings =
{
	fontLocation = "Resources/Fonts/Inconsolata.otf",
	fontTitleSize = 24,
	fontContentSize = 14,
	fontTitleName = "TitleFont",
	fontContentName = "ContentFont",
}

TimeManagerSettings = 
{
	tickTime = 1,
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
	locations = 22,
}

CityGeneratorSettings =
{
	maxAttempts = 10,
	radius = 50.0,
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
	radiusVillage = 10.0,
	radiusTown = 12.0,
	radiusSmallCity = 14.0,
	radiusLargeCity = 16.0,
}

LocationGeneratorSettings =
{
	maxAttempts = 20,
	cityRadius = 40.0,
	locationRadius = 60.0,
	mapSizeX = 1000,
	mapSizeY = 1000,
}

-- Dungeon Generation Settings
LocationSettings = 
{
	minStartThreat = 0,
	maxStartThreat = 100,
	maxThreatLow = 33,
	maxThreatLowName = "Weak",
	colorThreatLow = "#C68C8C",
	maxThreatMed = 66,
	maxThreatMedName = "Dangerous",
	colorThreadMed = "#851818",
	maxThreatHigh = 100,
	maxThreatHighName = "Deadly",
	colorThreatHigh = "#EE2C2C",
}

-- Resource Settings
ResourceFileSettings = 
{
	resourceBasePriceLocation = "Resources/Files/resource_base_price.config",
}

-- UI Settings
UIWindowSettings = 
{
	windowBackgroundColor = "0x666633",
	windowBackgroundAlpha = 1.0,
}

UIManagerSettings = 
{
	windowWorldInfoSizeX = 800,
	windowWorldInfoSizeY = 120,
	windowWorldInfoAnchor = "topright",
	windowWorldInfoColor = "#7C4712",

	windowMapSizeX = 800,
	windowMapSizeY = 648,
	windowMapAnchor = "bottomright",
	windowMapColor = "#33CC33",

	windowObjectInfoSizeX = 224,
	windowObjectInfoSizeY = 768,
	windowObjectAnchor = "left",
	windowObjectColor = "#7C4712",
}

UIElementSettings =
{
	textColor = "#000000",
	textPadding = 5.0,
}

-- Bag and Inventory Settings
BagSettings = 
{
	bagMaxSpaceBase = 100,
	bagMaxUpgradeLevel = 10,
	bagUpgradeBasePrice = 50,
	bagUpgradeLevelSpace = 20,
}

InventorySettings = 
{
	purseStartingLimit = 10000,
	numPerLine = 2,
}

-- City Inventory Settings
CityInventorySettings =
{
	minResourceQuantity = 1000,
}

-- NPC Settings
NPCSettings = 
{
	startingBagNumber = 1,
	startingGoldNumber = 1000,

	baseMoveSpeed = 100.0,
}

-- Game Settings
GameSettings = 
{
	startingNPCNumber = 2,
}

-- AI Goal Settings
AIGoalBias =
{
	biasDefaultMin = 0.5,
	biasDefaultMax = 1.5,
}