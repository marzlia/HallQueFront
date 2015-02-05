#pragma once



// CPlaySoundThread

class CPlaySoundThread : public CWinThread
{
	DECLARE_DYNCREATE(CPlaySoundThread)

protected:
	CPlaySoundThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CPlaySoundThread();
	afx_msg void AFX_MSG_CALL OnReset(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


