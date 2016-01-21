// 1312165_CK.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1312165_CK.h"
#include <objbase.h>
#include <ShlObj.h>

#define MAX_LOADSTRING 100
#define MARGIN 5
#define SIZE1 0.1
#define SIZE2 0.7

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

int width = -1, height = -1;
TCHAR path[2][MAX_LOADSTRING];
HANDLE mutex, thread[2];
HWND g_hWnd;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void onPaint(HWND);
void choosePath(HWND, int);
DWORD WINAPI ThreadFunc(LPVOID lpParam);

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
	LoadString(hInstance, IDC_MY1312165_CK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1312165_CK));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1312165_CK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY1312165_CK);
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

   g_hWnd = hWnd = CreateWindow(szWindowClass, L"Quick Image Viewer", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_MAXIMIZE);
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

	switch (message)
	{
	case WM_CREATE:
		path[0][0] = path[1][0] = '\0';
		mutex = CreateMutex(0, 0, 0);
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_CHON1:
			choosePath(hWnd, 0);
			break;
		case ID_CHON2:
			choosePath(hWnd, 1);
			break;
		case ID_XEM:
			if (wcslen(path[0]) == 0 || wcslen(path[1]) == 0)
			{
				MessageBox(hWnd, L"Chưa chọn đủ 2 folder", L"Lỗi", MB_OK);
				break;
			}
			for (int i = 0; i < 2; ++i)
			{
				TerminateThread(thread[i], 0);
				thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, (LPVOID)i, 0, 0);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		onPaint(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void onPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	RECT rc;
	GetClientRect(hWnd, &rc);
	width = rc.right;
	height = rc.bottom;

	HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	SelectObject(hdc, pen);
	MoveToEx(hdc, width / 2, 0, NULL);
	LineTo(hdc, width / 2, height);
	MoveToEx(hdc, 0, SIZE1*height, NULL);
	LineTo(hdc, width, SIZE1*height);
	MoveToEx(hdc, 0, (SIZE1 + SIZE2)*height, NULL);
	LineTo(hdc, width, (SIZE1 + SIZE2)*height);

	for (int id = 0; id < 2; ++id)
		TextOut(hdc, MARGIN + id*(width / 2), MARGIN, path[id], wcslen(path[id]));
	
	DeleteObject(pen);
	EndPaint(hWnd, &ps);
}

void choosePath(HWND hWnd, int id)
{
	BROWSEINFO br;
	TCHAR res[MAX_LOADSTRING];
	res[0] = '\0';
	br.hwndOwner = hWnd;
	br.pidlRoot = NULL;
	br.pszDisplayName = res;
	br.lpszTitle = L"Chọn folder";
	br.ulFlags = BIF_RETURNONLYFSDIRS | BIF_BROWSEINCLUDEURLS;
	br.lpfn = NULL;
	br.lParam = NULL;
	br.iImage = NULL;

	LPITEMIDLIST lpItem = SHBrowseForFolder(&br);
	if (lpItem != NULL)
	{
		SHGetPathFromIDList(lpItem, path[id]);
		if (path[id][wcslen(path[id]) - 1] != L'\\')
			wcscat(path[id], L"\\");
		InvalidateRgn(hWnd, NULL, TRUE);
	}
}

DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
	int id = (int)lpParam;
	int cnt = 0, maxW = 0, maxH = 0;
	TCHAR buffer[MAX_LOADSTRING], maxFile[MAX_LOADSTRING];
	maxFile[0] = '\0';

	WIN32_FIND_DATA fd;
	wsprintf(buffer, L"%s*.*", path[id]);

	HANDLE hFind = FindFirstFile(buffer, &fd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return 1;
	}

	do
	{
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			++cnt;
			wsprintf(buffer, L"%s%s", path[id], fd.cFileName);
			HBITMAP hbm = (HBITMAP)LoadImage(NULL, buffer, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			BITMAP bm;
			GetObject(hbm, sizeof(BITMAP), &bm);
			HDC memDC = CreateCompatibleDC(NULL);
			SelectObject(memDC, hbm);
			int newW = bm.bmWidth, newH = bm.bmHeight;
			if (newW > width / 2 - MARGIN * 2)
			{
				newH = newH * (width / 2 - MARGIN * 2) / newW;
				newW = width / 2 - MARGIN * 2;
			}
			if (newH > SIZE2*height - MARGIN * 2)
			{
				newW = newW * (SIZE2*height - MARGIN * 2) / newH;
				newH = SIZE2*height - MARGIN * 2;
			}

			if (bm.bmWidth*bm.bmHeight > maxW*maxH)
			{
				maxW = bm.bmWidth;
				maxH = bm.bmHeight;
				wcscpy(maxFile, fd.cFileName);
			}

			while (1)
			{
				if (WaitForSingleObject(mutex, INFINITE) == 0)
				{
					HDC hdc = GetDC(g_hWnd);
					HPEN pen = CreatePen(PS_NULL, 0, NULL);
					SelectObject(hdc, pen);
					Rectangle(hdc, MARGIN + id*(width / 2), SIZE1*height + MARGIN,
						(id+1)*(width/2) - MARGIN, (SIZE1 + SIZE2)*height - MARGIN);
					
					StretchBlt(hdc, MARGIN + id*(width / 2), SIZE1*height + MARGIN,
						newW, newH, memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

					ReleaseDC(g_hWnd, hdc);
					DeleteObject(pen);
					ReleaseMutex(mutex);
					Sleep(1000);
					break;
				}
			}
		}
	} while (FindNextFile(hFind, &fd));
	FindClose(hFind);

	wsprintf(buffer, L"Tổng số ảnh: %d\nẢnh lớn nhất: %s\nKích thước: %dx%d", cnt, maxFile, maxW, maxH);
	RECT rc;
	rc.left = MARGIN + id*(width / 2);
	rc.top = (SIZE1 + SIZE2)*height + MARGIN;
	rc.right = (id + 1)*(width / 2) - MARGIN;
	rc.bottom = height - MARGIN;

	while (1)
	{
		if (WaitForSingleObject(mutex, INFINITE) == 0)
		{
			HDC hdc = GetDC(g_hWnd);

			HPEN pen = CreatePen(PS_NULL, 0, NULL);
			SelectObject(hdc, pen);
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

			DrawText(hdc, buffer, wcslen(buffer), &rc, DT_LEFT | DT_TOP | DT_WORDBREAK);

			ReleaseDC(g_hWnd, hdc);
			DeleteObject(pen);
			ReleaseMutex(mutex);
			break;
		}
	}

	return 0;
}