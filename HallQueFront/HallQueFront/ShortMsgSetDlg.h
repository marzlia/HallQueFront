#pragma once
#include "HallQueFront.h"
#include "KeyBoardEdit.h"
#include "InlineQueData.h"
// CShortMsgSetDlg 对话框

class CShortMsgSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CShortMsgSetDlg)

public:
	CShortMsgSetDlg(CWnd* pParent,CInlineQueData& rInlineQueData);   // 标准构造函数
	virtual ~CShortMsgSetDlg();

	CString GetPhoneNum()const
	{
		return m_strPhoneNum;
	}

	CString GetQueNum() const
	{
		return m_strQueNum;
	}
// 对话框数据
	enum { IDD = IDD_DIALOG_SHORTMESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	CInlineQueData& m_rInlineQueData;//排队队列
	CKeyBoardEdit m_edit_CellPhone;
	CKeyBoardEdit m_edit_QueNum;
	CKeyBoardEdit m_edit_WaitNum;
	CComboBox m_com_Msg;
	CStatic m_sta_CellPhone;
	CStatic m_sta_QueNum;
	CStatic m_sta_WaitNum;
	CStatic m_sta_Msg;
	CString m_strMsgPath;
	CString m_strPhoneNum;
	CString m_strQueNum;
	CString m_strMsg;
	BOOL ReadMsgInfoFromFile();
	BOOL GetSendQueNum(CString& strQueNum,int iCount);

	int GetNumFromString(CString strNum);  //从字符串中提取数字 如“B0012”得到12
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnSetfocusEditCellphone();
	afx_msg void OnEnChangeEditCellphone();
	afx_msg void OnEnSetfocusEditQuenum();
	afx_msg void OnEnUpdateEditQuenum();
	afx_msg void OnEnSetfocusEditWaitnum();
	afx_msg void OnEnUpdateEditWaitnum();
};
