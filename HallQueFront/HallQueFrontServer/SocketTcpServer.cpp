#include "StdAfx.h"
#include "SocketTcpServer.h"
#include "ProducePacket.h"
#include "DbaConfig.h"

extern void MyWriteConsole(CString str);
CSocketTcpServer* g_scoketTcpServer;
CSocketTcpServer::CSocketTcpServer(void)
{
	m_uListenPort = 9527;
	m_uMaxListenCount = 5;
	m_index = 0;
	m_hAcceptThreadHandle = INVALID_HANDLE_VALUE;
	m_hWorhThreadHandle = INVALID_HANDLE_VALUE;
	m_bExiteThread = FALSE;
	g_scoketTcpServer = this;
	m_pDealData = CDealData::GetInstance();
//	m_uMaxDataLength = 
}
CSocketTcpServer::CSocketTcpServer(USHORT uListenPort,
								   UINT uMaxListenCount)
{
	m_uListenPort = uListenPort;
	m_uMaxListenCount = uMaxListenCount;

	m_index = 0;
	m_hAcceptThreadHandle = INVALID_HANDLE_VALUE;
	m_hWorhThreadHandle = INVALID_HANDLE_VALUE;
	m_bExiteThread = FALSE;
	g_scoketTcpServer = this;
	m_pDealData = CDealData::GetInstance();
}
CSocketTcpServer::~CSocketTcpServer(void)
{
	m_bExiteThread = TRUE;
//	WaitForSingleObject(m_hWorhThreadHandle, INFINITE); 
	TerminateThread(m_hAcceptThreadHandle,0);
	TerminateThread(m_hWorhThreadHandle,0);
	CloseHandle(m_hWorhThreadHandle);
	CloseHandle(m_hAcceptThreadHandle);

	if(INVALID_SOCKET != m_SockBound)  
	{  
		closesocket(m_SockBound);  
		m_SockBound = INVALID_SOCKET;  
	}  
	WSACleanup(); 
	SaveListenPort();
}

BOOL CSocketTcpServer::InitServer()
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
	m_SockBound = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,NULL,WSA_FLAG_OVERLAPPED);
	if (m_SockBound == INVALID_SOCKET)  
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
	iError = bind(m_SockBound,(LPSOCKADDR)&addr_Server,
		sizeof(addr_Server));
	if(iError == SOCKET_ERROR)
	{
		errcode.Format(_T("socket failed with error code: %d"),GetLastError());
		AfxMessageBox(errcode,MB_OK | MB_ICONERROR);
		closesocket(m_SockBound); //关闭套接字  
		WSACleanup();  
		return FALSE;
	}
	//侦听连接请求  
	iError = listen(m_SockBound, m_uMaxListenCount);  
	if (iError == SOCKET_ERROR)  
	{    
		errcode.Format(_T("socket failed with error code:%d"),GetLastError());
		AfxMessageBox(errcode,MB_OK | MB_ICONERROR);
		closesocket(m_SockBound); //关闭套接字  
		return FALSE; 
	}  
	
	m_hWorhThreadHandle = CreateThread(NULL,0,WorkThread,this,0,NULL);
	m_hAcceptThreadHandle = CreateThread(NULL,0,AcceptThread,this,0,NULL);
	return TRUE;
}

USHORT CSocketTcpServer::GetListenPort()
{
	CDbaConfig m_dbaConfig;
	CString port = m_dbaConfig.GetServerPort();
	int  iPort = 0;
	m_convert.CStringToint(iPort,port);
	return iPort;
}

BOOL CSocketTcpServer::SaveListenPort()
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

DWORD WINAPI CSocketTcpServer::AcceptThread(LPVOID lpParam)
{
	CSocketTcpServer* pServer = (CSocketTcpServer*)lpParam;
#ifdef _DEBUG
	CString errCode;
	errCode=_T("server start wait for client......");
	MyWriteConsole(errCode);
#endif
	while(1)
	{
		SOCKADDR_IN remote = {0};  
		int len = sizeof(remote);
		SOCKET sNew;
		try{
			ASSERT(&pServer->m_SockBound);
			sNew = accept(pServer->m_SockBound,(LPSOCKADDR)&remote,&len);
		}
		catch(...)
		{
			
		}
		if(INVALID_SOCKET == sNew)
		{
#ifdef _DEBUG
			errCode.Format(_T("accept err code:%d"),WSAGetLastError());
			MyWriteConsole(errCode);
#endif
			continue;
		} 
		else
		{
			CString ipAddress;
			pServer->m_convert.CharToCstring(ipAddress,inet_ntoa(remote.sin_addr));
#ifdef _DEBUG
			CString clientInfo;
			clientInfo.Format(_T("Client <%s : %d> come in..."),ipAddress,
				ntohs(remote.sin_port));
			MyWriteConsole(clientInfo); 
#endif
			pServer->m_ol[pServer->m_index] = new OVERLAPPEDINFO;
			pServer->m_ol[pServer->m_index]->sock = sNew;
			pServer->m_ol[pServer->m_index]->nOpType = OP_READ;
			memcpy(&(pServer->m_ol[pServer->m_index]->addr), &remote,
				sizeof(remote)); 
			if(SOCKET_ERROR == WSARecv(pServer->m_ol[pServer->m_index]->sock,
				&pServer->m_ol[pServer->m_index]->wsaBuf,
				1,&pServer->m_ol[pServer->m_index]->dwTrans,
				&pServer->m_ol[pServer->m_index]->dwFlags,
				&pServer->m_ol[pServer->m_index]->ol,
				CompleteRoutine))
			{
				if(WSA_IO_PENDING != WSAGetLastError())  
				{  
#ifdef _DEBUG
					errCode.Format(_T("WSARecv failed with error code: %d"),
						WSAGetLastError());
					MyWriteConsole(errCode);
#endif
					closesocket(pServer->m_ol[pServer->m_index]->sock);  
					delete pServer->m_ol[pServer->m_index];  
					pServer->m_ol[pServer->m_index] = NULL;  
					continue;  
				}  
			}
			pServer->m_index++;
		}
	}
	return 0;
}

DWORD WINAPI CSocketTcpServer::WorkThread(LPVOID lpParam)
{
	CSocketTcpServer* pServer = (CSocketTcpServer*) lpParam;
	CString errCode;
	while(!pServer->m_bExiteThread)  
	{  
		DWORD dwRet = SleepEx(INFINITE, TRUE);  
		if(WAIT_IO_COMPLETION == dwRet)  
		{  
			continue;
		}  
		else
		{
#ifdef _DEBUG
			errCode.Format(_T("SleepEx failed with error code: %d"),
				GetLastError());
			MyWriteConsole(errCode);
#endif
		}
	}  
	return 0;
}

void CALLBACK CSocketTcpServer::CompleteRoutine(DWORD dwError, 
												DWORD dwTrans,
												LPWSAOVERLAPPED Overlppad, 
												DWORD dwFlags)
{
	int nIndex = 0;  
	for(; nIndex<g_scoketTcpServer->m_index; nIndex++)  
	{  
		if(&g_scoketTcpServer->m_ol[nIndex]->ol == Overlppad)  
		{
			break;  
		}  
	} 
	CString ipAddress;
	g_scoketTcpServer->m_convert.CharToCstring(ipAddress,
		inet_ntoa(g_scoketTcpServer->m_ol[nIndex]->addr.sin_addr));
	CString errCode;
	if(0 != dwError || 0 == dwTrans)  
	{  
#ifdef _DEBUG
		errCode.Format(_T("Client: <%s : %d> leave...."),
			ipAddress,
			ntohs(g_scoketTcpServer->m_ol[nIndex]->addr.sin_port));
		MyWriteConsole(errCode);
#endif
		closesocket(g_scoketTcpServer->m_ol[nIndex]->sock);
#ifdef _DEBUG
		errCode.Format(_T("%d nIndex from array"),nIndex);
		MyWriteConsole(errCode);
#endif
		delete g_scoketTcpServer->m_ol[nIndex];
		// Error or closesocket by peer  
		for(int i=nIndex; i<g_scoketTcpServer->m_index-1; i++)  
		{  
			g_scoketTcpServer->m_ol[i] = g_scoketTcpServer->m_ol[i+1]; 
		}  
		g_scoketTcpServer->m_index--; 
	} 
	else
	{
		switch(g_scoketTcpServer->m_ol[nIndex]->nOpType)
		{
		case OP_READ:
			{
#ifdef _DEBUG
				CString msg;
				g_scoketTcpServer->m_convert.CharToCstring(msg,
					g_scoketTcpServer->m_ol[nIndex]->szBuf);
				errCode.Format(_T("recv <%s : %d> data: %s"),
					ipAddress,
					ntohs(g_scoketTcpServer->m_ol[nIndex]->addr.sin_port),
					msg);
				MyWriteConsole(errCode);
#endif
				g_scoketTcpServer->m_ol[nIndex]->nOpType = OP_WRITE;  
				memset(&(g_scoketTcpServer->m_ol[nIndex]->ol), 0, 
					sizeof(g_scoketTcpServer->m_ol[nIndex]->ol));
				//////////把接收到的报文拷贝到string
				std::string recvPacket(g_scoketTcpServer->m_ol[nIndex]->szBuf);
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
					g_scoketTcpServer->m_pDealData->AddPacket(recvPacket);
					break;
				}
				//返回报文
				std::string retPacket;
				retPacket = producePacket.ProduceSendRet(iErrCode,recvPacket);
				int size = retPacket.size();
				strncpy_s(g_scoketTcpServer->m_ol[nIndex]->szBuf,MAX_BUFFER,retPacket.c_str(),MAX_BUFFER-1);
				g_scoketTcpServer->m_ol[nIndex]->wsaBuf.buf = g_scoketTcpServer->m_ol[nIndex]->szBuf;
				g_scoketTcpServer->m_ol[nIndex]->wsaBuf.len=MAX_BUFFER;

				if(SOCKET_ERROR  == WSASend(g_scoketTcpServer->m_ol[nIndex]->sock,
					&(g_scoketTcpServer->m_ol[nIndex]->wsaBuf), 1, 
					&(g_scoketTcpServer->m_ol[nIndex]->dwTrans), 
					g_scoketTcpServer->m_ol[nIndex]->dwFlags, 
					&(g_scoketTcpServer->m_ol[nIndex]->ol), 
					CompleteRoutine))  
				{  
					if(WSA_IO_PENDING != WSAGetLastError())  
					{  
#ifdef _DEBUG
						errCode.Format(_T("WSASend failed with error code: %d"),
							WSAGetLastError());
						MyWriteConsole(errCode);
#endif
						closesocket(g_scoketTcpServer->m_ol[nIndex]->sock);
						// Error or closesocket by peer  
						for(int i=nIndex; i<g_scoketTcpServer->m_index-1; i++)  
						{  
							g_scoketTcpServer->m_ol[i] = g_scoketTcpServer->m_ol[i+1]; 
						}  
						g_scoketTcpServer->m_index--; 
					}  
				} 
			}
			break;
		case OP_WRITE:
			{
				g_scoketTcpServer->m_ol[nIndex]->nOpType = OP_READ;  
				dwFlags = 0;  
				memset(&(g_scoketTcpServer->m_ol[nIndex]->ol), 0,
					sizeof(g_scoketTcpServer->m_ol[nIndex]->ol));  
				memset(g_scoketTcpServer->m_ol[nIndex]->szBuf, 0, 
					sizeof(g_scoketTcpServer->m_ol[nIndex]->szBuf));  
				g_scoketTcpServer->m_ol[nIndex]->wsaBuf.buf = 
					g_scoketTcpServer->m_ol[nIndex]->szBuf;  
				dwTrans = g_scoketTcpServer->m_ol[nIndex]->wsaBuf.len = MAX_BUFFER;  
				if(SOCKET_ERROR == WSARecv(g_scoketTcpServer->m_ol[nIndex]->sock,
					&(g_scoketTcpServer->m_ol[nIndex]->wsaBuf), 1, 
					&dwTrans, &dwFlags, 
					&(g_scoketTcpServer->m_ol[nIndex]->ol), 
					CompleteRoutine))  
				{  
					if(WSA_IO_PENDING != WSAGetLastError())  
					{  
#ifdef _DEBUG
						errCode.Format(_T("WSARecv failed with error code: %d"),
							WSAGetLastError());
						MyWriteConsole(errCode);
#endif				
						closesocket(g_scoketTcpServer->m_ol[nIndex]->sock);
						// Error or closesocket by peer  
						for(int i=nIndex; i<g_scoketTcpServer->m_index-1; i++)  
						{  
							g_scoketTcpServer->m_ol[i] = g_scoketTcpServer->m_ol[i+1]; 
						}  
						g_scoketTcpServer->m_index--; 
					}  
				}  
			}
			break;
		}
	}
}

void CSocketTcpServer::SendMsg(CString msg)
{
	char buf[MAX_BUFFER]={0};
	m_convert.CStringToChar(msg,buf);
	for(int i=0;i<m_index;i++)
	{
		m_ol[i]->nOpType = OP_WRITE;
		memset(&m_ol[i]->ol, 0, sizeof(m_ol[i]->ol));
		strcpy_s(m_ol[i]->szBuf,MAX_BUFFER,buf);
		m_ol[i]->wsaBuf.buf=m_ol[i]->szBuf;
		m_ol[i]->wsaBuf.len = MAX_BUFFER;
		if(SOCKET_ERROR==WSASend(m_ol[i]->sock,&(m_ol[i]->wsaBuf), 1, 
			&(m_ol[i]->dwTrans), m_ol[i]->dwFlags, 
			&(m_ol[i]->ol), CompleteRoutine))
		{
#ifdef _DEBUG
			CString ipAddress;
			m_convert.CharToCstring(ipAddress,inet_ntoa(m_ol[i]->addr.sin_addr));
			CString errcode;
			errcode.AppendFormat(_T("client %s socket broken"),ipAddress);
			MyWriteConsole(ipAddress);
#endif
		}
	}
}
