// Swapping.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#define EXPORT _declspec(dllexport)

EXPORT void Swap(int &a, int &b)
{
	a = a + b;
	b = a - b;
	a = a - b;
}