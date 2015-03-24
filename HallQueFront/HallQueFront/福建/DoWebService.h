#pragma once
#include <string>
#include "Fuzhou.h"
using namespace std;
class SLZData;

class CDoWebService
{
public:
	CDoWebService(void);
	~CDoWebService(void);
	int GetCardLevelFromServer(const CString& host,const CString& cardNo,USHORT port,int waitTimeSec,CustLev* pCustLev);
	int SendDealBusMsg(const CString& host,const SLZData& data,USHORT port,int waitTimeSec,int leftNum,BOOL isEnd=FALSE);
private:
	////刷卡时生成对接字符串
	std::string ProduceSendCardMsg(const CString& cardNo,const CString& host,USHORT port);
	////分析刷卡返回字符串返回卡级别
	int AnaCustLevel(const std::string& recvMsg,CustLev* pCustLev);
	////办理业务时生成对接字符串
	std::string ProduceDealBusMsg(const SLZData& dealData,int leftNum,const CString& host,USHORT port,BOOL isEnd);
	////解析办理业务时返回报文
	int AnaBusErrcode(const std::string& recvMsg);
	BOOL InitClient();//初始化sokcet
	void StopClient();
	BOOL ConnectToServer(SOCKET& cliSock,const CString& host,USHORT port,int waitSec);
	void GetCustProduct(const string& recvMsg,CustLev* pCustLev);
private:
	USHORT m_uPort;//端口
	CString m_romoteIP;//IP
	UINT m_nTimeOut;//超时间(毫秒)
};
