// ListBoxPlus.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "ListBoxPlus.h"


// CListBoxPlus

IMPLEMENT_DYNAMIC(CListBoxPlus, CListBox)

CListBoxPlus::CListBoxPlus() :
m_LButtonDownFlag(FALSE)
,m_MouseMoveFlag(FALSE)
{
// 	m_LButtonDownFlag = FALSE;
// 	m_MouseMoveFlag = FALSE;
}

CListBoxPlus::~CListBoxPlus()
{
}


BEGIN_MESSAGE_MAP(CListBoxPlus, CListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CListBoxPlus 消息处理程序

void CListBoxPlus::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
	if(lpDrawItemStruct->itemID==CB_ERR)
		return;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(lpDrawItemStruct->itemID);
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	COLORREF crOldColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();
	//dc.SetTextColor(g_colorKey_colorValue[pCt->nColorKey]);
	dc.SetTextColor(pCt->iColor);
//	dc.SetTextColor(RGB(255,0,0));
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&  
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.SetBkColor(0x000000);
	}
	/*if(!pCt->strText)*/
	dc.DrawText(pCt->strText, pCt->strText.GetLength(), 
		&lpDrawItemStruct->rcItem, DT_WORDBREAK );

	dc.SetTextColor(crOldColor);
	dc.SetBkColor(crOldBkColor);
	dc.Detach();
}

void CListBoxPlus::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)   
{  
	// TODO: Add your code to determine the size of specified item  
	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);  

	CString strText(_T(""));  
	GetText(lpMeasureItemStruct->itemID, strText);  
	ASSERT(TRUE != strText.IsEmpty());  

	CRect rect;  
	GetItemRect(lpMeasureItemStruct->itemID, &rect);  

	CDC* pDC = GetDC();   
	lpMeasureItemStruct->itemHeight = pDC->DrawText(strText, -1, rect, DT_WORDBREAK | DT_CALCRECT);   
	ReleaseDC(pDC);  
}  

void CListBoxPlus::OnDestroy()
{
	CListBox::OnDestroy();

	for(int i = 0; i < GetCount(); i++)
	{
		COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(i);
		delete pCt;
	}
}

HBRUSH CListBoxPlus::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//pDC->SetBkColor(0x000000);
	//pDC->SetBkMode(TRANSPARENT);

	HBRUSH hbr = CListBox::OnCtlColor(pDC, pWnd, nCtlColor);

	//if(nCtlColor == IDC_LIST_STR_CONF)
	//{
	pDC->SetBkColor(0x000000);
	pDC->SetBkMode(TRANSPARENT);

	//}

	return hbr;
}

void CListBoxPlus::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonDown(nFlags, point);
	if(GetCurSel()!=-1)
	m_LButtonDownFlag = TRUE;
}

void CListBoxPlus::OnLButtonUp(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonUp(nFlags, point);
	m_LButtonDownFlag = FALSE;
	if(m_MouseMoveFlag)
	{
		m_MouseMoveFlag = FALSE;
		POINT pt;
		::GetCursorPos(&pt);
		CRect iRect;
		this->GetWindowRect(iRect);
		if(iRect.PtInRect(pt))//确定鼠标移动到了合适的位置
		{
			m_NewPosition = GetCurSel();
			if(m_NewPosition < m_OldPosition)
			{
				InsertString(m_NewPosition,m_DragString);
				DeleteString(m_OldPosition+1);
				this->SetCurSel(m_NewPosition);
				SetItemData(m_NewPosition,m_ItemData);
				TRACE("%d%d%d%d%d%d%d%d",GetItemData(0),GetItemData(1),
				GetItemData(2),GetItemData(3),GetItemData(4),
				GetItemData(5),GetItemData(6),GetItemData(7));
			}
			else
			{
				InsertString(m_NewPosition+1,m_DragString);
				DeleteString(m_OldPosition);
				this->SetCurSel(m_NewPosition);
				SetItemData(m_NewPosition,m_ItemData);
				TRACE("%d%d%d%d%d%d%d%d",GetItemData(0),GetItemData(1),
				GetItemData(2),GetItemData(3),GetItemData(4),
				GetItemData(5),GetItemData(6),GetItemData(7));
			}
		}
	}
}

void CListBoxPlus::OnMouseMove(UINT nFlags, CPoint point)
{
	CListBox::OnMouseMove(nFlags, point);
	if(m_LButtonDownFlag)
	{
		m_MouseMoveFlag = TRUE;
		m_OldPosition = GetCurSel();
		GetText(m_OldPosition,m_DragString);
		try
		{
			m_ItemData = GetItemData(m_OldPosition);
		}
		catch(...)
		{
			AfxMessageBox(_T("Wrong!"));
		}

		m_LButtonDownFlag = FALSE;
		//SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
	}
}

BOOL CListBoxPlus::SetColor(int nItem, COLORREF iColor)
{
	if(nItem < 0) return FALSE;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(nItem);
	if(!pCt) return FALSE;
	pCt->iColor = iColor;
	Invalidate(TRUE);
	return TRUE;
}

BOOL CListBoxPlus::SetText(int nItem, CString strText)
{
	if(nItem < 0) return FALSE;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(nItem);
	if(!pCt) return FALSE;
	pCt->strText = strText;
	Invalidate(TRUE);
	return TRUE;
}

BOOL CListBoxPlus::GetTextMy(int nItem, CString& strText)
{
	if(nItem < 0) return FALSE;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(nItem);
	if(!pCt) return FALSE;
	strText = pCt->strText;
	return TRUE;
}

BOOL CListBoxPlus::GetColor(int nItem, COLORREF& iColor)
{
	if(nItem < 0) return FALSE;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(nItem);
	if(!pCt) return FALSE;
	iColor = pCt->iColor;
	return TRUE;
}

BOOL CListBoxPlus::GetTextAndColor(int nItem, CString& strText, COLORREF& iColor)
{
	if(nItem < 0) return FALSE;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(nItem);
	if(!pCt) return FALSE;
	strText = pCt->strText;
	iColor = pCt->iColor;
	return TRUE;
}

int CListBoxPlus::DeleteStringMy(int nItem)
{
	if(nItem == -1) return LB_ERR;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(nItem);
	if(pCt)
	{
		delete pCt;
	}
	return DeleteString(nItem);
}

int CListBoxPlus::AddStringMy(LPCTSTR lpszText, COLORREF nColorKey)
{
	
	COLORTEXT* pCt = new COLORTEXT;
	pCt->iColor = nColorKey;
	pCt->strText = lpszText;
	
	int nItem = AddString(pCt->strText);
	SetItemDataPtr(nItem, pCt);
	SetCurSel(nItem);
	return nItem;
}

int CListBoxPlus::AddStringMy(COLORTEXT* pCt)
{
	ASSERT(pCt != NULL);
	int nItem = AddString(pCt->strText);
	SetItemDataPtr(nItem, pCt);
	SetCurSel(nItem);
	return nItem;
}

int CListBoxPlus::InsertStringMy(int nItem, LPCTSTR lpszText, COLORREF nColorKey)
{
	COLORTEXT* pCt = new COLORTEXT;
	pCt->iColor = nColorKey;
	pCt->strText = lpszText;
	nItem = InsertString(nItem, pCt->strText);
	SetItemDataPtr(nItem, pCt);
	SetCurSel(nItem);
	return nItem;
}

int CListBoxPlus::InsertStringMy(int nItem, COLORTEXT* pCt)
{
	ASSERT(pCt != NULL);
	nItem = InsertString(nItem, pCt->strText);
	SetItemDataPtr(nItem, pCt);
	SetCurSel(nItem);
	return nItem;
}