#pragma once
#include "resource.h"

// CWndContentDlg 对话框

class CWndContentDlg : public CDialog
{
	DECLARE_DYNAMIC(CWndContentDlg)

public:
	CWndContentDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWndContentDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WNDSCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
