#pragma once


// CListBoxPlus
//扩展功能的CListBox
class CListBoxPlus : public CListBox
{
	DECLARE_DYNAMIC(CListBoxPlus)
public:
	typedef struct _tagColorText{
		COLORREF	iColor;
		CString		strText;
	} COLORTEXT, *LPCOLORTEXT;

public:
	CListBoxPlus();
	virtual ~CListBoxPlus();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int AddStringMy(LPCTSTR lpszText, COLORREF iColor);
	int AddStringMy(COLORTEXT* pCt);
	int InsertStringMy(int nItem, LPCTSTR lpszText, COLORREF iColor);
	int InsertStringMy(int nItem, COLORTEXT* pCt);
	BOOL SetColor(int nItem, COLORREF iColor);
	BOOL SetText(int nItem, CString strText);
	BOOL GetTextMy(int nItem, CString& strText);
	BOOL GetColor(int nItem, COLORREF& iColor);
	BOOL GetTextAndColor(int nItem, CString& strText, COLORREF& iColor);
	int DeleteStringMy(int nItem);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	BOOL m_LButtonDownFlag;
	BOOL m_MouseMoveFlag;
	int m_OldPosition;
	int m_NewPosition;
	CString m_DragString;
	DWORD m_ItemData;
protected:
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};




