// 1312165_GK.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1312165_GK.h"
#include <string>
#include <vector>
#include <fstream>
#include "Struct.h"
#include <windowsx.h>
#include <codecvt>

using namespace std;

#define MAX_LOADSTRING 1000
#define FILEPATH "data.txt"

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

extern TCHAR loaiCT[6][100];
vector<ChiTiet> chiTiet;
int val[6]; // Cộng dồn

COLORREF color[] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0), RGB(255, 0, 255), RGB(0, 255, 255) };

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);

VOID readFile(HWND);
VOID writeFile();
VOID ve(HWND);

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
	LoadString(hInstance, IDC_MY1312165_GK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1312165_GK));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1312165_GK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY1312165_GK);
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
		memset(val, 0, sizeof(val));
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
		writeFile();
		DestroyWindow(hWnd);
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
	UNREFERENCED_PARAMETER(lParam);
	TCHAR buffer[MAX_LOADSTRING], buffer2[MAX_LOADSTRING];
	switch (message)
	{
	case WM_INITDIALOG:
		for (int i = 0; i < 6; ++i)
		{
			SendMessage(GetDlgItem(hDlg, IDC_COMBOLOAI), CB_ADDSTRING, NULL, (LPARAM) loaiCT[i]);
		}

		SendMessage(GetDlgItem(hDlg, IDC_COMBOLOAI), CB_SETCURSEL, NULL, 0);
		readFile(hDlg);

		return (INT_PTR)TRUE;

	case WM_PAINT:
		ve(GetDlgItem(hDlg, IDC_PICTURE));
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDC_EDITNOIDUNG: case IDC_EDITSOTIEN:
			if (Edit_GetTextLength(GetDlgItem(hDlg, IDC_EDITNOIDUNG)) && Edit_GetTextLength(GetDlgItem(hDlg, IDC_EDITSOTIEN)))
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTONTHEM), true);
			else
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTONTHEM), false);
			return (INT_PTR)TRUE;
		case IDC_BUTTONTHEM:
			int cursel = SendMessage(GetDlgItem(hDlg, IDC_COMBOLOAI), CB_GETCURSEL, NULL, NULL);
			Edit_GetText(GetDlgItem(hDlg, IDC_EDITSOTIEN), buffer, MAX_LOADSTRING);
			Edit_GetText(GetDlgItem(hDlg, IDC_EDITNOIDUNG), buffer2, MAX_LOADSTRING);
			ChiTiet ct(loaiCT[cursel], buffer2, buffer);
			chiTiet.push_back(ct);
			ct.addToList(GetDlgItem(hDlg, IDC_LISTDSCT));
			for (int i = cursel; i < 6; ++i)
				val[i] += _wtoi(buffer);
			SetDlgItemInt(hDlg, IDC_EDITTONG, val[5], false);

			ve(GetDlgItem(hDlg, IDC_PICTURE));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

VOID readFile(HWND hDlg)
{
	wifstream is(FILEPATH);

	TCHAR buffer[MAX_LOADSTRING];
	wstring str;
	is.imbue(std::locale(is.getloc(), new std::codecvt_utf8_utf16<TCHAR>));
	while (is.good())
	{
		getline(is, str);
		ChiTiet ct = ChiTiet(str.c_str());
		int sel;
		for (sel = 0; sel < 6; ++sel)
		{
			if (!wcscmp(ct.loai, loaiCT[sel]))
				break;
		}
		for (; sel < 6; ++sel)
			val[sel] += _wtoi(ct.tien);
		chiTiet.push_back(ct);
		ct.addToList(GetDlgItem(hDlg, IDC_LISTDSCT));
	}
	SetDlgItemInt(hDlg, IDC_EDITTONG, val[5], false);

	ve(GetDlgItem(hDlg, IDC_PICTURE));
}

VOID writeFile()
{
	if (chiTiet.empty())
		return;

	wofstream os(FILEPATH);

	if (!os.good())
		return;

	os.imbue(std::locale(os.getloc(), new std::codecvt_utf8_utf16<TCHAR>));

	chiTiet[0].write(os);
	for (int i = 1; i < chiTiet.size(); ++i)
	{
		os << endl;
		chiTiet[i].write(os);
	}
}

VOID ve(HWND picControl)
{
	if (!val[5]) return;
	HDC hdc = GetDC(picControl);
	RECT rc;

	GetClientRect(WindowFromDC(hdc), &rc);
	int w = rc.right, h = rc.bottom;

	for (int i = 5; i >= 0; --i)
	{
		SelectObject(hdc, CreateSolidBrush(color[i]));
		Rectangle(hdc, 0, 0, w*val[i] / val[5], h);
	}

	ReleaseDC(WindowFromDC(hdc), hdc);
}