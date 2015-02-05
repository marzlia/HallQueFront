#include "StdAfx.h"
#include "CardLevel.h"
UINT g_cardLevelSerialID=0;
IMPLEMENT_SERIAL(CCardLevel,CObject,1);
CCardLevel::CCardLevel(void) : m_cardLevel(-1)
, m_nLevAttchToPageID(-1)
{
	g_cardLevelSerialID++;
	m_cardLevSerialID = g_cardLevelSerialID;
}

CCardLevel::~CCardLevel(void)
{
}

CCardLevel::CCardLevel(const CCardLevel &obj)
{
	m_cardLevel = obj.m_cardLevel;
	m_cardLevName = obj.m_cardLevName;
	m_levAttchToQueID = obj.m_levAttchToQueID;
	m_cardLevSerialID = obj.m_cardLevSerialID;
	m_nLevAttchToPageID = obj.m_nLevAttchToPageID;
}

CCardLevel& CCardLevel::operator=(const CCardLevel& obj)
{
	if(this == &obj)//×Ô¸³Öµ
	{
		return *this;
	}
	m_cardLevel = obj.m_cardLevel;
	m_cardLevName = obj.m_cardLevName;
	m_levAttchToQueID = obj.m_levAttchToQueID;
	m_cardLevSerialID = obj.m_cardLevSerialID;
	m_nLevAttchToPageID = obj.m_nLevAttchToPageID;
	return *this;
}

void CCardLevel::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar<<m_cardLevSerialID<<m_cardLevel<<m_cardLevName<<m_levAttchToQueID<<m_nLevAttchToPageID;
	}
	else
	{
		ar>>m_cardLevSerialID>>m_cardLevel>>m_cardLevName>>m_levAttchToQueID>>m_nLevAttchToPageID;
	}
}