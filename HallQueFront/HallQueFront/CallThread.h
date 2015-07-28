#pragma once
#include "mythread.h"
#include "CallerCmd.h"
#include "InlineQueData.h"
#include "CalledQueData.h"
#include "CallerCmdData.h"
#include "FinshQueData.h"
#include <map>
#include "SLZStaff.h"
#include "SLZStaffQueryView.h"
#include "DiscardQueData.h"
#include "WaitQueData.h"
#include "QueueInfo.h"
#include "ShortMsgModem.h"

////////////////�������������
using namespace std;
class CCallThread : public CMyThread
{
public:
	CCallThread(CInlineQueData& rInlineQueData,
						CCalledQueData& rCalledQueData,
						CCallerCmdData& rCallerCmdData,
						CFinshQueData& rFinshQueData,
						CDiscardQueData& rDiscardQueData,
						CWaitQueData& rWaitQueData,
						LogicVariables& logicVariables,
						std::map<UINT,CString>& login,
						CMap<int,int,CQueueInfo,CQueueInfo&>& queInfo,
						SLZStaffQueryView& staffQueryView);
	virtual ~CCallThread(void);
	BOOL OpenMsgQueue();
	//ȡ��ʱ���غ������ȴ������仯
	BOOL ShowCallerWaitNum(const CString& queID);
	BOOL ShowCallerWaitNum(const CString& queID,int nWaitNum);
private:
	virtual void Run();
	void DispatchCallerCmd(CallerCmd& callerCmd);

	void OnLogin(CallerCmd& callerCmd);
	void OnQuit(CallerCmd& callerCmd);
	void OnCall(CallerCmd& callerCmd);
	void OnRecall(CallerCmd& callerCmd);
	void OnDiscard(CallerCmd& callerCmd);
	void OnWait(CallerCmd& callerCmd);
	void OnEvaReq(CallerCmd& callerCmd);
	void OnPause(CallerCmd& callerCmd);
	void OnResume(CallerCmd& callerCmd);
	void OnCallNum(CallerCmd& callerCmd);
	void OnCallSec(CallerCmd& callerCmd);
	void OnCallMana(CallerCmd& callerCmd);
	void OnCallBusc(CallerCmd& callerCmd);
	void OnExChange(CallerCmd& callerCmd);
	
	void DoEvaMsg(const MSG& msg);
	void ReturnToCaller(CallerCmd& callerCmd);
	CString GetQueInlineCount(const CString& queID);//��ȡ�ŶӶ���ʣ������
	CString GetQueNumFromID(const CString& queID);//���ݶ���ID��ȡ��������
	BOOL JudgeWindowHaveQue(const UINT iWindow);//�жϴ���ID�Ƿ���ڣ�ͬʱ�жϴ�����û�пɴ������
	BOOL SetLoginStaffID(const UINT iWinID,const CString& staffID);//���ô��ڵ�¼Ա��ID
	BOOL DelWindowLoginStaffID(const UINT iWinID);//�Ѵ��ڵ�¼Ա��IDɾ��
	CString GetCandoQueInlineCount(UINT iWinID);//��ȡ�ô����ܹ�����Ķ����Ŷ�����
	BOOL ShowViewWaitNum(const CString& queserial_id,const SLZData& data,CallerCmd& callerCmd);//��ʾ����ʣ������
	
private:
	CInlineQueData& m_rInlineQueData;//�ŶӶ���
	CCalledQueData& m_rCalledQueData;//���ں��ж���
	CCallerCmdData& m_rCallerCmdData;//���������
	CFinshQueData& m_rFinshQueData;//��ɶ���,�������
	CDiscardQueData& m_rDiscardQueData;//���Ŷ���
	CWaitQueData& m_rWaitQueData;//�Ⱥ����
	CMap<int,int,CQueueInfo,CQueueInfo&>& m_map_que;//����map
	std::map<UINT,CString>& m_map_login;//��¼map
	BOOL TheStaffIDIsHave(const CString& staffID);
	SLZStaffQueryView& m_staffQueryView;//Ա��
	LogicVariables& m_logicVariables;//ϵͳ�߼�����
	MSG m_msg;//��Ϣ����
	CShortMsgModem* m_pShortMsg;
};
