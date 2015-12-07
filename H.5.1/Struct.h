#pragma once

#include "stdafx.h"

#define TEXTWND 0
#define BITMAPWND 1

struct DATA
{
	INT wndType;
	HWND hWnd;
	HWND hEditWnd;
	BITMAP hBitmap;
	COLORREF color;
	LOGFONT logFont;
	INT drawMode;
};