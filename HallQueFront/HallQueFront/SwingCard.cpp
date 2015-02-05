#include "StdAfx.h"
#include "SwingCard.h"
UINT g_swingCardInfoID=0;
IMPLEMENT_SERIAL(CSwingCard,CObject,1);

CSwingCard::CSwingCard(void) : m_nAttchPageID(-1)
{
	g_swingCardInfoID++;
	m_cardInofID = g_swingCardInfoID;
	memset(m_disTinguishCard,0,4*sizeof(DISTINGUISHCARD));
//	m_count = 0;
//	m_disTinguishCard.SetSize(4,4);
}

CSwingCard::~CSwingCard(void)
{
}

CSwingCard::CSwingCard(const CSwingCard& obj)
{
	m_cardName = obj.m_cardName;
	m_attchQueID = obj.m_attchQueID;
	m_cardInofID = obj.m_cardInofID;
// 	int size = obj.m_disTinguishCard.GetCount();
// 	for(int i=0;i<size;i++)
// 	{
// 		m_disTinguishCard.Add(obj.m_disTinguishCard[i]);
// 	}
	m_nAttchPageID = obj.m_nAttchPageID;
	for(int i=0;i<4;i++)
	{
		m_disTinguishCard[i] = obj.m_disTinguishCard[i];
	}
}

CSwingCard& CSwingCard::operator = (const CSwingCard& obj)
{
	m_cardName = obj.m_cardName;
	m_attchQueID = obj.m_attchQueID;
	m_cardInofID = obj.m_cardInofID;
	m_nAttchPageID = obj.m_nAttchPageID;
// 	int size = obj.m_disTinguishCard.GetCount();
// 	for(int i=0;i<size;i++)
// 	{
// 		m_disTinguishCard.Add(obj.m_disTinguishCard[i]);
// 	}
	for(int i=0;i<4;i++)
	{
		m_disTinguishCard[i] = obj.m_disTinguishCard[i];
	}
	return *this;
}

void CSwingCard::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
// 		UINT count = m_disTinguishCard.GetCount();
// 		m_count = count;
		////////////////////////////////////////////////////
		ar<<m_cardName<<m_cardInofID<<m_attchQueID<<m_nAttchPageID;
		/////////////////////////////////////////////////////
		for(UINT i=0;i<4;i++)
		{
			ar<<m_disTinguishCard[i].postion<<m_disTinguishCard[i].toValue;
		}
	}
	else
	{
		ar>>m_cardName>>m_cardInofID>>m_attchQueID>>m_nAttchPageID;
		for(UINT i=0;i<4;i++)
		{
			DISTINGUISHCARD distinguishCard;
			ar>>distinguishCard.postion>>distinguishCard.toValue;
//			m_disTinguishCard.Add(distinguishCard);
			m_disTinguishCard[i] = distinguishCard;
		}
	}
}