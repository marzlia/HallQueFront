#include "StdAfx.h"
#include "CallerCmdData.h"

CCallerCmdData::CCallerCmdData(void)
{
}

CCallerCmdData::~CCallerCmdData(void)
{
}

BOOL CCallerCmdData::GetHead(CallerCmd& callerCmd)
{
	if(m_lstCallerCmd.GetCount() > 0)
	{
		m_mtCallerCmd.Lock();
		callerCmd = m_lstCallerCmd.GetHead();
		m_lstCallerCmd.RemoveHead();
		m_mtCallerCmd.Unlock();
		return TRUE;
	}
	return FALSE;
}

void CCallerCmdData::AddTail(CallerCmd& callerCmd)
{
	m_mtCallerCmd.Lock();
	m_lstCallerCmd.AddTail(callerCmd);
	m_mtCallerCmd.Unlock();
}

UINT CCallerCmdData::GetCount()
{
	return m_lstCallerCmd.GetCount();
}
