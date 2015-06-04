#pragma once
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

class SLZWindowQueryView;

class CUDPServer
{
public:
//	CUDPServer* GetInstance();
	CUDPServer(SLZWindowQueryView* pSLZWindowQueryView);
	~CUDPServer(void);
private:
	static DWORD WINAPI WorkThread(LPVOID pParam);
	SOCKADDR_IN m_addrRecviver;
	HANDLE m_hWorkHandle;
	SOCKET m_sClient;
	USHORT m_sockPort;
public:
	BOOL Start();
private:
	SLZWindowQueryView* m_pSLZWindowView;
};
