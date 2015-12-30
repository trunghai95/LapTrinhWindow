#pragma once

#include "stdafx.h"
#include <stdio.h>

#define BLOCK_SIZE 4096

class ThreadInfo
{
public:
	ThreadInfo() : _begin(0), _end(0), _outFile(NULL), _picCtrl(NULL), _totalSize(0) {}
	~ThreadInfo() { close(); }

	bool open(TCHAR* filePath, int begin, int end, HWND picCtrl)
	{
		close();
		if ((_outFile = _wfopen(filePath, L"wb")) == NULL)
			return false;

		_begin = begin;
		_end = end;
		_picCtrl = picCtrl;
		_totalSize = _end - _begin + 1;

		return true;
	}

	void close()
	{
		if (_outFile != NULL)
			fclose(_outFile), _outFile = NULL;
	}

	bool write(FILE* inFile)
	{
		if (_outFile == NULL || inFile == NULL)
			return false;

		int sz = _end - _begin + 1;
		
		if (sz == 0)
			return false;

		sz = min(BLOCK_SIZE, sz);

		fseek(inFile, _begin, SEEK_SET);
		fread(_buffer, 1, sz, inFile);
		fwrite(_buffer, 1, sz, _outFile);

		_begin += sz;
		return true;
	}

	void draw(HBRUSH fg)
	{
		double done = 1.0*(_end - _begin + 1) / _totalSize;
		done = 1.0 - done;

		HDC hdc = GetDC(_picCtrl);
		RECT rc;

		GetClientRect(WindowFromDC(hdc), &rc);

		SelectObject(hdc, fg);
		Rectangle(hdc, 0, 0, done * rc.right, rc.bottom);

		ReleaseDC(WindowFromDC(hdc), hdc);
	}

	void resetDraw(HBRUSH bg)
	{
		HDC hdc = GetDC(_picCtrl);
		RECT rc;

		GetClientRect(WindowFromDC(hdc), &rc);

		SelectObject(hdc, bg);
		Rectangle(hdc, 0, 0, rc.right, rc.bottom);

		ReleaseDC(WindowFromDC(hdc), hdc);
	}

private:
	int _begin, _end;
	int _totalSize;
	FILE* _outFile;
	HWND _picCtrl;
	char _buffer[BLOCK_SIZE];
};