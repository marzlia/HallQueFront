#pragma once
#include <list>
#include "afxmt.h"
#include "SLZData.h"
using namespace std;

class CDiscardQueData
{
public:
	CDiscardQueData(void);
	~CDiscardQueData(void);
	void Add(const SLZData& data);//增加到队列
	BOOL GetDiscardQueData(const UINT uWinID,SLZData& data);//获取
	BOOL GetDiscardQueData(SLZData& data);
private:
	std::list<SLZData> m_list_discardQueData;//过号队列
	CMutex m_mtDiscard;//过号队列锁
private:
	static UINT DoDiscardData(LPVOID pParam);
	CWinThread* m_pDoDiscardDataThread;//处理完成数据线程
	BOOL Start();
};
