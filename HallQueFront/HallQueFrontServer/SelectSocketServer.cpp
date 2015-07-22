#include "StdAfx.h"
#include "SelectSocketServer.h"
#include "DbaConfig.h"
#include "..\HallQueFront\DoFile.h"
#include "HallQueFrontServer.h"
#include "../HallQueFront/WriteLogError.h"
#include "../HallQueFront/ComputeFuncationTime.h"

CSelectSocketServer* pServer;
extern void MyWriteConsole(CString str);
CSelectSocketServer::CSelectSocketServer(void)
: m_iTotalConn(0)
,m_hAcceptThreadHandle(INVALID_HANDLE_VALUE)
,m_hWorhThreadHandle(INVALID_HANDLE_VALUE)
{
	m_pDealData = CDealData::GetInstance();
	pServer = this;
}

CSelectSocketServer::~CSelectSocketServer(void)
{
	if(m_hAcceptThreadHandle!=INVALID_HANDLE_VALUE)
	{
		TerminateThread(m_hAcceptThreadHandle,0);
		CloseHandle(m_hAcceptThreadHandle);
	}
	if(m_hWorhThreadHandle!=INVALID_HANDLE_VALUE)
	{
		TerminateThread(m_hWorhThreadHandle,0);
		CloseHandle(m_hWorhThreadHandle);
	}
	if(INVALID_SOCKET != m_ListeningSocket)  
	{  
		closesocket(m_ListeningSocket);  
		m_ListeningSocket = INVALID_SOCKET;  
	}  
	WSACleanup(); 
	SaveListenPort();
}

BOOL CSelectSocketServer::InitServer()
{
	WSADATA wsaData;
	SOCKADDR_IN addr_Server;//地址家族
	CString errcode;
	//winsock初始化
	int iError = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(iError != 0)
	{
		errcode.Format(_T("WSAStartup failed with error: %d"),GetLastError);
		AfxMessageBox(errcode);
		return FALSE;
	}
	//创建一个套接字绑定到一个特定的协议 
	//	m_SockBound = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	m_ListeningSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_ListeningSocket == INVALID_SOCKET)  
	{  
		errcode.Format(_T("WSAStartup failed with error: %d"),GetLastError);
		AfxMessageBox(errcode);
		WSACleanup();  //terminates use of the Winsock 2 DLL 
		return FALSE;  
	}  
	//构建本地地址信息  
	addr_Server.sin_family = AF_INET; //地址家族  
	USHORT port = GetListenPort();
	if(port > 0)
	{
		m_uListenPort = port;
	}
	//注意转化为网络字节序
	addr_Server.sin_port = htons(m_uListenPort);  
	//使用INADDR_ANY 指示任意地址 
	addr_Server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//绑定
	iError = bind(m_ListeningSocket,(LPSOCKADDR)&addr_Server,
		sizeof(addr_Server));
	if(iError == SOCKET_ERROR)
	{
		errcode.Format(_T("socket failed with error code: %d"),GetLastError());
		AfxMessageBox(errcode,MB_OK | MB_ICONERROR);
		closesocket(m_ListeningSocket); //关闭套接字  
		WSACleanup();  
		return FALSE;
	}
	//侦听连接请求  
	iError = listen(m_ListeningSocket, 5);  
	if (iError == SOCKET_ERROR)  
	{    
		errcode.Format(_T("socket failed with error code:%d"),GetLastError());
		AfxMessageBox(errcode,MB_OK | MB_ICONERROR);
		closesocket(m_ListeningSocket); //关闭套接字  
		return FALSE; 
	} 
	//创建工作者线程
	m_hWorhThreadHandle = CreateThread(NULL,0,WorkerThread,this,0,NULL);
	// 接受新的连接
	m_hAcceptThreadHandle = CreateThread(NULL,0,AcceptThread,this,0,NULL);
	return TRUE;
}

USHORT CSelectSocketServer::GetListenPort()
{
	CDbaConfig m_dbaConfig;
	CString port = m_dbaConfig.GetServerPort();
	int  iPort = 0;
	m_convert.CStringToint(iPort,port);
	return iPort;
}

BOOL CSelectSocketServer::SaveListenPort()
{
	CString strPath=m_convert.GetExeFullFilePath();
	CDoFile dofile;
	CString port;
	m_convert.intToCString(m_uListenPort,port);
	strPath+=_T("\\Port");
	dofile.CreateMyDirectory(strPath);
	strPath+=_T("\\ServerPort.ini");
	return WritePrivateProfileString(_T("port"),_T("PORT"),port,strPath);
}

DWORD WINAPI CSelectSocketServer::AcceptThread(LPVOID lpParam)
{
	CSelectSocketServer* pThis = (CSelectSocketServer*)lpParam;
	while(1)
	{
		SOCKET NewConnection;
		SOCKADDR_IN clientAddr;
		int len = sizeof(clientAddr);
		NewConnection = WSAAccept(pThis->m_ListeningSocket, (sockaddr*)&clientAddr, &len, ConditionFunc, 0);
		if(NewConnection != SOCKET_ERROR)
		{
			pThis->m_muServerLock.Lock();
			pThis->m_CliSocketArr[pThis->m_iTotalConn++] = NewConnection;
#ifdef _DEBUG
			CString ipAddress;
			pThis->m_convert.CharToCstring(ipAddress,
				inet_ntoa(clientAddr.sin_addr));
			CString clientInfo;
			clientInfo.Format(_T("Client <%s : %d> come in..."),ipAddress,
				ntohs(clientAddr.sin_port));
			MyWriteConsole(clientInfo); 
#endif
// 			CWriteLogError log;
// 			CString str;
// 			str.Format(_T("accept succ client num:%d"),pThis->m_iTotalConn);
// 			log.WriteErrLog(str);
			pThis->m_muServerLock.Unlock();
		}
		else
		{
#ifdef _DEBUG
			MyWriteConsole(_T("accpet error"));
#endif
			CWriteLogError log;
			CString str;
			str.Format(_T("accept error client num:%d"),pThis->m_iTotalConn);
			log.WriteErrLog(str);
		}
	}
	return 0;
}

DWORD WINAPI CSelectSocketServer::WorkerThread(LPVOID lpParam)
{
	CSelectSocketServer* pThis = (CSelectSocketServer*)lpParam;
	fd_set fdread;
	int ret;
	struct timeval tv = {1, 0};
	char szMessage[MAX_BUFFER+1]={0};
	while(TRUE)
	{
		memset(szMessage,0,MAX_BUFFER+1);
		FD_ZERO(&fdread);//初始化
		if(pThis->m_iTotalConn==0)
		{
			Sleep(20);
		}
		else
		{
			for(int i=0;i<pThis->m_iTotalConn;i++)
			{
				FD_SET(pThis->m_CliSocketArr[i], &fdread);//将新的连接加入集合fdread
			}
		//确定一个或多个套接字的状态，等待如果必要的话，执行同步I/O
			ret = select(0,&fdread,NULL,NULL,&tv);
			if (ret == 0)
			{
				// Time expired
				continue;
			}

			for(int i=0;i<pThis->m_iTotalConn;i++)
			{
				/*在调用select()函数后，用FD_ISSET来检测fd在fdset集合
				中的状态是否变化返回整型，当检测到fd状态发生变化时返回真，
				否则，返回假（0）*/
				if(FD_ISSET(pThis->m_CliSocketArr[i],&fdread))//检测
				{
#ifdef _DEBUG
					CComputeFuncationTime time;
					time.SetStartTime(clock());
#endif
					//接收
					ret = recv(pThis->m_CliSocketArr[i], szMessage, MAX_BUFFER, 0);
					if (ret == 0 || (ret == SOCKET_ERROR))
					{
						pThis->DeleteClient(i);
					}
					else
					{
						//接收到以后处理
#ifdef _DEBUG
						CString msg;
						pThis->m_convert.CharToCstring(msg,szMessage);
						MyWriteConsole(msg);
#endif
						std::string recvPacket(szMessage);
						std::string retPacket = pThis->DealMsg(recvPacket);
#ifdef _DEBUG
						CString wRetPacket(retPacket.c_str());
						MyWriteConsole(wRetPacket);
#endif
						int size = retPacket.size();
						int nTimeOut=1000;
						int actSendSize = 0;
						setsockopt(pThis->m_CliSocketArr[i],SOL_SOCKET,SO_RCVTIMEO,(char *)&nTimeOut,sizeof(UINT));
						while(true)
						{
							int tempSize = send(pThis->m_CliSocketArr[i], retPacket.c_str(), size, 0);
							if(tempSize == SOCKET_ERROR)
							{
								pThis->DeleteClient(i);
								CWriteLogError log;
								CString str;
								str.Format(_T("send errcode:%d client num:%d"),WSAGetLastError(),i);
								log.WriteErrLog(str);
								break;
							}
							else
							{
								actSendSize += tempSize;
								if(actSendSize == size)break;
							}
						}
					}
#ifdef _DEBUG
					time.SetFinshTime(clock());
					double dura = time.GetDuration();
					CString strDuration;
					strDuration.Format(_T("dealtime:%f"),dura);
					MyWriteConsole(strDuration);
#endif
				}
				else
				{
					pThis->DeleteClient(i);
					CWriteLogError log;
					CString str;
					str.Format(_T("FISSET errcode:%d client num:%d"),WSAGetLastError(),i);
					log.WriteErrLog(str);
				}
			}
		}
	}	
	return 0;
}

std::string CSelectSocketServer::DealMsg(const std::string recvPacket)
{
	///判断报文
	CProducePacket producePacket;
	int iErrCode = producePacket.JudgeSendPacket(recvPacket);
	switch(iErrCode)
	{
	case 1:
	case 26:
	case 27:
	case 28:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
		m_pDealData->AddPacket(recvPacket);
		break;
	}
	//返回报文
	std::string retPacket;
	retPacket = producePacket.ProduceSendRet(iErrCode,recvPacket);
	return retPacket;
}

void CSelectSocketServer::DeleteClient(int i)
{
	closesocket(m_CliSocketArr[i]);
#ifdef _DEBUG
	CString str;
	str.Format(_T("%d client"),m_iTotalConn);
	MyWriteConsole(str);
#endif
	if(i == m_iTotalConn-1)//删除最后一个
	{
		m_iTotalConn--;
		m_CliSocketArr[i]=0;
	}
	else if(i < m_iTotalConn-1)      
	{     
		m_CliSocketArr[i--] = m_CliSocketArr[--m_iTotalConn];     
	}
}

int CALLBACK CSelectSocketServer::ConditionFunc(LPWSABUF lpCallerId,LPWSABUF lpCallerData, LPQOS lpSQOS,LPQOS lpGQOS,LPWSABUF lpCalleeId, LPWSABUF lpCalleeData,GROUP FAR * g,DWORD dwCallbackData) 
{  
	if (pServer->m_iTotalConn < FD_SETSIZE)  
		return CF_ACCEPT;  
	else  
		return CF_REJECT;  
}  