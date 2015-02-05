#pragma once
#include "afxwin.h"
#include "atlimage.h"


// CShowWaitingDlg 对话框

class CShowWaitingDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowWaitingDlg)

public:
	CShowWaitingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowWaitingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WAITING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_sta_showWaitPic;
private:
	CImage m_picImage;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
