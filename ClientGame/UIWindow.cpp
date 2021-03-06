#include "stdafx.h"
#include "UIWindow.h"

namespace
{
	const auto UILayerNameFallBack = "UILayer";
	const auto windowBackgroundColorFallBack = "0x000000";
	const auto windowBackgroundAlphaFallBack = 1.0f;
}

UIWindow::UIWindow()
{
	theSwitchboard.SubscribeTo(this, "CameraChange");

	background.reset(new HUDActor);
	theWorld.Add(background.get());

	LoadPreferences();

	background->SetLayer(UILayerName);
	background->SetColor(Color::FromHexString(windowBackgroundColor));
	background->SetAlpha(windowBackgroundAlpha);
}

UIWindow::~UIWindow()
{
}

void UIWindow::UpdateWindowData()
{
	// Based on center, calculate the top left and bottom right corners of the window.
	windowTopLeft = windowCenter - (windowSize / 2);
	windowBottomRight = windowCenter + (windowSize / 2);

	BackgroundUpdate();
	AutoOrientation();
}

void UIWindow::SetWindowAnchor(const UIWindowAnchors anchor)
{
	auto offsetCenterX = windowSize.X / 2;
	auto offsetCenterY = windowSize.Y / 2;

	// Store anchor value for later reference.
	windowAnchor = anchor;

	// Find out where the center of the window is going to be.
	// Do so by finding the anchor and adjusting accordingly.
	switch (anchor)
	{
	case Center:
		windowAnchorName = "Center";
		windowCenter = Vector2(theCamera.GetWindowWidth() / 2, theCamera.GetWindowHeight() / 2); // NO ADJUSTMENT NEEDED.
		break;
	case Left:
		windowAnchorName = "Left";
		windowCenter = Vector2(offsetCenterX, theCamera.GetWindowHeight() / 2);
		break;
	case Right:
		windowAnchorName = "Right";
		windowCenter = Vector2(theCamera.GetWindowWidth() - offsetCenterX, theCamera.GetWindowHeight() / 2);
		break;
	case Top:
		windowAnchorName = "Top";
		windowCenter = Vector2(theCamera.GetWindowWidth() / 2, offsetCenterY);
		break;
	case Bottom:
		windowAnchorName = "Bottom";
		windowCenter = Vector2(theCamera.GetWindowWidth() / 2, theCamera.GetWindowHeight() - offsetCenterY);
		break;
	case TopLeft:
		windowAnchorName = "TopLeft";
		windowCenter = Vector2(offsetCenterX, offsetCenterY);
		break;
	case TopRight:
		windowAnchorName = "TopRight";
		windowCenter = Vector2(theCamera.GetWindowWidth() - offsetCenterX, offsetCenterY);
		break;
	case BottomLeft:
		windowAnchorName = "BottomLeft";
		windowCenter = Vector2(offsetCenterX, theCamera.GetWindowHeight() - offsetCenterY);
		break;
	case BottomRight:
		windowAnchorName = "BottomRight";
		windowCenter = Vector2(theCamera.GetWindowWidth() - offsetCenterX, theCamera.GetWindowHeight() - offsetCenterY);
		break;
	default:
		break;
	}

	UpdateWindowData();
}

void UIWindow::SetWindowAnchor(const std::string anchorName)
{
	// Not pretty...
	// Changes that can improve: Create a map to do lookups between enum and string. Adds a big function on initialization though.

	// Convert string to enum value.
	if (std::regex_match(anchorName, std::regex("center", std::regex_constants::ECMAScript | std::regex_constants::icase)))
	{
		windowAnchorName = "Center";
		SetWindowAnchor(UIWindowAnchors::Center);
		return;
	}

	if (std::regex_match(anchorName, std::regex("left", std::regex_constants::ECMAScript | std::regex_constants::icase)))
	{
		windowAnchorName = "Left";
		SetWindowAnchor(UIWindowAnchors::Left);
		return;
	}

	if (std::regex_match(anchorName, std::regex("right", std::regex_constants::ECMAScript | std::regex_constants::icase)))
	{
		windowAnchorName = "Right";
		SetWindowAnchor(UIWindowAnchors::Right);
		return;
	}

	if (std::regex_match(anchorName, std::regex("top", std::regex_constants::ECMAScript | std::regex_constants::icase)))
	{
		windowAnchorName = "Top";
		SetWindowAnchor(UIWindowAnchors::Top);
		return;
	}

	if (std::regex_match(anchorName, std::regex("bottom", std::regex_constants::ECMAScript | std::regex_constants::icase)))
	{
		windowAnchorName = "Bottom";
		SetWindowAnchor(UIWindowAnchors::Bottom);
		return;
	}

	if (std::regex_match(anchorName, std::regex("topleft", std::regex_constants::ECMAScript | std::regex_constants::icase)))
	{
		windowAnchorName = "TopLeft";
		SetWindowAnchor(UIWindowAnchors::TopLeft);
		return;
	}

	if (std::regex_match(anchorName, std::regex("topright", std::regex_constants::ECMAScript | std::regex_constants::icase)))
	{
		windowAnchorName = "TopRight";
		SetWindowAnchor(UIWindowAnchors::TopRight);
		return;
	}

	if (std::regex_match(anchorName, std::regex("bottomleft", std::regex_constants::ECMAScript | std::regex_constants::icase)))
	{
		windowAnchorName = "BottomLeft";
		SetWindowAnchor(UIWindowAnchors::BottomLeft);
		return;
	}

	if (std::regex_match(anchorName, std::regex("bottomright", std::regex_constants::ECMAScript | std::regex_constants::icase)))
	{
		windowAnchorName = "BottomRight";
		SetWindowAnchor(UIWindowAnchors::BottomRight);
		return;
	}
}

void UIWindow::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == "CameraChange")
	{
		SetWindowAnchor(windowAnchor);
		AdjustCurrentElements();
	}
}

void UIWindow::BackgroundUpdate()
{
	background->SetSize(windowSize);
	background->SetPosition(windowCenter);
}

void UIWindow::LoadPreferences()
{
	// Layer setup
	UILayerName = thePrefs.GetString("Settings", "UILayerName");
	if (UILayerName.empty())
		UILayerName = UILayerNameFallBack;

	windowBackgroundColor = thePrefs.GetString("UIWindowSettings", "windowBackgroundColor");
	if (windowBackgroundColor.empty())
		windowBackgroundColor = windowBackgroundColorFallBack;

	windowBackgroundAlpha = thePrefs.GetFloat("UIWindowSettings", "windowBackgroundAlpha");
	if (windowBackgroundAlpha == 0)
		windowBackgroundAlpha = windowBackgroundAlphaFallBack;
}

void UIWindow::AddElement(const int elementsToAdd /* = 1 */)
{
	Vec2i topLeft, bottomRight;

	for (int i = 0; i < elementsToAdd; i++)
	{
		if (windowElementOrientation) // If horizontal, divide width
		{
			windowElementSize.X = windowSize.X / (elementContainer.size() + 1);
			windowElementSize.Y = windowSize.Y;
		}
		else
		{
			windowElementSize.X = windowSize.X;
			windowElementSize.Y = windowSize.Y / (elementContainer.size() + 1);
		}

		for (unsigned int i = 0; i <= elementContainer.size(); i++)
		{
			// Check orientation.
			if (windowElementOrientation)
			{
				// Find the new top left and bottom right positions.
				topLeft.X = (i * windowElementSize.X) + windowTopLeft.X;
				topLeft.Y = windowTopLeft.Y;

				bottomRight.X = ((i + 1)*windowElementSize.X) + windowTopLeft.X;
				bottomRight.Y = windowBottomRight.Y;
			}
			else
			{
				// Find the new top left and bottom right positions.
				topLeft.X = windowTopLeft.X;
				topLeft.Y = (i * windowElementSize.Y) + windowTopLeft.Y;

				bottomRight.X = windowBottomRight.X;
				bottomRight.Y = ((i + 1)*windowElementSize.Y) + windowTopLeft.Y;
			}

			// Check if you reached the new element.
			if (i == elementContainer.size())
			{
				// Use the calculated topleft and bottomright from above to create a new element
				// and push it into the container.
				std::unique_ptr<UIElement> newElement;
				newElement.reset(new UIElement(topLeft, bottomRight));
				elementContainer.push_back(std::move(newElement));
				break;
			}
			else
				elementContainer.at(i)->AdjustPositionVectors(topLeft, bottomRight);
		}
	}
}

void UIWindow::AutoOrientation()
{
	if (windowSize.X > windowSize.Y)
		windowElementOrientation = true;
	else
		windowElementOrientation = false;
}

void UIWindow::AdjustCurrentElements()
{
	Vec2i topLeft, bottomRight;

	if (windowElementOrientation) // If horizontal, divide width
	{
		windowElementSize.X = windowSize.X / elementContainer.size();
		windowElementSize.Y = windowSize.Y;
	}
	else
	{
		windowElementSize.X = windowSize.X;
		windowElementSize.Y = windowSize.Y / elementContainer.size();
	}

	for (unsigned int i = 0; i < elementContainer.size(); i++)
	{
		// Check orientation.
		if (windowElementOrientation)
		{
			// Find the new top left and bottom right positions.
			topLeft.X = (i * windowElementSize.X) + windowTopLeft.X;
			topLeft.Y = windowTopLeft.Y;

			bottomRight.X = ((i + 1)*windowElementSize.X) + windowTopLeft.X;
			bottomRight.Y = windowBottomRight.Y;
		}
		else
		{
			// Find the new top left and bottom right positions.
			topLeft.X = windowTopLeft.X;
			topLeft.Y = (i * windowElementSize.Y) + windowTopLeft.Y;

			bottomRight.X = windowBottomRight.X;
			bottomRight.Y = ((i + 1)*windowElementSize.Y) + windowTopLeft.Y;
		}

		elementContainer.at(i)->AdjustPositionVectors(topLeft, bottomRight);
	}
}