#pragma once
#include "../HallQueFront/DoFile.h"
#include "../HallQueFront/CommonConvert.h"
#include <string>
#include "DealData.h"
#include "ProducePacket.h"
using namespace std;

#define MAX_BUFFER  1024  
#define MAX_OL_COUNT 1024

typedef enum _OPERATION_INFO  
{  
	OP_NULL,  
	OP_READ,  
	OP_WRITE  
}OPERATIONINFO;  

typedef struct _OVERLAPPED_INFO_  
{     
public:  
	_OVERLAPPED_INFO_()  
	{  
		Clean();  
	}  
	~_OVERLAPPED_INFO_()  
	{  
		Clean();  
	}  
protected:  
	void Clean()  
	{  
		ol.hEvent = NULL;  
		sock = INVALID_SOCKET;  
		memset(&addr, 0, sizeof(addr));  
		addr.sin_addr.S_un.S_addr = INADDR_ANY;  
		addr.sin_family = AF_INET;  
		addr.sin_port = htons(0);  
		wsaBuf.buf = szBuf;  
		wsaBuf.len = MAX_BUFFER;  
		memset(szBuf, 0, sizeof(szBuf));  
		dwTrans = 0;  
		dwFlags = 0;  
		nOpType = OP_NULL;  
	}  
public:  
	WSAOVERLAPPED ol;  
	SOCKET sock;  
	SOCKADDR_IN addr;  
	WSABUF wsaBuf;  
	char szBuf[MAX_BUFFER];  
	DWORD dwTrans;  
	DWORD dwFlags;  
	OPERATIONINFO nOpType;  
}OVERLAPPEDINFO, *LPOVERLAPPEDINFO; 
class CSocketTcpServer
{
public:
	CSocketTcpServer(void);
	CSocketTcpServer(USHORT uListenPort,UINT uMaxListenCount=MAX_OL_COUNT);
	~CSocketTcpServer(void);
public:
	//初始化服务端
	BOOL InitServer();
	USHORT GetListenPort();
	BOOL SaveListenPort();
	HANDLE m_hWorhThreadHandle;
	HANDLE m_hAcceptThreadHandle;
	//工作者线程
	static DWORD WINAPI WorkThread(LPVOID lpParam);
	//
	static DWORD WINAPI AcceptThread(LPVOID lpParam);
	static void CALLBACK CompleteRoutine(DWORD dwError,
		DWORD dwTrans, LPWSAOVERLAPPED Overlppad,
		DWORD dwFlags);  
	//发送消息
	void SendMsg(CString msg);
private:
	USHORT		m_uListenPort;//监听端口
	UINT			m_uMaxListenCount;
	UINT		    m_uMaxDataLength;
	SOCKET		m_SockBound;
	
	CDoFile        m_DoFile;
	CCommonConvert m_convert;
	OVERLAPPEDINFO* m_ol[MAX_OL_COUNT];
	int m_index;
	BOOL m_bExiteThread;
public:
	CDealData* m_pDealData;
private:
	CProducePacket m_producePacket;
};
