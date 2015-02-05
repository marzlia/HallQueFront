#pragma once
#include "QueSetDlg.h"
#include "StaffSet.h"
#include "PrintSetDlg.h"
#include "SLZWindowSetDlg.h"
#include "PropConnectInfo.h"

// CSetPropertySheet

class CSetPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSetPropertySheet)

public:
	CSetPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSetPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSetPropertySheet();
	
	CQueSetDlg m_queDlg;
	CStaffSetDlg m_StaffSetDlg;
	CPrintSetDlg m_PrintSetDlg;
	SLZWindowSetDlg m_WindowSetDlg;
	virtual BOOL OnInitDialog();
	HICON m_hIcon;
	CPropConnectInfo m_propConnect;
protected:
	DECLARE_MESSAGE_MAP()
};


