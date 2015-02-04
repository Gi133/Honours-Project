#pragma once
#include "UIBase.h"
class UIMap :
	public UIBase
{
private:

	void Setup() override;
	void SetupTextActors() override;

public:
	UIMap();
	~UIMap();

	void Update() override;
};
