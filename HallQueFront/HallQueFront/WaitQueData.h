#pragma once
#include "SLZData.h"
#include <list>
#include "afxmt.h"
using namespace std;
class CWaitQueData
{
public:
	CWaitQueData(void);
	~CWaitQueData(void);
	void Add(const SLZData& data);
	BOOL GetWaitQueData(const UINT winID,SLZData& data);
	BOOL IsHaveWaitQueData(const UINT winID);//判断是否该窗口下有一个人在等待
private:
	std::list<SLZData> m_list_waitQue;
	CMutex m_mtWaitQue;
};
