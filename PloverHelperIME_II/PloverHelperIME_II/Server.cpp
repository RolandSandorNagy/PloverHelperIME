#include "stdafx.h"
#include "Server.h"

#include "Resource.h"
#include "View.h"
#include "Globals.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <string.h>
#include <string>  


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")


#define WIN32_LEAN_AND_MEAN
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "12345"
#define MAX_LOADSTRING 100
#define FAILED(hr)      (((HRESULT)(hr)) < 0)



Server::Server()
{
}


Server::~Server()
{
}

extern HWND hWnd;
extern INT ln;


VOID serverThread(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// Lokális változók deklarálása
	WSADATA wsaData;
	int iResult;
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Socket-ek deklarálása és inicializálásas
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	// Lokális struktúrák deklarálása
	struct addrinfo *result = NULL;
	struct addrinfo hints;

	// Server szál futtatása a végtelenségig
	while (true)
	{
		// Initialize Winsock
		// Winsock inicializálása
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return;
		}

		// Create a SOCKET for connecting to server
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return;
		}

		// Setup the TCP listening socket
		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
			return;
		}

		freeaddrinfo(result);

		iResult = listen(ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return;
		}

		// Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return;
		}

		// No longer need server socket
		closesocket(ListenSocket);

		// Receive until the peer shuts down the connection
		do
		{
			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0)
			{
				processReceivedMsg(iResult, recvbuf, recvbuflen);
			}
			else if (iResult < 0)
			{
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return;
			}
		} while (iResult > 0);

		// shutdown the connection since we're done
		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return;
		}
		// cleanup
		closesocket(ClientSocket);
		WSACleanup();
	}
}

VOID processReceivedMsg(int iResult, char recvbuf[], int recvbuflen)
{
	PCHAR *msg;
	INT length = iResult + 1;

	msg = (PCHAR *)malloc((iResult + 1)* sizeof(PCHAR *));
	for (size_t i = 0; i < iResult; i++)
		memcpy(&msg[i], &recvbuf[i], sizeof(PCHAR *));
	msg[iResult + 1] = '\0';

	printf("Message received: %s\n", msg);

	std::wstring wsTmp(recvbuf[0], recvbuf[iResult]);
	std::wstring ws = wsTmp;

	POINT p = adjustPopUp();


	drawStringOnPopUp(ws, iResult, p);

}

POINT adjustPopUp()
{
	POINT p = getCaretPosition();
	if (p.y < 35)
	{
		ShowWindow(hWnd, SW_HIDE);
		UpdateWindow(hWnd);
		return p;
	}
	MoveWindow(hWnd, p.x - 150, p.y + 25, 300, 300, TRUE);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return p;
}

POINT getCaretPosition()
{
	POINT *point = new POINT();

	HWND Wnd = NULL;
	HWND Result = NULL;
	DWORD TId, PId;

	Result = GetFocus();
	if (!Result)
	{
		Wnd = GetForegroundWindow();
		if (Wnd)
		{
			TId = GetWindowThreadProcessId(Wnd, &PId);
			if (AttachThreadInput(GetCurrentThreadId(), TId, TRUE))
			{
				GetCaretPos(point);
				Result = GetFocus();
				ClientToScreen(Result, point);
				AttachThreadInput(GetCurrentThreadId(), TId, FALSE);
			}
		}
	}

	return *point;
}




VOID drawStringOnPopUp(std::wstring ws, INT length, POINT p)
{

	PAINTSTRUCT ps;
	HDC hDC = GetDC(hWnd);
	RECT rect = { 5, 5 + ln * 20, 305, 5 + (ln + 1) * 20 };

	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkColor(hDC, RGB(200, 200, 200));
	 
	std::wostringstream a;
	a << p.y;
	const std::wstring s(a.str());
	
	DrawText(hDC, s.c_str(), s.length(), &rect, 0);
	//DrawText(hDC, ws.c_str(), ws.length(), &rect, 0);

	EndPaint(hWnd, &ps);

	handleNextLine(hDC);

	//ShowWindow(hWnd, SW_SHOW);
	//UpdateWindow(hWnd);

}

VOID handleNextLine(HDC hDC)
{

	ln = (ln > 14 ? 0 : ln + 1);
	if (ln == 0) {
		RECT r = { 0, 0, 300, 300 };
		FillRect(hDC, &r, (HBRUSH)(LTGRAY_BRUSH));
		SetROP2(hDC, R2_NOTXORPEN);
	}
	

}

