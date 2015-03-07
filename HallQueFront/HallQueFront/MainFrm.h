// MainFrm.h : CMainFrame 类的接口
//
#pragma once
//#include "HardWareCaller.h"
#include "DataDef.h"
#define  WM_MY_TRAYICON (WM_USER+2049)

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//全屏
	void FullScreen();
// 实现
public:
	virtual ~CMainFrame();
	//退出全屏
	void EndFullScreen();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;

private:
//	WINDOWPLACEMENT m_oldWindowPlaceMent;
	//用于处理全屏的BOOL变量
	BOOL m_bFullScreen;
	CRect m_FullScreenRect;
	CRect m_windowRect;
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	//工具栏的ico imagelist
	CImageList m_imageList;
	CToolBar    m_wndToolBar;
	/////控件左对齐、右对齐等一系列操作
	afx_msg void OnIconLeft();
	afx_msg void OnIconMiddle();
	afx_msg void OnIconRight();
	afx_msg void OnIconWidth();
	afx_msg void OnIconHeight();
	afx_msg void OnIconSameSize();
	afx_msg void OnIconTop();
	afx_msg void OnIconBottom();
	afx_msg void OnIconHor();
	afx_msg void OnIconVer();
	/////////////////////////////////////////
private:
	CRect m_wndToolBarRect;
	//硬件呼叫器，在硬件呼叫器中实现了评价
	//因为当有呼叫信息产生时，会相应的去调用评价
	//让其处理数据所以这样处理
//	CHardWareCaller m_hardCaller;
protected:
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	BOOL ShutDown();
	BOOL DeleteLogFile();
private:
	//界面隐藏
	void AddTrayIcon(void);
	BOOL TaskBarAddIcon(HWND hwnd, UINT uID, HICON hIcon, LPCWSTR lpszTip);
	BOOL TaskBarDeleteIcon(HWND hwnd, UINT uID);
	void RemoveTrayIcon();
	afx_msg LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTipShow();
	afx_msg void OnTipExit();
	void SetFullScreen(BOOL bFullScreen){m_bFullScreen = bFullScreen;}
};


