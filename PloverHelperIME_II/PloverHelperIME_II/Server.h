#pragma once



class Server
{
public:
	Server();
	~Server();
};


VOID serverThread(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow);

VOID processReceivedMsg(int iResult, char recvbuf[], int recvbuflen);
POINT adjustPopUp();
POINT getCaretPosition();
VOID drawStringOnPopUp(std::wstring ws, INT length, POINT p);
VOID handleNextLine(HDC hDC);
