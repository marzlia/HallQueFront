#pragma once
#include "afxwin.h"
#include "ColorStatic.h"


// CPropEdText 对话框

class CPropEdText : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropEdText)

public:
	CPropEdText();
	virtual ~CPropEdText();

// 对话框数据
	enum { IDD = IDD_PROPEDTEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaTextcolor();
	afx_msg void OnStnClickedStaBkcolor();
	CColorStatic m_sta_bkcolor;
	CColorStatic m_sta_textcolor;

private:
	COLORREF m_bkColor;
	COLORREF m_textColor;
	CFont    m_textFont;
public:
	CEdit m_ed_content;
	virtual BOOL OnInitDialog();
	void InitStaticTextInfo();
	void InitStaticTextFont();
	CEdit m_ed_textwidth;
	CEdit m_ed_textheight;
	afx_msg void OnBnClickedBnTextfont();
	
	CComboBox m_com_alpha;

	int FindMinDvalue(int num[],int per,int size);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	afx_msg void OnBnClickedRaTextleft();
	afx_msg void OnBnClickedRaTextcenter();
	afx_msg void OnBnClickedRaTextright();
};
