#pragma once


// COwnerDrawListBox

class COwnerDrawListBox : public CListBox
{
	DECLARE_DYNAMIC(COwnerDrawListBox)

public:
	COwnerDrawListBox();
	virtual ~COwnerDrawListBox();
public:
	//用于图标和文字居左以及右的enum
	static enum Item_Pos{ITEM_RIGHT,ITEM_LEFT};
	void SetTextPosition(Item_Pos txPos);//设置文字居左、右
	void SetIconPosition(Item_Pos icPos);//设置图片居左、右
	int AddListBoxItem(const CString strItemName, HICON hDataIcon);//增加item
	void DeleteListBoxItem(UINT nItem);
protected:
	//ListBox数据结构体
	struct DataItems
	{
		CString strItemData;
		HICON hIcon;
	};
	DECLARE_MESSAGE_MAP()
private:
	Item_Pos m_textPos;
	Item_Pos m_iconPos;
	DataItems *m_pListBoxDataItems;
	CRect m_rectListBoxItem;
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void Destroy();
	afx_msg void OnDestroy();
};


