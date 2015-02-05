#pragma once
#include "afxwin.h"
#include "ColorStatic.h"
#include "QueueInfo.h"
#include "CommonConvert.h"

// CPropShowQueNum 对话框

class CPropShowQueNum : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropShowQueNum)

public:
	CPropShowQueNum();
	virtual ~CPropShowQueNum();

// 对话框数据
	enum { IDD = IDD_PROPSHOWQUENUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CColorStatic m_sta_textColor;
	CEdit m_ed_content;
	afx_msg void OnStnClickedStaColor();
	void InitInfo();
private:
	COLORREF m_textColor;
	CFont m_textFont;
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBnFont();
	CComboBox m_com_queInfo;
private:
	CMap<int,int,CQueueInfo,CQueueInfo&> m_map_que;
	void ReadQueInfoFromFile();
	CString m_infofile_path;//存放信息的文件地址
	CCommonConvert m_convert;
};
