#include "stdafx.h"
#include "UIElement.h"

namespace
{
	const auto textColorFallBack = "0x000000";
	const auto fontTitleNameFallBack = "TitleFont";
	const auto fontContentNameFallBack = "ContentFont";

	const auto UILayerTextNameFallBack = "UITextLayer";

	const auto textPaddingFallBack = 5.0f;
}

UIElement::UIElement(Vec2i topLeftPixel, Vec2i bottomRightPixel)
{
	LoadSettings();

	textTitle.reset(new TextActor);
	textContent.reset(new TextActor);

	textTitle->SetAlignment(TXT_Center);
	textContent->SetAlignment(TXT_Center);

	textTitle->SetColor(Color::FromHexString(textColor));
	textContent->SetColor(Color::FromHexString(textColor));

	textTitle->SetFont(fontTitleName);
	textContent->SetFont(fontContentName);

	textTitle->SetLayer(UILayerTextName);
	textContent->SetLayer(UILayerTextName);

	AdjustPositionVectors(topLeftPixel, bottomRightPixel);
}

UIElement::~UIElement()
{
}

void UIElement::AdjustPositionVectors(Vec2i topLeftPixel, Vec2i bottomRightPixel)
{
	// Setup the position vectors.
	positionTopLeft = topLeftPixel;
	positionBottomRight = bottomRightPixel;
	positionCenter = positionTopLeft + ((bottomRightPixel - topLeftPixel) / 2);

	AdjustTextPosition();
	AdjustTextActorSize();
}

void UIElement::LoadSettings()
{
	textColor = thePrefs.GetString("UIElementSettings", "textColor");
	if (textColor.empty())
		textColor = textColorFallBack;

	fontTitleName = thePrefs.GetString("FontSettings", "fontTitleName");
	if (fontTitleName.empty())
		fontTitleName = fontTitleNameFallBack;

	fontContentName = thePrefs.GetString("FontSettings", "fontContentName");
	if (fontContentName.empty())
		fontContentName = fontContentNameFallBack;

	textPadding = thePrefs.GetFloat("UIElementSettings", "textPadding");
	if (!textPadding)
		textPadding = textPaddingFallBack;

	UILayerTextName = thePrefs.GetString("Settings", "UILayerText");
	if (UILayerTextName.empty())
		UILayerTextName = UILayerTextNameFallBack;
}

void UIElement::AdjustTextPosition()
{
	// Find text compartments.
	titleTopLeft = positionTopLeft;
	titleBottomRight = Vec2i(positionBottomRight.X, positionCenter.Y * 1.05);

	contentTopLeft = Vec2i(positionTopLeft.X, positionCenter.Y * 1.1);
	contentBottomRight = positionBottomRight;

	Vec2i titleCenter, contentCenter;
	titleCenter.X = positionTopLeft.X + ((titleBottomRight.X - titleTopLeft.X) / 2);
	titleCenter.Y = positionTopLeft.Y + ((titleBottomRight.Y - titleTopLeft.Y) / 2);

	contentCenter.X = positionTopLeft.X + ((contentBottomRight.X - contentTopLeft.X) / 2);
	contentCenter.Y = positionCenter.Y + ((contentBottomRight.Y - contentTopLeft.Y) / 2);

	textTitle->SetPosition(MathUtil::ScreenToWorld(titleCenter));
	textContent->SetPosition(MathUtil::ScreenToWorld(contentCenter));
}

void UIElement::AdjustTextActorSize()
{
	compartmentSize = Vec2i(titleBottomRight.X - titleTopLeft.X, titleBottomRight.Y - titleTopLeft.Y);

	// Knowing the compartment size, use the padding value to adjust actor size.
	Vec2i paddingSize = compartmentSize * (textPadding / 100);

	Vector2 actorSize;
	actorSize.X = compartmentSize.X - paddingSize.X;
	actorSize.Y = compartmentSize.Y - paddingSize.Y;

	// Change size from pixels to GLunits.
	textTitle->SetSize(MathUtil::PixelsToWorldUnits(actorSize.X), MathUtil::PixelsToWorldUnits(actorSize.Y));
	textContent->SetSize(MathUtil::PixelsToWorldUnits(actorSize.X), MathUtil::PixelsToWorldUnits(actorSize.Y));
}