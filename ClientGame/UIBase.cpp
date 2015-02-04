#include "stdafx.h"
#include "UIBase.h"

UIBase::UIBase()
{
	centerPoint = bottomRight = topLeft = Vector2(0.0f, 0.0f);
	width = height = 0.0f;

	backgroundUI.reset(new HUDActor());
}

UIBase::~UIBase()
{
}

void UIBase::Update()
{
}

void UIBase::UpdateMinMax()
{
	Vector2 offset = Vector2(width / 2, height / 2);

	// Calculate top left and bottom right corners.
	topLeft = centerPoint - offset;
	bottomRight = centerPoint + offset;
}

void UIBase::Setup()
{
}

void UIBase::SetupTextActors()
{
}