// PloverHelperIME_II.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PloverHelperIME_II.h"
#include "Globals.h"
#include "View.h"
#include "Server.h"


extern VOID initGlobalStrings(HINSTANCE hInstance);
extern VOID windowThread(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow);
extern VOID serverThread(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow);




int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	
	// TODO: Place code here.
	
	// Initialize global strings
	initGlobalStrings(hInstance);

	std::thread wThread(windowThread, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	std::thread sThread(serverThread, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	wThread.join();
	sThread.join();

	return 0;
}




