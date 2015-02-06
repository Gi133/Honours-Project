-- Any tables that you put in here will be loaded into Preferences

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
}

LocationGeneratorSettings =
{
	maxAttempts = 10,
	radius = 50.0,
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