#include "StdAfx.h"
#include "FinshQueData.h"
#include "ComplSocketClient.h"
#include "HallQueFront.h"
#include "ComplSocketClient.h"
#include "ProducePacket.h"

CFinshQueData::CFinshQueData(void) : m_pDoFinshedDataThread(NULL)
{
	Start();
}

CFinshQueData::~CFinshQueData(void)
{
	if(m_pDoFinshedDataThread)
	{
//  		while(TRUE)
//  		{
//  			if(m_listFinshQue.size()==0)
//  			{
//  				break;
//  			}
// 			WaitForSingleObject(m_pDoFinshedDataThread->m_hThread,1000);
//  		}
		TerminateThread(m_pDoFinshedDataThread->m_hThread,0);
		delete m_pDoFinshedDataThread;
		m_pDoFinshedDataThread = NULL;
	}
}

void CFinshQueData::Add(const SLZData& data)
{
	m_mtFinshQue.Lock();
	m_listFinshQue.push_back(data);
	m_mtFinshQue.Unlock();
}

BOOL CFinshQueData::GetFinshedData(const UINT winID,SLZData& data)
{
	BOOL flag = FALSE;
	m_mtFinshQue.Lock();
	std::list<SLZData>::const_iterator itera = m_listFinshQue.begin();
	for(itera;itera!=m_listFinshQue.end();itera++)
	{
		if(itera->GetWindowId() == winID)
		{
			data = *itera;
			m_listFinshQue.erase(itera);
			flag = TRUE;
			break;
		}
	}
	m_mtFinshQue.Unlock();
	return flag;
}

BOOL CFinshQueData::GetFinshedData()
{
	SLZData data;
	m_mtFinshQue.Lock();
	std::list<SLZData>::const_iterator itera = m_listFinshQue.begin();
	data = *itera;
	m_listFinshQue.pop_front();
	BOOL flag = FALSE;
	m_mtFinshQue.Unlock();
	///////////发送到服务端//////////////
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		CComplSocketClient client;
		CProducePacket packet;
 		int leftNum = theApp.m_Controller.m_pInlineQueData->GetBussCount(data.GetBussinessType());
 		CString sendPacket = packet.ProduceSendPacket(data,leftNum,evateNumData);
 		std::string recvMsg;
 		int actRecvSize = 0;
 		flag = client.SendData(sendPacket,recvMsg,actRecvSize); 
 		int errCode = packet.JudgePacketRet(recvMsg);
 		if(errCode!=1) flag=FALSE; 
	}
	return flag;
}

BOOL CFinshQueData::Start()
{
	m_pDoFinshedDataThread = AfxBeginThread(DoFinshedData,this,0,0,0,NULL);
	if(m_pDoFinshedDataThread)
		return TRUE;
	else
		return FALSE;
}

UINT CFinshQueData::DoFinshedData(LPVOID pParam)
{
	CFinshQueData* pThis = (CFinshQueData*)pParam;
	while(TRUE)
	{
		if(pThis->m_listFinshQue.size()>0)
		{
			pThis->GetFinshedData();
		}
		else
		{
			Sleep(10);
		}
	}
	return 0;
}