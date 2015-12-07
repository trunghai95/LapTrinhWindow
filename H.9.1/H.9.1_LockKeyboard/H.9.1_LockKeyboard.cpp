// H.9.1_LockKeyboard.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "H.9.1_LockKeyboard.h"
#include <shellapi.h>

#define MAX_LOADSTRING 100
#define WM_TRAYICON (WM_USER + 1)
#define TRAY_ICON_ID 12345

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
NOTIFYICONDATA notifyIconData;

// Forward declarations of functions included in this code module:
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);
VOID InstallHook();
VOID RemoveHook();
VOID Minimize(HWND);
VOID Restore(HWND);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_H91_LOCKKEYBOARD, szWindowClass, MAX_LOADSTRING);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_H91_LOCKKEYBOARD));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hDlg;

   hInst = hInstance; // Store instance handle in our global variable

   hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG), NULL, DlgProc);

   if (!hDlg)
   {
      return FALSE;
   }

   ShowWindow(hDlg, SW_SHOW);
   memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));
   notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
   notifyIconData.hWnd = hDlg;
   notifyIconData.uID = TRAY_ICON_ID;
   notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE;
   notifyIconData.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_H91_LOCKKEYBOARD));
   notifyIconData.uCallbackMessage = WM_TRAYICON;

   Shell_NotifyIcon(NIM_DELETE, &notifyIconData);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

// Message handler for about box.
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == RegisterWindowMessage(L"TaskbarCreated") && !IsWindowVisible(hDlg))
	{
		Minimize(hDlg);
		return (INT_PTR)TRUE;
	}

	switch (message)
	{
	case WM_INITDIALOG:
		InstallHook();
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			DestroyWindow(hDlg);
			return (INT_PTR)TRUE;
		case IDC_BUTTONTRAY:
			Minimize(hDlg);
			return (INT_PTR)TRUE;
		}
		break;

	case WM_TRAYICON:
		if (wParam == TRAY_ICON_ID && lParam == WM_LBUTTONUP)
			Restore(hDlg);
		return (INT_PTR)TRUE;

	case WM_CLOSE: case WM_DESTROY:
		RemoveHook();
		PostQuitMessage(0);
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}

VOID InstallHook()
{
	typedef VOID (*MYPROC)();

	HINSTANCE hInstLib;
	MYPROC func;

	hInstLib = LoadLibrary(L"H.9.1_DLL.dll");
	if (hInstLib != NULL)
	{
		func = (MYPROC) GetProcAddress(hInstLib, "InstallHook");
		if (func != NULL)
			func();
	}
}

VOID RemoveHook()
{
	typedef VOID(*MYPROC)();

	HINSTANCE hInstLib;
	MYPROC func;

	hInstLib = LoadLibrary(L"H.9.1_DLL.dll");
	if (hInstLib != NULL)
	{
		func = (MYPROC)GetProcAddress(hInstLib, "RemoveHook");
		if (func != NULL)
			func();
	}
}

VOID Minimize(HWND hDlg)
{
	Shell_NotifyIcon(NIM_ADD, &notifyIconData);
	ShowWindow(hDlg, SW_HIDE);
}

VOID Restore(HWND hDlg)
{
	Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
	ShowWindow(hDlg, SW_SHOW);
}