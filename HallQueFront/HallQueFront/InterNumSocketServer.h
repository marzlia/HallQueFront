#pragma once

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")      // Socket编程需用的动态链接库
#include "afxmt.h"
#include <string>
using namespace std;
#define MAX_BUFFER  1024

class CInlineQueData;

class CInterNumSocketServer
{
public:
	CInterNumSocketServer(CInlineQueData* pInlineQueData);
	~CInterNumSocketServer(void);

	BOOL InitServer();

private:
	static DWORD WINAPI WorkerThread(LPVOID lpParam);
	static DWORD WINAPI AcceptThread(LPVOID lpParam);
private:
	int    m_iTotalConn;//连接数
	SOCKET m_CliSocketArr[FD_SETSIZE];//Socket数组
	SOCKET m_ListeningSocket;//监听socket
	UINT m_uListenPort;
	HANDLE m_hAcceptThreadHandle;
	HANDLE m_hWorhThreadHandle;
private:
//	std::string DealMsg(const std::string recvPacket);
	CMutex m_muServerLock;
	void DeleteClient(int i);
	static int CALLBACK ConditionFunc(LPWSABUF lpCallerId,
		LPWSABUF lpCallerData, LPQOS lpSQOS,LPQOS lpGQOS,
		LPWSABUF lpCalleeId, LPWSABUF lpCalleeData,
		GROUP FAR * g,DWORD dwCallbackData); 

	CMutex m_muDealMsgLock;

private:
	void DealMsg(const string& recvPacket,string& retPacket);
	CInlineQueData* m_pInlineQueData;
};
