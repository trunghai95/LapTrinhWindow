// P.3.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "P.3.h"
#include <windowsx.h>
#include "Struct.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

#define MAX_LOADSTRING 100
#define BOXSIZE 20
#define MARGIN 4
#define SPEED 2
#define TIMER_TIME 25
#define SLEEP_TIME 1000

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

int cnt[4], width = -1, height = -1;
std::vector<Box> line[4];
Box boxInCheck;
HBRUSH brush;
HPEN whitePen, colorPen[4];
HANDLE mutex, thread[3];
int dx[] = { 1, 0, 1, 0 };
int dy[] = { 0, -1, 0, 1 };
HWND g_hDlg;

// Forward declarations of functions included in this code module:
BOOL				InitInstance(HINSTANCE, int);
INT_PTR CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);

void onStart(HWND);
void onTimer(HWND);
// Thread tương ứng với các robot kiểm tra thùng hàng, lpParam là số thứ tự của robot
DWORD WINAPI ThreadFunc(LPVOID lpParam);
// Thread có vai trò tạo các thread ThreadFunc
DWORD WINAPI ManagerThread(LPVOID lpParam);
// Kiểm tra xem thùng có nằm trong màn hình không
bool checkIn(int, int);
void update(HDC, Box&, int);

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
	LoadString(hInstance, IDC_P3, szWindowClass, MAX_LOADSTRING);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_P3));

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

   /*hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);*/

   g_hDlg = hWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAINDLG), NULL, DlgProc);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Message handler for about box.
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		srand(time(0));
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITTOTAL), L"5");
		
		brush = CreateSolidBrush(RGB(255, 255, 255));
		whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		colorPen[0] = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		colorPen[1] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		colorPen[2] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		colorPen[3] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		mutex = CreateMutex(0, 0, 0);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTNSTART:
			onStart(hDlg);
			break;
		}
		break;

	case WM_PAINT:
		if (width == -1)
		{
			RECT rc;
			GetClientRect(GetDlgItem(hDlg, IDC_PICCTRL), &rc);
			width = rc.right;
			height = rc.bottom;
		}
		break;

	case WM_TIMER:
		onTimer(hDlg);
		break;

	case WM_CLOSE:
		DeleteObject(brush);
		DeleteObject(whitePen);
		for (int i = 0; i < 4; ++i)
			DeleteObject(colorPen[i]);
		PostQuitMessage(0);
		break;
	}
	return (INT_PTR)FALSE;
}

void onStart(HWND hDlg)
{
	TCHAR buffer[MAX_LOADSTRING], *tmp;
	int len;

	Edit_GetText(GetDlgItem(hDlg, IDC_EDITTOTAL), buffer, MAX_LOADSTRING);
	tmp = buffer;
	while (*tmp == '0' && *tmp != '\0') ++tmp;
	len = wcslen(tmp);

	// Số lượng không hợp lệ
	if (len == 0 || len > 10 || (len == 10 && *tmp > 1))
	{
		MessageBox(hDlg, L"The number of boxes must be between 1 and 1999999999.", L"Invalid value", MB_OK);
		return;
	}

	SetTimer(hDlg, 0, TIMER_TIME, NULL);

	EnableWindow(GetDlgItem(hDlg, IDC_BTNSTART), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_EDITTOTAL), FALSE);

	cnt[0] = _wtoi(tmp);
	cnt[1] = cnt[2] = cnt[3] = 0;
	_itow(--cnt[0], buffer, 10);
	Edit_SetText(GetDlgItem(hDlg, IDC_EDITREMAIN), buffer);
	Edit_SetText(GetDlgItem(hDlg, IDC_EDITCNT1), L"0");
	Edit_SetText(GetDlgItem(hDlg, IDC_EDITCNT2), L"0");
	Edit_SetText(GetDlgItem(hDlg, IDC_EDITCNT3), L"0");

	line[0].push_back(Box((rand() % 3) + 1, BOXSIZE, height/2));
	boxInCheck.type = -1;

	// Vẽ bản đồ
	HDC hdc = GetDC(GetDlgItem(hDlg, IDC_PICCTRL));

	SelectObject(hdc, brush);
	SelectObject(hdc, whitePen);
	Rectangle(hdc, 0, 0, width, height);

	SelectObject(hdc, colorPen[0]);

	// Trái trên
	Rectangle(hdc, 0, 0, width / 2 - BOXSIZE - MARGIN, height / 2 - BOXSIZE - MARGIN);

	// Phải trên
	Rectangle(hdc, width / 2 + BOXSIZE + MARGIN, 0, width, height / 2 - BOXSIZE - MARGIN);

	// Trái dưới
	Rectangle(hdc, 0, height / 2 + BOXSIZE + MARGIN, width / 2 - BOXSIZE - MARGIN, height);

	// Phải dưới
	Rectangle(hdc, width / 2 + BOXSIZE + MARGIN, height / 2 + BOXSIZE + MARGIN, width, height);

	// Vẽ thùng đầu tiên
	Rectangle(hdc, line[0][0].cx - BOXSIZE, line[0][0].cy - BOXSIZE,
		line[0][0].cx + BOXSIZE, line[0][0].cy + BOXSIZE);

	_itow(line[0][0].type, buffer, 10);
	TextOut(hdc, line[0][0].cx, line[0][0].cy, buffer, wcslen(buffer));

	ReleaseDC(WindowFromDC(hdc), hdc);
}

void onTimer(HWND hDlg)
{
	// Thông báo kết thúc
	if (line[0].empty() && line[1].empty() && line[2].empty() && line[3].empty())
	{
		KillTimer(hDlg, 0);
		TCHAR msg[MAX_LOADSTRING];
		wsprintf(msg, L"Line 1: %d\nLine 2: %d\nLine 3: %d", cnt[1], cnt[2], cnt[3]);
		MessageBox(hDlg, msg, L"Done", MB_OK);
		EnableWindow(GetDlgItem(hDlg, IDC_BTNSTART), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_EDITTOTAL), TRUE);
		return;
	}

	TCHAR buffer[MAX_LOADSTRING];
	HDC hdc = GetDC(GetDlgItem(hDlg, IDC_PICCTRL));
	SelectObject(hdc, brush);

	// Nếu không có thùng hàng nào đang được kiểm tra
	// thì tiếp tục chạy line 0 để đưa hàng vào
	if (boxInCheck.type < 0)
	{
		// Di chuyển các thùng hàng trên line 0
		for (int i = 0; i < line[0].size(); ++i)
			update(hdc, line[0][i], 0);

		if (!line[0].empty())
		{
			// Đẩy thêm thùng vào nếu có đủ chỗ
			if (line[0].back().cx >= BOXSIZE * 3 + MARGIN && cnt[0] > 0)
			{
				--cnt[0];
				line[0].push_back(Box((rand() % 3) + 1, line[0].back().cx - BOXSIZE * 2 - MARGIN, height / 2));
				_itow(cnt[0], buffer, 10);
				Edit_SetText(GetDlgItem(hDlg, IDC_EDITREMAIN), buffer);
			}

			// Nếu một thùng hàng nằm ở vị trí giữa thì tiến hành kiểm tra nó
			if (line[0][0].cx == width / 2)
			{
				boxInCheck = line[0][0];
				line[0].erase(line[0].begin());

				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ManagerThread, (LPVOID)hDlg, 0, 0);
			}
		}
	}

	// Di chuyển các line 1, 2, 3
	for (int i = 1; i <= 3; ++i)
	{
		if (line[i].empty())
			continue;

		for (int j = 0; j < line[i].size(); ++j)
			update(hdc, line[i][j], i);

		// Nếu thùng hàng nằm bên ngoài khung hình thì không hiển thị nó nữa
		if (!checkIn(line[i][0].cx, line[i][0].cy))
			line[i].erase(line[i].begin());
	}

	ReleaseDC(WindowFromDC(hdc), hdc);
}

bool checkIn(int cx, int cy)
{
	return (cx >= BOXSIZE && cx < width - BOXSIZE && cy >= BOXSIZE && cy < height - BOXSIZE);
}

void update(HDC hdc, Box &b, int type)
{
	TCHAR buffer[3];

	// Xóa
	SelectObject(hdc, whitePen);
	Rectangle(hdc, b.cx - BOXSIZE, b.cy - BOXSIZE,
		b.cx + BOXSIZE, b.cy + BOXSIZE);

	// Thay đổi vị trí thùng hàng
	b.cx += SPEED*dx[type];
	b.cy += SPEED*dy[type];

	// Nếu thùng hàng sau khi di chuyển vẫn nằm trong khung hình thì vẽ lại nó
	if (checkIn(b.cx, b.cy))
	{
		SelectObject(hdc, colorPen[type]);
		Rectangle(hdc, b.cx - BOXSIZE, b.cy - BOXSIZE,
			b.cx + BOXSIZE, b.cy + BOXSIZE);

		_itow(b.type, buffer, 10);
		TextOut(hdc, b.cx - BOXSIZE + 1, b.cy - BOXSIZE + 1, buffer, wcslen(buffer));
	}
}

DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
	TCHAR buffer[MAX_LOADSTRING];
	int type = (int)lpParam;

	while (1)
	{
		if (WaitForSingleObject(mutex, INFINITE) == 0)
		{
			if (boxInCheck.type > 0)
			{
				HDC hdc = GetDC(GetDlgItem(g_hDlg, IDC_PICCTRL));

				// Đổi màu thùng hàng
				SelectObject(hdc, colorPen[type]);
				Rectangle(hdc, boxInCheck.cx - BOXSIZE, boxInCheck.cy - BOXSIZE,
					boxInCheck.cx + BOXSIZE, boxInCheck.cy + BOXSIZE);
				_itow(boxInCheck.type, buffer, 10);
				TextOut(hdc, boxInCheck.cx - BOXSIZE + 1, boxInCheck.cy - BOXSIZE + 1, buffer, wcslen(buffer));
				ReleaseDC(WindowFromDC(hdc), hdc);

				Sleep(SLEEP_TIME);
				if (boxInCheck.type == type)
				{
					line[type].push_back(boxInCheck);
					boxInCheck.type = -1;
					++cnt[type];
				}
			}
			ReleaseMutex(mutex);
			break;
		}
	}
	return 0;
}

DWORD WINAPI ManagerThread(LPVOID lpParam)
{
	TCHAR buffer[MAX_LOADSTRING];
	HWND hDlg = (HWND)lpParam;

	for (int i = 0; i < 3; ++i)
		thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, (LPVOID)(i + 1), 0, 0);

	WaitForMultipleObjects(3, thread, TRUE, INFINITE);

	_itow(cnt[1], buffer, 10);
	Edit_SetText(GetDlgItem(hDlg, IDC_EDITCNT1), buffer);
	_itow(cnt[2], buffer, 10);
	Edit_SetText(GetDlgItem(hDlg, IDC_EDITCNT2), buffer);
	_itow(cnt[3], buffer, 10);
	Edit_SetText(GetDlgItem(hDlg, IDC_EDITCNT3), buffer);

	return 0;
}