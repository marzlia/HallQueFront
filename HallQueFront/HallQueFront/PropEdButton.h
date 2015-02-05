#pragma once
#include "afxwin.h"
#include "ColorStatic.h"
#include "QueueInfo.h"
#include "CommonConvert.h"
#include <map>
using namespace std;
// CPropEdButton 对话框

class CPropEdButton : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropEdButton)

public:
	CPropEdButton();
	virtual ~CPropEdButton();

// 对话框数据
	enum { IDD = IDD_PROPEDBUTTON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_com_trans;
	
	afx_msg void OnStnClickedStaColor();
	CColorStatic m_sta_color;
	afx_msg void OnBnClickedBnTitlefont();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CEdit m_ed_width;
	CEdit m_ed_height;
//	CEdit m_ed_title;
	CEdit m_ed_title;

	COLORREF m_bkColor;
	COLORREF m_textColor;
	CFont m_hTextFont;
	//初始化字体
	void InitFont();
	//初始化按钮的info
	void InitButtonInfo();
	virtual BOOL OnApply();
	virtual void OnOK();
	///找到透明度按比例计算以后
	int FindMinDvalue(int num[],int per,int size);
private:
	CMap<int,int,CQueueInfo,CQueueInfo&> m_map_que;//链接的队列
	map<int,UINT> m_map_pageID;//链接界面ID
	void ReadQueInfoFromFile();
	CString m_infofile_path;//存放信息的文件地址
	CCommonConvert m_convert;
	CString m_BkPicPath;   //背景图片地址
	CStatic m_sta_bkpic;
	BOOL LoadPicImage(CString PicPath);
	BOOL DrawImage(CWnd* pWnd);
	CImage m_img; 
public:
	CComboBox m_com_queInfo;
	afx_msg void OnBnClickedBtnSetbk();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnClrbk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
