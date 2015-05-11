#pragma once

class CUDPBrodcast
{
public:
	CUDPBrodcast(void);
	~CUDPBrodcast(void);
public:
	BOOL BroadCast(const CString& msg);
private:
	BOOL Start();
	SOCKET m_sockClient;
	USHORT m_sockPort;
};
