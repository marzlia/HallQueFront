// CalledQueData.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "CalledQueData.h"


// CCalledQueData

CCalledQueData::CCalledQueData(SLZWindowQueryView& rWindowTable) :
m_rWindowTable(rWindowTable)
{
}

CCalledQueData::~CCalledQueData()
{
}


BOOL CCalledQueData::GetCalledQueData(const UINT iWinId, SLZData& rdata)
{
	if(m_lstCalledQue.GetCount() < 1)
	{
		return FALSE;
	}
	CStringArray arrStrQueId;
	SLZWindow Window;
	BOOL flag = m_rWindowTable.QueryWindowById(iWinId,Window);
	if(!flag)return FALSE;
	Window.GetArrayQueId(arrStrQueId);
	if(arrStrQueId.GetCount() < 1)
	{
		return FALSE;
	}
	POSITION pos = m_lstCalledQue.GetHeadPosition();
	BOOL bFind = FALSE;
	m_mtCalledQue.Lock();
	while(pos)
	{
		SLZData data;
		data = m_lstCalledQue.GetNext(pos);
		if(data.GetWindowId() == iWinId)
		{
			rdata = data;
			bFind = TRUE;
			break;
		}
	}
	m_mtCalledQue.Unlock();
	return bFind;
}

void CCalledQueData::Add(const SLZData& data)
{
	m_mtCalledQue.Lock();
	m_lstCalledQue.AddTail(data);
	m_mtCalledQue.Unlock();
}

UINT CCalledQueData::GetCount()
{
	return m_lstCalledQue.GetCount();
}

BOOL CCalledQueData::DeleteCalledQueData(const SLZData& rdata)
{
	m_mtCalledQue.Lock();
	BOOL flag = FALSE;
	POSITION pos = m_lstCalledQue.GetHeadPosition();
	POSITION posLast;
	while(pos)
	{
		posLast = pos;
		if(m_lstCalledQue.GetNext(pos).GetSerialId() == rdata.GetSerialId())
		{
			m_lstCalledQue.RemoveAt(posLast);
			flag = TRUE;
			break;
		}
	}
	m_mtCalledQue.Unlock();
	return flag;
}

// CCalledQueData 成员函数
BOOL CCalledQueData::ModifyCalledQueData(const SLZData& rdata)
{
	m_mtCalledQue.Lock();
	BOOL flag = FALSE;
	POSITION pos = m_lstCalledQue.GetHeadPosition();
	POSITION posLast;
	while(pos)
	{
		posLast = pos;
		if(m_lstCalledQue.GetNext(pos).GetSerialId() == rdata.GetSerialId())
		{
			m_lstCalledQue.SetAt(posLast,rdata);
			flag = TRUE;
			break;
		}
	}
	m_mtCalledQue.Unlock();
	return flag;
}