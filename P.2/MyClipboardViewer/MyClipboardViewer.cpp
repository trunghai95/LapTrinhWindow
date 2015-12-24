// MyClipboardViewer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MyClipboardViewer.h"
#include <Richedit.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

HINSTANCE hInstDll;
HWND hWndNextViewer, hWndAnsi, hWndUnicode, g_hWnd;
int winW, winH;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void getAnsi();
void getUnicode();
void getBitmap();
void getClipboard();

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
	LoadString(hInstance, IDC_MYCLIPBOARDVIEWER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYCLIPBOARDVIEWER));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYCLIPBOARDVIEWER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYCLIPBOARDVIEWER);
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
	case WM_CREATE:
		g_hWnd = hWnd;
		hInstDll = LoadLibrary(L"Msftedit.dll");

		hWndAnsi = CreateWindowEx(0, MSFTEDIT_CLASS, NULL,
			ES_SELECTIONBAR | WS_CHILD | WS_VISIBLE | ES_MULTILINE |
			ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_HSCROLL | WS_VSCROLL |
			WS_BORDER | ES_READONLY, 0, 0, 0, 0, hWnd, NULL, hInst, 0);
		hWndUnicode = CreateWindowEx(0, MSFTEDIT_CLASS, NULL,
			ES_SELECTIONBAR | WS_CHILD | WS_VISIBLE | ES_MULTILINE |
			ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_HSCROLL | WS_VSCROLL |
			WS_BORDER | ES_READONLY, 0, 0, 0, 0, hWnd, NULL, hInst, 0);

		hWndNextViewer = SetClipboardViewer(hWnd);
		getClipboard();
		if (hWndNextViewer != NULL)
			SendMessage(hWndNextViewer, message, wParam, lParam);

		break;
	case WM_SIZE:
		winW = LOWORD(lParam);
		winH = HIWORD(lParam);
		MoveWindow(hWndAnsi, 0, 0, winW / 2, winH / 4, FALSE);
		MoveWindow(hWndUnicode, winW / 2 + 1, 0, winW / 2, winH / 4, FALSE);
		break;
	case WM_CHANGECBCHAIN:
		if (wParam == (WPARAM)hWndNextViewer)
			hWndNextViewer = (HWND)lParam;
		else if (hWndNextViewer != NULL)
			SendMessage(hWndNextViewer, message, wParam, lParam);
		break;
	case WM_DRAWCLIPBOARD:
		getClipboard();
		if (hWndNextViewer != NULL)
			SendMessage(hWndNextViewer, message, wParam, lParam);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		getClipboard();
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		FreeLibrary(hInstDll);
		DestroyWindow(hWndAnsi);
		DestroyWindow(hWndUnicode);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void getAnsi()
{
	OpenClipboard(g_hWnd);
	char* hGlb;
	int len;
	
	if (hGlb = (char*)GetClipboardData(CF_TEXT))
	{
		GlobalLock(hGlb);
		len = strlen(hGlb);

		char* buffer = (char*)VirtualAlloc(NULL, (len + 1)*sizeof(char), MEM_COMMIT, PAGE_READWRITE);
		strcpy(buffer, hGlb);
		GlobalUnlock(hGlb);

		TCHAR* bufferL = (TCHAR*)VirtualAlloc(NULL, (len + 1)*sizeof(TCHAR), MEM_COMMIT, PAGE_READWRITE);
		mbstowcs(bufferL, buffer, len + 1);
		SetWindowText(hWndAnsi, bufferL);

		VirtualFree(buffer, (len + 1)*sizeof(char), MEM_FREE);
		VirtualFree(bufferL, (len + 1)*sizeof(TCHAR), MEM_FREE);
	}
	else SetWindowText(hWndAnsi, L"");

	CloseClipboard();
}

void getUnicode()
{
	OpenClipboard(g_hWnd);
	TCHAR* hGlb;
	int len;

	if (hGlb = (TCHAR*)GetClipboardData(CF_UNICODETEXT))
	{
		GlobalLock(hGlb);
		len = wcslen(hGlb);

		TCHAR* buffer = (TCHAR*)VirtualAlloc(NULL, (len + 1)*sizeof(TCHAR), MEM_COMMIT, PAGE_READWRITE);
		wcscpy(buffer, hGlb);
		GlobalUnlock(hGlb);

		SetWindowText(hWndUnicode, buffer);

		VirtualFree(buffer, (len + 1)*sizeof(TCHAR), MEM_FREE);
	}
	else SetWindowText(hWndUnicode, L"");

	CloseClipboard();
}

void getBitmap()
{
	OpenClipboard(g_hWnd);
	HBITMAP hGlb;
	if (hGlb = (HBITMAP)GetClipboardData(CF_BITMAP))
	{
		GlobalLock(hGlb);
		HDC memDC = CreateCompatibleDC(NULL);
		SelectObject(memDC, hGlb);
		HDC hdc = GetDC(g_hWnd);
		BitBlt(hdc, 0, winH / 4 + 1, winW, 3 * winH / 4, memDC, 0, 0, SRCCOPY);
		GlobalUnlock(hGlb);
		ReleaseDC(g_hWnd, hdc);
		DeleteDC(memDC);
	}

	CloseClipboard();
}

void getClipboard()
{
	HDC hdc = GetDC(g_hWnd);
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	HPEN hPen = CreatePen(PS_NULL, 0, NULL);
	SelectObject(hdc, hBrush);
	SelectObject(hdc, hPen);
	Rectangle(hdc, 0, 0, winW, winH);
	
	DeleteObject(hBrush);
	DeleteObject(hPen);
	ReleaseDC(g_hWnd, hdc);

	getAnsi();
	getUnicode();
	getBitmap();
}