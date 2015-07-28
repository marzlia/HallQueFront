#pragma once

#include "SLZData.h"
#include <afxmt.h>
#include "SLZWindowQueryView.h"
// CInlineQueData ����Ŀ��

class CInlineQueData : public CObject
{
	friend class CCallThread;
public:
	CInlineQueData(SLZWindowQueryView& rWindowTable);
	virtual ~CInlineQueData();
	//��ȡ�����Ŷӵ��׸�
	BOOL GetInlineQueData(const UINT iWinId, SLZData& rdata);
	void Add(SLZData& data);
	void AddHeadData(const SLZData& data);
	UINT GetCount();//��ȡ������
	UINT GetBussCount(const CString& strBussId);//��ȡ��Ӧ���е��Ŷ�����
//	BOOL GetInlineQueData(SLZData& rdata);//��ȡ��һ������
	void RemoveAllData();
	int GetMaxQueNum(const CString queID);
	UINT GetCandoQueCount(UINT iWinID);//��ȡ�����ܹ�����Ķ����Ŷ������ܺ�
	BOOL AlterData(const CString QueNum,const CString strPhoneNum,const CString strMsg);
	BOOL GetInlineQueData(int i,SLZData& data);
//	BOOL GetLastInlineQueWaitNum(const CString& strBussid,UINT* pWaitNum);
//	BOOL GetLastInlineQueWaitNumSub(const CString& strBussid,UINT* pWaitNum);
	void GetAllBussCount(const CString& strBussid,UINT* pWaitNum);
	BOOL DeleteInlineClientData(BOOL bIsUsePower,const CStringArray& queIDArray,const CString& organId,SLZData* pData);//ɾ�������е�һ���ͻ�������
	BOOL GetWindowCanDoQue(UINT nWindowID,CStringArray& queerial_id_array,CString& callStaffID,BOOL* pIsUsePower);//��ȡ�����ܹ�����Ķ������ȼ���ߵĶ����Լ���ʹ�����ȼ��Ķ���,�Լ��Ƿ�ʹ�����ȼ�
private:
	CList<SLZData> m_lstInlineQue;//�ŶӶ���
	CMutex m_mtInlineQue;
	SLZWindowQueryView& m_rWindowTable;
private:
	void GetCandoQueHaveData(CStringArray& queIDArray,const CStringArray& canDoQueIDArray,UINT uWndID = 0);
	BOOL GetFirstTakeNumData(SLZData& data,const CStringArray& arrStrQueId);
	BOOL RemoveFirstTakeNumData(const SLZData& data);
};


