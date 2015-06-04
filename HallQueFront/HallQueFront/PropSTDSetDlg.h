#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "ListCtrlCl.h"
#include "afxwin.h"
#include <list>
using namespace std;

// CPropSTDSetDlg 对话框
class CStbContent;

class CPropSTDSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CPropSTDSetDlg)

public:
	CPropSTDSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPropSTDSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_STD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlCl m_listctr_std;
// 	CEdit m_ed_stbnum;
// 	CEdit m_ed_stbname;
// 	CEdit m_ed_stbtitle;
// 	CEdit m_ed_stbnotice;
	CEdit m_ed_titlepic;
	afx_msg void OnBnClickedBnChoosepic();
	virtual BOOL OnInitDialog();
private:
	list<CStbContent*> m_list_stdContent;
	int m_nCurrentItem;
	CString m_strExePath;
public:
	afx_msg void OnBnClickedBnAddstd();
	afx_msg void OnBnClickedBnDelstd();
	afx_msg void OnLvnItemchangedListStd(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_cs_stdnum;
	CString m_cs_stdname;
	CString m_cs_stdtitle;
	CString m_cs_stbnotice;
	CString m_cs_titlepic;
private:
	void ShowEdSTBInfo();
	CStbContent* GetSelectStb();
	void ClearStbInfo();
	BOOL WriteStbInfoToFile();
	BOOL ReadStbInofFormFile();
	void ShowListCtrStbInfo();
public:
	afx_msg void OnEnChangeEditStbnum();
	afx_msg void OnEnChangeEditStbname();
	afx_msg void OnEnChangeEditStbtitle();
	afx_msg void OnEnChangeEditStbnotice();
	afx_msg void OnBnClickedOk();
	CString m_cs_linenum;
	afx_msg void OnEnChangeEditLinenum();
};
