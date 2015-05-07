#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ListCtrlCl.h"
#include "SLZWindow.h"
// #include <list>
// using namespace std;

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
public:
	CListCtrlCl m_listctr_wndscreen;
	CString m_cs_wndscrid;
	CString m_cs_phyid;
	CString m_cs_comid;
	CString m_cs_pipeid;
	CString m_cs_throughip;
	afx_msg void OnBnClickedBnAddwnd();
	afx_msg void OnBnClickedBnDelwnd();
	virtual BOOL OnInitDialog();
public:
	SLZWindow m_slzCurSelectWindow;
private:
	BOOL GetCurrentSelectWindow();
	void ShowContent();
	BOOL ModifyListWndInfo(SLZWindow& slzWindow);
	void ShowEditBoxInfo();
//	list<CThroughWndScreenInfo> m_list_wndScreenInfo;
public:
	afx_msg void OnLvnItemchangedListWndcontent(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int m_nCurrentSelectItem;
public:
	afx_msg void OnEnChangeEditWndscrid();
	afx_msg void OnEnChangeEditComscrid();
	afx_msg void OnEnChangeEditLedphyid();
	afx_msg void OnEnChangeEditLedipid();
	afx_msg void OnEnChangeEditLedpipe();
};
