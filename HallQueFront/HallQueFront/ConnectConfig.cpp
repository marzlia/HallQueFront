#include "StdAfx.h"
#include "ConnectConfig.h"

CConnectConfig::CConnectConfig(void)
{
	m_strServerInfoPath = m_convert.GetExeFullFilePath();
	m_strServerInfoPath +=_T("\\ConnectInfo");
	m_doFile.CreateMyDirectory(m_strServerInfoPath);
	m_strServerInfoPath+=_T("\\ConnectInfo.ini");

	GetRomoteIP();
	GetRomotePort();
}

CConnectConfig::~CConnectConfig(void)
{
}

UINT CConnectConfig::GetRomotePort()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("connect"),_T("PORT"),NULL,wbuf,255,m_strServerInfoPath);
	CString port(wbuf);
	int  iPort = 0;
	m_convert.CStringToint(iPort,port);
	m_romotePort = iPort;
	return m_romotePort;
}

CString CConnectConfig::GetRomoteIP()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("connect"),_T("IP"),NULL,wbuf,255,m_strServerInfoPath);
	CString IpAddress(wbuf);
	m_romoteIP = IpAddress;
	return m_romoteIP;
}

BOOL CConnectConfig::SaveInfo()
{
	CString c_port;
	m_convert.intToCString(m_romotePort,c_port);
	CString c_timeOut;
	c_timeOut.Format(_T("%d"),m_nTimeOut);
	return WritePrivateProfileString(_T("connect"),_T("PORT"),
		c_port,m_strServerInfoPath) && 
		WritePrivateProfileString(_T("connect"),_T("IP"),
		m_romoteIP,m_strServerInfoPath) && 
		WritePrivateProfileString(_T("connect"),_T("TIMEOUT"),c_timeOut,m_strServerInfoPath);
}

UINT CConnectConfig::GetTimeOut()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("connect"),_T("TIMEOUT"),NULL,wbuf,255,m_strServerInfoPath);
	CString timeOut(wbuf);
	int nTimeOut=0;
	m_convert.CStringToint(nTimeOut,timeOut);
	m_nTimeOut = nTimeOut;
	return m_nTimeOut;
}

void CConnectConfig::SetRomoteIP(const CString& ip)
{
	m_romoteIP = ip;
}

void CConnectConfig::SetRomotePort(const UINT port)
{
	m_romotePort = port;
}

void CConnectConfig::SetTimeOut(const UINT timeOut)
{
	m_nTimeOut = timeOut;
}