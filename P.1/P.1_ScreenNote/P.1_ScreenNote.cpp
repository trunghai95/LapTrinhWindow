// P.1_ScreenNote.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "P.1_ScreenNote.h"
#include <windowsx.h>
#include <Richedit.h>

#define MAX_LOADSTRING 100
#define ACTIVATE 123456789

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
TCHAR szTransWindowClass[MAX_LOADSTRING] = L"TransparentClass";
TCHAR szTextWindowClass[MAX_LOADSTRING] = L"TextClass";
TCHAR szBitmapWindowClass[MAX_LOADSTRING] = L"BitmapClass";

HWND hTransWnd, hTextWnd, hBitmapWnd, hEditWnd, hButtonsDlg;
HINSTANCE hLib;
HBITMAP hbm;
HDC memDC;
int drawType, scrWidth, scrHeight, x1, y1, x2, y2;
bool on;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
ATOM				RegisterClassTrans();
ATOM				RegisterClassText();
ATOM				RegisterClassBitmap();
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	TransWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	TextWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	BitmapWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);
VOID OnMouseMove(HWND, LPARAM);
VOID OnLButtonDown(HWND, LPARAM);
VOID OnLButtonUp(HWND, LPARAM);

extern VOID InstallHook(HWND);

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
	LoadString(hInstance, IDC_P1_SCREENNOTE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	RegisterClassTrans();
	RegisterClassText();
	RegisterClassBitmap();

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_P1_SCREENNOTE));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_P1_SCREENNOTE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

ATOM RegisterClassTrans()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = TransWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szTransWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

ATOM RegisterClassText()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = TextWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szTextWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

ATOM RegisterClassBitmap()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = BitmapWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szBitmapWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
   
   hTransWnd = CreateWindow(szTransWindowClass, NULL, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);
   ShowWindow(hTransWnd, SW_SHOWMAXIMIZED);

   hTextWnd = CreateWindow(szTextWindowClass, NULL, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);
   SetWindowPos(hTextWnd, HWND_NOTOPMOST, 0.05 * scrWidth, 0.1 * scrHeight,
	   0.4 * scrWidth, 0.8 * scrHeight, SWP_SHOWWINDOW);
   ShowWindow(hTextWnd, SW_SHOW);

   hBitmapWnd = CreateWindow(szBitmapWindowClass, NULL, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);
   SetWindowPos(hBitmapWnd, HWND_NOTOPMOST, 0.55 * scrWidth, 0.1 * scrHeight,
	   0.4 * scrWidth, 0.8 * scrHeight, SWP_SHOWWINDOW);
   ShowWindow(hBitmapWnd, SW_SHOW);

   hButtonsDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_BUTTONS), hWnd, DlgProc);
   RECT rc;
   GetClientRect(hButtonsDlg, &rc);
   SetWindowPos(hButtonsDlg, HWND_NOTOPMOST, 0.95*scrWidth - rc.right, 0.1*scrHeight - rc.bottom,
	   rc.right, rc.bottom, SWP_SHOWWINDOW);
   ShowWindow(hButtonsDlg, SW_SHOW);

   SetFocus(hButtonsDlg);
   SetFocus(hTextWnd);
   SetFocus(hBitmapWnd);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow);
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
	switch (message)
	{
	case WM_CREATE:
		hLib = LoadLibrary(L"Msftedit.dll");
		SetFocus(hTransWnd);
		InstallHook(hWnd);
		on = TRUE;
		break;
	case WM_RBUTTONDOWN:
		if (wParam == ACTIVATE)
		{
			on = !on;
			ShowWindow(hTransWnd, on ? SW_SHOWMAXIMIZED : SW_HIDE);
			ShowWindow(hTextWnd, on ? SW_SHOW : SW_HIDE);
			ShowWindow(hBitmapWnd, on ? SW_SHOW : SW_HIDE);
			ShowWindow(hButtonsDlg, on ? SW_SHOW : SW_HIDE);
			SetFocus(hTransWnd);
		}
		break;
	case WM_COMMAND:
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_PAINT:
		break;
	case WM_DESTROY:
		FreeLibrary(hLib);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK TransWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HMONITOR hMonitor;
	MONITORINFO mInfo;

	switch (message)
	{
	case WM_CREATE:
		hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
		mInfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hMonitor, &mInfo);
		scrWidth = mInfo.rcMonitor.right - mInfo.rcMonitor.left;
		scrHeight = mInfo.rcMonitor.bottom - mInfo.rcMonitor.top;

		SetWindowLong(hWnd, GWL_STYLE, 0);
		SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd, 0, 1, LWA_ALPHA);
		break;
	case WM_SETFOCUS:
		SetFocus(hButtonsDlg);
		SetFocus(hTextWnd);
		SetFocus(hBitmapWnd);
		break;
	case WM_COMMAND:
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK TextWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		SetWindowLong(hWnd, GWL_STYLE, WS_BORDER);
		hEditWnd = CreateWindowEx(0, MSFTEDIT_CLASS, NULL, WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | ES_MULTILINE,
			0, 0, 0, 0, hWnd, NULL, hInst, NULL);
		ShowWindow(hEditWnd, SW_SHOW);
		break;
	case WM_SIZE:
		MoveWindow(hEditWnd, 0, 0, LOWORD(lParam), HIWORD(lParam), false);
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK BitmapWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		SetWindowLong(hWnd, GWL_STYLE, WS_BORDER);
		memDC = CreateCompatibleDC(NULL);
		hbm = NULL;
		drawType = IDC_BUTTONF;
		break;
	case WM_COMMAND:
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		if (hbm == NULL)
		{
			hbm = CreateCompatibleBitmap(hdc, 2000, 2000);
			SelectObject(memDC, hbm);
			SelectObject(memDC, CreateSolidBrush(RGB(255, 255, 255)));
			Rectangle(memDC, -1, -1, 2000, 2000);
		}
		BitBlt(hdc, 0, 0, 2000, 2000, memDC, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(hWnd, lParam);
		break;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON)
			OnMouseMove(hWnd, lParam);
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(hWnd, lParam);
		break;
	case WM_DESTROY:
		DeleteDC(memDC);
		DeleteObject(hbm);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hDlg, IDC_BUTTONF), BM_SETSTATE, TRUE, NULL);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTONE: case IDC_BUTTONF: case IDC_BUTTONL: case IDC_BUTTONR:
			drawType = LOWORD(wParam);
			SendMessage(GetDlgItem(hDlg, IDC_BUTTONE), BM_SETSTATE, FALSE, NULL);
			SendMessage(GetDlgItem(hDlg, IDC_BUTTONF), BM_SETSTATE, FALSE, NULL);
			SendMessage(GetDlgItem(hDlg, IDC_BUTTONL), BM_SETSTATE, FALSE, NULL);
			SendMessage(GetDlgItem(hDlg, IDC_BUTTONR), BM_SETSTATE, FALSE, NULL);
			SendMessage(GetDlgItem(hDlg, LOWORD(wParam)), BM_SETSTATE, TRUE, NULL);
			break;
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return (INT_PTR)FALSE;
}

VOID OnLButtonDown(HWND hWnd, LPARAM lParam)
{
	x1 = x2 = GET_X_LPARAM(lParam);
	y1 = y2 = GET_Y_LPARAM(lParam);

	if (drawType == IDC_BUTTONF)
	{
		HDC hdc = GetDC(hWnd);
		SetROP2(memDC, R2_COPYPEN);
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, x1, y1);
		BitBlt(hdc, 0, 0, 2000, 2000, memDC, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, hdc);
	}
}

VOID OnMouseMove(HWND hWnd, LPARAM lParam)
{
	HDC hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);

	switch (drawType)
	{
	case IDC_BUTTONE:
		Ellipse(hdc, x1, y1, x2, y2);
		x2 = GET_X_LPARAM(lParam);
		y2 = GET_Y_LPARAM(lParam);
		Ellipse(hdc, x1, y1, x2, y2);
		break;
	case IDC_BUTTONF:
		SetROP2(memDC, R2_COPYPEN);
		MoveToEx(memDC, x2, y2, NULL);
		x2 = GET_X_LPARAM(lParam);
		y2 = GET_Y_LPARAM(lParam);
		LineTo(memDC, x2, y2);
		BitBlt(hdc, 0, 0, 2000, 2000, memDC, 0, 0, SRCCOPY);
		break;
	case IDC_BUTTONL:
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x2, y2);
		x2 = GET_X_LPARAM(lParam);
		y2 = GET_Y_LPARAM(lParam);
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x2, y2);
		break;
	case IDC_BUTTONR:
		Rectangle(hdc, x1, y1, x2, y2);
		x2 = GET_X_LPARAM(lParam);
		y2 = GET_Y_LPARAM(lParam);
		Rectangle(hdc, x1, y1, x2, y2);
		break;
	}

	ReleaseDC(hWnd, hdc);
}

VOID OnLButtonUp(HWND hWnd, LPARAM lParam)
{
	HDC hdc = GetDC(hWnd);
	SetROP2(memDC, R2_MASKPEN);
	x2 = GET_X_LPARAM(lParam);
	y2 = GET_Y_LPARAM(lParam);

	switch (drawType)
	{
	case IDC_BUTTONE:
		Ellipse(memDC, x1, y1, x2, y2);
		break;
	case IDC_BUTTONL:
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, x2, y2);
		break;
	case IDC_BUTTONR:
		Rectangle(memDC, x1, y1, x2, y2);
		break;
	}

	BitBlt(hdc, 0, 0, 2000, 2000, memDC, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hdc);
}