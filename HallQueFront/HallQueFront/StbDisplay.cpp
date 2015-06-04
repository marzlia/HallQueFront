#include "StdAfx.h"
#include "StbDisplay.h"
#include "MyString.h"
#include "MyCommon.h"
#include <list>
using namespace std;

StbDisplay::StbDisplay(void)
	: m_strModuleDir(_T(""))
	, m_strWebRootDir(_T(""))
	, m_strHtmlTitleBegin(_T(""))
	, m_strHtmlTitleEnd(_T(""))
	, m_strHtmlNoticeBegin(_T(""))
	, m_strHtmlNoticeEnd(_T(""))
	, m_strHtmlInstMsgBegin(_T(""))
	, m_strHtmlInstMsgEnd(_T(""))
	, m_strHtmlQueueInfoBegin(_T(""))
	, m_strHtmlQueueInfoEnd(_T(""))
{
	m_strModuleDir = MyCommon::GetModuleDir();

	m_strWebRootDir = m_strModuleDir + _T("webRoot\\");
}

StbDisplay::~StbDisplay()
{
//	::TerminateProcess(m_piNetServer.hProcess,0);
}

void StbDisplay::LoadDisplayConf()
{
	CString strStbKeshiPath = m_strModuleDir + _T("sys\\stb_keshi.ini");

	m_strHtmlTitleBegin = _T("<!DOCTYPE HTML><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">body{text-align: left; color: #0000ff; float:left; font:  bold 120px/120px Verdana, Geneva, sans-serif;}#center{ margin-left: auto; margin-right: auto; vertical-align: middle; }</style><title>医院标题</title></head><body><div style=\"textalign:left\"><img src=\"../img/hspt.jpg\" width=\"120\" height=\"120\" style=\"float:left\">");
	m_strHtmlTitleBegin = MyCommon::GetProfileString(
		_T("sys"),_T("TITLEBEGIN"), m_strHtmlTitleBegin, strStbKeshiPath);
	m_strHtmlTitleEnd = _T("</div></body></html>");
	m_strHtmlTitleEnd = MyCommon::GetProfileString(
		_T("sys"),_T("TITLEEND"), m_strHtmlTitleEnd, strStbKeshiPath);

	m_strHtmlNoticeBegin = _T("<!DOCTYPE HTML><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">body{text-align: center; color: #ff0000; font:  bold 80px/80px Verdana, Geneva, sans-serif;}#center{ margin-left: auto; margin-right: auto; vertical-align: middle; }</style><title>通知</title></head><body><div><marquee scrollamount=\"5\">");
	m_strHtmlNoticeBegin = MyCommon::GetProfileString(
		_T("sys"),_T("NOTICEBEGIN"), m_strHtmlNoticeBegin, strStbKeshiPath);
	m_strHtmlNoticeEnd = _T("</marquee></div></body></html>");
	m_strHtmlNoticeEnd = MyCommon::GetProfileString(
		_T("sys"),_T("NOTICEEND"), m_strHtmlNoticeEnd, strStbKeshiPath);

	m_strHtmlInstMsgBegin = _T("<!DOCTYPE HTML><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">body{text-align: center; color: #ff0000; font:bold 130px/130px Verdana, Geneva, sans-serif;}#center{ margin-left: auto; margin-right: auto; vertical-align: middle; }#customers{font-family:\"Trebuchet MS\", Arial, Helvetica, sans-serif;width:100%%;border-collapse:collapse;}</style><title>呼叫信息</title></head><body><div style=\"width: 1920px; margin-left: 0%%; margin-right: auto; margin-top: 0px;\">");
	m_strHtmlInstMsgBegin = MyCommon::GetProfileString(
		_T("sys"),_T("INSTMSGBEGIN"), m_strHtmlInstMsgBegin, strStbKeshiPath);
	m_strHtmlInstMsgEnd = _T("</div></body></html>");
	m_strHtmlInstMsgEnd = MyCommon::GetProfileString(
		_T("sys"),_T("INSTMSGEND"), m_strHtmlInstMsgEnd, strStbKeshiPath);

	m_strHtmlQueueInfoBegin = _T("<!DOCTYPE HTML><html><head><meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\" /><style type=\"text/css\">body{text-align: center; color: #000000; font:bold 80px/80px Verdana, Geneva, sans-serif;}#center{ margin-left: auto; margin-right: auto; vertical-align: middle; }#customers{font-family:\"Trebuchet MS\", Arial, Helvetica, sans-serif;width:100%;border-collapse:collapse;}#customers td, #customers th {font-size:1em;border:1px solid #98bf21;padding:3px 7px 2px 7px;}#customers th {font-size:1.1em;padding-top:5px;padding-bottom:4px;color:#008B00;}#customers tr.ord td {color:#000000;}#customers tr.alt td {color:#000000;}</style><title>排队信息</title></head><body><div style=\"width: 1920px; margin-left: 0%; margin-right: auto; margin-top: 0px;\"><div><table id=\"customers\"; width=\"1920px\"; border=\"1\" cellpadding=\"0\" cellspacing=\"0\">");
	m_strHtmlQueueInfoBegin = MyCommon::GetProfileString(
		_T("sys"),_T("QUEUEINFOBEGIN"), m_strHtmlQueueInfoBegin, strStbKeshiPath);
	m_strHtmlQueueInfoEnd = _T("</table></div></div></body></html>");
	m_strHtmlQueueInfoEnd = MyCommon::GetProfileString(
		_T("sys"),_T("QUEUEINFOEND"), m_strHtmlQueueInfoEnd, strStbKeshiPath);
}

BOOL StbDisplay::InitDisplay()
{
	LoadDisplayConf();

	MyCommon::CreatePath(m_strWebRootDir);
	MyCommon::CreatePath(m_strWebRootDir + _T("img"));
	MyCommon::CreateFile(m_strWebRootDir + _T("img/hspt.jpg"));

	//更新文件fdfile.xml
	CString strFilePath;
	strFilePath = m_strModuleDir + _T("fdfile_tp.xml");
	CString strFileStr = _T("");
	CStdioFile file;
	if(file.Open(strFilePath, CFile::modeRead|CFile::typeText))
	{
		CString strRead = _T("");
		for(; file.ReadString(strRead); strRead.Empty())
		{
			strFileStr += strRead + _T("\n");
		}
		file.Close();
	}
	else
	{
		AfxMessageBox(_T("fdfile_tp.xml文件缺失！"));
	}
	strFileStr = MyString::Replace(strFileStr, _T("%webRoot%"), m_strWebRootDir);

	
	CStringA a_strFileStr(strFileStr.GetBuffer(0));
	strFileStr.ReleaseBuffer(0);
	string s_strFileStr = a_strFileStr.GetBuffer(0);
	a_strFileStr.ReleaseBuffer(0);
	
	strFilePath = m_strModuleDir + _T("fdfile.xml");
	if(file.Open(strFilePath, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		file.Write(s_strFileStr.c_str(),s_strFileStr.size());
		file.Close();
	}

	return TRUE;
}

BOOL StbDisplay::InitNetServer()
{
	//获取系统版本信息，windows XP 及以下版本则启动CassiniDev.exe；否则不启动。
	OSVERSIONINFOEX osinfoex;
	osinfoex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(!GetVersionEx((LPOSVERSIONINFO)&osinfoex))
	{
		return FALSE;
	}

	if(osinfoex.dwMajorVersion > 5)//Windows VISTA 以上版本
	{
		return FALSE;
	}

	HWND hWndNetServer = ::FindWindow(NULL, _T("多媒体信息发布服务端"));
	if(!hWndNetServer)
	{
		STARTUPINFO si = {0};
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		memset(&m_piNetServer, 0, sizeof(PROCESS_INFORMATION));

		CString strCmdLine(_T("CassiniDev.exe"));

		if(!::CreateProcess(NULL, strCmdLine.GetBuffer(), NULL, NULL, FALSE, NULL, NULL, m_strModuleDir, &si, &m_piNetServer))
		{
			AfxMessageBox(_T("液晶显示服务端无法自动启动，请手动启动!"));
			return FALSE;
		}
		//else
		//{
		//	::CloseHandle(m_piNetServer.hProcess);
		//	::CloseHandle(m_piNetServer.hThread);
		//}
	}

	return TRUE;
}

BOOL StbDisplay::StbUpdateTitleHtml(CString strTitle, CString strStbId)
{
	CString str(m_strHtmlTitleBegin);
	str += strTitle;
	str += m_strHtmlTitleEnd;

	return WriteToStbHtmlFile(str, strStbId, _T("webinfo1.html"));
}

BOOL StbDisplay::StbUpdateNoticeHtml(CString strNotice, CString strStbId)
{
	CString str(m_strHtmlNoticeBegin);
	str += strNotice;
	str += m_strHtmlNoticeEnd;
	return WriteToStbHtmlFile(str, strStbId, _T("webinfo2.html"));
}

BOOL StbDisplay::StbUpdateCallMsg(CString strIMsg, CString strStbId)
{
	strIMsg = CovertColorString(strIMsg);
	CString str(m_strHtmlInstMsgBegin);
	str += strIMsg;
	str += m_strHtmlInstMsgEnd;

	return WriteToStbHtmlFile(str, strStbId, _T("webinfo.html"));
}






BOOL StbDisplay::WriteToStbHtmlFile(CString& strHtmlStr, CString strStbId, CString strHtmlFileName)
{
	if(strStbId.IsEmpty() || strStbId == _T("0")) return FALSE;
	BOOL bRet = TRUE;

	CString strPath = m_strWebRootDir + strStbId;
	if(MyCommon::CreatePath(strPath))
	{
		strPath = strPath + _T("/") + strHtmlFileName;
		bRet = WriteToFile(strHtmlStr, strPath);
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}


BOOL StbDisplay::WriteToFile(CString& strText, CString& strFilePath)
{
	CFile htmlFile;
	if(!htmlFile.Open(strFilePath, CFile::modeWrite | CFile::modeCreate))
	{
		return FALSE;
	}

	CT2CA htmlStr(strText, CP_UTF8);

	htmlFile.Write(htmlStr, ::strlen(htmlStr));

	htmlFile.Close();

	return TRUE;
}

CString StbDisplay::CovertColorString(CString strSrc)
{
	int iIndex = strSrc.Find('#');
	if(iIndex != -1)
	{
		int iType;
		if(strSrc.GetAt(iIndex +1) == '0' && strSrc.GetAt(iIndex +2) == '#') 
		{
			iType = 0;
		}
		else if(strSrc.GetAt(iIndex +1) == '1' && strSrc.GetAt(iIndex +2) == '#')
		{
			iType = 1;
		}
		else if(strSrc.GetAt(iIndex +1) == '2' && strSrc.GetAt(iIndex +2) == '#')
		{
			iType = 2;
		}
		else
		{
			iType = -1;
		}

		int iIndex2;
		if(iIndex < strSrc.GetLength() - 3)
		{
			iIndex2 = strSrc.Find('#', iIndex + 3);
			if(iIndex2 == -1)
			{
				iIndex2 = strSrc.GetLength() -1;
			}
		}
		else
		{
			iIndex2 = strSrc.GetLength() -1;
		}
		CString strLeft = strSrc.Left(iIndex);
		CString strMid = strSrc.Mid(iIndex + 3, iIndex2 - (iIndex + 3));
		CString strRight = strSrc.Mid(iIndex2);

		CString strColor;

		switch(iType)
		{
		case 0:
			{
				strColor = _T("<font color='Red'>");
			}
			break;
		case 1:
			{
				strColor = _T("<font color='Green'>");
			}
			break;
		case 2:
			{
				strColor = _T("<font color='Yellow'>");
			}
			break;
		default:
			{
				return strSrc;
			}
			break;
		}

		strSrc = strLeft + strColor + strMid + _T("</font>") + strRight;

		return CovertColorString(strSrc);
	}
	else
	{
		return strSrc;
	}
}

BOOL StbDisplay::Start()
{
	return InitDisplay() && InitNetServer();
}