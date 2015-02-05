// OwnerDrawListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "OwnerDrawListBox.h"

extern void MyWriteConsole(CString str);
// COwnerDrawListBox

IMPLEMENT_DYNAMIC(COwnerDrawListBox, CListBox)

COwnerDrawListBox::COwnerDrawListBox() : m_pListBoxDataItems(NULL)
, m_textPos(ITEM_LEFT)
, m_iconPos(ITEM_RIGHT)
{
}

COwnerDrawListBox::~COwnerDrawListBox()
{
}


BEGIN_MESSAGE_MAP(COwnerDrawListBox, CListBox)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// COwnerDrawListBox 消息处理程序

void COwnerDrawListBox::SetTextPosition(Item_Pos txPos)
{
	m_textPos = txPos;
}

void COwnerDrawListBox::SetIconPosition(Item_Pos icPos)
{
	m_iconPos = icPos;
}


void COwnerDrawListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(!GetCount())return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect* rectText = NULL;CRect* rectIcon = NULL;
//	COLORREF crOldTextColor = pDC->GetTextColor();//文字颜色
	/*CFont crOldTextFont = pDC->GetCurrentFont();//字体*/
//	COLORREF crOldBkColor = pDC->GetBkColor();//背景色
	CRect rectClient;
	GetClientRect(rectClient);
	m_rectListBoxItem = lpDrawItemStruct->rcItem;
	
	DataItems* pListDataItem = (DataItems*)GetItemDataPtr(lpDrawItemStruct->itemID);
	ASSERT(pListDataItem!=NULL);
	if (m_textPos == ITEM_RIGHT )
	{
		if (m_iconPos == ITEM_LEFT)//图片在左,文字在右
		{
			rectIcon = new CRect(m_rectListBoxItem.left+10,m_rectListBoxItem.top,m_rectListBoxItem.left+10+32,m_rectListBoxItem.bottom); 
			rectText = new CRect(rectIcon->right,m_rectListBoxItem.top,m_rectListBoxItem.right,m_rectListBoxItem.bottom);	
		}else
		{
			rectIcon = new CRect(m_rectListBoxItem.left+10,m_rectListBoxItem.top,m_rectListBoxItem.left+10+32,m_rectListBoxItem.bottom); 
			rectText = new CRect(rectIcon->right,m_rectListBoxItem.top,m_rectListBoxItem.right,m_rectListBoxItem.bottom);	
		}
	}else if (m_textPos == ITEM_LEFT )
	{
		if (m_iconPos == ITEM_RIGHT)
		{
			rectText = new CRect(m_rectListBoxItem.left+10,m_rectListBoxItem.top,m_rectListBoxItem.left+m_rectListBoxItem.right-32,m_rectListBoxItem.bottom);
			rectIcon = new CRect(rectText->right,m_rectListBoxItem.top,m_rectListBoxItem.right,m_rectListBoxItem.bottom);
		}else
		{
			CSize textSize;
			GetTextExtentPoint32(lpDrawItemStruct->hDC,pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),&textSize);
			rectText = new CRect(m_rectListBoxItem.left+10,m_rectListBoxItem.top,m_rectListBoxItem.left+10+textSize.cx,m_rectListBoxItem.bottom);
			rectIcon = new CRect(rectText->right+10,m_rectListBoxItem.top,rectText->right+10+32,m_rectListBoxItem.bottom);
		}
	}
	if((lpDrawItemStruct->itemAction | ODA_FOCUS) &&
		(lpDrawItemStruct->itemState & ODS_FOCUS))

	{
		pDC->DrawFocusRect(m_rectListBoxItem);
	}
	
	
	else if (	(lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{	
		pDC->FillSolidRect(&lpDrawItemStruct->rcItem,::GetSysColor(COLOR_HIGHLIGHT));		
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));	

		if ( m_textPos == ITEM_LEFT )
		{
			pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText,DT_VCENTER|DT_SINGLELINE|DT_VCENTER);

			if (m_iconPos == ITEM_RIGHT)
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
			else if (m_iconPos == ITEM_LEFT )
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
		}
		else if (m_textPos == ITEM_RIGHT)
		{
			pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText,DT_LEFT|DT_SINGLELINE|DT_VCENTER);

			if(m_iconPos == ITEM_LEFT)
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
			else if ( m_iconPos == ITEM_RIGHT )
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
		}
		else if ( m_iconPos == ITEM_RIGHT )
		{
			pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);

			if ( m_textPos == ITEM_LEFT)
				pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText,DT_VCENTER|DT_SINGLELINE|DT_VCENTER);
			else if ( m_textPos == ITEM_RIGHT)
				pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);

		}
		else if ( m_iconPos == ITEM_LEFT)
		{
			pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);

			if ( m_textPos == ITEM_RIGHT)
				pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			else if ( m_textPos == ITEM_LEFT)
				pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText,DT_VCENTER|DT_SINGLELINE|DT_VCENTER);
		}
		else
		{
			pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
		}

	}
	else
	{
		// Fill the item rect with white
		COLORREF witheColor=RGB(255,255,255);
		COLORREF blakColor = RGB(0,0,0);
		pDC->FillSolidRect(&lpDrawItemStruct->rcItem, witheColor);
		pDC->SetBkColor(witheColor);
		pDC->SetTextColor(blakColor);

		if ( m_textPos == ITEM_LEFT )
		{
			pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText,DT_VCENTER|DT_SINGLELINE|DT_VCENTER);

			if (m_iconPos == ITEM_RIGHT)
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
			else if (m_iconPos == ITEM_LEFT )
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
		}
		else if (m_textPos == ITEM_RIGHT)
		{
			pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText,DT_LEFT|DT_SINGLELINE|DT_VCENTER);

			if(m_iconPos == ITEM_LEFT)
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
			else if ( m_iconPos == ITEM_RIGHT )
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
		}
		else if ( m_iconPos == ITEM_RIGHT )
		{
			pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);

			if ( m_textPos == ITEM_LEFT)
				pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText,DT_VCENTER|DT_SINGLELINE|DT_VCENTER);
			else if ( m_textPos == ITEM_RIGHT)
				pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);

		}
		else if ( m_iconPos == ITEM_LEFT)
		{
			pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);

			if ( m_textPos == ITEM_RIGHT)
				pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			else if ( m_textPos == ITEM_LEFT)
				pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText,DT_VCENTER|DT_SINGLELINE|DT_VCENTER);
		}
		else
		{
			pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			pDC->DrawText(pListDataItem->strItemData,pListDataItem->strItemData.GetLength(),rectText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
		}
	}

	if(rectIcon)
		delete rectIcon;
	if(rectText)
		delete rectText;
	// TODO:  添加您的代码以绘制指定项
}

int COwnerDrawListBox::AddListBoxItem(const CString strItemName, HICON hDataIcon)
{
	m_pListBoxDataItems = new DataItems;
	m_pListBoxDataItems->strItemData = strItemName;
	m_pListBoxDataItems->hIcon = hDataIcon;
	///获取index
	int index = AddString(_T(""));
	SetItemDataPtr(index, m_pListBoxDataItems);
	return index;
}

void COwnerDrawListBox::DeleteListBoxItem(UINT nItem)
{
	DataItems* pDataItems = (DataItems*)GetItemDataPtr(nItem);
	if(pDataItems)
	{
		delete pDataItems;
		pDataItems = NULL; 
	}
	DeleteString(nItem);
}

void COwnerDrawListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 32;
	// TODO:  添加您的代码以确定指定项的大小
}
void COwnerDrawListBox::Destroy()
{
	int ItemCount = GetCount();

	if (ItemCount != 0 )
		for (int i = 0; i < ItemCount; i++)
		{
			m_pListBoxDataItems = (DataItems*) GetItemDataPtr(i);
			delete m_pListBoxDataItems;
		}
}
void COwnerDrawListBox::OnDestroy()
{
	Destroy();
	CListBox::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
}
