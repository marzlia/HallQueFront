#include "StdAfx.h"
#include "ComInit.h"
#include "DoFile.h"
#include "SLZCardReader.h"

CComInit::CComInit(void) :
m_hComReadCard(INVALID_HANDLE_VALUE)
,m_hComWndScreen(INVALID_HANDLE_VALUE)
,m_hComMsg(INVALID_HANDLE_VALUE)
{
	InitCom();
	for(int i=0;i<10;i++)
	{
		int result = TryCom(i);
		m_canUse[i] = result;
	}

	ReadComInfo();
}

CComInit::~CComInit(void)
{
	CloseHandle(m_read_os.hEvent);
	CloseHandle(m_write_os.hEvent);
	CloseHandle(m_hComWndScreen);
	CloseHandle(m_hComReadCard);
	CloseHandle(m_hComMsg);
}
/*
尝试打开串口，保存在数组中
*/
int CComInit::TryCom(int nCom)
{
	if(nCom<0) return -1;
	if(m_hComWndScreen != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComWndScreen);
		m_hComWndScreen=INVALID_HANDLE_VALUE;
	}
	CString strWndCom;
	strWndCom.Format(_T("COM%d"),nCom);
	HANDLE hCom=CreateFile(strWndCom,GENERIC_READ | GENERIC_WRITE,0,
		NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);
	if(INVALID_HANDLE_VALUE != hCom)
	{
		CloseHandle(hCom);
		hCom = INVALID_HANDLE_VALUE;
		return nCom;
	}
	else
	{
		return -1;
	}
}
/*
初始化
*/
void CComInit::InitCom()
{
	CDoFile dofile;
	m_strPath = dofile.GetExeFullFilePath();
	m_strPath += _T("\\cominfo.ini");
}
/*
打开串口
*/
HANDLE CComInit::OpenComm(int ncom)
{
	memset( &m_read_os, 0, sizeof( OVERLAPPED ) ) ;    
	memset( &m_write_os, 0, sizeof( OVERLAPPED ) ) ;
	
	CString Com;
	Com.Format(_T("COM%d"),ncom);
	HANDLE hCom = CreateFile(Com,GENERIC_READ | GENERIC_WRITE,
		0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);

	if(INVALID_HANDLE_VALUE==hCom)
	{
		CloseHandle(hCom);
		return hCom;
	}
	//创建事件
	m_read_os.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_write_os.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(NULL == m_read_os.hEvent || NULL == m_write_os.hEvent){
		CloseHandle(hCom);
		CloseHandle(m_read_os.hEvent);
		CloseHandle(m_write_os.hEvent);
		hCom = INVALID_HANDLE_VALUE;
		return hCom;
	}
	//设置com事件类型，参见msdn
	SetCommMask(hCom,EV_RXCHAR | EV_TXEMPTY);
	SetupComm( hCom, 1024,512 );//设置缓冲区大小;
	PurgeComm( hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR
		| PURGE_RXCLEAR ); //清干净输入、输出缓冲区

	DCB dcb={0};
	dcb.DCBlength=sizeof(dcb);
	BOOL bres=GetCommState(hCom,&dcb);
	if(!bres)
	{
		CloseHandle(hCom);
		hCom=INVALID_HANDLE_VALUE;
		return hCom;
	}
	dcb.ByteSize=8;//后改
	dcb.fParity=0;
	dcb.BaudRate=CBR_9600;
	dcb.StopBits=ONESTOPBIT;

	bres=SetCommState(hCom,&dcb);
	if(!bres)
	{
		CloseHandle(hCom);
		hCom=INVALID_HANDLE_VALUE;
		return hCom;
	}
	COMMTIMEOUTS comm_time={0};
	comm_time.ReadIntervalTimeout=MAXDWORD;
	bres=SetCommTimeouts(hCom,&comm_time);
//	SetupComm(m_hComWndScreen, 4096, 1024);

	if(!bres)
	{
		CloseHandle(hCom);
		CloseHandle(m_read_os.hEvent);
		CloseHandle(m_write_os.hEvent);
		hCom = INVALID_HANDLE_VALUE;
		return hCom;
	}
	return hCom;
}
/*
打开呼叫器串口
*/
int CComInit::OpenWndScreen(int ncom)
{
	m_cWndComm.Format(_T("%d"),ncom);
	if(m_hComWndScreen != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComWndScreen);
		m_hComWndScreen=INVALID_HANDLE_VALUE;
	}
	if(ncom== 0) return 0;
	m_hComWndScreen = OpenComm(ncom);
	if(m_hComWndScreen != INVALID_HANDLE_VALUE)
	{
		return 1;
	}
	else
	{
		m_cWndComm = L"0";
		return -1;
	}
}
/*
打开刷卡器串口
*/
int CComInit::OpenCardComm(int ncom)
{
	m_cCardComm.Format(_T("%d"),ncom);
	if(m_hComReadCard!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComReadCard);
		m_hComReadCard = INVALID_HANDLE_VALUE;
	}
	if(ncom==0)return 0;
	m_hComReadCard = OpenComm(ncom);
	if(m_hComReadCard != INVALID_HANDLE_VALUE)
	{
		return 1;
	}
	else
	{
		m_cCardComm = L"0";
		return -1;
	}
}



/*
保存上次打开串口如:com1到文件
*/
void CComInit::SaveComm()
{
	WritePrivateProfileString(_T("com"),_T("CARDCOM"),m_cCardComm,m_strPath);
	WritePrivateProfileString(_T("com"),_T("WNDCOM"),m_cWndComm,m_strPath);
	WritePrivateProfileString(_T("com"),_T("MSGCOM"),m_cMsgComm,m_strPath);
//	WritePrivateProfileString(_T("com"),_T("NEWCARDCOM"),m_cNewCardComm,m_strPath);
}
/*
读取刷卡器COM口
*/
CString CComInit::GetCardComm()
{
// 	wchar_t wbuf[255];
// 	ZeroMemory(wbuf,255);
// 	GetPrivateProfileString(_T("com"),_T("CARDCOM"),NULL,wbuf,255,m_strPath);
// 	m_cCardComm.Format(_T("%s"),wbuf);
	return m_cCardComm;
}
/*
获取呼叫器和屏串口
*/
CString CComInit::GetWndComm()
{
// 	wchar_t wbuf[255];
// 	ZeroMemory(wbuf,255);
// 	GetPrivateProfileString(_T("com"),_T("WNDCOM"),NULL,wbuf,255,m_strPath);
// 	m_cWndComm.Format(_T("%s"),wbuf);
	return m_cWndComm; 
}

CString CComInit::GetMsgComm()
{
// 	wchar_t wbuf[255];
// 	ZeroMemory(wbuf,255);
// 	GetPrivateProfileString(_T("com"),_T("MSGCOM"),NULL,wbuf,255,m_strPath);
// 	m_cMsgComm.Format(_T("%s"),wbuf);
	return m_cMsgComm; 
}

void CComInit::ReadComInfo()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("com"),_T("CARDCOM"),NULL,wbuf,255,m_strPath);
	m_cCardComm.Format(_T("%s"),wbuf);

	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("com"),_T("WNDCOM"),NULL,wbuf,255,m_strPath);
	m_cWndComm.Format(_T("%s"),wbuf);


// 	ZeroMemory(wbuf,255);
// 	GetPrivateProfileString(_T("com"),_T("NEWCARDCOM"),NULL,wbuf,255,m_strPath);
// 	m_cNewCardComm.Format(_T("%s"),wbuf);

	
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("com"),_T("MSGCOM"),NULL,wbuf,255,m_strPath);
	m_cMsgComm.Format(_T("%s"),wbuf);
}

// CString CComInit::GetNewCardComm()
// {
// 	return m_cNewCardComm;
// }

// void CComInit::SetNewCardComm(const CString& strNewCardComm)
// {
// 	m_cNewCardComm = strNewCardComm;
// }

// int CComInit::OpenNewCardComm(CString ICCardComm)
// {
// 	SLZCardReader* pCardReader = SLZCardReader::GetInstance();
// 	return pCardReader->OpenICCard(ICCardComm);
// }

// int CComInit::CloseNewCardComm()
//{
//	SLZCardReader* pCardReader = SLZCardReader::GetInstance();
//	return pCardReader->CloseICCard();
//}