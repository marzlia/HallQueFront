// PlaySoundThread.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "PlaySoundThread.h"
#include "DataDef.h"

// CPlaySoundThread

IMPLEMENT_DYNCREATE(CPlaySoundThread, CWinThread)

CPlaySoundThread::CPlaySoundThread()
{
}

CPlaySoundThread::~CPlaySoundThread()
{
}

BOOL CPlaySoundThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CPlaySoundThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPlaySoundThread, CWinThread)
		ON_THREAD_MESSAGE(CALL_MSG, &CPlaySoundThread::OnReset)
END_MESSAGE_MAP()


// CPlaySoundThread 消息处理程序
void AFX_MSG_CALL CPlaySoundThread::OnReset(WPARAM wParam, LPARAM lParam)
{
	// 加入自己的实现代码
}