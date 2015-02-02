#include "stdafx.h"
#include "TheGameManager.h"

int main(int argc, char* argv[])
{
	// Initialize a new log file.
	FileLog *fileLog = new FileLog(FileLog::MakeLogFileName("StartupLog"));
	sysLog.AddLog(fileLog);
	fileLog->Log("TEST");

	std::string windowTableName = "WindowSettings";

	if (!theWorld.Initialize())
	{
		// World could not be initialized.
		// Write down the info in log.
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

	sysLog.Log("TEST");

	//adds the default grid so you can more easily place Actors
	theWorld.Add(new GridActor(), -1);

	// Add custom game manager
	theWorld.SetGameManager(&theGameManger);

	// do all your setup first, because this function won't return until you're exiting
	theWorld.StartGame();

	// any cleanup can go here
	theWorld.Destroy();

	return 0;
}