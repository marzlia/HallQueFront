#include "StdAfx.h"
#include "EvaThread.h"
#include "DataDef.h"
#include "HallQueFront.h" 

extern void MyWriteConsole(CString str); 

CEvaThread::CEvaThread(CEvaCmdData& evaData,CCallThread* pCallThread) : 
m_evaCmdData(evaData)
,m_pCallThread(pCallThread)
{
//	MyCreateEvent();
}

CEvaThread::~CEvaThread(void)
{
}

void CEvaThread::Run()
{
	if(m_evaCmdData.GetEvaListCount()>0)
	{
// #ifdef _DEBUG
// 		CString str;
// 		str.Format(_T("ping jia count %d"),m_evaCmdData.GetEvaListCount());
// 		MyWriteConsole(str);
// #endif
		SLZEvaData data;
		m_evaCmdData.GetHeadEvaData(data);
		DispatchCallerCmd(data);
	}
	else
	{
		Sleep(10);
	}
}

void CEvaThread::DispatchCallerCmd(SLZEvaData& evaData)
{
// #ifdef _DEBUG
// 	MyWriteConsole(_T("dispath cmd"));
// #endif
//	::WaitForSingleObject(theApp.m_hStartEvent,INFINITE);
//	CloseHandle(theApp.m_hStartEvent);
	SLZEvaData* pEvaData = new SLZEvaData;
	pEvaData->SetEvaluatorId(evaData.GetEvaluatorId());
	pEvaData->SetEvaLevel(evaData.GetEvaLevel());
 	if(!PostThreadMessage(m_pCallThread->GetThreadId(),EVA_MSG,(WPARAM)pEvaData,0))//post thread msg
 	{
// #ifdef _DEBUG
// 		MyWriteConsole(_T("postmessageFailed"));
// #endif
		Sleep(10);
		PostThreadMessage(m_pCallThread->GetThreadId(),EVA_MSG,(WPARAM)pEvaData,0);
 		return;
 	}
	else
	{
		//生成文件或者发送到服务端
// #ifdef _DEBUG
// 		MyWriteConsole(_T("PostMessage succed"));
// #endif
	}
}

BOOL CEvaThread::MyCreateEvent()
{
	theApp.m_hStartEvent = ::CreateEvent(0,FALSE,FALSE,0);//创建事件
	if(theApp.m_hStartEvent==0)
	{
// #ifdef _DEBUG
// 		MyWriteConsole(_T("carete event fialed"));
// #endif
		return FALSE;
	}
	return TRUE;
}