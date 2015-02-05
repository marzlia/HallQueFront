#include "stdafx.h"
#include "JttsAPI.h"
#include "CommonStrMethod.h"

JttsAPI* JttsAPI::m_pThis = NULL;

JttsAPI::JttsAPI()
: m_szInitPath(NULL)
, m_szSerialNo(NULL)
, m_szVoiceInfo(NULL)
//, m_pevtKillThread(NULL)
{
	m_szInitPath = new char[_MAX_PATH];
	m_szSerialNo = new char[_MAX_PATH];
	m_szVoiceInfo = new char[_MAX_PATH];
}

JttsAPI::~JttsAPI()
{
	delete[] m_szInitPath;
	delete[] m_szSerialNo;
	delete[] m_szVoiceInfo;
	if(m_pThis)
	{
		m_pThis->EndTTS();
	}
}

JttsAPI* JttsAPI::GetInstance()
{
	if(!m_pThis)
	{
		m_pThis = new JttsAPI;
		m_pThis->InitTTS();
	}
	return m_pThis;
}

BOOL JttsAPI::InitTTS()
{
	WriteReg();

	ERRCODE err;
	err = jTTS_Init(NULL, NULL);
	if (err != ERR_NONE && err != ERR_ALREADYINIT)
	{
		AfxMessageBox(_T("jTTS语音初始化出错"));
		//ErrorMessage(err);
		return FALSE; // 初始化失败
	}
	// 预载当前音色
	/*
	JTTS_CONFIG config;
	jTTS_Get(&config);
	CString strTmp = GetProfileString(_T("Init"), _T("szLastGuid"), _T("93A47B28-FE41-4e99-9458-7A781484A467"));
	WChar2Char(config.szVoiceID, VOICEID_LEN, strTmp.GetBuffer());
	*/
	/*
	err = jTTS_PreLoad(config.szVoiceID);
	if (err != ERR_NONE)
	{
		AfxMessageBox(_T("jTTS_PreLoad error"));
		ErrorMessage(err);
		return FALSE; // 初始化失败
	}
	*/
	// 设置当前属性
	/*
	err = jTTS_Set(&config);
	if (err != ERR_NONE)
	{
		AfxMessageBox(_T("jTTS_Set error"));
		ErrorMessage(err);
		return FALSE; // 初始化失败
	}
	*/

	return TRUE;
}

BOOL JttsAPI::EndTTS()
{
	BOOL bRet = FALSE;
	if(m_pThis)
	{
		bRet = (ERR_NONE == jTTS_End());
		delete m_pThis;
		m_pThis = NULL;
	}
	return bRet;
}

BOOL JttsAPI::SetPlay(
			 UINT uDeviceID,
			 HWND hwnd,
			 JTTS_CALLBACKPROC lpfnCallback,
			 DWORD dwUserData
			 )
{
	return ERR_NONE == jTTS_SetPlay(0xFFFFFFFF, NULL, NULL, 0);
}

BOOL JttsAPI::Play(CString strVoice)
{
	if(!m_pThis)
	{
		GetInstance();
	}
	int nLength = strVoice.GetLength() * 2 + 1;
	char* buf = (char*)malloc(nLength);
	memset(buf, 0, nLength);
	CommonStrMethod::WChar2Char(buf, nLength, strVoice.GetBuffer());

	ERRCODE errcode = jTTS_Play(buf, PLAYMODE_SYNC);

	free(buf);

	return ERR_NONE == errcode;
}

/*
void JttsAPI::ErrorMessage(ERRCODE err)
{
	for (int i = 0; i < sizeof(aErrorMessage) / sizeof(aErrorMessage[0]); i ++)
	{
		if (err == aErrorMessage[i].err)
		{
			AfxMessageBox(aErrorMessage[i].szPrompt);
			return;
		}
	}

	CString str;
	str.Format(_T("Unknow error! ERRCODE is %d"), (int)err);
	AfxMessageBox(str);
}
*/
BOOL JttsAPI::WriteReg(void)
{
	CString strModulePath = CommonStrMethod::GetModuleDir();
	//CString strSerialNo = GetProfileString(_T("Config"), _T("SerialNo"), _T("4C3E77924B936234"));
	CString strJttsSerialNo(_T("4C3E77924B936234"));

	HKEY hKey;
	LONG ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		_T("Software\\SinoVoice\\jTTS4_Professional\\"), 0, KEY_WRITE, &hKey);
	if(ERROR_SUCCESS != ret)
	{

		LPCTSTR lpSubKey = _T("Software\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegCreateKey:HKEY_LOCAL_MACHINE\\Software\\"));
			return FALSE;
		}
		lpSubKey = _T("SinoVoice\\jTTS4_Professional\\");
		ret = ::RegCreateKey(hKey, lpSubKey, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegCreateKey:SinoVoice\\jTTS4_Professional\\"));
			return FALSE;
		}
		ret = ::RegOpenKeyEx(hKey, NULL, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegOpenKeyEx:HKEY_LOCAL_MACHINE\\Software\\SinoVoice\\jTTS4_Professional\\"));
			return FALSE;
		}
	}

	ret = ::RegSetValueEx(hKey, _T("LibPath4"), 0, REG_SZ,
		(const BYTE*)strModulePath.GetBuffer(), strModulePath.GetLength()*2+1);
	if(ERROR_SUCCESS != ret)
	{
		//AfxMessageBox(_T("Error:RegSetValue:Write LibPath4"));
		return FALSE;
	}
	ret = ::RegSetValueEx(hKey, _T("SerialNo"), 0, REG_SZ,
		(const BYTE*)strJttsSerialNo.GetBuffer(), strJttsSerialNo.GetLength()*2);
	if(ERROR_SUCCESS != ret)
	{
		//AfxMessageBox(_T("Error:RegSetValue:Write SerialNo"));
		return FALSE;
	}

	return TRUE;
}

/*
BOOL TTSAPI JTTSCallBackProc(WORD wParam, LONG lParam, DWORD dwUserData)
{
CString str;
CMainFrame * pFrame  = (CMainFrame *) AfxGetMainWnd();
CStatusBar * pBar = &pFrame->m_wndStatusBar;
switch(wParam)
{
case NOTIFY_BEGIN:
{
g_lPrevPos = 0;
str.Format("Notify - Begin - Total Size: %ld", lParam);
break;
}
case NOTIFY_END:
{
str.Format("Notify - End - Return Code: %ld", lParam);
break;
}
case NOTIFY_SENTEND:
{
g_lPrevPos = lParam;	// 因为一句的结束和下一句开始几乎同时，所以为显示清晰，先保留此值，等到发送下句开始时再统一显示
break;
}
case NOTIFY_SENTBEGIN:
{
str.Format("Notify - Reading From %ld To %ld", g_lPrevPos, lParam);
break;
}
case NOTIFY_MARK:
{
DWORD dwOffset = GETMARKOFFSET(lParam);
WORD wLen = GETMARKLEN(lParam);
str.Format("Notify - Mark Offset: %ld Len: %d", dwOffset, wLen);
break;
}
default:
str.Format("Notify - Unknow, lParam: %ld", lParam);
break;
}

SendMessage(pBar->m_hWnd, (UINT) SB_SETTEXT, 0, (LPARAM)(LPCTSTR)str);

return TRUE;
}
*/

