#pragma once

#include <cstdlib>
#include "stdafx.h"

class Mouse
{
public:
	Mouse(int maxX, int maxY) : _x(rand() % maxX), _y(rand() % maxY), _status(1) {}

	void draw(HDC &hdc, HBITMAP hbm)
	{
		HDC hdcMem = CreateCompatibleDC(hdc);
		BITMAP bitmap;
		HGDIOBJ oldBitmap;

		// TODO: Add any drawing code here...
		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, hbm);

		GetObject(hbm, sizeof(bitmap), &bitmap);
		BitBlt(hdc, _x, _y, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hbm);
		DeleteDC(hdcMem);
	}

	int& getStatus() { return _status; }

	int& getX() { return _x; }
	int& getY() { return _y; }

	void changeStatus() { _status = 1 - _status; }

protected:
	int _x, _y;
	int _status; // 0: dead, 1: alive
};