#pragma once
#include "ConnectConfig.h"
#include "CommonConvert.h"
#include "SocketBufDef.h"
#include <string>
#include <list>
using namespace std;
class CComplSocketClient
{
public:
	CComplSocketClient(void);
	~CComplSocketClient(void);
	BOOL InitClient();
	void StopClient();
	BOOL SendData(const CString& msg,std::string& recvMsg,int& actRecvSize);
	BOOL SendData(USHORT port,CString IP,const CString& msg,std::string& recvMsg,int& actRecvSize);
	BOOL SendData(USHORT port,CString IP,const std::string& msg,int size,std::string& recvMsg,int& actRecvSize);
	BOOL SendData(USHORT port,CString IP,char buf[],int size);
private:
	CConnectConfig m_connect;
	CString m_romoteIP;
	UINT m_romotePort;
	SOCKET m_sClient;
	UINT m_nTimeOut;
	CString m_caChePath;
private:
	///������
	void DealCache(const CString& msg);
	void ReSendMsg();
	BOOL AppendListMsg();
};
