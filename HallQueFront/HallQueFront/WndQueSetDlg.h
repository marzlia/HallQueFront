#pragma once
#include "CommonConvert.h"
#include "ListBoxPlus.h"
// CWndQueSetDlg 对话框
//可处理队列对话框
class CWndQueSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CWndQueSetDlg)

public:
	CWndQueSetDlg(CWnd* pParent );   // 标准构造函数
	virtual ~CWndQueSetDlg();

// 对话框数据
	enum { IDD = IDD_WNDQUEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list_undstque;
	CListBox m_list_ableque;
	CMap<int,int,CQueueInfo,CQueueInfo&> m_map_UndstQueInfo;
	CList<CQueueInfo,CQueueInfo&> m_list_UndstQueInfo;
	CList<CQueueInfo,CQueueInfo&> m_list_AbleQueInfo;
	CStringArray m_AbleQueId;			//可处理队列ID 数组
	CString m_AbleQueName;			//可处理队列名称
	BOOL m_bMakeAll;		//是否应用到所有窗口
private:
	BOOL ReadQueInfoFromFile();
	BOOL ReadWndQueInfo();
	CString m_Queinfo_path;
	CCommonConvert convert;
	void ArrayOrder(CArray<int,int>& iArray);		//数组排序
	CButton m_check_makeall;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAddque();
	afx_msg void OnBnClickedBtnDelque();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
	afx_msg void OnBnClickedCheckMkall();
};
