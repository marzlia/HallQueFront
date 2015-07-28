#include "StdAfx.h"
#include "DoComInOut.h"
#include "ComInit.h"
#include "CommonConvert.h"
extern void MyWriteConsole(CString str); 

CDoComInOut::CDoComInOut(void) :
m_hReadCallerThread(NULL)
, m_isDoneThroughInit(FALSE)
, m_hWriteComThread(NULL)
{
	m_pSlzCaller = SLZCCaller::GetInstance();
	m_pSlzEvaluator = SLZCEvaluator::GetInstance();
	m_pSlzWndScreen = SLZCWndScreen::GetInstance();
	Start();
}

CDoComInOut::~CDoComInOut(void)
{
	if(m_hReadCallerThread)
	{
		TerminateThread(m_hReadCallerThread,0);
		CloseHandle(m_hReadCallerThread);
		m_hReadCallerThread = NULL;
	}
	if(m_hWriteComThread)
	{
		TerminateThread(m_hWriteComThread,0);
		CloseHandle(m_hWriteComThread);
		m_hWriteComThread = NULL;
	}
}

DWORD WINAPI CDoComInOut::ReadCallerThread(LPVOID pParam)
{
	while(TRUE)
	{
		CDoComInOut* pThis=(CDoComInOut*)pParam;
		char buf[1024]={0};
		DWORD dwReaded=0;//�����Ĵ�С
		BOOL bres=FALSE;
		DWORD dwErrorFlags;
		COMSTAT ComStat;
//		OVERLAPPED m_osRead;
//		memset(&m_osRead,0,sizeof(OVERLAPPED));
		CComInit* pComInit = CComInit::GetInstance();
		if(pComInit->m_hComWndScreen
			!=INVALID_HANDLE_VALUE)
		{	
			//////���Ǽ���
			ClearCommError(pComInit->m_hComWndScreen,&dwErrorFlags,&ComStat);
			Sleep(100);
			bres=ReadFile(pComInit->m_hComWndScreen,buf,1024,&dwReaded,&pComInit->m_read_os);
			if(!bres)
			{
				if(GetLastError()==ERROR_IO_PENDING)
				{
					while(!GetOverlappedResult(pComInit->m_hComWndScreen, &pComInit->m_read_os,&dwReaded,TRUE))
					{
						dwErrorFlags = GetLastError();
						if(dwErrorFlags == ERROR_IO_INCOMPLETE) continue;
						else{
							ClearCommError(pComInit->m_hComWndScreen,&dwErrorFlags, &ComStat ) ;
							break;
						}
					}
				}
			}
			if(dwReaded>0)//������
			{
#ifdef _DEBUG
				unsigned char uBuf[1024]={0};
				memcpy(uBuf,buf,dwReaded);
				CString temp;
				for(UINT i=0;i<dwReaded;i++)
				{
					temp+=_T("0x");
					temp.AppendFormat(_T("%08x"),uBuf[i]);
					temp+=_T(" ");
				}
				MyWriteConsole(temp);
				MyWriteConsole(_T("------------------------------------------"));
				CString strBuf;
				CCommonConvert convert;
				convert.CharToCstring(strBuf,buf);
				MyWriteConsole(strBuf);
				MyWriteConsole(_T("------------------------------------------"));
				CString strDwread;
				MyWriteConsole(strDwread);
#endif
				//////���Ǻ����������۵�����,ע��������������������0xff,0x68��ʼ,0x16����
				if(buf[0]==(char)0xA0 && buf[1]==(char)0x90 && buf[dwReaded-1]==(char)0xAA && 
					buf[dwReaded-2]==(char)0xA5)
				{
					int wndID = buf[2];//����ַ
#ifdef _DEBUG
					CString str;
					str.Format(_T("����ַ:%d���ͳɹ�"),wndID);
					MyWriteConsole(str);
#endif
					//�������ۺ������ݣ����ͳɹ�
				}
				else if((buf[0]!=(char)0xff&&buf[2]!=(char)0x68) && buf[dwReaded-1]!=(char)0x16)
				{
					//ͨ������
					if(!pThis->m_isDoneThroughInit)
					{
// #ifdef _DEBUG
// 						MyWriteConsole(_T("ͬƵ����"));
// #endif

// 						SLZCWndScreen* pWindowScreen = SLZCWndScreen::GetInstance();
// 						pWindowScreen->AddThroughInitStr(buf,dwReaded);

//						SLZCWndScreen* pWindowScreen = SLZCWndScreen::GetInstance();
//						pWindowScreen->AddThroughInitStr(buf,dwReaded);

					}
				}
				else
				{
					if(buf[5]>0xffffff80)
					{/////////ƽ������Ϣ
						//���ظ���������Ϣ
						char evabuf[8]={0};
						pThis->m_pSlzEvaluator->SystemSendToEva(evabuf,buf);
						WriteComMsg* pMsg = new WriteComMsg;;
 						memset(pMsg->buf,0,DATABUFLEN);
						memcpy(pMsg->buf,evabuf,8);
						pMsg->length = 8;
						pThis->AddWriteComMsg(pMsg);
						WaitForSingleObject(pThis->m_hWriteComThread,80);
						//�ж����۳�ʱ����ֵ(��ǰһ������û����ʱ����ͬһ����������
						//����һ�����ۣ���Ҫ�жϸ�ֵ
						pThis->m_pSlzEvaluator->IsOutTimeAndReser(buf);
						//������������
						pThis->m_pSlzEvaluator->DoEvaltorMsg(buf);
					}
					else
					{//////////��������Ϣ
						//��������Ϣ
						WriteComMsg* pMsg = new WriteComMsg;
						memset(pMsg->buf,0,DATABUFLEN);
						memcpy(pMsg->buf,buf,dwReaded);
						pMsg->length = dwReaded;
						pThis->AddWriteComMsg(pMsg);
						WaitForSingleObject(pThis->m_hWriteComThread,80);
						pThis->m_pSlzCaller->DoReadMsg(dwReaded,buf);
					}	
				}
			}
			////////////////����
		}
		else
		{
			Sleep(20);
		}
	}
	return 0;
}

BOOL CDoComInOut::Start()
{
	BOOL flag = FALSE;
	if(m_hReadCallerThread==NULL)
	{
		m_hReadCallerThread = CreateThread(NULL,0,ReadCallerThread,this,0,NULL);
		if(m_hReadCallerThread)
		{
			flag = TRUE;
		}
	}
	if(m_hWriteComThread==NULL)
	{
		m_hWriteComThread = CreateThread(NULL,0,WriteComThread,this,0,NULL);
		if(m_hWriteComThread)
		{
			flag = TRUE;
		}
	}
	return flag;
}

DWORD WINAPI CDoComInOut::WriteComThread(LPVOID pParam)
{
	while(TRUE)
	{
		CDoComInOut* pThis = (CDoComInOut*)pParam;
		if(pThis->m_list_writeComMsg.size()==0)
		{
			Sleep(10);
		}
		else
		{
			pThis->m_readComLock.Lock();
			list<WriteComMsg*>::const_iterator itera = pThis->m_list_writeComMsg.begin();
			WriteComMsg* pMsg = *itera;
//			pThis->m_list_writeComMsg.erase(itera);
			pThis->m_list_writeComMsg.pop_front();
			pThis->m_readComLock.Unlock();
			CComInit* pComInit = CComInit::GetInstance();
			
			DWORD       dwErrorFlags;
			DWORD   	dwError;
			COMSTAT     ComStat;
#ifdef _DEBUG
			CString strLength;
			strLength.Format(_T("msg length:%d"),pMsg->length);
			MyWriteConsole(strLength);
#endif
//			OVERLAPPED osWrite;
//			memset(&osWrite,0,sizeof(osWrite));
//			do
//			{
				DWORD dwTrans;
				BOOL bWriteStat = WriteFile(pComInit->m_hComWndScreen,pMsg->buf,
					pMsg->length,NULL,&pComInit->m_write_os);
				if(!bWriteStat)
				{
					if(GetLastError()==ERROR_IO_PENDING)
					{	 
						while(!GetOverlappedResult(pComInit->m_hComWndScreen,&pComInit->m_write_os,&dwTrans,TRUE)){
							dwError = GetLastError();
							if(dwError == ERROR_IO_INCOMPLETE){
								continue;
							}
							else{
								 ClearCommError(pComInit->m_hComWndScreen, &dwErrorFlags, &ComStat ) ;
							}
						}
					}
				}
				
// #ifdef _DEBUG
// 				CString strCount;
// 				strCount.Format(_T("dwAcWrite:%d"),dwAcWrite);
// 				MyWriteConsole(strCount);
// #endif
//			}while(dwAcWrite < (UINT)pMsg->length);
			
// 			PurgeComm( pComInit->m_hComWndScreen, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR
// 				| PURGE_RXCLEAR );
			delete pMsg;
		}
	}
	return 0;
}

void CDoComInOut::AddWriteComMsg(WriteComMsg* pMsg)
{
	m_readComLock.Lock();
	m_list_writeComMsg.push_back(pMsg);
	m_readComLock.Unlock();
}