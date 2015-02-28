#pragma once
#include "stdafx.h"
#include "UIElement.h"
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

class UIWindow :
	public MessageListener
{
private:
	std::unique_ptr<HUDActor> background;

	Vector2 windowSize, windowTopLeft, windowBottomRight, windowCenter;

	std::string UILayerName, windowBackgroundColor, windowAnchorName;
	float windowBackgroundAlpha;

	bool windowElementOrientation; // True : Horizontal, False : Vertical
	Vec2ui windowElementSize;

	virtual void ReceiveMessage(Message *message);

	void UpdateWindowData(); // Bad name for "Update all the position vectors related to this window".
	void BackgroundUpdate(); // Update the background position and size.
	void LoadPreferences();

	void AutoOrientation(); // Set the element orientation.

protected:
	std::vector<std::shared_ptr<UIElement>> elementContainer;
	UIWindowAnchors windowAnchor;

public:
	UIWindow();
	~UIWindow();

	void AddElement(const int elementsToAdd = 1);
	void AdjustCurrentElements();

	void SetWindowSize(const Vector2 size) { windowSize = size; }
	void SetWindowSize(const int width, const int height) { windowSize = Vector2(width, height); }

	void SetWindowWidth(const int width) { windowSize.X = width; }
	void SetWindowHeight(const int height) { windowSize.Y = height; }

	void SetWindowAnchor(const UIWindowAnchors anchor);
	void SetWindowAnchor(const std::string anchorName);

	void SetWindowColorHex(const std::string colorHex) { background->SetColor(Color::FromHexString(colorHex)); }
	void SetWindowColorRGBA(const float r, const float g, const float b, const float a = 1.0f) { background->SetColor(r, g, b, a); }

	void SetLayer(const std::string layerName) { background->SetLayer(layerName); } // To change the layer name from the default (UILayer).
	int GetLayer() { return background->GetLayer(); }

	Vector2 GetBackgroundSize() { return background->GetSize(); }

	Vector2 GetCenterPosition() { return windowCenter; }

	std::string GetWindowAnchor() { return windowAnchorName; }
};
