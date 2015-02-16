#include "stdafx.h"
#include "TheGameManager.h"

namespace
{
	const auto UILayerNameFallBack = "UILayer";
	const auto UILayerTextNameFallBack = "UITextLayer";
	const auto UIMapLayerNameFallBack = "UIMapLayer";

	const auto fontLocationFallBack = "Resources/Fonts/Inconsolata.otf";
	const auto fontTitleSizeFallBack = 25;
	const auto fontContentSizeFallBack = 14;

	const auto fontTitleNameFallBack = "TitleFont";
	const auto fontContentNameFallBack = "ContentFont";
}

int main(int argc, char* argv[])
{
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

#pragma region ValueLoading
	// Layer setup
	auto UIMapLayerName = thePrefs.GetString("Settings", "UIMapLayer");
	if (UIMapLayerName.empty())
		UIMapLayerName = UIMapLayerNameFallBack;
	theWorld.NameLayer(UIMapLayerName, 1);

	auto UILayerName = thePrefs.GetString("Settings", "UILayerName");
	if (UILayerName.empty())
		UILayerName = UILayerNameFallBack;
	theWorld.NameLayer(UILayerName, 6);

	auto UILayerText = thePrefs.GetString("Settings", "UILayerText");
	if (UILayerText.empty())
		UILayerText = UILayerTextNameFallBack;
	theWorld.NameLayer(UILayerTextNameFallBack, 7);

	// Register Text Font
	auto fontLocation = thePrefs.GetString("FontSettings", "fontLocation");
	if (fontLocation.empty())
		fontLocation = fontLocationFallBack;

	auto fontTitleSize = thePrefs.GetInt("FontSettings", "fontTitleSize");
	if (!fontTitleSize)
		fontTitleSize = fontTitleSizeFallBack;

	auto fontTitleName = thePrefs.GetString("FontSettings", "fontTitleName");
	if (fontTitleName.empty())
		fontTitleName = fontTitleNameFallBack;

	auto fontContentSize = thePrefs.GetInt("FontSettings", "fontContentSize");
	if (!fontContentSize)
		fontContentSize = fontContentSizeFallBack;

	auto fontContentName = thePrefs.GetString("FontSettings", "fontContentName");
	if (fontContentName.empty())
		fontContentName = fontContentNameFallBack;
#pragma endregion

	RegisterFont(fontLocation, fontTitleSize, fontTitleName);
	RegisterFont(fontLocation, fontContentSize, fontContentName);

	//adds the default grid so you can more easily place Actors
	if (thePrefs.GetInt("WindowSettings", "enableDebugGrid"))
		theWorld.Add(new GridActor(), 1);

	// Add a background full screen actor.
	std::unique_ptr<FullScreenActor> backgroundActor;

	// Check if resizable or full screen, if not, then don't bother.
	if (thePrefs.GetInt("WindowSettings", "resizable") || thePrefs.GetInt("WindowSettings", "fullScreen"))
	{
		backgroundActor.reset(new FullScreenActor);

		if (thePrefs.GetInt("WindowSettings", "enableBackground"))
			backgroundActor->SetSprite("Resources/Images/background.jpg");
		else
			backgroundActor->SetColor(1.0f, 0.5f, 0.5f);
	}

	theWorld.Add(backgroundActor.get());

	// Add custom game manager
	theWorld.SetGameManager(&theGameManger);

	// do all your setup first, because this function won't return until you're exiting
	theWorld.StartGame();

	// any cleanup can go here
	theWorld.Destroy();

	return 0;
}