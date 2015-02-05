#pragma once
#include <afxmt.h>
#include <list>
#include "SLZData.h"
#include <string>
using namespace std;

using namespace std;

class CFinshQueData
{
public:
	CFinshQueData(void);
	~CFinshQueData(void);
	void Add(const SLZData& data);//增加数据
	BOOL GetFinshedData(const UINT winID,SLZData& data);//获取数据,并删除
	BOOL GetFinshedData();
private:
	std::list<SLZData> m_listFinshQue;//完成队列
	CMutex m_mtFinshQue;//完成队列锁
private:
	static UINT DoFinshedData(LPVOID pParam);
	CWinThread* m_pDoFinshedDataThread;//处理完成数据线程
	BOOL Start();
};
