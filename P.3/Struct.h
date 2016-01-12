#pragma once

#include "stdafx.h"

struct Box
{
	// Loại hộp (1, 2 hoặc 3)
	int type;

	// Tọa độ tâm hộp
	int cx, cy;

	Box() {}
	Box(int t, int x, int y) : type(t), cx(x), cy(y) {}
};