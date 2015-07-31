#pragma once
#include "SLZCallerData.h"

#include "SLZCCaller.h"
#include "SLZCEvaluator.h"
#include "SLZData.h"
#include "SLZEvaData.h"              
#include "SLZPrinter.h"
//#include "HallQueFrontDoc.h"
//#include "HallQueFrontView.h"
#include "SLZStaff.h"
#include "SLZWindow.h"
#include "QueueInfo.h"
#include "CommonConvert.h"
#include "SLZPrinter.h"
#include "QueSetDlg.h"
#include "SLZStaffQueryView.h"
#include "SLZWindowQueryView.h"
#include "CallerCmd.h"
#include "CallThread.h"
#include "EvaCmdData.h"
#include "EvaThread.h"
#include <map>
#include "PlaySound.h"
#include "SocketBufDef.h"
#define WM_SHOWMSG WM_USER+3096
#define WM_SHOWPAGE WM_USER+3097
using namespace std;

class CInterNumSocketServer; 
class CUDPServer;
class SLZCardReader;

class SLZController//�������������߼���           
{
public:
	SLZController(void);
	~SLZController(void);
	BOOL Start();//��ʼ
	BOOL End();//����
	CInlineQueData* m_pInlineQueData;//�ŶӶ���
private:
	SLZCCaller* m_pCaller;//������ָ�����
	SLZCEvaluator* m_pEvaluator;//������ָ�����
	///////////////////////////////////
	CCalledQueData* m_pCalledQueData;//���ж���
	CFinshQueData* m_pFinshQueData;//��ɶ���
	CDiscardQueData m_discardQueData;//���Ŷ���
	CWaitQueData m_waitQueData;//�Ⱥ����
	CCallerCmdData m_CallerCmdData;//���и�������������
	CEvaCmdData m_EvaCmdData;//����ָ��������
private:
	//��ȡӲ��������ָ�������߳�
	static UINT GetHardCallerCmdProc(LPVOID pParam);
	CWinThread* m_pGetHardCallerCmd;
	static UINT GetHardEvaltorCmdProc(LPVOID pParam);
	CWinThread* m_pGetHardEvaCmd;
	//��ȡˢ�����ݵ��߳�
	static UINT TakingNumThreadProc(LPVOID pParam);
	CWinThread* m_pTakingNumThread;
	//��ȡ�ӽ�����ȡ�ŵ��߳�
	static UINT TakingViewThreadProc(LPVOID pParam);
	CWinThread* m_pTakingViewThread;
	//������������߳������
	CCallThread* m_pCallThread;
	//���������߳������
	CEvaThread* m_pEvaThread;
	SLZCardReader* m_pCardread; //�����������֤����
	SLZPrinter m_print;//��ӡ����
	////////////////////////////////////////
	BOOL ReadQueInfoFromFile();//��������Ϣ
	CString m_infofile_path; //���л�����Ϣ���ļ���ַ
	CCommonConvert m_convert;//ת��
	CQueueInfo m_queinfo;
	BOOL VerifyCountLimit(const CString& QueId);
	SLZStaffQueryView m_staffTable;//���ļ�������Ա����Ϣ��
	SLZWindowQueryView m_windowTable;//���ļ������Ĵ�����Ϣ��
	CMap<int,int,CQueueInfo,CQueueInfo&> m_map_que;//����map
	BOOL DataNumOut(CString QueId);
//	void InitThroughScreen();//��ʼ��ͨ��
	///ά������������0��Ϊ1�Ĺ������߳�
	static UINT CountToCallerAlarm(LPVOID pParam);
	CWinThread* m_pAlarmToCaller;
	CMutex m_mtAlarm;
	CMutex m_mtWriteFile;
	//////////
	struct alarmStatus
	{
		BOOL isZero;
		BOOL isMaxZero;
	};
	std::map<CString,alarmStatus> m_map_alarmStatus;
public:
	BOOL ReFlushStaffTable();//ˢ��Ա���ӿ�
	BOOL ReFlushWindowTable();//ˢ�´��ڽӿ�
	BOOL ReFlushQueInfoTable();//ˢ�¶��нӿ�
	BOOL ReFlushPrintInfoTable();//ˢ�´�ӡ�ӿ�
	BOOL ReFlushSysLogicVarlibles();//ˢ���߼������ӿ�
	BOOL ReFlushSwingCardTable();//ˢ��ˢ����������Ϣ
	BOOL JudgeWorkTimeOut(const CString& queID);//�жϹ���ʱ���Ƿ񳬳�����������TRUE
	CString GetQueNameFromID(const CString& queID);//����ҵ��ID��ȡҵ������
	UINT GetQueBeginNum(const CString& queID);
	UINT GetQueEndNum(const CString& queID);
	BOOL WriteInlineDataToFile();//���Ŷ�����д���ļ�
	BOOL ReadInlineDataFromFile();//���Ŷ�����
	BOOL IsHaveLastData();//�ļ��е������Ƿ��ǽ����
	void InitShowInlineQueNum();//��ʼ��ʱ��ʾ�Ŷ�����
	////////////////////////////ά��ϵͳ�ĵ�¼��
	std::map<UINT,CString> m_mapLoginList;//�ѵ�¼�б�����ID--Ա��ID
	BOOL ClearSystemData();//���ϵͳ����
private:
	//�����ӡ�����쳣
	void DoPrintStatus(EnumPrintStaus status,const SLZData& data,const UINT waitNum);
	CString m_InlineDataPath;//�Ŷ������ļ�·��

	int GetMaxQueNum(const CString QueID);//��ȡ�����ŶӺ���
	
	CString m_MapQuePath;
	BOOL ReadListQueFromFile();

	
	
	BOOL JudgeTodayOrNot(SLZData data);
public:
	SoundPlay* m_pPlaySound;//���������Ķ���
//private:
// 	BOOL WriteCacheMsg();
// 	CString m_caChePath;
// 	BOOL ReadCacheMsg();
	BOOL ReadCommDaoOrgInfo();//�����ݿ������Ϣ
	BOOL ReadCommDaoQueInfo();
	BOOL ReadCommDaoWndInfo();
	BOOL ReadCommDaoStaffInfo();
private:
	void AnaOrgPacket(const std::string strPacket);
	void AnaQuePacket(const std::string strPacket);
	void AnaStaffPacket(const std::string strPacket);
	void AnaWndPacket(const std::string strPacket);
	///�жϸ������Ƿ����
	BOOL JudgeParOrgIsAt();
public:
	BOOL SendOrgData();//���ͻ������ݵ������
	BOOL SendQueData();//���Ͷ������ݵ�������
	BOOL SendWndData();//���ʹ������ݵ������
	BOOL SendStaffData();//����Ա�����ݵ������
	BOOL SendDelQue();//����ɾ����������
	BOOL SendDelWnd();//����ɾ������
	BOOL SendDelStaff();//����ɾ��Ա��
	BOOL InitLoginMap();//��ʼ����¼��
private:
	CMutex m_fileWriteInlineFile;
public:
	CString GetStaffNameByID(const CString& staffID);
	CString GetWindowNameByID(UINT nWindowID);
	CString GetWindowCallNameByID(UINT nWindowID);

private:
	BOOL ShortMsgNum(const CString& queserial_id);//����ȡ�Ų���
	void TakeViewNum(const CString& queserial_id);//ȡ�Ų���
	unsigned int GetQueNum(const CString& queserial_id,UINT* pInlineNum,BOOL* pIsClientData,SLZData* pData);//����ȡ��ʱ���ŶӺ���

private:
	CInterNumSocketServer* m_pInterNumServer;
	BOOL InitInterNumServer();


	CUDPServer* m_pUDPServer;
	void InitUDPServer();

	
	CMutex m_mtModifyQueLock;
public:
	CMap<CString,LPCTSTR,UINT,UINT&> map_QueNum;//�洢�������е�ǰȡ�����ֵ
	BOOL GetQueSerialIDByManQueNum(CString& queserial_id,const CString& manQueNum);
	BOOL ModifyQueNum(const CString& queserial_id,UINT* pQueNum);//�޸��ŶӺ������ֵ

	BOOL GetManQueNumByQueSerialID(const CString& queserial_id,CString& manQueNum);
	BOOL GetQueueInfoBySerialID(const CString& queserial_id,CQueueInfo& queInfo);
private:
	void DoPrint(const SLZData& data,UINT inLineNum);
	void ReturnMainFrame(const SLZData& data);
	void TakeNumSetData(SLZData& data,int nQueNum);
public:
	CList<SLZData,SLZData&> m_list_Data;
	BOOL InsertListData(SLZData data);
	BOOL WriteListQueIntoFile();
};