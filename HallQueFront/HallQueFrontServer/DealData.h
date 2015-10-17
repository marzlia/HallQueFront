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
		CString dataType;//��������
		CString organName;//��������
		CString organID;//����id
		CString clientID;//�ŶӺ�
		CString timeEnter;//ȡ��ʱ��
		CString queueID;//���б��
		CString timeCall;//����ʱ��
		CString timeLeave;//���ʱ��
		CString workerID;//Ա������
		CString windowID;//���ڱ��
		CString timeScore;//����ʱ��
		EvaLevel evalLev;//���۵ȼ�
		CString cardNum;//����
		CardType cardType;//������
	};
	struct CommDaoQue
	{
		CString queID;//���б��
		CString queName;//��������
		CString orgID;//����ID
//		CString onlyID;//Ψһ��ʶ
	};
	struct CommDaoWnd
	{
		CString wndID;//���ڱ��
		CString wndName;//��������
		CString orgID;//����ID
//		CString onlyID;//Ψһ��ʶ
	};
	struct CommDaoStaff
	{
		CString staffID;//Ա��ID
		CString staffName;//Ա������
		CString orgID;//����ID
//		CString onlyID;//Ψһ��ʶ
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
		BOOL bState;//0�����뿪1������ͣ
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
