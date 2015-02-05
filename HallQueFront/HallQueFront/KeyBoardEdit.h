#pragma once


// CKeyBoardEdit

class CKeyBoardEdit : public CEdit
{
	DECLARE_DYNAMIC(CKeyBoardEdit)

public:
	CKeyBoardEdit();
	virtual ~CKeyBoardEdit();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnKillfocus();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	BOOL m_bOskOpen;
	CString m_strKey;
	CFont m_keyboardfont;
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};


