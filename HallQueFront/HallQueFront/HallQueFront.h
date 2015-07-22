// HallQueFront.h : HallQueFront 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "HallQueFrontDoc.h"
#include "HallQueFrontView.h"
#include "ComInit.h"
#include "CommDaoData.h"
#include <list>
using namespace std;



// CHallQueFrontApp:
// 有关此类的实现，请参阅 HallQueFront.cpp
//

class CHallQueFrontApp : public CWinApp
{
public:
	CHallQueFrontApp();
// 重写
public:
	virtual BOOL InitInstance();
// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	CHallQueFrontView* m_pView;
	CComInit* m_pComInit;//串口先行
//	SLZCEvaluator m_slzEvaluator;

	SLZController m_Controller;//数据处理集中
	LogicVariables m_logicVariables;//系统逻辑变量
	HANDLE m_hStartEvent;//用于评价线程和呼叫线程之间通信的事件

	list<CString> m_list_caCheMsg;//数据缓存cache
public:
	BOOL ReadLogicVariablesFromFile();//从文件读出系统逻辑变量
	BOOL IsLocal();//联机取号中是否为主机
private:
	BOOL AddAutoRun(CString strValueName);
public:
	list<CommDaoOrg> m_list_comOrg;
	list<CommDaoQue> m_list_comQue;
	list<CommDaoWnd> m_list_comWnd;
	list<CommDaoStaff> m_list_comStaff;
};

extern CHallQueFrontApp theApp;