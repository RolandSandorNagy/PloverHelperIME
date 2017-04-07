#include "stdafx.h"
#include "View.h"

#include "Resource.h"
#include "Globals.h"

View::View()
{
}


View::~View()
{
}



extern HINSTANCE hInst;								// current instance
extern TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
extern TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name


INT x = 0;
INT y = 0;
INT ln = 0;
HWND hWnd;


VOID windowThread(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!initWindowThread(hInstance, nCmdShow))
		return;
	runWindowThread(hInstance);
}

BOOL initWindowThread(_In_ HINSTANCE hInstance, _In_ int nCmdShow)
{
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitWindowInstance(hInstance, nCmdShow)) return FALSE;

	return TRUE;
}

VOID runWindowThread(_In_ HINSTANCE hInstance)
{
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PLOVERHELPERIME_II));

	MSG msg;
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);


	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(LTGRAY_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		//_Draw();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL InitWindowInstance(HINSTANCE hInstance, int nCmdShow)
{

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
		szWindowClass,
		szTitle,
		WS_POPUP,
		0,
		0,
		300,
		300,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	//ShowWindow(hWnd, nCmdShow);
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);

	return TRUE;
}


