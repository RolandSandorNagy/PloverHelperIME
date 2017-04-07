#pragma once

class View
{
public:
	View();
	~View();
};


VOID windowThread(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow);

BOOL initWindowThread(_In_ HINSTANCE hInstance, _In_ int nCmdShow);
VOID runWindowThread(_In_ HINSTANCE hInstance);
ATOM MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL InitWindowInstance(HINSTANCE hInstance, int nCmdShow);



