// H.4.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "H.4.h"
#include <windowsx.h>
#include <string>
#include <vector>

#define MAX_LOADSTRING 100
#define MARGIN 10

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ColorFigure(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ListBox(HWND, UINT, WPARAM, LPARAM);

int _color, _figure;

void drawFigure(HWND, int, int);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	_color = IDC_BLACK;
	_figure = IDC_RECT;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_H4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_H4));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_H4));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_H4);
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
		case ID_FILE_COLOR:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_COLORFIGURE), hWnd, ColorFigure);
			break;
		case ID_FILE_LISTBOX:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_LIST), hWnd, ListBox);
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

INT_PTR CALLBACK ColorFigure(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	static HWND picControl;
	static int color, figure;
	switch (message)
	{
	case WM_INITDIALOG:
		color = _color;
		figure = _figure;
		CheckRadioButton(hDlg, IDC_BLACK, IDC_WHITE, color);
		CheckRadioButton(hDlg, IDC_RECT, IDC_ELLIPSE, figure);
		picControl = GetDlgItem(hDlg, IDC_PIC);
		drawFigure(picControl, color, figure);
		SetFocus(GetDlgItem(hDlg, color));
		return (INT_PTR)FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			_color = color;
			_figure = figure;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDC_BLACK: case IDC_BLUE: case IDC_GREEN: case IDC_CYAN:
		case IDC_RED: case IDC_MAGENTA: case IDC_YELLOW: case IDC_WHITE:
			color = LOWORD(wParam);
			CheckRadioButton(hDlg, IDC_BLACK, IDC_WHITE, color);
			CheckRadioButton(hDlg, IDC_RECT, IDC_ELLIPSE, figure);
			drawFigure(picControl, color, figure);
			return (INT_PTR)TRUE;
		case IDC_RECT: case IDC_ELLIPSE:
			figure = LOWORD(wParam);
			CheckRadioButton(hDlg, IDC_RECT, IDC_ELLIPSE, figure);
			CheckRadioButton(hDlg, IDC_BLACK, IDC_WHITE, color);
			drawFigure(picControl, color, figure);
			return (INT_PTR)TRUE;
		}
		break;
	case WM_PAINT:
		drawFigure(picControl, _color, _figure);
		break;
	case WM_CLOSE:
		EndDialog(hDlg, IDCANCEL);
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ListBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static std::vector<std::wstring> strs;
	TCHAR buffer[MAX_LOADSTRING + 1];
	int tmp;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTONADD), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTONDEL), FALSE);
		if (strs.empty())
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTONDELALL), FALSE);
		else for (int i = 0; i < strs.size(); ++i)
			SendMessage(GetDlgItem(hDlg, IDC_LIST), LB_ADDSTRING, NULL, (LPARAM)strs[i].c_str());
		return (INT_PTR)FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		case IDC_LIST:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTONDEL), TRUE);
			return (INT_PTR)TRUE;

		case IDC_EDITNAME:
			if (HIWORD(wParam) == EN_CHANGE)
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTONADD),
						GetWindowTextLength(GetDlgItem(hDlg, IDC_EDITNAME)) != 0);
			return (INT_PTR)TRUE;

		case IDC_BUTTONADD:
			Edit_GetText(GetDlgItem(hDlg, IDC_EDITNAME), buffer, MAX_LOADSTRING);
			SendMessage(GetDlgItem(hDlg, IDC_LIST), LB_ADDSTRING, NULL, (LPARAM)buffer);
			strs.push_back(std::wstring(buffer));
			Edit_SetText(GetDlgItem(hDlg, IDC_EDITNAME), NULL);
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTONDELALL), TRUE);
			return (INT_PTR)TRUE;

		case IDC_BUTTONDEL:
			tmp = SendMessage(GetDlgItem(hDlg, IDC_LIST), LB_GETCURSEL, NULL, NULL);
			strs.erase(strs.begin() + tmp);
			SendMessage(GetDlgItem(hDlg, IDC_LIST), LB_DELETESTRING, tmp, NULL);
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTONDEL), FALSE);
			if (strs.empty())
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTONDELALL), FALSE);
			return (INT_PTR)TRUE;

		case IDC_BUTTONDELALL:
			SendMessage(GetDlgItem(hDlg, IDC_LIST), LB_RESETCONTENT, NULL, NULL);
			strs.clear();
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTONDEL), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTONDELALL), FALSE);
			return (INT_PTR)TRUE;
		}
		break;

	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return (INT_PTR)FALSE;
}

void drawFigure(HWND picControl, int color, int figure)
{
	HDC hdc = GetDC(picControl);
	COLORREF c;
	RECT rc;

	GetClientRect(WindowFromDC(hdc), &rc);

	switch (color)
	{
	case IDC_BLACK: c = RGB(0, 0, 0); break;
	case IDC_BLUE: c = RGB(0, 0, 255); break;
	case IDC_GREEN: c = RGB(0, 255, 0); break;
	case IDC_CYAN: c = RGB(0, 255, 255); break;
	case IDC_RED: c = RGB(255, 0, 0); break;
	case IDC_MAGENTA: c = RGB(255, 0, 255); break;
	case IDC_YELLOW: c = RGB(255, 255, 0); break;
	case IDC_WHITE: c = RGB(255, 255, 255); break;
	}

	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	
	SelectObject(hdc, CreateSolidBrush(c));

	if (figure == IDC_RECT) Rectangle(hdc, rc.left + MARGIN, rc.top + MARGIN,
										rc.right - MARGIN, rc.bottom - MARGIN);
	else Ellipse(hdc, rc.left + MARGIN, rc.top + MARGIN,
					rc.right - MARGIN, rc.bottom - MARGIN);

	ReleaseDC(WindowFromDC(hdc), hdc);

	return;
}