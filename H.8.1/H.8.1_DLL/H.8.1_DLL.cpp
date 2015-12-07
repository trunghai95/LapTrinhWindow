// H.8.1_DLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#define EXPORT __declspec(dllexport)

EXPORT int Cong(int a, int b) { return a + b; }

EXPORT int Tru(int a, int b) { return a - b; }

EXPORT int Nhan(int a, int b) { return a*b; }

EXPORT int Chia(int a, int b) { return (b == 0) ? a : a / b; }