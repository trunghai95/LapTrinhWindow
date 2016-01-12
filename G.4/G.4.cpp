// G.4.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "G.4.h"
#include <stdlib.h>
#include <time.h>
#include <windowsx.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

int TongSoBong, BongConLai, DoiHienTai, SoBongDaLay[2], TongThoiGian, ThoiGianConLai, SoLanGiuKho[2];
TCHAR* TenDoi[2];
HANDLE mutex;
HANDLE thread[2];

struct ParaThread
{
	int index;
} data[2];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI ThreadFunc1(LPVOID data);
DWORD WINAPI ThreadFunc2(LPVOID data);

void onInit(HWND);
void onStart(HWND);

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
	LoadString(hInstance, IDC_G4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_G4));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_G4));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_G4);
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

   hWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAINDLG), NULL, DlgProc);

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
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		onInit(hDlg);
		_itow(TongSoBong, buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITTONGSOBONG), buffer);
		_itow(TongThoiGian, buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITTONGTHOIGIAN), buffer);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTNSTART:
			onStart(hDlg);
			break;
		}
		break;

	case WM_TIMER:
		ThoiGianConLai -= 200;
		if (ThoiGianConLai <= 0 || BongConLai <= 0)
			KillTimer(hDlg, 1);
		_itow(ThoiGianConLai, buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITTHOIGIANCONLAI), buffer);
		_itow(BongConLai, buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITBONGCONLAI), buffer);
		_itow(SoBongDaLay[0], buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITDALAY1), buffer);
		_itow(SoBongDaLay[1], buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITDALAY2), buffer);
		_itow(SoLanGiuKho[0], buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITGIUKHO1), buffer);
		_itow(SoLanGiuKho[1], buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITGIUKHO2), buffer);

		Edit_SetText(GetDlgItem(hDlg, IDC_EDITNAME1), TenDoi[0]);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITNAME2), TenDoi[1]);

		Edit_SetText(GetDlgItem(hDlg, IDC_EDITDOIHIENTAI), TenDoi[DoiHienTai]);


		if (ThoiGianConLai == 0)
		{

		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return (INT_PTR)FALSE;
}

void onInit(HWND hDlg)
{
	srand(time(NULL));
	BongConLai = TongSoBong = rand() % 31 + 30;
	//BongConLai = TongSoBong = 1000000;
	TongThoiGian = ThoiGianConLai = (rand() % 31 + 30)*1000;
	mutex = CreateMutex(0, 0, 0);
	SoBongDaLay[0] = SoBongDaLay[1] = 0;
	SoLanGiuKho[0] = SoLanGiuKho[1] = 0;
	data[0].index = 0;
	data[1].index = 1;
}

void onStart(HWND hDlg)
{
	SetTimer(hDlg, 1, 200, NULL);
	int id = rand() % 2;
	if (id)
	{
		thread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc1, data, 0, 0);
		thread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc2, data + 1, 0, 0);
	}
	else
	{
		thread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc2, data, 0, 0);
		thread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc1, data + 1, 0, 0);
	}
}

DWORD WINAPI ThreadFunc1(LPVOID data)
{
	ParaThread* para = (ParaThread*)data;
	while (1)
	{
		if (WaitForSingleObject(mutex, INFINITE) == 0)
		{
			TenDoi[para->index] = L"Team 2";
			ReleaseMutex(mutex);
			break;
		}
	}
	while (1)
	{
		if (WaitForSingleObject(mutex, INFINITE) == 0)
		{
			DoiHienTai = para->index;
			Sleep(1000);
			BongConLai -= 2;
			SoBongDaLay[para->index] += 2;
			SoLanGiuKho[para->index]++;
			ReleaseMutex(mutex);
		}
	}
}

DWORD WINAPI ThreadFunc2(LPVOID data)
{
	ParaThread* para = (ParaThread*)data;
	while (1)
	{
		if (WaitForSingleObject(mutex, INFINITE) == 0)
		{
			TenDoi[para->index] = L"Team 1";
			ReleaseMutex(mutex);
			break;
		}
	}
	while (1)
	{
		if (WaitForSingleObject(mutex, INFINITE) == 0)
		{
			DoiHienTai = para->index;
			Sleep(1500);
			BongConLai -= 1;
			SoBongDaLay[para->index]++;
			SoLanGiuKho[para->index]++;
			ReleaseMutex(mutex);
		}
	}
}