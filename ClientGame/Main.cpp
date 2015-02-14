#include "stdafx.h"
#include "TheGameManager.h"

namespace
{
	const auto UILayerNameFallBack = "UILayer";
	const auto UILayerTextNameFallBack = "UITextLayer";
}

int main(int argc, char* argv[])
{
	// Initialize a new log file.
	sysLog.AddLog(new FileLog(FileLog::MakeLogFileName("StartupLog")));

	std::string windowTableName = "WindowSettings";

	if (!theWorld.Initialize())
	{
		// World could not be initialized.
		// Write down the info in log.
		// ISSUE : File logging does not appear to work properly, look into it.
		sysLog.Log("Error Initializing theWorld.\n");
		sysLog.Log("Parameters used:\n");
		sysLog.Log("Window Width: " + thePrefs.GetInt(windowTableName, "width"));
		sysLog.Log("Window Height: " + thePrefs.GetInt(windowTableName, "height"));
		sysLog.Log("Window Name: " + thePrefs.GetString(windowTableName, "name"));
		sysLog.Log("AA: " + thePrefs.GetInt(windowTableName, "antiAliasing"));
		sysLog.Log("Resizeable: " + thePrefs.GetInt(windowTableName, "resizable"));

		//Display a Message Box for the user.
		MessageBox(NULL, L"Something went horribly wrong.\nLog file saved.\nThe application will now exit.", L"ERROR!", MB_OK); // Windows message box code.
		return 0;
	}

	// Layer setup
	auto UILayerName = thePrefs.GetString("Settings", "UILayerName");
	if (UILayerName.empty())
		UILayerName = UILayerNameFallBack;
	theWorld.NameLayer(UILayerName, 1);

	auto UILayerText = thePrefs.GetString("Settings", "UILayerText");
	if (UILayerText.empty())
		UILayerText = UILayerTextNameFallBack;
	theWorld.NameLayer(UILayerTextNameFallBack, 2);

	// Register Text Font
	RegisterFont("Resources/Fonts/Inconsolata.otf", 25, "UITitleFont");
	RegisterFont("Resources/Fonts/Inconsolata.otf", 16, "UIContentFont");

	//adds the default grid so you can more easily place Actors
	theWorld.Add(new GridActor(), 0);

	// Add custom game manager
	theWorld.SetGameManager(&theGameManger);

	// do all your setup first, because this function won't return until you're exiting
	theWorld.StartGame();

	// any cleanup can go here
	theWorld.Destroy();

	return 0;
}