#pragma once
#include "CommonConvert.h"
#include "DoFile.h"
/*
保存链接的参数
*/
class CConnectConfig
{
public:
	CConnectConfig(void);
	~CConnectConfig(void);
	UINT GetRomotePort();
	CString GetRomoteIP();
	void SetRomotePort(const UINT port);
	void SetRomoteIP(const CString& ip);
	UINT GetTimeOut();
	void SetTimeOut(const UINT timeOut);
	BOOL SaveInfo();
private:
	CDoFile m_doFile;
	CCommonConvert m_convert;
	CString m_strServerInfoPath;
	CString m_romoteIP;
	UINT     m_romotePort;
	UINT m_nTimeOut;
};
