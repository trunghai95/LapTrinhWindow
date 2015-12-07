#pragma once

#include "stdafx.h"
#include <vector>

#define RED 0
#define GREEN 1
#define BLUE 2

class Line
{
public:
	Line();
	Line(int x1, int y1, int x2, int y2, int c);

	// Paints the line on DC
	void paint(const HDC &hdc) const;

	// Paints a set of lines on DC
	static void paintLines(const HDC &dc, std::vector<Line> &v);

protected:

	// 2 points' coordinates
	int _x1, _y1, _x2, _y2;

	// Line's color
	int _color;
};