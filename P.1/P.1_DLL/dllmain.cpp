// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#define EXPORT _declspec(dllexport)
#define ACTIVATE 123456789

HHOOK hHook;
HINSTANCE hInst;
HWND hAppWnd;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(hHook, nCode, wParam, lParam);

	if (wParam == WM_RBUTTONDOWN && HIWORD(GetKeyState(VK_CONTROL)))
		SendMessage(hAppWnd, wParam, ACTIVATE, NULL);

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

EXPORT VOID InstallHook(HWND hWnd)
{
	if (hHook != NULL)
		return;
	hAppWnd = hWnd;
	hHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInst, 0);
}

VOID RemoveHook()
{
	if (hHook == NULL)
		return;

	UnhookWindowsHookEx(hHook);
	hHook = NULL;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hInst = hModule;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		RemoveHook();
		break;
	}
	return TRUE;
}

