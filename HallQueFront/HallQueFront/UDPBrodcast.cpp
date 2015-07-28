#include "StdAfx.h"
#include "UDPBrodcast.h"
#include "CommonConvert.h"

CUDPBrodcast::CUDPBrodcast(void) : m_sockPort(61000)
{
	Start();
}

CUDPBrodcast::~CUDPBrodcast(void)
{
	closesocket(m_sockClient);
	WSACleanup();
}

BOOL CUDPBrodcast::Start()
{
	WSADATA wsaData;
	//初始化
	int errcode = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(errcode)
	{
		WSACleanup();
		return FALSE;
	}
	m_sockClient = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	return TRUE;
}


BOOL CUDPBrodcast::BroadCast(const CString& msg)
{
	
	SOCKADDR_IN addrin = {0};
	addrin.sin_family = AF_INET;
	addrin.sin_port = htons(m_sockPort);
	addrin.sin_addr.S_un.S_addr = ::htonl( INADDR_BROADCAST );

	//设置广播模式
	BOOL bBroadcast = TRUE;
	int iRet = ::setsockopt( m_sockClient, SOL_SOCKET, SO_BROADCAST,
		(char *)&bBroadcast,sizeof( bBroadcast ) );
	if(SOCKET_ERROR == iRet)
	{
		return FALSE;
	}
	

	int size = CCommonConvert::CStringToChar(msg,NULL);
	char* buf = new char[size + 1];
	CCommonConvert::CStringToChar(msg,buf);
	
	int len = sendto(m_sockClient,buf,size,0,(SOCKADDR*)&addrin,sizeof(SOCKADDR));
	if(len == SOCKET_ERROR)
	{
		int errcode = WSAGetLastError();
		return FALSE;
	}

	delete [] buf;

	bBroadcast = FALSE;
	iRet = ::setsockopt( m_sockClient, SOL_SOCKET, SO_BROADCAST,
		(char *)&bBroadcast,sizeof( bBroadcast ) );

	return TRUE;
}
