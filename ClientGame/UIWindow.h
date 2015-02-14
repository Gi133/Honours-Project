#pragma once
#include "stdafx.h"
#include <regex>

enum UIWindowAnchors
{
	Center,
	Left,
	Right,
	Top,
	Bottom,
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight
};

class UIWindow:
	public MessageListener
{
private:
	std::unique_ptr<HUDActor> background;
	std::vector<std::shared_ptr<TextActor>> textContainer;

	Vector2 windowSize, windowTopLeft, windowBottomRight, windowCenter;
	UIWindowAnchors windowAnchor;

	std::string UILayerName, UILayerTextName, windowBackgroundColor, windowAnchorName;
	float windowBackgroundAlpha;

	void ReceiveMessage(Message *message);

	void UpdateWindowData(); // Bad name for "Update all the position vectors related to this window".
	void BackgroundUpdate(); // Update the background position and size.
	void LoadPreferences();

public:
	UIWindow();
	~UIWindow();

	void SetWindowSize(const Vector2 size){ windowSize = size; UpdateWindowData(); }
	void SetWindowSize(const int width, const int height){ windowSize = Vector2(width, height); UpdateWindowData(); }

	void SetWindowWidth(const int width){ windowSize.X = width; UpdateWindowData(); }
	void SetWindowHeight(const int height){ windowSize.Y = height; UpdateWindowData(); }

	void SetWindowAnchor(const UIWindowAnchors anchor);
	void SetWindowAnchor(const std::string anchorName);

	void SetWindowColorHex(const std::string colorHex){ background->SetColor(Color::FromHexString(colorHex)); }
	void SetWindowColorRGBA(const float r, const float g, const float b, const float a = 1.0f){ background->SetColor(r, g, b, a); }
};

