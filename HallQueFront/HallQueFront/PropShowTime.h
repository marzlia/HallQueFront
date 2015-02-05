#pragma once
#include "afxwin.h"
#include "TrackDef.h"
#include "ColorStatic.h"

#define ShowTime 9567
// CPropShowTime 对话框

class CPropShowTime : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropShowTime)

public:
	CPropShowTime();
	virtual ~CPropShowTime();

// 对话框数据
	enum { IDD = IDD_PROPSHOWTIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_com_timeStyle;
	CEdit m_ed_example;
	afx_msg void OnBnClickedBnFont();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComTimestyle();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	void InitFont();
private:
	CFont m_textFont;
	COLORREF m_textColor;
	StaticTimeFormat m_timeFormat;
public:
	CColorStatic m_sta_color;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaColor();
	virtual void OnOK();
};
