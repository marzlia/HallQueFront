#include "StdAfx.h"
#include "ShortMsgModem.h"
#include "ComInit.h"
#include "CommonConvert.h"
#include "CommonStrMethod.h"
CShortMsgModem::CShortMsgModem(void):m_ReadThread(NULL)
{
	m_lCom = 0;
	if (InitDemoCat())
	{
		CComInit* pComInit = CComInit::GetInstance();
		CString MsgCom = pComInit->GetMsgComm();
		int i_MsgCom = 0;
		CCommonConvert::CStringToint(i_MsgCom,MsgCom);
		CloseMsgComm();//lj修改当没有ini文件时出现bug,不知道到底怎么产生的
		if(i_MsgCom)//lj修改
			OpenMsgComm(i_MsgCom);
		m_ReadThread = CreateThread(NULL,0,ReadProc,this,0,0);
	}
	m_strLogFilePath = CommonStrMethod::GetModuleDir() + _T("ShortMsglog/");
}

CShortMsgModem::~CShortMsgModem(void)
{
	TerminateThread(m_ReadThread,0);
	if (m_ReadThread)
	{
		CloseHandle(m_ReadThread);
		m_ReadThread = NULL;
	}
	CloseMsgComm();
	if(hInsDLL!=NULL)
		FreeLibrary(hInsDLL);
	
}

BOOL CShortMsgModem::InitDemoCat()
{
	hInsDLL = NULL;
	hInsDLL=LoadLibrary(L"MonDem.dll");
	if(hInsDLL==NULL) hInsDLL=LoadLibrary(L"..\\MonDem");
	if(hInsDLL==NULL){
		AfxMessageBox(L"装载动态连接库MonDem.dll错误");
		return FALSE;
	} 
	else
	{
		fnInitModem=(LPFNDLLFUNC1)GetProcAddress(hInsDLL,"fnInitModem");
		fnCloseModem=(LPFNDLLFUNC2)GetProcAddress(hInsDLL,"fnCloseModem");
		fnSendMsg=(LPFNDLLFUNC3)GetProcAddress(hInsDLL,"fnSendMsg");
		fnReadMsgEx=(LPFNDLLFUNC4)GetProcAddress(hInsDLL,"fnReadMsgEx");
		fnReadMsgExNotDelete=(LPFNDLLFUNC4)GetProcAddress(hInsDLL,"fnReadMsgExNotDelete");
		fnGetStatus=(LPFNDLLFUNC5)GetProcAddress(hInsDLL,"fnGetStatus");
		fnGetSndCount=(LPFNDLLFUNC5)GetProcAddress(hInsDLL,"fnGetSndCount");
		fnGetRecCount=(LPFNDLLFUNC5)GetProcAddress(hInsDLL,"fnGetRecCount");
		fnClrSndBuf=(LPFNDLLFUNC5)GetProcAddress(hInsDLL,"fnClrSndBuf");
		fnClrRecBuf=(LPFNDLLFUNC5)GetProcAddress(hInsDLL,"fnClrRecBuf");
		fnDogSerial=(LPFNDLLFUNC6)GetProcAddress(hInsDLL,"fnDogSerial");
		fnSetModemType=(LPFNDLLFUNC7)GetProcAddress(hInsDLL,"fnSetModemType");
		fnDeleteSimMsg=(LPFNDLLFUNC7)GetProcAddress(hInsDLL,"fnDeleteSimMsg");
		fnWapPushCvt=(LPFNDLLFUNC8)GetProcAddress(hInsDLL,"fnWapPushCvt");
	}


	return TRUE;
}

BOOL CShortMsgModem::OpenMsgComm(int iCom)
{
	m_lCom = iCom-1;	
	for(int i=0;i<10;i++) fnSetModemType(i,0);
		int iresult = fnInitModem(m_lCom);
		if (iresult==0)
		{
			return TRUE;
		}
		else
		{
			/*WriteErrLog(L"初始化短信猫串口错误");*/
			return FALSE;
		}
}

BOOL CShortMsgModem::SendMsg(CString strPhoneNum,CString strMsg)
{
	int numlength = CCommonConvert::CStringToChar(strPhoneNum,NULL);
	char* szHeader = new char[numlength+1];
	CCommonConvert::CStringToChar(strPhoneNum,szHeader);
	int HeaderLength = CCommonConvert::CStringToChar(strMsg,NULL);
	char* szMsg = new char[HeaderLength+1];
	CCommonConvert::CStringToChar(strMsg,szMsg);
	int nRc=fnSendMsg(m_lCom,szHeader,szMsg);
	if (nRc==m_lCom)
	{
		WriteLogWithTime(L"Send:"+strPhoneNum);
		return TRUE;
	}
	else
	{
		WriteErrLog(L"发送短信失败");
		return FALSE;
	}
}

BOOL CShortMsgModem::ReadMsg()
{
	int icount = fnGetRecCount(m_lCom);
	if (icount>0)
	{
		for (int i=0;i<icount;i++)
		{
			char szHeader[100]={0};
			char szMsg[500]={0};
			CString csTmp;
			int nRc=fnReadMsgEx(m_lCom,szHeader,szMsg);
			if((nRc==0)) 
			{
				int nRecType=atoi(szHeader);
				int pno=atoi(szHeader+2);
				CString strHeader;
				CString strMsg;
				CCommonConvert::CharToCstring(strHeader,szHeader);
				CCommonConvert::CharToCstring(strMsg,szMsg);
				switch(nRecType){
				case 0:csTmp.Format(L"端口:%d, 类型:接收信息",pno+1);break;
				case 1:csTmp.Format(L"端口:%d, 类型:发送成功报告",pno+1); break;
				case 2:csTmp.Format(L"端口:%d, 类型:发送失败报告",pno+1); break;
				case 3:csTmp.Format(L"端口:%d, 类型:发送失败超时1",pno+1); break;
				case 4:csTmp.Format(L"端口:%d, 类型:发送失败超时2",pno+1); break;
				case 5:csTmp.Format(L"端口:%d, 类型:取消发送",pno+1); break;
				case 6:csTmp.Format(L"端口:%d, 类型:接收状态报告",pno+1); break;
				default:
						csTmp.Format(L"端口:%d, 类型:未知状态",pno+1); break;
						break;
				}
				csTmp+=L",参数:"+strHeader+L",信息:"+strMsg;
				WriteLogWithTime(csTmp);
			}
		}
	}
	return TRUE;
}

DWORD WINAPI  CShortMsgModem::ReadProc(LPVOID pParam)
{
	CShortMsgModem* ShortMsg = (CShortMsgModem*)pParam;
	while(1)
	{
		ShortMsg->ReadMsg();
		Sleep(200);
	}
	return 0;
}

BOOL CShortMsgModem::ClearSendBox()
{
	int nRc=fnClrSndBuf(m_lCom);
	return TRUE;
}

void CShortMsgModem::WriteErrLog(CString strSockLog)
{
	CString str = _T("Error: ") + strSockLog;
	WriteLogWithTime(str);
}

void CShortMsgModem::WriteLogWithTime(CString strSockLog)
{
	CTime time = CTime::GetCurrentTime();
	CString str = time.Format(_T("%Y-%m-%d %H:%M:%S")) + _T("  \t") + strSockLog + _T("\r\n\r\n");
	CString strLogFile = time.Format(_T("log_%Y%m%d.log"));
	if(!CommonStrMethod::PathFileExist(m_strLogFilePath))
	{
		if(!CommonStrMethod::CreatePath(m_strLogFilePath))
		{
			return;
		}
	}
	AppendWriteFile(str, m_strLogFilePath + strLogFile);
}

BOOL CShortMsgModem::AppendWriteFile(CString strText, CString strFileName)
{
	CFile file;
	if(!file.Open(strFileName, 
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		//CString str;
		//str.Format(_T("创建或打开日志文件失败,您所使用的计算机帐号没有相应的磁盘写权限:\r\n\t%s"), g_pControl->m_strLogFilePath);
		//AfxMessageBox(str);
		return FALSE;
	}
	CHAR* szBuf = (CHAR*)malloc(strText.GetLength()*2 + 1);
	memset(szBuf, 0, strText.GetLength()*2 + 1);
	CommonStrMethod::WChar2Char(szBuf, 
		strText.GetLength()*2 + 1, strText.GetBuffer());
	file.SeekToEnd();
	file.Write(szBuf, strlen(szBuf));
	file.Close();

	return TRUE;
}

void CShortMsgModem::CloseMsgComm()
{
		int nRc=fnCloseModem(m_lCom);
}