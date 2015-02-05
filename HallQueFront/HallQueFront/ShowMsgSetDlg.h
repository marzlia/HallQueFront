#pragma once
#include "ListBoxPlus.h"
#include "MyListBox.h"
#include "DataDef.h"
// CShowMsgSetDlg 对话框

class CShowMsgSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowMsgSetDlg)

public:
	//CShowMsgSetDlg(CString& strConf,CWnd* pParent = NULL);   // 标准构造函数
	CShowMsgSetDlg(pShowInfo showinfo,CWnd* pParent = NULL);
	virtual ~CShowMsgSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SHOWMSGSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editTextAdd;
	CMyListBox m_mylbList; 
	CComboBox m_combColor;
	CString& m_strConf;
	CString& m_strAdMsg;
	UINT& m_iShowTime;
	BOOL m_bMkShowAll;
	CButton m_check_mkshowall;
	CButton m_Sel_Chinese;
	CButton m_Sel_English;
	CString m_ed_strAdmsg;
	CString m_ed_strShowTime;
	afx_msg void OnBnClickedBtnAddtext();
	afx_msg void OnLbnSelchangeListShowmsg();
	afx_msg void OnBnClickedBtnChangemsg();
	afx_msg void OnBnClickedBtnDeletemsg();
	afx_msg void OnBnClickedBtnQuenum();
	afx_msg void OnBnClickedBtnStaffname();
	afx_msg void OnBnClickedBtnWndname();
	afx_msg void OnCbnSelchangeComboShowcolor();
	int SplitColorStringToArray(CString strSrc, CArray<CString>& arrStr);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnMoveup();
	afx_msg void OnBnClickedBtnMovedown();
	afx_msg void OnLbnDblclkListShowmsg();
	afx_msg void OnBnClickedCheckMkshall();
	afx_msg void OnBnClickedBtnBusname();
};
