#pragma once
#ifndef _MY_MAX_IP_LENGTH
#define _MY_MAX_IP_LENGTH	64
#endif
#include "afxwin.h"
#include "WinSock2.h"


struct SocketPackage
{
	char PackageLen[8];   // 报文长度
	char Request[4];	   // 交易代码
	char Response[4];	   // 响应代码
	char Reserved[8];	   // 保留使用
	char XMLBuf[231];	// 包体
};

class CTCPConnect
{
public:
	CTCPConnect(void);
	virtual ~CTCPConnect(void);

	int GetCustLevel(CString StrCardNum,CString strIP,CString strPort,UINT iOverTime);  //输入卡号 对接的服务器IP和端口超时时间 得到客户等级
private:
	int GetLastError();
	void WriteErrLog(CString strErr);
	void WriteLogWithTime(CString strSockLog);			//写入错误日志
	BOOL AppendWriteFile(CString strText, CString strFileName);
	SocketPackage MakePackage(char c_CardNum[]);		//生成socket包
	int  GetLevel(char c_Request[]);
	BOOL StartSocket(CString strPort,CString strIP);
	BOOL CloseSocket();
	BOOL SendTo(CHAR * pData, UINT nLen);
	BOOL RecvFrom(CHAR * buf, int iLen);

	BOOL Startup();
	BOOL CreateSocket();
	BOOL _Connect();
	BOOL _CloseSocket();
	BOOL Cleanup();
	USHORT m_uPort;
	CString m_strIP;
	SOCKET m_tSocket;
	BOOL m_bIsCreate;
	CString m_strLogFilePath;
};
