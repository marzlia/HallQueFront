#include "StdAfx.h"
#include "QueueInfo.h"



unsigned int gobal_queserial_id;//唯一ID全局变量


IMPLEMENT_SERIAL(CQueueInfo,CObject,1);

CQueueInfo::CQueueInfo(void) : m_queNumStart(0)
, m_queNumEnd(0)
, m_AmLimitCustomer(0)
, m_PmLimitCustomer(0)
, m_queNumber(0)
{
	CString queid;
// 	CCommonConvert convert;
// 	convert.intToCString(gobal_queserial_id,queid);
	/*queid.Format(_T("%d"),gobal_queserial_id);
	m_queserial_id = queid;*/
	m_DayLimitCustomer = m_AmLimitCustomer+m_DayLimitCustomer;
	COleDateTime OleStart;
	OleStart.SetTime(0,0,0);
	SetQueWorkTimeStart(OleStart);
	COleDateTime OleEnd;
	OleEnd.SetTime(23,59,0);
	SetQueWorkTimeEnd(OleEnd);
}

CQueueInfo::~CQueueInfo(void)
{

}

CQueueInfo::CQueueInfo(const CQueueInfo& obj)
{
	m_AmLimitCustomer=obj.m_AmLimitCustomer;
	m_PmLimitCustomer=obj.m_PmLimitCustomer;
	m_DayLimitCustomer=obj.m_AmLimitCustomer+obj.m_PmLimitCustomer;
	m_queNumber=obj.m_queNumber;
	m_queNumEnd=obj.m_queNumEnd;
	m_queNumStart=obj.m_queNumStart;
	//m_stbId=obj.m_stbId;
	m_queserial_id=obj.m_queserial_id;
	m_queName=obj.m_queName;
	m_queCallName=obj.m_queCallName;
	m_queFrontID=obj.m_queFrontID;
	m_tQueWorkStart=obj.m_tQueWorkStart;
	m_tQueWorkEnd=obj.m_tQueWorkEnd;
	m_queManNum = obj.m_queManNum;
	
}

CQueueInfo& CQueueInfo::operator=(CQueueInfo& obj)
{
	m_AmLimitCustomer=obj.m_AmLimitCustomer;
	m_PmLimitCustomer=obj.m_PmLimitCustomer;
	m_DayLimitCustomer=obj.m_AmLimitCustomer+obj.m_PmLimitCustomer;
	m_queNumber=obj.m_queNumber;
	m_queNumEnd=obj.m_queNumEnd;
	m_queNumStart=obj.m_queNumStart;
	//m_stbId=obj.m_stbId;
	m_queserial_id=obj.m_queserial_id;
	m_queName=obj.m_queName;
	m_queCallName=obj.m_queCallName;
	m_queFrontID=obj.m_queFrontID;
	m_tQueWorkStart=obj.m_tQueWorkStart;
	m_tQueWorkEnd=obj.m_tQueWorkEnd;
	m_queManNum = obj.m_queManNum;
	
	return *this;
}

void CQueueInfo::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar<<m_queCallName<<m_queFrontID<<m_queName<<m_queNumber
			<<m_queNumEnd<<m_queNumStart<<m_queserial_id<<m_AmLimitCustomer<<m_PmLimitCustomer
			<<m_DayLimitCustomer<<m_tQueWorkStart<<m_tQueWorkEnd<<m_queManNum;
	}
	else
	{
		ar>>m_queCallName>>m_queFrontID>>m_queName>>m_queNumber
			>>m_queNumEnd>>m_queNumStart>>m_queserial_id>>m_AmLimitCustomer>>m_PmLimitCustomer
			>>m_DayLimitCustomer>>m_tQueWorkStart>>m_tQueWorkEnd>>m_queManNum;
	}
}

