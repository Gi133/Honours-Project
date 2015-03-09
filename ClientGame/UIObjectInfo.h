#pragma once
#include "UIWindow.h"

class UIObjectInfo :
	public UIWindow
{
private:
	void InitializeElements();
	void AdjustCurrentElements();

	std::weak_ptr<NPC> currentNPC;

public:
	UIObjectInfo();
	~UIObjectInfo();

	void SetCurrentNPC(const std::weak_ptr<NPC> newCurrentNPC) { currentNPC = newCurrentNPC; AdjustCurrentElements(); }

	void Update();
};
