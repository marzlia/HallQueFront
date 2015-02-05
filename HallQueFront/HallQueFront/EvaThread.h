#pragma once
#include "mythread.h"
#include "EvaCmdData.h"
#include "SLZEvaData.h"
#include "CallThread.h"
class CEvaThread : public CMyThread
{
public:
	CEvaThread(CEvaCmdData& evaData,CCallThread* pCallThread);
	virtual ~CEvaThread(void);
	BOOL MyCreateEvent();
private:
	virtual void Run();
	void DispatchCallerCmd(SLZEvaData& evaData);
private:
	CEvaCmdData& m_evaCmdData;
	CCallThread* m_pCallThread;
};
