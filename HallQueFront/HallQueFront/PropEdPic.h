#pragma once
#include "afxwin.h"
#include <atlimage.h>


// CPropEdPic 对话框

class CPropEdPic : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropEdPic)

public:
	CPropEdPic();
	virtual ~CPropEdPic();

// 对话框数据
	enum { IDD = IDD_PROPEDPIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_com_alpha;
	CEdit m_ed_path;
	afx_msg void OnBnClickedBnChoosepic();
	CEdit m_ed_picheight;
	CEdit m_ed_picwidth;
	virtual BOOL OnInitDialog();
	void InitStaticPicInfo();
	CString m_picType;
	virtual void OnOK();
	int FindMinDvalue(int num[],int per,int size);

	CImage m_picImage;
	BOOL LoadPicImage(const CString& path);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CStatic m_sta_pic;
};
