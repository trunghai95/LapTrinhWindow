// DLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#define EXPORT _declspec(dllexport)

HHOOK hHook = NULL;
HINSTANCE hInst;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || wParam != WM_KEYDOWN)
		return CallNextHookEx(hHook, nCode, wParam, lParam);

	KBDLLHOOKSTRUCT* str = (KBDLLHOOKSTRUCT*)lParam;
	if (str->vkCode == VK_F6)
		WinExec("calc", SW_SHOW);
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

EXPORT void InstallHook()
{
	if (hHook == NULL)
		hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInst, 0);
}

EXPORT void RemoveHook()
{
	if (hHook != NULL)
	{
		UnhookWindowsHookEx(hHook);
		hHook = NULL;
	}
}