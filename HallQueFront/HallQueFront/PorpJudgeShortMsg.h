#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ListCtrlCl.h"
#include "JudyeShortMsg.h"
#include <list>
using namespace std;
// CPorpJudgeShortMsg 对话框

class CPorpJudgeShortMsg : public CPropertyPage
{
	DECLARE_DYNAMIC(CPorpJudgeShortMsg)

public:
	CPorpJudgeShortMsg();
	virtual ~CPorpJudgeShortMsg();

// 对话框数据
	enum { IDD = IDD_PROPMSG_JUDGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void PreInitDialog();
public:
	CButton m_check_openMsg;
	CEdit m_ed_phoneNum;
	CListCtrlCl m_listctr_content;
	afx_msg void OnBnClickedBnAddmsg();
	afx_msg void OnBnClickedBnDelmsg();
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
private:
	list<CJudgeShortMsg*> m_list_judgeMsg;
	CString m_shortMsgContent;
	int m_curSelectItem;
	CString m_curSelectShortMsg;
	CString m_filePath;
private:
	void ReleaseShortMsg();
	BOOL WriteShortMsgToFile();
	BOOL ReadShortMsgFromFile();
public:
	afx_msg void OnLvnItemchangedListContent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditPhonenum();
	CString m_ed_strPhoneNum;
	afx_msg void OnNMDblclkListContent(NMHDR *pNMHDR, LRESULT *pResult);
};
