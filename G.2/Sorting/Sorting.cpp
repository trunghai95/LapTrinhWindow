// Sorting.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <sstream>


#define EXPORT _declspec(dllexport)

void Swap(int &a, int &b)
{
	typedef void(*MYPROC)(int&, int&);

	HINSTANCE hInstLib;
	MYPROC func;

	hInstLib = LoadLibrary(L"Swapping.dll");
	if (hInstLib != NULL)
	{
		func = (MYPROC)GetProcAddress(hInstLib, "Swap");
		if (func != NULL)
			func(a, b);
	}
}

EXPORT void Sort(int *arr, int n)
{
	for (int i = 0; i < n; ++i)
	for (int j = i + 1; j < n; ++j)
	{
		if (arr[i] > arr[j]) Swap(arr[i], arr[j]);
	}
}

EXPORT void List(int *arr, int n)
{
	std::wstringstream str;

	for (int i = 0; i < n; ++i)
		str << arr[i] << ", ";

	MessageBox(NULL, str.str().c_str(), L"Message", MB_OK);
}