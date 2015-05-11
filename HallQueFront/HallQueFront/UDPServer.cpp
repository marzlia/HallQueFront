#include "StdAfx.h"
#include "UDPServer.h"
#include "HallQueFront.h"
#include "DealInterMsg.h"
#include "SLZWindowQueryView.h"
#include "SLZCCaller.h"
#include <string>
using namespace std;

#define DEFAULT_BUFFER_LEN 256

extern void MyWriteConsole(CString str);

CUDPServer::CUDPServer(SLZWindowQueryView* pSLZWindowQueryView) : m_sockPort(61000)
, m_hWorkHandle(NULL)
{
	m_pSLZWindowView = pSLZWindowQueryView;
}

CUDPServer::~CUDPServer(void)
{
	TerminateThread(m_hWorkHandle,0);
	if(m_hWorkHandle)
	{
		CloseHandle(m_hWorkHandle);
		m_hWorkHandle = NULL;
	}
	closesocket(m_sClient);
	// 释放Windows Socket DLL的相关资源
	WSACleanup();
}


// CUDPServer* CUDPServer::GetInstance()
// {
// 	static CUDPServer Instance;
// 	return &Instance;
// }

BOOL CUDPServer::Start()
{
	WSADATA wsaData;
	//初始化
	WSAStartup(MAKEWORD(2,2),&wsaData);
	///////////////////////////////////////////////
	m_addrRecviver.sin_family = AF_INET;
	m_addrRecviver.sin_addr.S_un.S_addr = INADDR_ANY;
	m_addrRecviver.sin_port = htons( m_sockPort );
	///////////////////////////////////////////////
	m_sClient = socket( AF_INET, SOCK_DGRAM , IPPROTO_UDP );
	if( m_sClient == INVALID_SOCKET )
	{
		WSACleanup();
		return FALSE;
	}
	u_long nMode = 1; //非阻塞模式
	int nError = ioctlsocket( m_sClient, FIONBIO, &nMode );
	if( nError!=0 )
	{
		WSACleanup();
		return FALSE;
	}
	nError = bind( m_sClient, (const sockaddr *)&m_addrRecviver, sizeof(m_addrRecviver) );
	if( nError == SOCKET_ERROR )
	{
		WSACleanup();
		return FALSE;
	}
	/////////////////////////////////////创建工作者线程
	m_hWorkHandle = CreateThread(NULL,0,WorkThread,this,0,NULL);
	if(m_hWorkHandle == NULL)
	{
		return FALSE;
	}
	/////////////////////////////////////
	return TRUE;
}

DWORD WINAPI CUDPServer::WorkThread(LPVOID pParam)
{
	CUDPServer* pThis = (CUDPServer*)pParam;
	SOCKADDR_IN addrSender;
	int addrSenderLen = sizeof(addrSender);
	char recvBuff[DEFAULT_BUFFER_LEN]={0};
	while(TRUE)
	{
		memset(recvBuff,0,DEFAULT_BUFFER_LEN);

		int nError = recvfrom( pThis->m_sClient, recvBuff, DEFAULT_BUFFER_LEN, 0, 
			(sockaddr *)&addrSender, &addrSenderLen );
		if( nError == SOCKET_ERROR )
		{
#ifdef _DEBUG
			int i_errcode = WSAGetLastError();
			CString str = _T("recvfrom error:");
			str.AppendFormat(_T("%d"),i_errcode);
			MyWriteConsole(str);
#endif
			continue;
		}
#ifdef _DEBUG
		CString ipAddress;
		CCommonConvert::CharToCstring(ipAddress,inet_ntoa(addrSender.sin_addr));
		CString clientInfo;
		clientInfo.Format(_T("Client <%s : %d> come in..."),ipAddress,
			ntohs(addrSender.sin_port));
		MyWriteConsole(clientInfo); 
#endif
		///接收到数据以后做处理
		//printf("%s\n",recvBuff);
		std::string recvMsg(recvBuff);
		

		string::size_type pos1 = recvMsg.find("<headCode>");
		string::size_type pos2 = recvMsg.find("</headCode>");
		if(pos1 == recvMsg.npos || pos2 == recvMsg.npos)
			return FALSE;

		string headCode = recvMsg.substr(pos1 + strlen("<headCode>"),pos2 - pos1 - strlen("<headCode>"));
		if(headCode == "retAlertMsg")
		{
			CString wQueManNum;
			CDealInterMsg::AnaRetAlterCallerMsg(recvMsg,wQueManNum);
			CString wQueSerialID;
			theApp.m_Controller.GetQueSerialIDByManQueNum(wQueSerialID,wQueManNum);

			std::map<UINT,SLZWindow>::const_iterator iteraWindow =
				pThis->m_pSLZWindowView->m_mapIdWindow.begin();
			for(iteraWindow;iteraWindow != pThis->m_pSLZWindowView->m_mapIdWindow.end();
				iteraWindow++)
			{
				CStringArray queArrary;
				SLZWindow window = iteraWindow->second;
				window.GetArrayQueId(queArrary);
				for(int j=0; j<queArrary.GetCount(); j++)
				{
					if(wQueSerialID == queArrary[j])
					{
						SLZCallerData callerData;
						callerData.SetCallerId(window.GetCallerId());
						callerData.SetCmdType(callerCmdShowAlarm);
						SLZCCaller* pCaller = SLZCCaller::GetInstance();
						pCaller->AddWriteCallerData(callerData);
					}
				}
			}
		}
		else if(headCode == "retBrodcastNumMsg")
		{
			UINT nWaitNum = 0;
			CString wQueManNum;
			CDealInterMsg::AnaRetBrodcastNumMsg(wQueManNum,&nWaitNum,recvMsg);
			CString wQueSerialID;
			theApp.m_Controller.GetQueSerialIDByManQueNum(wQueSerialID,wQueManNum);
			theApp.m_pView->ShowWaitNum(wQueSerialID,nWaitNum);
		}
	}
	Sleep(100);
	return 0;
}