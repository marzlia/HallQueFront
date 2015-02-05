#pragma once
#include <atlimage.h>
#include "TrackDef.h"
// CTransButton
#include "ComputeFuncationTime.h"


class CTransButton : public CButton
{
	DECLARE_DYNAMIC(CTransButton)

public:
	CTransButton(CWnd* pParentWnd);
	virtual ~CTransButton();

protected:
	DECLARE_MESSAGE_MAP()

public:
	////设置Button的各种变量	
	void SetBkColor(const COLORREF color);
	void SetTextColor(const COLORREF color);
	void SetAlpha(const UINT nAlpha);
	void SetFont(const LOGFONT lfFont);
	void SetButtonType(const ButtonTypeEnum ButtonType);
	void SetBkPic(const CString StrBkPic);
	///////////////////////////////
	//获取Button的各种变量
	COLORREF GetBkColor()const {return m_bkColor;}
	COLORREF GetTextColor()const {return m_textColor;}
	UINT GetAlpha()const {return m_nAlpha;}
	LOGFONT GetFont()const {return m_lfFont;}
	ButtonTypeEnum GetButtonType()const {return m_ButtonType;}
	CString GetAttchAddress()const {return m_address;}
	CString GetBkPic()const {return m_BkPic;}
private:
	BOOL m_bOver;
	BOOL m_bDown;
	BOOL m_bDisable;
	BOOL m_bTracking;
	COLORREF m_bkColor;
	COLORREF m_textColor;
	UINT m_nAlpha;
	CWnd* m_pParentWnd;
	LOGFONT m_lfFont;
	ButtonTypeEnum m_ButtonType;
	CString m_address;//链接处理的队列
	int m_nLinkPageID;//链接界面ID,切换界面
	CString m_BkPic;// 背景图片地址

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);//画按钮
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnable(BOOL bEnable);
private:
	void ButtonInit();
	void DrawButton();
	void DrawButton(HDC hDestDC);
//	CRect m_destRect;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//功能函数，确定按钮点击产生的结果,链接队列或者网页
	void AttachToQueue(const CString& address);
	CComputeFuncationTime m_computeTime;//计算时间
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//功能函数,设置按钮链接到哪个界面
	void AttachToPage(int nPageID){m_nLinkPageID = nPageID;}
	int GetAttachPageID(){return m_nLinkPageID;}
};