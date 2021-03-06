#pragma once

#include <string>
#include "stdafx.h"
#include <fstream>
#include <sstream>

using namespace std;

#define MAX 1000

TCHAR loaiCT[6][100] = { L"Ăn uống", L"Di chuyển", L"Nhà cửa", L"Xe cộ", L"Nhu yếu phẩm", L"Dịch vụ" };

struct ChiTiet
{
	TCHAR loai[MAX], noiDung[MAX], tien[MAX];

	ChiTiet() {}
	ChiTiet(TCHAR* _loai, TCHAR* _noiDung, TCHAR* _tien)
	{
		memcpy(loai, _loai, sizeof(loai));
		memcpy(noiDung, _noiDung, sizeof(noiDung));
		memcpy(tien, _tien, sizeof(tien));
	}

	ChiTiet(const TCHAR* line)
	{
		TCHAR buffer[MAX];
		wstringstream str;
		str.clear();
		str << line;
		
		str.getline(buffer, MAX, L'\t');
		wcscpy(loai, loaiCT[_wtoi(buffer)]);

		str.getline(buffer, MAX, L'\t');
		wcscpy(noiDung, buffer);

		str.getline(buffer, MAX);
		wcscpy(tien, buffer);
	}

	void addToList(HWND listbox)
	{
		wstring buffer;
		buffer += wstring(loai) + wstring(L", ") + wstring(noiDung) + wstring(L", ") + wstring(tien);
		SendMessage(listbox, LB_ADDSTRING, NULL, (LPARAM)buffer.c_str());
	}

	void write(wofstream &os)
	{
		int i;
		for (i = 0; i < 6; ++i)
		{
			if (!wcscmp(loai, loaiCT[i]))
				break;
		}

		TCHAR str[MAX];
		_swprintf(str, L"%d\t%s\t%s", i, noiDung, tien);
		os << str;
	}
};