#pragma once
#include "stdafx.h"
class UIBase
{
public:
	UIBase();
	~UIBase();

protected:
	std::unique_ptr<HUDActor> backgroundUI;
};

