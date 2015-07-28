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

////////////////处理呼叫命令类
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
	//取号时返回呼叫器等待人数变化
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
	CString GetQueInlineCount(const CString& queID);//获取排队队列剩余人数
	CString GetQueNumFromID(const CString& queID);//根据队列ID获取队列名称
	BOOL JudgeWindowHaveQue(const UINT iWindow);//判断窗口ID是否存在，同时判断窗口有没有可处理队列
	BOOL SetLoginStaffID(const UINT iWinID,const CString& staffID);//设置窗口登录员工ID
	BOOL DelWindowLoginStaffID(const UINT iWinID);//把窗口登录员工ID删除
	CString GetCandoQueInlineCount(UINT iWinID);//获取该窗口能够处理的队列排队人数
	BOOL ShowViewWaitNum(const CString& queserial_id,const SLZData& data,CallerCmd& callerCmd);//显示界面剩余人数
	
private:
	CInlineQueData& m_rInlineQueData;//排队队列
	CCalledQueData& m_rCalledQueData;//正在呼叫队列
	CCallerCmdData& m_rCallerCmdData;//呼叫命令表
	CFinshQueData& m_rFinshQueData;//完成队列,处理完成
	CDiscardQueData& m_rDiscardQueData;//过号队列
	CWaitQueData& m_rWaitQueData;//等侯队列
	CMap<int,int,CQueueInfo,CQueueInfo&>& m_map_que;//队列map
	std::map<UINT,CString>& m_map_login;//登录map
	BOOL TheStaffIDIsHave(const CString& staffID);
	SLZStaffQueryView& m_staffQueryView;//员工
	LogicVariables& m_logicVariables;//系统逻辑变量
	MSG m_msg;//消息队列
	CShortMsgModem* m_pShortMsg;
};
