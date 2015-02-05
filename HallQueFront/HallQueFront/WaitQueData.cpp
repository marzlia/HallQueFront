#include "StdAfx.h"
#include "WaitQueData.h"

CWaitQueData::CWaitQueData(void)
{
}

CWaitQueData::~CWaitQueData(void)
{
}

void CWaitQueData::Add(const SLZData& data)
{
	m_mtWaitQue.Lock();
	m_list_waitQue.push_back(data);
	m_mtWaitQue.Unlock();
}

BOOL CWaitQueData::GetWaitQueData(const UINT winID,SLZData& data)
{
	BOOL flag = FALSE;
	m_mtWaitQue.Lock();
	std::list<SLZData>::const_iterator itera = m_list_waitQue.begin();
	for(itera;itera!=m_list_waitQue.end();itera++)
	{
		if(winID==itera->GetWindowId())
		{
			flag = TRUE;
			data = *itera;
			m_list_waitQue.erase(itera);//É¾³ý
			break;
		}
	}
	m_mtWaitQue.Unlock();
	return flag;
}

BOOL CWaitQueData::IsHaveWaitQueData(const UINT winID)
{
	BOOL flag = FALSE;
	std::list<SLZData>::const_iterator itera = m_list_waitQue.begin();
	for(itera;itera!=m_list_waitQue.end();itera++)
	{
		if(winID==itera->GetWindowId())
		{
			flag = TRUE;
			break;
		}
	}
	return flag;
}