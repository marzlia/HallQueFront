#pragma once

#include "SLZEvaData.h"
#include "afxmt.h"
#include "HardWareDef.h"
#include "SLZWindowQueryView.h"


// SLZCEvaluator 命令目标

class SLZCEvaluator
{
	friend class CDoComInOut;
	friend class SLZCCaller;
private:
	SLZCEvaluator();
public:
	//////////////接口
	BOOL HasData();
	SLZEvaData GetData();
	~SLZCEvaluator();
	///////////////////////得到唯一实例
	static SLZCEvaluator* GetInstance();//工厂方法(用来获得实例) 
	//////////////////////////
	BOOL ReFlushWindowTable();//刷新窗口类
private:
	CList<SLZEvaData,SLZEvaData&> m_pEvaDataList; 
	CMutex m_evaLock;
private:
	static void CALLBACK MyDoOutTimerMsg(
		HWND hwnd, // handle of window for timer messages
		UINT uMsg, // WM_TIMER message
		UINT idEvent, // timer identifier
		DWORD dwTime // current system time	
		);///回调函数处理时间
	CList<EvaltOutTime,EvaltOutTime&> m_listEvaltOutTime;
	BOOL Start();
	//////////////添加数据
	void AddEvaData(SLZEvaData data);
private:
	void SystemSendToEva(char* evabuf,const char* buf);
	//处理评价数据
	void DoEvaltorMsg(char* buf);
	//判断评价超时并改值
	void IsOutTimeAndReser(char* buf);
	void StartEvaltor(char* buf);//开启评价超时
private:
	SLZWindowQueryView m_windowTable;//从文件读出的窗口信息类
};


