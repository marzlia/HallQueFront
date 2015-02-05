#pragma once


// CSetTimeFormatDlg 对话框

class CSetTimeFormatDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetTimeFormatDlg)

public:
	CSetTimeFormatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetTimeFormatDlg();

// 对话框数据
	enum { IDD = IDD_TIMEFORMAT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_TimeFormat_control;
	CString m_TimeFormat_Text;
	int   m_cursel;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
