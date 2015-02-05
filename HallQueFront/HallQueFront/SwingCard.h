#pragma once
#include "ConfigCardDef.h"
class CSwingCard : public CObject
{
public:
	CSwingCard(void);
	~CSwingCard(void);
	//////////////////////////////////////拷贝构造和赋值
	CSwingCard(const CSwingCard& obj);
	CSwingCard& operator=(const CSwingCard& obj);
	///////////////////////////////////////////////////////
	UINT GetCardInfoID()const {return m_cardInofID;}//唯一ID
	CString GetAttchQueID()const {return m_attchQueID;}
	void SetAttchQueID(const CString& queID){m_attchQueID = queID;}
	void SetCardName(const CString& cardName){m_cardName = cardName;}
	CString GetCardName()const {return m_cardName;}
	int GetAttchPageID()const {return m_nAttchPageID;}
	void SetAttchPageID(int nAttchPageID){m_nAttchPageID = nAttchPageID;}
private:
	CString m_cardName;//名称
	UINT m_cardInofID;//刷卡项的ID
	CString m_attchQueID;//关联的队列ID
	int m_nAttchPageID;//关联的页面ID
//	UINT m_count;//CArray的大小
public:
	////卡识别信息
//	CArray<DISTINGUISHCARD>m_disTinguishCard;
	DISTINGUISHCARD m_disTinguishCard[4];
	virtual void Serialize( CArchive& ar );//支持序列化
	DECLARE_SERIAL(CSwingCard)
};
