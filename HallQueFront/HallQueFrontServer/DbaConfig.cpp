#include "StdAfx.h"
#include "DbaConfig.h"
#include "../HallQueFront/DoFile.h"
CDbaConfig::CDbaConfig(void)
{
	CDoFile dofile;
	m_strPath = dofile.GetExeFullFilePath();
	m_strPath+=_T("//DbaBase");
	dofile.CreateMyDirectory(m_strPath);
	m_strPath+=_T("//base.ini");
}

CDbaConfig::~CDbaConfig(void)
{
}

CString CDbaConfig::GetServerIP()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("dba"),_T("BASEIP"),NULL,wbuf,255,m_strPath);
	m_serverIP.Format(_T("%s"),wbuf);
	return m_serverIP;
}

CString CDbaConfig::GetServerAcount()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("dba"),_T("BASEACOUNT"),NULL,wbuf,255,m_strPath);
	m_serverAcount.Format(_T("%s"),wbuf);
	return m_serverAcount;
}

CString CDbaConfig::GetServerPassword()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("dba"),_T("BASEPASSWORD"),NULL,wbuf,255,m_strPath);
	m_serverPassword.Format(_T("%s"),wbuf);
	return m_serverPassword;
}

CString CDbaConfig::GetServerPort()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("dba"),_T("PORT"),NULL,wbuf,255,m_strPath);
	m_serverPort.Format(_T("%s"),wbuf);
	return m_serverPort;
}

CString CDbaConfig::GetDBPort()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("dba"),_T("DBPORT"),NULL,wbuf,255,m_strPath);
	m_DBPort.Format(_T("%s"),wbuf);
	return m_DBPort;
}

void CDbaConfig::Save()
{
	WritePrivateProfileString(_T("dba"),_T("BASEIP"),m_serverIP,m_strPath);
	WritePrivateProfileString(_T("dba"),_T("BASEACOUNT"),m_serverAcount,m_strPath);
	WritePrivateProfileString(_T("dba"),_T("BASEPASSWORD"),m_serverPassword,m_strPath);
	WritePrivateProfileString(_T("dba"),_T("PORT"),m_serverPort,m_strPath);
	WritePrivateProfileString(_T("dba"),_T("DBPORT"),m_DBPort,m_strPath);
}
