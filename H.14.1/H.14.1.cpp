// H.14.1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "H.14.1.h"
#include <commdlg.h>
#include <ShlObj.h>
#include <windowsx.h>
#include "ThreadInfo.h"

#define MAX_LOADSTRING 200

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

int label[] = { IDC_LBTHREAD1, IDC_LBTHREAD2, IDC_LBTHREAD3, IDC_LBTHREAD4, IDC_LBTHREAD5 };
int picCtrl[] = { IDC_PCT1, IDC_PCT2, IDC_PCT3, IDC_PCT4, IDC_PCT5 };
int threadNum;
TCHAR srcPath[MAX_LOADSTRING], dstPath[MAX_LOADSTRING], fOutName[5][MAX_LOADSTRING];
HBRUSH whiteBrush, redBrush;
ThreadInfo threadInfo[5];
HANDLE mutex, thread[5];
FILE* inFile;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);

void showThread(HWND, int);
void onInit(HWND);
void onCopy(HWND);
bool openFile(HWND, TCHAR*);
bool chooseDir(HWND, TCHAR*);
void WINAPI ThreadFunc(_In_ LPVOID);

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
	LoadString(hInstance, IDC_H141, szWindowClass, MAX_LOADSTRING);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_H141));

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
	TCHAR buffer[MAX_LOADSTRING];

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		onInit(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTNSELFILE:
			if (openFile(hDlg, buffer))
			{
				wcscpy(srcPath, buffer);
				Edit_SetText(GetDlgItem(hDlg, IDC_EDITPATHSRC), buffer);
				if (wcslen(dstPath) > 0)
					EnableWindow(GetDlgItem(hDlg, IDC_BTNCOPY), TRUE);
			}
			break;
		case IDC_BTNSELFOLDER:
			if (chooseDir(hDlg, buffer))
			{
				wcscpy(dstPath, buffer);
				Edit_SetText(GetDlgItem(hDlg, IDC_EDITPATHDST), buffer);
				if (wcslen(srcPath) > 0)
					EnableWindow(GetDlgItem(hDlg, IDC_BTNCOPY), TRUE);
			}
			break;
		case IDC_CBTHREADNUM:
			threadNum = SendMessage(GetDlgItem(hDlg, IDC_CBTHREADNUM), CB_GETCURSEL, NULL, NULL) + 1;
			showThread(hDlg, threadNum);
			break;
		case IDC_BTNCOPY:
			onCopy(hDlg);
			break;
		}
		break;

	case WM_PAINT:
		if (whiteBrush == NULL)
		{
			redBrush = CreateSolidBrush(RGB(255, 0, 0));
			whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
		}
		break;

	case WM_CLOSE:
		DeleteObject(redBrush);
		DeleteObject(whiteBrush);
		PostQuitMessage(0);
		break;
	}
	return (INT_PTR)FALSE;
}

void showThread(HWND hDlg, int num)
{
	for (int i = 0; i < num; ++i)
	{
		ShowWindow(GetDlgItem(hDlg, label[i]), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, picCtrl[i]), SW_SHOW);
	}

	for (int i = num; i < 5; ++i)
	{
		ShowWindow(GetDlgItem(hDlg, label[i]), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, picCtrl[i]), SW_HIDE);
	}
}

void onInit(HWND hDlg)
{
	TCHAR buffer[5];

	for (int i = 1; i <= 5; ++i)
	{
		_itow(i, buffer, 10);
		SendMessage(GetDlgItem(hDlg, IDC_CBTHREADNUM), CB_ADDSTRING, NULL, (LPARAM)buffer);
	}

	threadNum = 5;
	SendMessage(GetDlgItem(hDlg, IDC_CBTHREADNUM), CB_SETCURSEL, 4, NULL);

	whiteBrush = redBrush = NULL;
}

void onCopy(HWND hDlg)
{
	EnableWindow(GetDlgItem(hDlg, IDC_BTNCOPY), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_BTNSELFILE), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_BTNSELFOLDER), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CBTHREADNUM), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_BTNSTOP), TRUE);

	mutex = CreateMutex(NULL, FALSE, NULL);

	if (inFile != NULL)
		fclose(inFile);

	inFile = _wfopen(srcPath, L"rb");

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);

	TCHAR* fName = srcPath + wcslen(srcPath) - 1;
	while (fName != srcPath && *fName != L'\\')
		--fName;
	if (fName[0] == L'\\') ++fName;
	for (int i = 0; i < threadNum; ++i)
	{
		wsprintf(fOutName[i], L"%s%s.%03d", dstPath, fName, i + 1);
		threadInfo[i].open(fOutName[i], i*(fileSize/threadNum),
			min((i+1)*(fileSize/threadNum), fileSize) - 1, GetDlgItem(hDlg, picCtrl[i]));
		thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, (LPVOID)i, 0, 0);
	}

	WaitForMultipleObjects(threadNum, thread, TRUE, INFINITE);

	for (int i = 0; i < threadNum; ++i)
		threadInfo[i].close();

	TCHAR cmd[MAX_LOADSTRING * 6];
	TCHAR outName[MAX_LOADSTRING];
	wsprintf(outName, L"%s%s", dstPath, fName);
	wsprintf(cmd, L"del \"%s\"", outName);
	_wsystem(cmd);
	wsprintf(cmd, L"copy /B \"%s", fOutName[0]);
	for (int i = 1; i < threadNum; ++i)
		wcscat(cmd, L"\"+\""), wcscat(cmd, fOutName[i]);
	wcscat(cmd, L"\" \""), wcscat(cmd, outName), wcscat(cmd, L"\"");
	_wsystem(cmd);

	for (int i = 0; i < threadNum; ++i)
	{
		wsprintf(cmd, L"del \"%s\"", fOutName[i]);
		_wsystem(cmd);
	}

	MessageBox(hDlg, L"Done!", L"Copy done", MB_OK);

	fclose(inFile);
	inFile = NULL;
	EnableWindow(GetDlgItem(hDlg, IDC_BTNCOPY), TRUE);
	EnableWindow(GetDlgItem(hDlg, IDC_BTNSELFILE), TRUE);
	EnableWindow(GetDlgItem(hDlg, IDC_BTNSELFOLDER), TRUE);
	EnableWindow(GetDlgItem(hDlg, IDC_CBTHREADNUM), TRUE);
}

bool openFile(HWND hWnd, TCHAR* buffer)
{
	OPENFILENAME ofn;
	TCHAR szFile[MAX_LOADSTRING];
	TCHAR szFilter[] = TEXT("Any\0 *.*\0");
	szFile[0] = '\0';
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn)) {
		wcscpy(buffer, ofn.lpstrFile);
		return true;
	}
	else
		return false;
}
bool chooseDir(HWND hWnd, TCHAR* buffer)
{
	BROWSEINFO br;
	TCHAR res[MAX_LOADSTRING];
	res[0] = '\0';
	br.hwndOwner = hWnd;
	br.pidlRoot = NULL;
	br.pszDisplayName = res;
	br.lpszTitle = L"Select destination folder";
	br.ulFlags = BIF_RETURNONLYFSDIRS | BIF_BROWSEINCLUDEURLS;
	br.lpfn = NULL;
	br.lParam = NULL;
	br.iImage = NULL;

	LPITEMIDLIST lpItem = SHBrowseForFolder(&br);
	if (lpItem != NULL)
	{
		SHGetPathFromIDList(lpItem, buffer);
		if (buffer[wcslen(buffer) - 1] != L'\\')
			wcscat(buffer, L"\\");
		return true;
	}
	else
		return false;
}

void WINAPI ThreadFunc(_In_ LPVOID lpParameter)
{
	int id = (int)lpParameter;
	threadInfo[id].resetDraw(whiteBrush);
	threadInfo[id].draw(redBrush);

	while (1)
	{
		bool f = true;

		while (1)
		{
			if (!WaitForSingleObject(mutex, 1000))
			{
				f = threadInfo[id].write(inFile);
				ReleaseMutex(mutex);
				break;
			}
		}

		threadInfo[id].draw(redBrush);
		if (!f)
			break;
	}
}