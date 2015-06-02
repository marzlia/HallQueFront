#include "StdAfx.h"
#include "StbContent.h"

IMPLEMENT_SERIAL(CStbContent,CObject,1)
UINT g_stbSerialID;
CStbContent::CStbContent(void) : m_nLineNum(5)
{
	g_stbSerialID++;
	m_nSerialID = g_stbSerialID;
}

CStbContent::~CStbContent(void)
{
}


CStbContent::CStbContent(const CStbContent& obj)
{
	m_nSerialID = obj.m_nSerialID;
	m_strName = obj.m_strName;
	m_strNotice = obj.m_strNotice;
	m_strTitle = obj.m_strTitle;
	m_strTitlePicPath = obj.m_strTitlePicPath;
	m_strNum = obj.m_strNum;
	m_nLineNum = obj.m_nLineNum;
}

CStbContent& CStbContent::operator =(const CStbContent& obj)
{
	m_nSerialID = obj.m_nSerialID;
	m_strName = obj.m_strName;
	m_strNotice = obj.m_strNotice;
	m_strTitle = obj.m_strTitle;
	m_strTitlePicPath = obj.m_strTitlePicPath;
	m_strNum = obj.m_strNum;
	m_nLineNum = obj.m_nLineNum;
	return *this;
}

void CStbContent::Serialize( CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar<<m_nSerialID<<m_strName<<m_strNotice<<m_strTitle<<m_strTitlePicPath<<m_strNum<<m_nLineNum;
	}
	else
	{
		ar>>m_nSerialID>>m_strName>>m_strNotice>>m_strTitle>>m_strTitlePicPath>>m_strNum>>m_nLineNum;
		g_stbSerialID = g_stbSerialID > m_nSerialID ? g_stbSerialID : m_nSerialID;
	}
}