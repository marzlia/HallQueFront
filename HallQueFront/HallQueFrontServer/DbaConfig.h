#pragma once

class CDbaConfig
{
public:
	CDbaConfig(void);
	~CDbaConfig(void);
public:
	CString GetServerIP();
	void SetServerIP(const CString& ip){m_serverIP=ip;}
	CString GetServerAcount();
	void SetServerAcount(const CString& acount){m_serverAcount=acount;}
	CString GetServerPassword();
	void SetServerPassword(const CString& password){m_serverPassword=password;}
	
	void SetDBPort(const CString& DBport){m_DBPort = DBport;}
	CString GetDBPort();

	void SetServerPort(const CString& port){m_serverPort = port;}
	CString GetServerPort();
	void Save();
private:
	CString m_serverIP;
	CString m_serverAcount;
	CString m_serverPassword;
	CString m_serverPort;
	CString m_strPath;
	CString m_DBPort;
};
