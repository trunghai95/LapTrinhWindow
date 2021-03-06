// H.3.1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "H.3.1.h"
#include <string>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int state;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	state = 0;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_H31, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_H31));

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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_H31));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_H31);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void choose(HWND &hWnd, int &id)
{
	HMENU hm = GetMenu(hWnd);
	CheckMenuItem(hm, ID_DRAW_ELLIPSE, MF_UNCHECKED | MF_BYCOMMAND);
	CheckMenuItem(hm, ID_DRAW_PIXEL, MF_UNCHECKED | MF_BYCOMMAND);
	CheckMenuItem(hm, ID_DRAW_RECTANGLE, MF_UNCHECKED | MF_BYCOMMAND);
	CheckMenuItem(hm, ID_DRAW_LINE, MF_UNCHECKED | MF_BYCOMMAND);

	CheckMenuItem(hm, id, MF_CHECKED | MF_BYCOMMAND);
}

void showMB(HWND &hWnd, int &id)
{
	LPWSTR str = new TCHAR[100];
	GetMenuString(GetMenu(hWnd), id, str, 100, MF_BYCOMMAND);
	MessageBox(hWnd, (L"Bạn vừa chọn chức năng " + std::wstring(str)).c_str(), NULL, MB_OK);
	delete str;
}

void changeMenuBar(HWND &hWnd)
{
	int id[] = { ID_FILE_OPEN, ID_FILE_SAVE, ID_FILE_SAVEAS, ID_FORMAT_CHOOSECOLOR };
	int nFlag;
	HMENU hm = GetMenu(hWnd);
	if (!state)
	{
		nFlag = MF_GRAYED;
		ModifyMenu(hm, ID_DRAW_PIXEL, nFlag | MF_BYCOMMAND, ID_DRAW_PIXEL, L"C&ut");
		ModifyMenu(hm, ID_DRAW_RECTANGLE, nFlag | MF_BYCOMMAND, ID_DRAW_RECTANGLE, L"&Copy");
		ModifyMenu(hm, ID_DRAW_ELLIPSE, nFlag | MF_BYCOMMAND, ID_DRAW_ELLIPSE, L"&Paste");
		ModifyMenu(hm, ID_DRAW_LINE, nFlag | MF_BYCOMMAND, ID_DRAW_LINE, L"Select &All");
		ModifyMenu(hm, ID_FORMAT_CHOOSEBRUSH, nFlag | MF_BYCOMMAND, ID_FORMAT_CHOOSEBRUSH, L"Choose &Font...");
	}
	else
	{
		nFlag = MF_ENABLED;
		ModifyMenu(hm, ID_DRAW_PIXEL, nFlag | MF_BYCOMMAND, ID_DRAW_PIXEL, L"&Pixel");
		ModifyMenu(hm, ID_DRAW_RECTANGLE, nFlag | MF_BYCOMMAND, ID_DRAW_RECTANGLE, L"&Rectangle");
		ModifyMenu(hm, ID_DRAW_ELLIPSE, nFlag | MF_BYCOMMAND, ID_DRAW_ELLIPSE, L"&Ellipse");
		ModifyMenu(hm, ID_DRAW_LINE, nFlag | MF_BYCOMMAND, ID_DRAW_LINE, L"&Line");
		ModifyMenu(hm, ID_FORMAT_CHOOSEBRUSH, nFlag | MF_BYCOMMAND, ID_FORMAT_CHOOSEBRUSH, L"Choose &Brush...");
	}

	for (int i = 0; i < 4; ++i)
	{
		EnableMenuItem(hm, id[i], nFlag | MF_BYCOMMAND);
	}

	state = 1 - state;
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_DRAW_ELLIPSE: case ID_DRAW_LINE: case ID_DRAW_PIXEL: case ID_DRAW_RECTANGLE:
			choose(hWnd, wmId);
		case ID_FILE_NEW: case ID_FILE_OPEN: case ID_FILE_SAVE: case ID_FILE_SAVEAS:
		case ID_FORMAT_CHOOSEBRUSH: case ID_FORMAT_CHOOSECOLOR:
			showMB(hWnd, wmId);
			break;
		case ID_CHANGEMENUBAR:
			changeMenuBar(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
