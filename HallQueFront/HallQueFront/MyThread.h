#pragma once

// CMyThread 命令目标

class CMyThread : public CObject
{
public:
	CMyThread();
	virtual ~CMyThread();
	void Start();
	void Suspend();
	void Resume();
	void Exit();
	DWORD GetThreadId()
	{
		return m_pThread->m_nThreadID;
	}

protected:
	virtual void Run();	//子类重写该函数，将线程循环体内执行的代码放入该函数

private:
	CWinThread* m_pThread;
	BOOL m_bContinue;
	static UINT ThreadProc(LPVOID pParam);
};


