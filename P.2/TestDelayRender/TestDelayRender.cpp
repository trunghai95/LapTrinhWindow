// TestDelayRender.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TestDelayRender.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

int x1, x2, y1, y2, xc1, xc2, yc1, yc2;
int type;
HDC memDC;
HBITMAP hBitmap = NULL;
HPEN dashPen, solidPen;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void onLButtonDown(HWND, LPARAM);
void onMouseMove(HWND, LPARAM);
void onRenderFormat(HWND, UINT);
void onRenderAllFormats(HWND);
void swap(int &x, int &y);

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
	LoadString(hInstance, IDC_TESTDELAYRENDER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTDELAYRENDER));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTDELAYRENDER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTDELAYRENDER);
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
		memDC = CreateCompatibleDC(NULL);
		x1 = x2 = y1 = y2 = -1;
		solidPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		dashPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
		type = ID_FILE_DRAW;
		break;
	case WM_RENDERFORMAT:
		onRenderFormat(hWnd, wParam);
		break;
	case WM_RENDERALLFORMATS:
		onRenderAllFormats(hWnd);
		break;
	case WM_DESTROYCLIPBOARD:
		break;
	case WM_LBUTTONDOWN:
		onLButtonDown(hWnd, lParam);
		break;
	case WM_LBUTTONUP:
		if (type == ID_FILE_DRAW)
			x1 = y1 = x2 = y2 = -1;
		break;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON)
			onMouseMove(hWnd, lParam);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_DRAW:
			type = ID_FILE_DRAW;
			x1 = x2 = y1 = y2 = -1;
			hdc = GetDC(hWnd);
			BitBlt(hdc, 0, 0, 2000, 2000, memDC, 0, 0, SRCCOPY);
			ReleaseDC(hWnd, hdc);
			CheckMenuItem(GetMenu(hWnd), ID_FILE_DRAW, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_FILE_SELECT, MF_BYCOMMAND | MF_UNCHECKED);
			break;
		case ID_FILE_SELECT:
			type = ID_FILE_SELECT;
			CheckMenuItem(GetMenu(hWnd), ID_FILE_DRAW, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_FILE_SELECT, MF_BYCOMMAND | MF_CHECKED);
			break;
		case ID_FILE_COPY:
			OpenClipboard(hWnd);
			EmptyClipboard();
			SetClipboardData(CF_BITMAP, NULL);
			CloseClipboard();
			xc1 = min(x1, x2);
			xc2 = max(x1, x2);
			yc1 = min(y1, y2);
			yc2 = max(y1, y2);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_MENUSELECT:
		if (type != ID_FILE_SELECT || x1 == x2 || y1 == y2)
			EnableMenuItem(GetMenu(hWnd), ID_FILE_COPY, MF_BYCOMMAND | MF_DISABLED);
		else
			EnableMenuItem(GetMenu(hWnd), ID_FILE_COPY, MF_BYCOMMAND | MF_ENABLED);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		if (hBitmap == NULL)
		{
			hBitmap = CreateCompatibleBitmap(hdc, 2000, 2000);
			SelectObject(memDC, hBitmap);
			HBRUSH brushtmp = CreateSolidBrush(RGB(255, 255, 255));
			HPEN pentmp = CreatePen(PS_NULL, 0, NULL);
			SelectObject(memDC, brushtmp);
			SelectObject(memDC, pentmp);
			Rectangle(memDC, 0, 0, 2000, 2000);
			DeleteObject(pentmp);
			DeleteObject(brushtmp);

			SelectObject(memDC, solidPen);
		}
		BitBlt(hdc, 0, 0, 2000, 2000, memDC, 0, 0, SRCCOPY);
		if (type == ID_FILE_SELECT && x1 != x2 && y1 != y2)
		{
			SelectObject(hdc, dashPen);
			Rectangle(hdc, x1, y1, x2, y2);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(hBitmap);
		DeleteDC(memDC);
		DeleteObject(dashPen);
		DeleteObject(solidPen);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void onLButtonDown(HWND hWnd, LPARAM lParam)
{
	x1 = x2 = GET_X_LPARAM(lParam);
	y1 = y2 = GET_Y_LPARAM(lParam);
	HDC hdc = GetDC(hWnd);

	if (type == ID_FILE_DRAW)
	{
		SetROP2(memDC, R2_COPYPEN);
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, x1, y1);
	}
	BitBlt(hdc, 0, 0, 2000, 2000, memDC, 0, 0, SRCCOPY);

	ReleaseDC(hWnd, hdc);
}

void onMouseMove(HWND hWnd, LPARAM lParam)
{
	HDC hdc = GetDC(hWnd);

	if (type == ID_FILE_DRAW)
	{
		SetROP2(memDC, R2_COPYPEN);
		MoveToEx(memDC, x2, y2, NULL);
		LineTo(memDC, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		x2 = GET_X_LPARAM(lParam);
		y2 = GET_Y_LPARAM(lParam);
		BitBlt(hdc, 0, 0, 2000, 2000, memDC, 0, 0, SRCCOPY);
	}
	else
	{
		SelectObject(hdc, dashPen);
		SetROP2(hdc, R2_NOTXORPEN);
		Rectangle(hdc, x1, y1, x2, y2);
		x2 = GET_X_LPARAM(lParam);
		y2 = GET_Y_LPARAM(lParam);
		Rectangle(hdc, x1, y1, x2, y2);
	}

	ReleaseDC(hWnd, hdc);
}

void onRenderFormat(HWND hWnd, UINT nFormat)
{
	if (nFormat != CF_BITMAP)
		return;

	HDC hdc = GetDC(hWnd);
	HBITMAP clipboardBitmap = CreateCompatibleBitmap(hdc, xc2 - xc1, yc2 - yc1);
	ReleaseDC(hWnd, hdc);
	hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, clipboardBitmap);
	BitBlt(hdc, 0, 0, xc2 - xc1, yc2 - yc1, memDC, xc1, yc1, SRCCOPY);
	SetClipboardData(CF_BITMAP, clipboardBitmap);
	DeleteObject(clipboardBitmap);
	DeleteDC(hdc);
}

void onRenderAllFormats(HWND hWnd)
{
	OpenClipboard(hWnd);
	EmptyClipboard();
	onRenderFormat(hWnd, CF_BITMAP);
	CloseClipboard();
}

void swap(int &x, int &y)
{
	int tmp = x;
	x = y;
	y = tmp;
}