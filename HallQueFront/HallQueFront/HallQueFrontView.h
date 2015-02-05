// HallQueFrontView.h : CHallQueFrontView 类的接口
//


#pragma once
#include "TrackContrl.h"
#include "TrackDef.h"
#include "SLZController.h"
#include <afxmt.h>
#include "WaringDlg.h"
#include "ShowPageDlg.h"
#include <list>
using namespace std;

#define  FORSHOWTIME 9568
#define  WORKTIME 9578
#define  DOINITThROUGHSCREEN 9579
#define  BUTTONNUM 200

class CHallQueFrontView : public CView
{
protected: // 仅从序列化创建
	CHallQueFrontView();
	DECLARE_DYNCREATE(CHallQueFrontView)

// 属性
public:
	CHallQueFrontDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CHallQueFrontView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnQuitmanage();
private:
	//判断控件是否处于编辑状态
	BOOL m_isManage;
public:
	afx_msg void OnManage();
	afx_msg void OnAddbutton();
	afx_msg void OnAddtext();
	afx_msg void OnAddpic();
	afx_msg void OnQuithallque();
	afx_msg void OnQuitsys();
	///trackContrl对象处理控件的移动、大小等操作
	list<CTrackContrl*> m_list_trackCtrl;//多级界面处理
	CTrackContrl* m_pTrackCtrl;//显示的当前页面
	//////////////////////////////////////////////////
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnErasebackpic();
public:
//	CString m_backPicpath;
	CImage m_backImage;
	CRect m_clientRect;
	//SLZController control;
private:
	CString m_picType;
	CString m_strPicFilter;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	///画背景
	void MyDraw(CDC* pDC);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//编辑控件
	afx_msg void OnEditctr();
	//让控件链接到具体功能
	afx_msg void OnLinkctr();
	//删除一个控件
	afx_msg void OnDeletectr();
	//加载背景图片
	BOOL MyLoadBackImage(CString path);
	BOOL MyLoadBackImage(UINT nPageID);

	//判断控件的各种状态
	BOOL m_isEdit;//判断是否编辑现有控件和新添加控件
	BOOL m_isAddButton;//添加按钮
	BOOL m_isAddText;//添加文本
	BOOL m_isAddPic;//添加图片
	BOOL m_isShowTime;//显示时间
	BOOL m_isShowQueNum;//显示排队人数
	///用于写文件的结构体
	CList<WINDOWCTRINFO,WINDOWCTRINFO&> m_list_allCtrInfo;
	//控件信息写入/读取 文件
	BOOL WriteCtrInfoToFile();
	BOOL ReadCtrInfoFromFile();
	virtual void OnInitialUpdate();
	//把背景图片的路径写入\读出文件
//	BOOL WriteBackPicPathToFile();
//	BOOL ReadBackPicPathFromFile();
	//从文件读出数据后，创建所有的控件
	void CreateAllCtr();
	//保存背景DC
	void GetBackDC();
//	afx_msg void OnSysseting();
	afx_msg void OnShowTime();
	afx_msg void OnShowQuenum();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//响应按钮
	afx_msg void OnButtonClick(UINT uID);
public:
	//////////////////////////////////////
	//界面接口
	BOOL HasData();
	CString GetData();
	void ShowWaitNum(const CString& queID,const int waitNum );//显示等待人数
	//////////////////////////////////////
private:
	CList<CString> m_list_address;
	CMutex m_Mlock;
	const CString m_workTimeOut;
	void WriteCtrlDataToList();
public:
	afx_msg void OnQueset();
	afx_msg void OnStaffset();
	afx_msg void OnWindowset();
	afx_msg void OnPrintset();
	afx_msg void OnCardset();
	afx_msg void OnComprehset();
private:
	void JudgeButtonWorkOut();
	CWaringDlg* m_pWaringDlg;
	CRect m_rWaringRect;
	CRect m_rShowPageRect;
	///写有几个界面到文件
	CString m_strPageFilePath;
	BOOL WritePageToFile();
	BOOL ReadPageFromFile();
	//清除界面内存
	void ClearPage();
public:
	afx_msg void OnStbset();
	afx_msg LRESULT OnMyShowMessage(WPARAM wParam, LPARAM lParam);//接受界面显示缺纸消息函数
	afx_msg LRESULT OnMyShowPage(WPARAM wParam,LPARAM lParam);//接受界面重画消息,用于slzcontrol线程调用
	ShowVariables m_showVaria;
	afx_msg void OnViewminsize();
public:
	CShowPageDlg* m_pShowPageDlg;
	void ShowPage(int nPageID);
	afx_msg void OnLeadinwav();
};

#ifndef _DEBUG  // HallQueFrontView.cpp 中的调试版本
inline CHallQueFrontDoc* CHallQueFrontView::GetDocument() const
   { return reinterpret_cast<CHallQueFrontDoc*>(m_pDocument); }
#endif