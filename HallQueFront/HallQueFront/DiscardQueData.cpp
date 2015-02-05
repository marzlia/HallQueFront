#include "StdAfx.h"
#include "DiscardQueData.h"
#include "HallQueFront.h"
#include "ComplSocketClient.h"

CDiscardQueData::CDiscardQueData(void) : m_pDoDiscardDataThread(NULL)
{
	Start();
}

CDiscardQueData::~CDiscardQueData(void)
{
	if(m_pDoDiscardDataThread)
	{
		TerminateThread(m_pDoDiscardDataThread->m_hThread,0);
		delete m_pDoDiscardDataThread;
		m_pDoDiscardDataThread = NULL;
	}
}

void CDiscardQueData::Add(const SLZData& data)
{
	m_mtDiscard.Lock();
	m_list_discardQueData.push_back(data);
	m_mtDiscard.Unlock();
}

BOOL CDiscardQueData::GetDiscardQueData(const UINT uWinID,SLZData& data)
{
	BOOL flag = FALSE;
	m_mtDiscard.Lock();
	std::list<SLZData>::const_iterator itera = m_list_discardQueData.begin();
	for(itera;itera!=m_list_discardQueData.end();itera++)
	{
		if(itera->GetWindowId()==uWinID)
		{
			data = *itera;
			m_list_discardQueData.erase(itera);
			flag = TRUE;
			break;
		}
	}
	m_mtDiscard.Unlock();
	return flag;
}

BOOL CDiscardQueData::GetDiscardQueData(SLZData& data)
{
	m_mtDiscard.Lock();
	if(m_list_discardQueData.size()==0)
	{
		m_mtDiscard.Unlock();
		return FALSE;
	}
	std::list<SLZData>::const_iterator itera = m_list_discardQueData.begin();
	data = *itera;
	m_list_discardQueData.pop_front();
	m_mtDiscard.Unlock();
	return TRUE;
}

BOOL CDiscardQueData::Start()
{
	m_pDoDiscardDataThread = AfxBeginThread(DoDiscardData,this,0,0,0,NULL);
	if(m_pDoDiscardDataThread)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

UINT CDiscardQueData::DoDiscardData(LPVOID pParam)
{
	CDiscardQueData* pThis = (CDiscardQueData*)pParam;
	while(TRUE)
	{
		if(pThis->m_list_discardQueData.size()>0)
		{
			SLZData data;
			pThis->GetDiscardQueData(data);
			if(theApp.m_logicVariables.IsAutoSendToServer)//发送到服务端
			{
				CComplSocketClient clinet;
				char buf[MAXRECVBUF]={0};
				int actRecvSize = 0;
//				clinet.SendData(data.GetSerialId(),buf,actRecvSize);
			}
		}
		else
		{
			Sleep(10);
		}
	}
	return 0;
}