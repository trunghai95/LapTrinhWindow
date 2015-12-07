// H.9.1_DLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#define EXPORT _declspec(dllexport)

HHOOK hHook;
HINSTANCE hInst;
bool locked;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || (wParam != WM_KEYDOWN && wParam != WM_SYSKEYDOWN))
		return CallNextHookEx(hHook, nCode, wParam, lParam);

	KBDLLHOOKSTRUCT* str = (KBDLLHOOKSTRUCT*)lParam;

	if (HIWORD(GetKeyState(VK_CONTROL)) && HIWORD(GetKeyState(VK_SHIFT)))
	{
		if (str->vkCode == 'U')
		{
			locked = false;
			return 1;
		}
		if (str->vkCode == 'L')
		{
			locked = true;
			return 1;
		}
	}

	if (locked) return 1;

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

EXPORT VOID InstallHook()
{
	if (hHook != NULL) return;

	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInst, 0);
	locked = false;
}

EXPORT VOID RemoveHook()
{
	if (hHook == NULL) return;
	UnhookWindowsHookEx(hHook);
	hHook = NULL;
}