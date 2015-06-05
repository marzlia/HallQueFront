#pragma once

#include "SLZData.h"
#include <afxmt.h>
#include "SLZWindowQueryView.h"
// CInlineQueData 命令目标

class CInlineQueData : public CObject
{
	friend class CCallThread;
public:
	CInlineQueData(SLZWindowQueryView& rWindowTable);
	virtual ~CInlineQueData();
	//获取队列排队的首个
	BOOL GetInlineQueData(const UINT iWinId, SLZData& rdata);
	void Add(SLZData& data);
	void AddHeadData(const SLZData& data);
	UINT GetCount();//获取总人数
	UINT GetBussCount(const CString& strBussId);//获取对应队列的排队人数
//	BOOL GetInlineQueData(SLZData& rdata);//获取第一个数据
	void RemoveAllData();
	int GetMaxQueNum(const CString queID);
	UINT GetCandoQueCount(UINT iWinID);//获取窗口能够处理的队列排队人数总和
	BOOL AlterData(const CString QueNum,const CString strPhoneNum,const CString strMsg);
	BOOL GetInlineQueData(int i,SLZData& data);
//	BOOL GetLastInlineQueWaitNum(const CString& strBussid,UINT* pWaitNum);
//	BOOL GetLastInlineQueWaitNumSub(const CString& strBussid,UINT* pWaitNum);
	void GetAllBussCount(const CString& strBussid,UINT* pWaitNum);
	BOOL DeleteInlineClientData(BOOL bIsUsePower,const CStringArray& queIDArray,const CString& organId,SLZData* pData);//删除队列中的一个客户机数据
	BOOL GetWindowCanDoQue(UINT nWindowID,CStringArray& queerial_id_array,CString& callStaffID,BOOL* pIsUsePower);//获取窗口能够处理的队列优先级最高的队列以及不使用优先级的队列,以及是否使用优先级
private:
	CList<SLZData> m_lstInlineQue;//排队队列
	CMutex m_mtInlineQue;
	SLZWindowQueryView& m_rWindowTable;
private:
	void GetCandoQueHaveData(CStringArray& queIDArray,const CStringArray& canDoQueIDArray,UINT uWndID = 0);
	BOOL GetFirstTakeNumData(SLZData& data);
};


