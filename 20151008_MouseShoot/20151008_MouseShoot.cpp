// 20151008_MouseShoot.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "20151008_MouseShoot.h"
#include <commdlg.h>
#include <windowsx.h>
#include "Mouse.h"
#include <vector>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
TCHAR livePath[MAX_LOADSTRING];
TCHAR deadPath[MAX_LOADSTRING];
HBITMAP liveMouse, deadMouse;
std::vector<Mouse> mouses;
std::vector<Mouse> deadMouses;
int gameStatus; // 1: running, 0: not running
int maxMouseNum;
int timeInterval;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	settingProc(HWND, UINT, WPARAM, LPARAM);
bool openFile(HWND hWnd, TCHAR* buffer);

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
	LoadString(hInstance, IDC_MY20151008_MOUSESHOOT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY20151008_MOUSESHOOT));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY20151008_MOUSESHOOT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY20151008_MOUSESHOOT);
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
	static RECT rc;
	TCHAR buffer[100];

	switch (message)
	{
	case WM_CREATE:
		timeInterval = 50;
		maxMouseNum = 1;
		SetTimer(hWnd, IDT_TIMER, timeInterval, NULL);
		EnableMenuItem(GetMenu(hWnd), ID_FILE_END, MFS_DISABLED | MF_BYCOMMAND);
		EnableMenuItem(GetMenu(hWnd), ID_FILE_START, MFS_DISABLED | MF_BYCOMMAND);
		GetClientRect(hWnd, &rc);
		break;
	case WM_TIMER:
		if (gameStatus == 1)
		{
			mouses.clear();
			while (mouses.size() < maxMouseNum)
				mouses.push_back(Mouse(rc.right - 20, rc.bottom - 20));
			InvalidateRgn(hWnd, NULL, TRUE);
		}
		break;
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
		case ID_FILE_SETTINGS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTING), hWnd, settingProc);
			if (wcslen(livePath) && wcslen(deadPath))
				EnableMenuItem(GetMenu(hWnd), ID_FILE_START, MFS_ENABLED | MF_BYCOMMAND);
			SetTimer(hWnd, IDT_TIMER, timeInterval, NULL);
			break;
		case ID_FILE_START:
			gameStatus = 1;
			EnableMenuItem(GetMenu(hWnd), ID_FILE_START, MFS_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hWnd), ID_FILE_END, MFS_ENABLED | MF_BYCOMMAND);
			break;
		case ID_FILE_END:
			gameStatus = 0;
			EnableMenuItem(GetMenu(hWnd), ID_FILE_END, MFS_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hWnd), ID_FILE_START, MFS_ENABLED | MF_BYCOMMAND);
			_itow(deadMouses.size(), buffer, 10);
			MessageBox(hWnd, buffer, L"Points", MB_OK);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		
	case WM_LBUTTONDOWN:
		if (gameStatus)
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			for (int i = 0; i < mouses.size(); ++i)
			{
				if (mouses[i].getStatus() == 0) continue;
				BITMAP bm;
				GetObject(liveMouse, sizeof(BITMAP), &bm);
				if (x >= mouses[i].getX() && y >= mouses[i].getY()
					&& x <= mouses[i].getX() + bm.bmWidth && y <= mouses[i].getY() + bm.bmHeight)
				{
					deadMouses.push_back(mouses[i]);
					mouses.erase(mouses.begin() + i);
				}
			}
			InvalidateRgn(hWnd, NULL, TRUE);
		}
		break;
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		for (int i = 0; i < mouses.size(); ++i)
		{
			//if (mouses[i].getStatus() == 1)
				mouses[i].draw(hdc, liveMouse);
		}
		for (int i = 0; i < deadMouses.size(); ++i)
		{
			//if (deadMouses[i].getStatus() == 0)
				deadMouses[i].draw(hdc, deadMouse);
		}
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

INT_PTR CALLBACK settingProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR _live[MAX_LOADSTRING];
	static TCHAR _dead[MAX_LOADSTRING];
	TCHAR buffer[MAX_LOADSTRING];
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		Edit_SetText(GetDlgItem(hDlg, IDC_PATHLIVE), livePath);
		Edit_SetText(GetDlgItem(hDlg, IDC_PATHDEAD), deadPath);
		_itow(maxMouseNum, buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_MAXMOUSE), buffer);
		_itow(timeInterval, buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_APPEARTIME), buffer);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			memcpy(livePath, _live, (wcslen(_live) + 1) * sizeof(TCHAR));
			memcpy(deadPath, _dead, (wcslen(_dead) + 1) * sizeof(TCHAR));
			if (wcslen(deadPath) > 0)
				deadMouse = (HBITMAP)LoadImage(NULL, deadPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			if (wcslen(livePath) > 0)
				liveMouse = (HBITMAP)LoadImage(NULL, livePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			Edit_GetText(GetDlgItem(hDlg, IDC_MAXMOUSE), buffer, MAX_LOADSTRING);
			maxMouseNum = _wtoi(buffer);
			Edit_GetText(GetDlgItem(hDlg, IDC_APPEARTIME), buffer, MAX_LOADSTRING);
			timeInterval = _wtoi(buffer);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDC_BROWSELIVE:
			if (!openFile(hDlg, buffer))
				MessageBox(hDlg, L"File not found", L"Error", MB_OK);
			else
			{
				memcpy(_live, buffer, MAX_LOADSTRING);
				Edit_SetText(GetDlgItem(hDlg, IDC_PATHLIVE), _live);
			}
			return(INT_PTR)TRUE;
		case IDC_BROWSEDEAD:
			if (!openFile(hDlg, buffer))
				MessageBox(hDlg, L"File not found", L"Error", MB_OK);
			else
			{
				memcpy(_dead, buffer, MAX_LOADSTRING);
				Edit_SetText(GetDlgItem(hDlg, IDC_PATHDEAD), _dead);
			}
			return(INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}

bool openFile(HWND hWnd,TCHAR* buffer)
{
	OPENFILENAME ofn; // CTDL dùng cho dialog open
	TCHAR szFile[256];
	TCHAR szFilter[] = TEXT("Bitmap file\0 *.bmp\0");
	szFile[0] = '\0';
	// Khởi tạo struct
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd; // handle của window cha
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile; // chuỗi tên file trả về
	ofn.nMaxFile = sizeof(szFile);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn)) {
		memcpy(buffer, ofn.lpstrFile, (wcslen(ofn.lpstrFile) + 1) * sizeof(TCHAR)); // xử lý mở file
		return true;
	}
	else
		return false;
}