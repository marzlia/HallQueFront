// HallQueFront.h : HallQueFront Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "HallQueFrontDoc.h"
#include "HallQueFrontView.h"
#include "ComInit.h"
#include "CommDaoData.h"
#include <list>
using namespace std;



// CHallQueFrontApp:
// �йش����ʵ�֣������ HallQueFront.cpp
//

class CHallQueFrontApp : public CWinApp
{
public:
	CHallQueFrontApp();
// ��д
public:
	virtual BOOL InitInstance();
// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	CHallQueFrontView* m_pView;
	CComInit* m_pComInit;//��������
//	SLZCEvaluator m_slzEvaluator;

	SLZController m_Controller;//���ݴ�����
	LogicVariables m_logicVariables;//ϵͳ�߼�����
	HANDLE m_hStartEvent;//���������̺߳ͺ����߳�֮��ͨ�ŵ��¼�

	list<CString> m_list_caCheMsg;//���ݻ���cache
public:
	BOOL ReadLogicVariablesFromFile();//���ļ�����ϵͳ�߼�����
	BOOL IsLocal();//����ȡ�����Ƿ�Ϊ����
private:
	BOOL AddAutoRun(CString strValueName);
public:
	list<CommDaoOrg> m_list_comOrg;
	list<CommDaoQue> m_list_comQue;
	list<CommDaoWnd> m_list_comWnd;
	list<CommDaoStaff> m_list_comStaff;
};

extern CHallQueFrontApp theApp;