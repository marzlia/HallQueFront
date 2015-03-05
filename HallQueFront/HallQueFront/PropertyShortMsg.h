#pragma once
#include "MsgSetDlg.h"
#include "PorpJudgeShortMsg.h"


// CPropertyShortMsg

class CPropertyShortMsg : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropertyShortMsg)

public:
	CPropertyShortMsg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropertyShortMsg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPropertyShortMsg();
public:
	CMsgSetDlg m_dMsgSetDlg;
	CPorpJudgeShortMsg m_dJudgeShortMsgDlg; 
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};


