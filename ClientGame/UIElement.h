#pragma once
class UIElement
{
private:
	std::unique_ptr<TextActor> textTitle;
	std::unique_ptr<TextActor> textContent;

	Vec2i positionTopLeft, positionBottomRight, positionCenter,
		titleTopLeft, titleBottomRight, contentTopLeft, contentBottomRight,
		compartmentSize;

	std::string textColor, fontTitleName, fontContentName, UILayerTextName;
	float textPadding;

	void LoadSettings();

public:
	UIElement(Vec2i topLeftPixel, Vec2i bottomRightPixel);
	~UIElement();

	void AdjustPositionVectors(Vec2i topLeftPixel, Vec2i bottomRightPixel);
	void AdjustTextActorSize();
	void AdjustTextPosition();

	void SetTitleText(std::string newTitle) { textTitle->SetDisplayString(newTitle); }
	void SetContentText(std::string newContent) { textContent->SetDisplayString(newContent); }
};
