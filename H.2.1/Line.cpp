#include "Line.h"
#include "stdafx.h"

COLORREF color[] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) };

Line::Line() : _x1(0), _y1(0), _x2(0), _y2(0), _color(0) {}

Line::Line(int x1, int y1, int x2, int y2, int c) : _x1(x1), _y1(y1), _x2(x2), _y2(y2), _color(c) {}

void Line::paint(const HDC &hdc) const
{
	MoveToEx(hdc, _x1, _y1, NULL);
	LineTo(hdc, _x2, _y2);
}

void Line::paintLines(const HDC &hdc, std::vector<Line> &v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		SelectObject(hdc, CreatePen(PS_SOLID, 0, color[v[i]._color]));
		v[i].paint(hdc);
	}
}