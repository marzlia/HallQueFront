#pragma once
#include "afxwin.h"
#include "atlimage.h"


// CWaringDlg 对话框

class CWaringDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaringDlg)

public:
	CWaringDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaringDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WARING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	CStatic m_sta_showPic;
private:
	CImage m_picImage;
protected:
	virtual void PostNcDestroy();
};
