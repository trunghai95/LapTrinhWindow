// H.1.1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "H.1.1.h"
#include <string>
#include <sstream>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// Điểm bắt đầu chương trình
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	// Các tham số không được dùng đến trong hàm.
	// Gọi để tránh bị cảnh báo "tham số không được sử dụng".
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	// Khai báo biến msg để chứa message
	// và hAccelTable lưu bảng phím tắt
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	// Đọc các chuỗi từ resource String Table
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_H11, szWindowClass, MAX_LOADSTRING);
	// Gọi hàm đăng ký lớp cửa sổ cho ứng dụng
	MyRegisterClass(hInstance);

	// Perform application initialization:
	// Khởi tạo ứng dụng và kiểm tra có khởi tạo thành công không
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// Đọc các phím tắt từ bảng phím tắt
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_H11));

	// Main message loop:
	// Vòng lặp nhận thông điệp (message loop)
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// Dịch các thông điệp nếu có trong bảng phím tắt từ WM_KEYDOWN hoặc WM_SYSKEYDOWN
		// thành các thông điệp WM_COMMAND hoặc WM_SYSCOMMAND
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			// Dịch các thông điệp gõ phím thành các thông điệp ký tự
			TranslateMessage(&msg);
			// Gửi thông điệp tới hệ điều hành
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
// Hàm đăng ký lớp của sổ
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	// Khai báo biến chứa thông tin về lớp cửa sổ
	WNDCLASSEX wcex;

	// Thuộc tính lưu kích thước của WNDCLASSEX
	wcex.cbSize = sizeof(WNDCLASSEX);

	// Kiểu lớp: CS_HREDRAW/CS_VREDRAW: Vẽ lại cửa sổ nếu
	// có sự thay đổi chiều rộng/chiều cao của vùng client
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	// Con trỏ hàm đến hàm xử lý
	wcex.lpfnWndProc	= WndProc;
	// Số byte thêm vào của lớp cửa sổ
	wcex.cbClsExtra		= 0;
	// Số byte thêm vào của mỗi đối tượng cửa số thuộc lớp
	wcex.cbWndExtra		= 0;
	// Handle trỏ đến instance hiện tại
	wcex.hInstance		= hInstance;
	// Handle trỏ đến resource icon
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_H11));
	// Handle trỏ đến resource cursor (con trỏ chuột)
	wcex.hCursor		= LoadCursor(NULL, IDC_CROSS);
	// Tô màu cho background vùng client
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	// Chuỗi xác định menu của lớp cửa sổ
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_H11);
	// Tên của lớp cửa sổ
	wcex.lpszClassName	= szWindowClass;
	// Handle icon nhỏ
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
// Tạo một đối tượng cửa sổ
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	// Biến dùng để lưu handle đến đối tượng cửa sổ
	HWND hWnd;

	// Lưu instance vào biến toàn cục hInst
	hInst = hInstance; // Store instance handle in our global variable

	// Tạo cửa sổ và lưu handle vào hWnd
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	// Nếu tạo không thành công
	if (!hWnd)
	{
		return FALSE;
	}

	// Nếu thành công, hiện cửa sổ
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
	std::wstring text = L"Day la chuong trinh dau tien...\n";
	text += L"... viet bang Visual C++...\n";
	text += L"... tren HDH Windows...\n";
	text += L"... bang thu vien Win32 API.";
	LPCWSTR lptext = const_cast<wchar_t*>(text.c_str());
	RECT r;
	int xPos, yPos;
	std::wostringstream str;

	switch (message)
	{
	case WM_CREATE:
		MessageBox(hWnd, L"Chuong trinh bai tap H.1 - Mon lap trinh Windows\nTac gia: Bui Trung Hai 1312165",
			L"Gioi thieu", MB_OK);
		break;
	case WM_COMMAND:
		// ID của menu được gọi
		wmId    = LOWORD(wParam);

		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		// Xử lý từng loại command
		switch (wmId)
		{
		// Khi người dùng nhấn vào about
		case IDM_ABOUT:
			// Hiện dialog IDD_ABOUTBOX
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		// Khi người dùng nhấn vào exit
		case IDM_EXIT:
			if (MessageBox(hWnd, L"Ban co muon thoat ung dung?", L"Exit", MB_YESNO) == IDYES)
				// Hủy cửa sổ
				DestroyWindow(hWnd);
			break;
		case ID_FILE_NEW:
			MessageBox(hWnd, L"Ban vua chon chuc nang New.", L"New", MB_OK);
			break;
		case ID_FILE_OPEN:
			MessageBox(hWnd, L"Ban vua chon chuc nang Open.", L"Open", MB_OK);
			break;
		case ID_FILE_SAVE:
			MessageBox(hWnd, L"Ban vua chon chuc nang Save.", L"Save", MB_OK);
			break;
		case ID_EDIT_UNDO:
			MessageBox(hWnd, L"Ban vua chon chuc nang Undo.", L"Undo", MB_OK);
			break;
		case ID_EDIT_CUT:
			MessageBox(hWnd, L"Ban vua chon chuc nang Cut.", L"Cut", MB_OK);
			break;
		case ID_EDIT_COPY:
			MessageBox(hWnd, L"Ban vua chon chuc nang Copy.", L"Copy", MB_OK);
			break;
		case ID_EDIT_PASTE:
			MessageBox(hWnd, L"Ban vua chon chuc nang Paste.", L"Paste", MB_OK);
			break;
		case ID_EDIT_SELECTALL:
			MessageBox(hWnd, L"Ban vua chon chuc nang Select All.", L"Select All", MB_OK);
			break;
		default:
			// Gọi hàm xử lý mặc định
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		//TextOut(hdc, 50, 50, lptext, wcslen(lptext));
		SetRect(&r, 0, 0, 500, 300);
		DrawText(hdc, lptext, wcslen(lptext), &r, DT_LEFT);
		EndPaint(hWnd, &ps);
		break;
	// Message gửi đến khi có lệnh hủy cửa sổ
	case WM_DESTROY:
		// Báo cho hệ thống biết tiểu trình sắp dừng với exit code là 0
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		if (MessageBox(hWnd, L"Ban co muon thoat ung dung?", L"Exit", MB_YESNO) == IDYES)
			DestroyWindow(hWnd);
		break;
	case WM_LBUTTONDOWN:
		xPos = LOWORD(lParam), yPos = HIWORD(lParam);
		str << L"Ban vua nhan chuot trai\nTai vi tri X = " << xPos << "  Y = " << yPos;
		MessageBox(hWnd, str.str().c_str(), L"Click", MB_OK);
		break;
	case WM_RBUTTONDOWN:
		xPos = LOWORD(lParam), yPos = HIWORD(lParam);
		str << L"Ban vua nhan chuot phai\nTai vi tri X = " << xPos << "  Y = " << yPos;
		MessageBox(hWnd, str.str().c_str(), L"Click", MB_OK);
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
