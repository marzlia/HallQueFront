#include "StdAfx.h"
#include "WriteLogError.h"
#include "CommonConvert.h"
#include "DoFile.h"

CWriteLogError::CWriteLogError(void)
{
	CDoFile doFile;
	m_strLogFilePath = doFile.GetExeFullFilePath() + _T("\\log\\");
	doFile.CreateMyDirectory(m_strLogFilePath);
}

CWriteLogError::~CWriteLogError(void)
{
}

void CWriteLogError::WriteErrLog(CString strSockLog)
{
	CString str = _T("Error: ") + strSockLog;
	WriteLogWithTime(str);
}

void CWriteLogError::WriteLogWithTime(CString strSockLog)
{
	CDoFile doFile;
	CTime time = CTime::GetCurrentTime();
	CString str = time.Format(_T("%Y-%m-%d %H:%M:%S")) + _T("  \t") + strSockLog + _T("\r\n\r\n");
	CString strLogFile = time.Format(_T("log_%Y%m%d.log"));
// 	if(!doFile.IsFileExit(m_strLogFilePath))//CommonStrMethod::PathFileExist(m_strLogFilePath))
// 	{
// 		if(!doFile.CreateMyDirectory(m_strLogFilePath))
// 		{
// 			return;
// 		}
// 	}
	AppendWriteFile(str, m_strLogFilePath + strLogFile);
}

BOOL CWriteLogError::AppendWriteFile(CString strText, CString strFileName)
{
	CFile file;
	if(!file.Open(strFileName, 
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		return FALSE;
	}
	int len = CCommonConvert::CStringToChar(strText,NULL);
	char* szBuf = new char[len+1];
	memset(szBuf,0,0);
	CCommonConvert::CStringToChar(strText,szBuf);
	file.SeekToEnd();
	file.Write(szBuf,strlen(szBuf));
	delete [] szBuf;
	file.Close();
	return TRUE;
}