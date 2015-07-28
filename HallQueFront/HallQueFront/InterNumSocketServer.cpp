#include "StdAfx.h"
#include "InterNumSocketServer.h"
#include "CommonConvert.h"
#include "WriteLogError.h"
#include "DealInterMsg.h"
#include "HallQueFront.h"
#include "QueueInfo.h"

extern void MyWriteConsole(CString str);

CInterNumSocketServer* pServer;

CInterNumSocketServer::CInterNumSocketServer(CInlineQueData* pInlineQueData)
: m_iTotalConn(0)
,m_hAcceptThreadHandle(INVALID_HANDLE_VALUE)
,m_hWorhThreadHandle(INVALID_HANDLE_VALUE)
,m_uListenPort(INTERPORT)
,m_pInlineQueData(NULL)
{
	pServer = this;
	m_pInlineQueData = pInlineQueData;
}

CInterNumSocketServer::~CInterNumSocketServer(void)
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
}

BOOL CInterNumSocketServer::InitServer()
{
	WSADATA wsaData;
	SOCKADDR_IN addr_Server;//��ַ����
	CString errcode;
	//winsock��ʼ��
	int iError = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(iError != 0)
	{
		errcode.Format(_T("WSAStartup failed with error: %d"),GetLastError);
		AfxMessageBox(errcode);
		return FALSE;
	}
	//����һ���׽��ְ󶨵�һ���ض���Э�� 
	//	m_SockBound = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	m_ListeningSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_ListeningSocket == INVALID_SOCKET)  
	{  
		errcode.Format(_T("WSAStartup failed with error: %d"),GetLastError);
		AfxMessageBox(errcode);
		WSACleanup();  //terminates use of the Winsock 2 DLL 
		return FALSE;  
	}  
	//�������ص�ַ��Ϣ  
	addr_Server.sin_family = AF_INET; //��ַ����  
	//ע��ת��Ϊ�����ֽ���
	addr_Server.sin_port = htons(m_uListenPort);  
	//ʹ��INADDR_ANY ָʾ�����ַ 
	addr_Server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//��
	iError = bind(m_ListeningSocket,(LPSOCKADDR)&addr_Server,
		sizeof(addr_Server));
	if(iError == SOCKET_ERROR)
	{
		errcode.Format(_T("socket failed with error code: %d"),GetLastError());
		AfxMessageBox(errcode,MB_OK | MB_ICONERROR);
		closesocket(m_ListeningSocket); //�ر��׽���  
		WSACleanup();  
		return FALSE;
	}
	//������������  
	iError = listen(m_ListeningSocket, 5);  
	if (iError == SOCKET_ERROR)  
	{    
		errcode.Format(_T("socket failed with error code:%d"),GetLastError());
		AfxMessageBox(errcode,MB_OK | MB_ICONERROR);
		closesocket(m_ListeningSocket); //�ر��׽���  
		return FALSE; 
	} 
	//�����������߳�
	m_hWorhThreadHandle = CreateThread(NULL,0,WorkerThread,this,0,NULL);
	// �����µ�����
	m_hAcceptThreadHandle = CreateThread(NULL,0,AcceptThread,this,0,NULL);
	return TRUE;
}

DWORD WINAPI CInterNumSocketServer::AcceptThread(LPVOID lpParam)
{
	CInterNumSocketServer* pThis = (CInterNumSocketServer*)lpParam;
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
			CCommonConvert::CharToCstring(ipAddress,inet_ntoa(clientAddr.sin_addr));
			CString clientInfo;
			clientInfo.Format(_T("Client <%s : %d> come in..."),ipAddress,
				ntohs(clientAddr.sin_port));
			MyWriteConsole(clientInfo); 
#endif
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

DWORD WINAPI CInterNumSocketServer::WorkerThread(LPVOID lpParam)
{
	CInterNumSocketServer* pThis = (CInterNumSocketServer*)lpParam;
	fd_set fdread;
	int ret;
	struct timeval tv = {1, 0};
	char szMessage[MAX_BUFFER+1]={0};
	while(TRUE)
	{
		memset(szMessage,0,MAX_BUFFER+1);
		FD_ZERO(&fdread);//��ʼ��
		if(pThis->m_iTotalConn==0)
		{
			Sleep(20);
		}
		else
		{
			for(int i=0;i<pThis->m_iTotalConn;i++)
			{
				FD_SET(pThis->m_CliSocketArr[i], &fdread);//���µ����Ӽ��뼯��fdread
			}
		//ȷ��һ�������׽��ֵ�״̬���ȴ������Ҫ�Ļ���ִ��ͬ��I/O
			ret = select(0,&fdread,NULL,NULL,&tv);
			if (ret == 0)
			{
				// Time expired
				continue;
			}

			for(int i=0;i<pThis->m_iTotalConn;i++)
			{
				/*�ڵ���select()��������FD_ISSET�����fd��fdset����
				�е�״̬�Ƿ�仯�������ͣ�����⵽fd״̬�����仯ʱ�����棬
				���򣬷��ؼ٣�0��*/
				if(FD_ISSET(pThis->m_CliSocketArr[i],&fdread))//���
				{
					//����
					ret = recv(pThis->m_CliSocketArr[i], szMessage, MAX_BUFFER, 0);
					if (ret == 0 || (ret == SOCKET_ERROR))
					{
						pThis->DeleteClient(i);
					}
					else
					{
						//���յ��Ժ���
#ifdef _DEBUG
						CString msg;
						CCommonConvert::CharToCstring(msg,szMessage);
						MyWriteConsole(msg);
#endif
						std::string recvPacket(szMessage);
						std::string retPacket;
						pThis->DealMsg(recvPacket,retPacket);
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

void CInterNumSocketServer::DeleteClient(int i)
{
	closesocket(m_CliSocketArr[i]);
#ifdef _DEBUG
	CString str;
	str.Format(_T("%d client closed"),m_iTotalConn);
	MyWriteConsole(str);
#endif
	if(i == m_iTotalConn-1)//ɾ�����һ��
	{
		m_iTotalConn--;
		m_CliSocketArr[i]=0;
	}
	else if(i < m_iTotalConn-1)      
	{     
		m_CliSocketArr[i--] = m_CliSocketArr[--m_iTotalConn];     
	}
}

int CALLBACK CInterNumSocketServer::ConditionFunc(LPWSABUF lpCallerId,LPWSABUF lpCallerData, LPQOS lpSQOS,LPQOS lpGQOS,LPWSABUF lpCalleeId, LPWSABUF lpCalleeData,GROUP FAR * g,DWORD dwCallbackData) 
{  
	if (pServer->m_iTotalConn < FD_SETSIZE)  
		return CF_ACCEPT;  
	else  
		return CF_REJECT;  
}  


void CInterNumSocketServer::DealMsg(const string& recvPacket,string& retPacket)
{
	string::size_type pos1 = recvPacket.find("<headCode>");
	string::size_type pos2 = recvPacket.find("</headCode>");
	if(pos1 == recvPacket.npos || pos2 == recvPacket.npos)
		return;

	string headCode = recvPacket.substr(pos1 + strlen("<headCode>"),pos2 - pos1 - strlen("<headCode>"));
	if(headCode == "sendInnumMsg")
	{
		CString queManNum,queserial_id;

		CDealInterMsg::AnaSendInNumMsg(recvPacket,queManNum);
		theApp.m_Controller.GetQueSerialIDByManQueNum(queserial_id,queManNum);//�õ�queid

		UINT nWaitNum = 0;
		if(m_pInlineQueData)
		{
			m_pInlineQueData->GetAllBussCount(queserial_id,&nWaitNum);
		}

		CDealInterMsg::ProduceRetInNumMsg(nWaitNum,retPacket);
	}
	else if(headCode == "sendInterMsg")//�ͻ�������ȡ������
	{
		CString queManNum,queserial_id,organId,organName;

		CDealInterMsg::AnaSendInterMsg(recvPacket,queManNum,organId,organName);//�õ�queManNum
		theApp.m_Controller.GetQueSerialIDByManQueNum(queserial_id,queManNum);//�õ�queid
		
		UINT maxQueNum = 0;
		UINT nWaitNum = 0;

		theApp.m_Controller.ModifyQueNum(queserial_id,&maxQueNum);
		CQueueInfo queInfo;
		theApp.m_Controller.GetQueueInfoBySerialID(queserial_id,queInfo);	

		SLZData data;//�������һ����,���ɿͻ�������
		data.SetBussinessType(queserial_id);
		data.SetQueSerialID(queManNum);
		data.SetBussName(queInfo.GetBussName());
		data.SetIntQueNum(maxQueNum);
		CString StrQueNum;//�ŶӺ���str
		StrQueNum.Format(_T("%03d"),maxQueNum);
		StrQueNum=queInfo.GetFrontID()+StrQueNum;//��ǰ׺
		///////////////////////////////////////////
		data.SetQueueNumber(StrQueNum);//����data�ŶӺ���
		CTime GetTime;//����ȡ��ʱ��
		GetTime=CTime::GetCurrentTime();
		data.SetTakingNumTime(GetTime);
		data.SetOrganId(organId);
		data.SetOrganName(organName);


		if (!theApp.m_Controller.InsertListData(data))////д�뵱ǰ�ŶӺ��뵽list����
		{
			theApp.m_Controller.m_list_Data.AddTail(data);
		}
		theApp.m_Controller.WriteListQueIntoFile();///����Listд���ļ���ϵͳ����ʱ��Ҫ��ȡ
		m_pInlineQueData->Add(data);

		if(m_pInlineQueData)
		{
			m_pInlineQueData->GetAllBussCount(queserial_id,&nWaitNum);
		}

		theApp.m_pView->ShowWaitNum(queserial_id,nWaitNum);///������ʾ�ȴ�����

		if(theApp.IsLocal())
			theApp.m_Controller.WriteInlineDataToFile();
#ifdef _DEBUG
		CString sendMsg(retPacket.c_str());
		MyWriteConsole(sendMsg);
#endif
		CDealInterMsg::ProduceRetInterMsg(&data,nWaitNum,retPacket);
	}
	else if(headCode == "sendCallMsg")
	{
		CStringArray queManNumArray,queserial_id_array;
		CString organId,queserial_id;
		BOOL bIsUsePower = FALSE;
		CDealInterMsg::AnaSendCallMsg(recvPacket,queManNumArray,organId,&bIsUsePower);
		
		for(int i=0;i<queManNumArray.GetCount();++i)
		{
			theApp.m_Controller.GetQueSerialIDByManQueNum(queserial_id,queManNumArray.GetAt(i));//�õ�queid
			if(!queserial_id.IsEmpty())
				queserial_id_array.Add(queserial_id);
		}
		
		//////
		SLZData data;
		BOOL isSucced = m_pInlineQueData->DeleteInlineClientData(bIsUsePower,queserial_id_array,organId,&data);

		CDealInterMsg::ProduceRetCallMsg(isSucced,retPacket,&data);
		
		UINT nWaitNum = 0;
		if(m_pInlineQueData)
		{
			m_pInlineQueData->GetAllBussCount(data.GetBussinessType(),&nWaitNum);
		}
		if(isSucced)
			theApp.m_pView->ShowWaitNum(data.GetBussinessType(),nWaitNum);///������ʾ�ȴ�����

		if(theApp.IsLocal())
			///����дfile������û�������У�������
			theApp.m_Controller.WriteInlineDataToFile();
	}
}