// H.5.1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "H.5.1.h"
#include <sstream>
#include "Struct.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND hWndMDIClient;
HWND hFrameWnd;
INT txtWndCnt, bmpWndCnt;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				RegisterFrameWndClass();
BOOL				RegisterTextWndClass();
BOOL				RegisterBitmapWndClass();
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	FrameWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	TextWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	BitmapWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK	MDICloseProc(HWND, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
VOID				CreateNewTextWnd();
VOID				CreateNewBitmapWnd();
VOID				enableItems(BOOL txt, BOOL bmp);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	txtWndCnt = bmpWndCnt = 0;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_H51, szWindowClass, MAX_LOADSTRING);
	//MyRegisterClass(hInstance);
	RegisterFrameWndClass();
	RegisterTextWndClass();
	RegisterBitmapWndClass();

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_H51));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateMDISysAccel(hWndMDIClient, &msg) &&
			!TranslateAccelerator(hFrameWnd, hAccelTable, &msg))
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_H51));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL RegisterFrameWndClass()
{
	WNDCLASS wc;
	wc.lpszClassName = L"MDI_FRAME";
	wc.lpfnWndProc = (WNDPROC)FrameWndProc;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_H51));
	wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDC_H51);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	if (!RegisterClass(&wc))
		return FALSE;
	return TRUE;
}

BOOL RegisterTextWndClass()
{
	WNDCLASS wc;
	wc.lpszClassName = TEXT("MDI_TEXT_CHILD");
	wc.lpfnWndProc = (WNDPROC)TextWndProc;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wc.hIcon = LoadIcon(hInst, (LPCWSTR)IDI_H51);
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.cbWndExtra = 4; 
	wc.cbClsExtra = 0;
	if (!RegisterClass(&wc))
		return FALSE;
	return TRUE;
}

BOOL RegisterBitmapWndClass()
{
	WNDCLASS wc;
	wc.lpszClassName = L"MDI_BITMAP_CHILD";
	wc.lpfnWndProc = (WNDPROC)BitmapWndProc;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CROSS));
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_H51));
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.cbWndExtra = 4;
	wc.cbClsExtra = 0;
	if (!RegisterClass(&wc))
		return FALSE;
	return TRUE;
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

   hWnd = CreateWindow(L"MDI_FRAME", szTitle, WS_OVERLAPPEDWINDOW,
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

LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hActiveChild;
	switch (message)
	{
	case WM_CREATE:
		CLIENTCREATESTRUCT ccs;
		ccs.hWindowMenu = GetSubMenu(GetMenu(hWnd), 4);
		hWndMDIClient = CreateWindow(L"MDICLIENT", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL,
			0, 0, 0, 0, hWnd, (HMENU)NULL, hInst, &ccs);
		ShowWindow(hWndMDIClient, SW_SHOW);
		hFrameWnd = hWnd;
		return 0;
	case WM_SIZE:
		UINT w, h;
		w = LOWORD(lParam);
		h = HIWORD(lParam);
		MoveWindow(hWndMDIClient, 0, 0, w, h, TRUE);
		return DefFrameProc(hWnd, hWndMDIClient, message, wParam, lParam);
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_NEWTEXT:
			CreateNewTextWnd();
			break;
		case ID_FILE_NEWBITMAP:
			CreateNewBitmapWnd();
			break;
		case ID_EDIT_SELECTALL: case ID_EDIT_PASTE: case ID_EDIT_COPY: case ID_EDIT_CUT:
		case ID_DRAW_LINE: case ID_DRAW_PIXEL: case ID_DRAW_RECTANGLE: case ID_DRAW_ELLIPSE:
		case ID_FORMAT_CHOOSECOLOR: case ID_FORMAT_CHOOSEFONT:
			if ((hActiveChild = GetTopWindow(hWndMDIClient)) != NULL)
				SendMessage(hActiveChild, message, wParam, lParam);
			break;
		case ID_WINDOW_CASCADE:
			SendMessage(hWndMDIClient, WM_MDICASCADE, NULL, NULL);
			break;
		case ID_WINDOW_TILEHORIZONTAL:
			SendMessage(hWndMDIClient, WM_MDITILE, MDITILE_HORIZONTAL | MDITILE_SKIPDISABLED, NULL);
			break;
		case ID_WINDOW_TILEVERTICAL:
			SendMessage(hWndMDIClient, WM_MDITILE, MDITILE_VERTICAL | MDITILE_SKIPDISABLED, NULL);
			break;
		case ID_WINDOW_CLOSEALL:
			EnumChildWindows(hWndMDIClient, MDICloseProc, NULL);
			enableItems(FALSE, FALSE);
			break;
		}
		return DefFrameProc(hWnd, hWndMDIClient, message, wParam, lParam);
	case WM_MENUSELECT:
		if ((hActiveChild = GetTopWindow(hWndMDIClient)) != NULL)
			SendMessage(hActiveChild, message, wParam, lParam);
		else enableItems(FALSE, FALSE);
		break;
	default:
		return DefFrameProc(hWnd, hWndMDIClient, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK TextWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DATA* data = (DATA*)GetWindowLong(hWnd, 0);
	CHOOSECOLOR cc;
	CHOOSEFONT cf;
	COLORREF custColor[16];
	switch (message)
	{
	case WM_CREATE:
		data = (DATA*)VirtualAlloc(NULL, sizeof(DATA), MEM_COMMIT, PAGE_READWRITE);
		data->wndType = TEXTWND;
		data->color = RGB(0, 0, 0);
		data->hEditWnd = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE,
			0, 0, 0, 0, hWnd, NULL, hInst, 0L);
		ShowWindow(data->hEditWnd, SW_NORMAL);
		data->hWnd = hWnd;
		SetWindowLong(hWnd, 0, (LONG)data);
		SendMessage(data->hEditWnd, WM_SETFONT, (WPARAM)CreateFontIndirect(&data->logFont), NULL);
		return 0;
	case WM_MDIACTIVATE:
		enableItems(TRUE, FALSE);
		return DefMDIChildProc(hWnd, message, wParam, lParam);
	case WM_SIZE:
		MoveWindow(data->hEditWnd, 0, 0, LOWORD(lParam), HIWORD(lParam), 0);
		return DefMDIChildProc(hWnd, message, wParam, lParam);
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FORMAT_CHOOSECOLOR:
			ZeroMemory(&cc, sizeof(CHOOSECOLOR));
			cc.lStructSize = sizeof(CHOOSECOLOR);
			cc.hwndOwner = hWnd;
			cc.lpCustColors = custColor;
			cc.rgbResult = data->color;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;
			if (ChooseColor(&cc))
			{
				data->color = cc.rgbResult;
				//...............
			}
			break;
		case ID_FORMAT_CHOOSEFONT:
			ZeroMemory(&cf, sizeof(CHOOSEFONT));
			cf.lStructSize = sizeof(CHOOSEFONT);
			cf.hwndOwner = hWnd;
			cf.lpLogFont = &(data->logFont);
			cf.rgbColors = data->color;
			cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
			if (ChooseFont(&cf))
			{
				data->logFont = *cf.lpLogFont;
				data->color = cf.rgbColors;
				SendMessage(data->hEditWnd, WM_SETFONT, (WPARAM)CreateFontIndirect(&data->logFont), NULL);
			}
			InvalidateRgn(hWnd, NULL, TRUE);
			break;
		case ID_EDIT_COPY:
			SendMessage(data->hEditWnd, WM_COPY, NULL, NULL);
			break;
		case ID_EDIT_CUT:
			SendMessage(data->hEditWnd, WM_CUT, NULL, NULL);
			break;
		case ID_EDIT_PASTE:
			SendMessage(data->hEditWnd, WM_PASTE, NULL, NULL);
			break;
		case ID_EDIT_SELECTALL:
			SendMessage(data->hEditWnd, EM_SETSEL, 0, -1);
			break;
		}
		return 0;
	case WM_MENUSELECT:
		int a;
		a = SendMessage(data->hEditWnd, EM_GETSEL, NULL, NULL);
		if (LOWORD(a) != HIWORD(a))
		{
			EnableMenuItem(GetMenu(hFrameWnd), ID_EDIT_CUT, MFS_ENABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hFrameWnd), ID_EDIT_COPY, MFS_ENABLED | MF_BYCOMMAND);
		}
		else
		{
			EnableMenuItem(GetMenu(hFrameWnd), ID_EDIT_CUT, MFS_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hFrameWnd), ID_EDIT_COPY, MFS_DISABLED | MF_BYCOMMAND);
		}
	default:
		return DefMDIChildProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK BitmapWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DATA* data = (DATA*)GetWindowLong(hWnd, 0);
	CHOOSECOLOR cc;
	COLORREF custColor[16];

	switch (message)
	{
	case WM_CREATE:
		data = (DATA*)VirtualAlloc(NULL, sizeof(DATA), MEM_COMMIT, PAGE_READWRITE);
		data->wndType = BITMAPWND;
		data->hWnd = hWnd;
		data->color = RGB(0, 0, 0);
		data->drawMode = ID_DRAW_LINE;
		//...............
		SetWindowLong(hWnd, 0, (LONG)data);
		return 0;
	case WM_MDIACTIVATE:
		enableItems(FALSE, TRUE);
		CheckMenuItem(GetMenu(hFrameWnd), ID_DRAW_ELLIPSE, MFS_UNCHECKED | MF_BYCOMMAND);
		CheckMenuItem(GetMenu(hFrameWnd), ID_DRAW_LINE, MFS_UNCHECKED | MF_BYCOMMAND);
		CheckMenuItem(GetMenu(hFrameWnd), ID_DRAW_PIXEL, MFS_UNCHECKED | MF_BYCOMMAND);
		CheckMenuItem(GetMenu(hFrameWnd), ID_DRAW_RECTANGLE, MFS_UNCHECKED | MF_BYCOMMAND);
		CheckMenuItem(GetMenu(hFrameWnd), data->drawMode, MFS_CHECKED | MF_BYCOMMAND);
		return DefMDIChildProc(hWnd, message, wParam, lParam);
	case WM_SIZE:
		//................
		return DefMDIChildProc(hWnd, message, wParam, lParam);
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FORMAT_CHOOSECOLOR:
			ZeroMemory(&cc, sizeof(CHOOSECOLOR));
			cc.lStructSize = sizeof(CHOOSECOLOR);
			cc.hwndOwner = hWnd;
			cc.lpCustColors = custColor;
			cc.rgbResult = data->color;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;
			if (ChooseColor(&cc))
			{
				data->color = cc.rgbResult;
			}
			break;
		case ID_DRAW_ELLIPSE: case ID_DRAW_LINE: case ID_DRAW_PIXEL: case ID_DRAW_RECTANGLE:
			data->drawMode = LOWORD(wParam);
			CheckMenuItem(GetMenu(hFrameWnd), ID_DRAW_ELLIPSE, MFS_UNCHECKED | MF_BYCOMMAND);
			CheckMenuItem(GetMenu(hFrameWnd), ID_DRAW_LINE, MFS_UNCHECKED | MF_BYCOMMAND);
			CheckMenuItem(GetMenu(hFrameWnd), ID_DRAW_PIXEL, MFS_UNCHECKED | MF_BYCOMMAND);
			CheckMenuItem(GetMenu(hFrameWnd), ID_DRAW_RECTANGLE, MFS_UNCHECKED | MF_BYCOMMAND);
			CheckMenuItem(GetMenu(hFrameWnd), data->drawMode, MFS_CHECKED | MF_BYCOMMAND);
			break;
		}
		return 0;
	default:
		return DefMDIChildProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL CALLBACK MDICloseProc(HWND hMDIWnd, LPARAM lParam)
{
	SendMessage(hWndMDIClient, WM_MDIDESTROY, (WPARAM)hMDIWnd, 0L);
	return 1;
}

VOID CreateNewTextWnd()
{
	++txtWndCnt;
	std::wstringstream ss;
	ss << L"Text " << txtWndCnt;
	MDICREATESTRUCT mdiCreate;
	wchar_t cc[MAX_LOADSTRING];
	memcpy(cc, ss.str().c_str(), MAX_LOADSTRING);

	mdiCreate.szClass = L"MDI_TEXT_CHILD";
	mdiCreate.szTitle = cc;
	mdiCreate.hOwner = hInst;
	mdiCreate.x = mdiCreate.y = mdiCreate.cx = mdiCreate.cy = CW_USEDEFAULT;
	mdiCreate.style = 0;
	mdiCreate.lParam = NULL;

	SendMessage(hWndMDIClient, WM_MDICREATE, 0, (LONG)(LPMDICREATESTRUCT)&mdiCreate);
}

VOID CreateNewBitmapWnd()
{
	++bmpWndCnt;
	std::wstringstream ss;
	ss << L"Bitmap " << bmpWndCnt;
	MDICREATESTRUCT mdiCreate;
	wchar_t cc[MAX_LOADSTRING];
	memcpy(cc, ss.str().c_str(), MAX_LOADSTRING);

	mdiCreate.szClass = L"MDI_BITMAP_CHILD";
	mdiCreate.szTitle = cc;
	mdiCreate.hOwner = hInst;
	mdiCreate.x = mdiCreate.y = mdiCreate.cx = mdiCreate.cy = CW_USEDEFAULT;
	mdiCreate.style = 0;
	mdiCreate.lParam = NULL;

	SendMessage(hWndMDIClient, WM_MDICREATE, 0, (LONG)(LPMDICREATESTRUCT)&mdiCreate);
}

VOID enableItems(BOOL txt, BOOL bmp)
{
	UINT txtFlag = txt ? MFS_ENABLED : MFS_DISABLED;
	UINT bmpFlag = bmp ? MFS_ENABLED : MFS_DISABLED;
	UINT flag = (txt || bmp) ? MFS_ENABLED : MFS_DISABLED;
	EnableMenuItem(GetMenu(hFrameWnd), ID_FORMAT_CHOOSECOLOR, flag | MF_BYCOMMAND);
	EnableMenuItem(GetMenu(hFrameWnd), ID_FORMAT_CHOOSEFONT, txtFlag | MF_BYCOMMAND);
	EnableMenuItem(GetMenu(hFrameWnd), ID_EDIT_PASTE, txtFlag | MF_BYCOMMAND);
	EnableMenuItem(GetMenu(hFrameWnd), ID_EDIT_SELECTALL, txtFlag | MF_BYCOMMAND);
	EnableMenuItem(GetMenu(hFrameWnd), ID_EDIT_COPY, MFS_DISABLED | MF_BYCOMMAND);
	EnableMenuItem(GetMenu(hFrameWnd), ID_EDIT_CUT, MFS_DISABLED | MF_BYCOMMAND);
	EnableMenuItem(GetMenu(hFrameWnd), ID_DRAW_ELLIPSE, bmpFlag | MF_BYCOMMAND);
	EnableMenuItem(GetMenu(hFrameWnd), ID_DRAW_PIXEL, bmpFlag | MF_BYCOMMAND);
	EnableMenuItem(GetMenu(hFrameWnd), ID_DRAW_RECTANGLE, bmpFlag | MF_BYCOMMAND);
	EnableMenuItem(GetMenu(hFrameWnd), ID_DRAW_LINE, bmpFlag | MF_BYCOMMAND);
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
