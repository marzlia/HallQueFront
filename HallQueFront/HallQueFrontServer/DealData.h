#pragma once
#include <string>
#include <list>
#include "afxmt.h"
#include "../HallQueFront/DataDef.h"
#include "ConnectToMySql.h"
using namespace std;
class CDealData
{
private:
	CDealData(void);
public:
	~CDealData(void);
	void AddPacket(const std::string packet);
	BOOL Start();
	static CDealData* GetInstance();
private:
	std::list<std::string> m_list_recvPacket;
	CMutex m_mtRecvLock;
	struct CommDao 
	{
		CString dataType;//数据类型
		CString organName;//机构名称
		CString organID;//机构id
		CString clientID;//排队号
		CString timeEnter;//取号时间
		CString queueID;//队列编号
		CString timeCall;//呼叫时间
		CString timeLeave;//完成时间
		CString workerID;//员工工号
		CString windowID;//窗口编号
		CString timeScore;//评价时间
		EvaLevel evalLev;//评价等级
		CString cardNum;//卡号
		CardType cardType;//卡类型
	};
	struct CommDaoQue
	{
		CString queID;//队列编号
		CString queName;//队列名称
		CString orgID;//机构ID
//		CString onlyID;//唯一标识
	};
	struct CommDaoWnd
	{
		CString wndID;//窗口编号
		CString wndName;//窗口名称
		CString orgID;//机构ID
//		CString onlyID;//唯一标识
	};
	struct CommDaoStaff
	{
		CString staffID;//员工ID
		CString staffName;//员工姓名
		CString orgID;//机构ID
//		CString onlyID;//唯一标识
	};
	struct CommDaoOrg
	{
		CString parOrgID;
		CString curOrgID;
		CString parOrgName;
		CString curOrgName;
//		CString onlyID;
	};
	struct CommDaoTime
	{
		CString curOrgID;
		CString staffID;
		CString windowID;
		CString startTime;
		CString endTime;
		CString overTime;
	};
	
	static UINT AnaPacket(LPVOID lpParam);
	CWinThread* m_pAnaPecket;
	void TranslatePacket(const std::string strPacket,CommDao& commDao);
	void TranslatePacket(const std::string strPacket,CommDaoQue& commDaoQue);
	void TranslatePacket(const std::string strPacket,CommDaoStaff& commDaoStaff);
	void TranslatePacket(const std::string strPacket,CommDaoWnd& commDaoWnd);
	void TranslatePacket(const std::string strPacket,CommDaoOrg& commDaoOrg);
	void TranslatePacket(const std::string strPacket,CommDaoTime& commDaoTime);
	BOOL WriteDataToDB(const CommDao& commDao);
	BOOL WriteDataToDB(const CommDaoQue& commDaoQue);
	BOOL WriteDataToDB(const CommDaoStaff& commDaoStaff);
	BOOL WriteDataToDB(const CommDaoWnd& commDaoWnd);
	BOOL WriteDataToDB(const CommDaoOrg& commDaoOrg);
	BOOL WriteDataToDB(const CommDaoTime& commDaoTime);
	void TranslateDelQue(const std::string strPacket,CommDaoQue& commDaoQue);
	void TranslateDelWnd(const std::string strPacket,CommDaoWnd& commDaoWnd);
	void TranslateDelStaff(const std::string strPacket,CommDaoStaff& commDaoStaff);
	BOOL WriteDelQue(const CommDaoQue& commDaoQue);
	BOOL WriteDelWnd(const CommDaoWnd& commDaoWnd);
	BOOL WriteDelStaff(const CommDaoStaff& commDaoStaff);
	CString m_strConn;
private:
	CConnectToMySql m_conMySql;
};
