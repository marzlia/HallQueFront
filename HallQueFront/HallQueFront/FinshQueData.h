#pragma once
#include <afxmt.h>
#include <list>
#include "SLZData.h"
#include <string>
#include "JudyeShortMsg.h"
#include "ConfigCardDef.h"
using namespace std;

class CFinshQueData
{
public:
	CFinshQueData(void);
	~CFinshQueData(void);
	void Add(const SLZData& data);//增加数据
	BOOL GetFinshedData(const UINT winID,SLZData& data);//获取数据,并删除
	BOOL GetFinshedData();
private:
	std::list<SLZData> m_listFinshQue;//完成队列
	std::list<CJudgeShortMsg*> m_list_shortmsg;//发送评价消息队列
	CMutex m_mtFinshQue;//完成队列锁
	CString m_filePath;//读文件路径
private:
	static UINT DoFinshedData(LPVOID pParam);
	CWinThread* m_pDoFinshedDataThread;//处理完成数据线程
	BOOL Start();
	void ReleaseListShorMsg();
	BOOL ReadJudgeShortMsg();
	BOOL SendMsgToPhone(const SLZData& data);
public:
	BOOL ReFlushListShortMsg();//接口刷新出现差评发送短信的队列
private:
	CARDCONNECTINFO m_cardConnectInfo;//卡连接远程服务器判断信息
	BOOL ReadCardConnectInfo();//读取卡连接信息
public:
	BOOL ReFlushCardConnectInfo();//刷新卡连接信息
};
