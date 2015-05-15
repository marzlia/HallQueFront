#pragma once

class CUDPBrodcast
{
public:
	CUDPBrodcast(void);
	~CUDPBrodcast(void);
public:
	BOOL BroadCast(const CString& msg);
	BOOL SendData(const CString& IP,const char* buf,int len);
private:
	BOOL Start();
//	SOCKET m_sockClient;
	USHORT m_sockPort;
};
