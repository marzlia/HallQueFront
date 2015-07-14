#pragma once
#include "SLZCallerData.h"
#include "SLZCardReader.h"
#include "SLZCCaller.h"
#include "SLZCEvaluator.h"
#include "SLZData.h"
#include "SLZEvaData.h"              
//#include "HallQueFrontDoc.h"
//#include "HallQueFrontView.h"
#include "SLZStaff.h"
#include "SLZWindow.h"
#include "QueueInfo.h"
#include "CommonConvert.h"
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
class SLZPrinter;

class SLZController//处理整个数组逻辑类           
{
public:
	SLZController(void);
	~SLZController(void);
	BOOL Start();//开始
	BOOL End();//结束
	CInlineQueData* m_pInlineQueData;//排队队列
private:
	SLZCCaller* m_pCaller;//呼叫器指针变量
	SLZCEvaluator* m_pEvaluator;//评价器指针变量
	///////////////////////////////////
	CCalledQueData* m_pCalledQueData;//呼叫队列
	CFinshQueData* m_pFinshQueData;//完成队列
	CDiscardQueData m_discardQueData;//过号队列
	CWaitQueData m_waitQueData;//等候队列
	CCallerCmdData m_CallerCmdData;//呼叫各种命令的类变量
	CEvaCmdData m_EvaCmdData;//评价指令的类变量
private:
	//获取硬件呼叫器指令数据线程
	static UINT GetHardCallerCmdProc(LPVOID pParam);
	CWinThread* m_pGetHardCallerCmd;
	static UINT GetHardEvaltorCmdProc(LPVOID pParam);
	CWinThread* m_pGetHardEvaCmd;
	//获取刷卡数据的线程
	static UINT TakingNumThreadProc(LPVOID pParam);
	CWinThread* m_pTakingNumThread;
	//获取从界面上取号的线程
	static UINT TakingViewThreadProc(LPVOID pParam);
	CWinThread* m_pTakingViewThread;
	//处理命令呼叫线程类对象
	CCallThread* m_pCallThread;
	//处理评价线程类对象
	CEvaThread* m_pEvaThread;
	SLZCardReader m_cardread; //读卡，读身份证变量
	SLZPrinter* m_pPrint;//打印变量
	////////////////////////////////////////
	BOOL ReadQueInfoFromFile();//读队列信息
	CString m_infofile_path; //队列基本信息的文件地址
	CCommonConvert m_convert;//转换
	CQueueInfo m_queinfo;
	BOOL VerifyCountLimit(const CString& QueId);
	SLZStaffQueryView m_staffTable;//从文件读出的员工信息类
	SLZWindowQueryView m_windowTable;//从文件读出的窗口信息类
	CMap<int,int,CQueueInfo,CQueueInfo&> m_map_que;//队列map
	BOOL DataNumOut(CString QueId);
//	void InitThroughScreen();//初始化通屏
	///维护队列人数从0变为1的工作者线程
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
	BOOL ReFlushStaffTable();//刷新员工接口
	BOOL ReFlushWindowTable();//刷新窗口接口
	BOOL ReFlushQueInfoTable();//刷新队列接口
	BOOL ReFlushPrintInfoTable();//刷新打印接口
	BOOL ReFlushSysLogicVarlibles();//刷新逻辑变量接口
	BOOL ReFlushSwingCardTable();//刷新刷卡的设置信息
	BOOL JudgeWorkTimeOut(const CString& queID);//判断工作时间是否超出。超出返回TRUE
	CString GetQueNameFromID(const CString& queID);//根据业务ID获取业务类型
	UINT GetQueBeginNum(const CString& queID);
	UINT GetQueEndNum(const CString& queID);
	BOOL WriteInlineDataToFile();//把排队数据写入文件
	BOOL ReadInlineDataFromFile();//读排队数据
	BOOL IsHaveLastData();//文件中的数据是否是今天的
	void InitShowInlineQueNum();//初始化时显示排队人数
	////////////////////////////维护系统的登录表
	std::map<UINT,CString> m_mapLoginList;//已登录列表，窗口ID--员工ID
	BOOL ClearSystemData();//清空系统数据
private:
	//处理打印机的异常
	void DoPrintStatus(EnumPrintStaus status,const SLZData& data,const UINT waitNum);
	CString m_InlineDataPath;//排队数据文件路径

	int GetMaxQueNum(const CString QueID);//获取最大的排队号码
	
	CString m_MapQuePath;
	BOOL ReadListQueFromFile();

	
	
	BOOL JudgeTodayOrNot(SLZData data);
public:
	SoundPlay* m_pPlaySound;//播放声音的对象
//private:
// 	BOOL WriteCacheMsg();
// 	CString m_caChePath;
// 	BOOL ReadCacheMsg();
	BOOL ReadCommDaoOrgInfo();//读数据库机构信息
	BOOL ReadCommDaoQueInfo();
	BOOL ReadCommDaoWndInfo();
	BOOL ReadCommDaoStaffInfo();
private:
	void AnaOrgPacket(const std::string strPacket);
	void AnaQuePacket(const std::string strPacket);
	void AnaStaffPacket(const std::string strPacket);
	void AnaWndPacket(const std::string strPacket);
	///判断父机构是否存在
	BOOL JudgeParOrgIsAt();
public:
	BOOL SendOrgData();//发送机构数据到服务端
	BOOL SendQueData();//发送队列数据到服务器
	BOOL SendWndData();//发送窗口数据到服务端
	BOOL SendStaffData();//发送员工数据到服务端
	BOOL SendDelQue();//发送删除队列数据
	BOOL SendDelWnd();//发送删除窗口
	BOOL SendDelStaff();//发送删除员工
	BOOL InitLoginMap();//初始化登录表
private:
	CMutex m_fileWriteInlineFile;
public:
	CString GetStaffNameByID(const CString& staffID);
	CString GetWindowNameByID(UINT nWindowID);
	CString GetWindowCallNameByID(UINT nWindowID);

private:
	BOOL ShortMsgNum(const CString& queserial_id);//短信取号操作
	void TakeViewNum(const CString& queserial_id);//取号操作
	unsigned int GetQueNum(const CString& queserial_id,UINT* pInlineNum,BOOL* pIsClientData,SLZData* pData,BOOL *pIsConnected);//设置取号时的排队号码

private:
	CInterNumSocketServer* m_pInterNumServer;
	BOOL InitInterNumServer();


	CUDPServer* m_pUDPServer;
	void InitUDPServer();

	
	CMutex m_mtModifyQueLock;
public:
	CMap<CString,LPCTSTR,UINT,UINT&> map_QueNum;//存储各个队列当前取号最大值
	BOOL GetQueSerialIDByManQueNum(CString& queserial_id,const CString& manQueNum);
	BOOL ModifyQueNum(const CString& queserial_id,UINT* pQueNum);//修改排队号码最大值

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

	CString GetCandoWndName(const SLZData& data);
};