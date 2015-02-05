#pragma once
#include "HallQueFront.h"

// CMsgSetDlg 对话框

class CMsgSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CMsgSetDlg)

public:
	CMsgSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMsgSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MSGSET};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	CListBox m_list_Msg;
	CEdit m_edit_Msg;
	CString m_strMsgPath;
	CStringArray m_arr_Msg;
	BOOL WriteMsgIntoFile();
	BOOL ReadMsgFromFile();
public:
	afx_msg void OnBnClickedButtonAddmsg();
	afx_msg void OnBnClickedButtonDelmsg();
	afx_msg void OnLbnSelchangeListMsg();
	afx_msg void OnEnChangeEditMsg();
	afx_msg void OnBnClickedOk();
};
