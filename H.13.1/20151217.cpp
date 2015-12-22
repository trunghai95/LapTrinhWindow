// 20151217.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "20151217.h"
#include <Vfw.h>
#include <commdlg.h>
#include <windowsx.h>

#pragma comment(lib, "vfw32.lib")

#define MAX_LOADSTRING 100
#define STOP 0
#define PLAY 1
#define PAUSE 2

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND g_hwndMCIWnd;
int volume;
int state;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);

bool openFile(HWND hWnd, TCHAR* buffer);
void enableRWFW(HWND hDlg, bool en);
void seek(int val);

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
	LoadString(hInstance, IDC_MY20151217, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY20151217));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY20151217));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY20151217);
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

   hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), NULL, DlgProc);

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
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR buffer[200];

	LRESULT lResult;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTNOPEN:
			if (openFile(hDlg, buffer))
			{
				if (g_hwndMCIWnd != NULL)
					MCIWndClose(g_hwndMCIWnd);
				if (g_hwndMCIWnd = MCIWndCreate(hDlg,
					hInst,
					WS_CHILD | WS_VISIBLE |    // standard styles
					MCIWNDF_NOPLAYBAR |        // hides toolbar 
					MCIWNDF_NOTIFYMODE,        // notifies of mode changes
					buffer))
				{
					SetWindowText(hDlg, buffer);
					state = STOP;
					volume = MCIWndGetVolume(g_hwndMCIWnd);
					EnableWindow(GetDlgItem(hDlg, IDC_BTNPLAY), TRUE);
					_itow(volume/10, buffer, 10);
					Edit_SetText(GetDlgItem(hDlg, IDC_EDTVOL), buffer);
					EnableWindow(GetDlgItem(hDlg, IDC_BTNVOLUP), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDC_BTNVOLDOWN), TRUE);
				}
				else
				{
					EnableWindow(GetDlgItem(hDlg, IDC_BTNPLAY), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDC_BTNVOLUP), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDC_BTNVOLDOWN), FALSE);
				}
				SetDlgItemText(hDlg, IDC_BTNPLAY, L"Play");
				EnableWindow(GetDlgItem(hDlg, IDC_BTNSTOP), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTNRW), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTNFW), FALSE);
			}
			break;
		case IDC_BTNPLAY:
			switch (state)
			{
			case STOP:
				MCIWndPlay(g_hwndMCIWnd);
				state = PLAY;
				SetDlgItemText(hDlg, IDC_BTNPLAY, L"Pause");
				EnableWindow(GetDlgItem(hDlg, IDC_BTNSTOP), TRUE);
				enableRWFW(hDlg, TRUE);
				break;
			case PLAY:
				MCIWndPause(g_hwndMCIWnd);
				state = PAUSE;
				SetDlgItemText(hDlg, IDC_BTNPLAY, L"Resume");
				enableRWFW(hDlg, FALSE);
				break;
			case PAUSE:
				MCIWndResume(g_hwndMCIWnd);
				state = PLAY;
				SetDlgItemText(hDlg, IDC_BTNPLAY, L"Pause");
				enableRWFW(hDlg, TRUE);
				break;
			}
			break;
		case IDC_BTNSTOP:
			MCIWndStop(g_hwndMCIWnd);
			state = STOP;
			MCIWndSeek(g_hwndMCIWnd, 0);
			SetDlgItemText(hDlg, IDC_BTNPLAY, L"Play");
			EnableWindow(GetDlgItem(hDlg, IDC_BTNSTOP), FALSE);
			enableRWFW(hDlg, FALSE);
			break;
		case IDC_BTNRW:
			seek(-1000);
			MCIWndPlay(g_hwndMCIWnd);
			break;
		case IDC_BTNFW:
			seek(1000);
			MCIWndPlay(g_hwndMCIWnd);
			break;
		case IDC_BTNVOLUP:
			volume += 50;
			if (volume > 1000) volume = 1000;
			MCIWndSetVolume(g_hwndMCIWnd, volume);
			_itow(volume/10, buffer, 10);
			Edit_SetText(GetDlgItem(hDlg, IDC_EDTVOL), buffer);
			break;
		case IDC_BTNVOLDOWN:
			volume -= 50;
			if (volume < 0) volume = 0;
			MCIWndSetVolume(g_hwndMCIWnd, volume);
			_itow(volume/10, buffer, 10);
			Edit_SetText(GetDlgItem(hDlg, IDC_EDTVOL), buffer);
			break;
		}
		break;

	case WM_CLOSE:
		if (g_hwndMCIWnd)
			MCIWndDestroy(g_hwndMCIWnd);
		PostQuitMessage(0);
		break;
	}
	return (INT_PTR)FALSE;
}

bool openFile(HWND hWnd, TCHAR* buffer)
{
	OPENFILENAME ofn; // CTDL dùng cho dialog open
	TCHAR szFile[256];
	TCHAR szFilter[] = TEXT("Media files\0 *.avi;*.wma;*.wmv;*.mp3\0");
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

void enableRWFW(HWND hDlg, bool en)
{
	EnableWindow(GetDlgItem(hDlg, IDC_BTNRW), en);
	EnableWindow(GetDlgItem(hDlg, IDC_BTNFW), en);
}

void seek(int dis)
{
	int pos = MCIWndGetPosition(g_hwndMCIWnd);
	pos += dis;
	int len = MCIWndGetLength(g_hwndMCIWnd);
	if (pos > len) pos = len;
	if (pos < 0) pos = 0;
	MCIWndSeek(g_hwndMCIWnd, pos);
}