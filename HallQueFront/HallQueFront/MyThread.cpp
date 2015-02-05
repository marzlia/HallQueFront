// MyThread.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "MyThread.h"


// CMyThread

CMyThread::CMyThread()
: m_pThread(NULL)
{
}

CMyThread::~CMyThread()
{
}

void CMyThread::Start()
{
	if(!m_pThread)
	{
		m_pThread = ::AfxBeginThread(ThreadProc, this);
	}
}

void CMyThread::Suspend()
{
	if(m_pThread)
	{
		m_pThread->SuspendThread();
	}
}

void CMyThread::Resume()
{
	if(m_pThread)
	{
		while(m_pThread->ResumeThread());
	}
}

void CMyThread::Exit()
{
	if(m_pThread)
	{
		m_bContinue = FALSE;
		::WaitForSingleObject(m_pThread->m_hThread, 20);
		TerminateThread(m_pThread->m_hThread,0);
		delete m_pThread;
		m_pThread = NULL;
	}
}

void CMyThread::Run()
{

}

UINT CMyThread::ThreadProc(LPVOID pParam)
{
	CMyThread* pThis = (CMyThread*)pParam;
	pThis->m_bContinue = TRUE;
	
	while(pThis->m_bContinue)
	{
		pThis->Run();
	}

	::ExitThread(0);
	return 0;
}


// CMyThread 成员函数
