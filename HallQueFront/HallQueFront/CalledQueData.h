#pragma once

#include "SLZData.h"
#include <afxmt.h>
#include "SLZWindowQueryView.h"
// CCalledQueData 命令目标

class CCalledQueData : public CObject
{
	friend class CallThread;
public:
	CCalledQueData(SLZWindowQueryView& rWindowTable);
	virtual ~CCalledQueData();
	////获取正在呼叫的队列数据
	BOOL GetCalledQueData(const UINT iWinId, SLZData& rdata);
	////删除正在呼叫的队列数据
	BOOL DeleteCalledQueData(const SLZData& rdata);
	////增加正在呼叫的队列数据
	void Add(const SLZData& data);
	BOOL ModifyCalledQueData(const SLZData& rdata);
	////获取大小
	UINT GetCount();
private:
	CList<SLZData> m_lstCalledQue;//正在呼叫队列
	CMutex m_mtCalledQue;
	SLZWindowQueryView& m_rWindowTable;
};