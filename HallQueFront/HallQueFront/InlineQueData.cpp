// InlineQueData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "InlineQueData.h"
#include "CommonConvert.h"


// CInlineQueData
extern void MyWriteConsole(CString str);

CInlineQueData::CInlineQueData(SLZWindowQueryView& rWindowTable)
: m_rWindowTable(rWindowTable)
{
}

CInlineQueData::~CInlineQueData()
{
}

/*
��ȡ�Ŷ� �����׸�����
*/
BOOL CInlineQueData::GetInlineQueData(const UINT iWinId, 
									  SLZData& rdata)
{
	if(m_lstInlineQue.GetCount() < 1)
	{
		return FALSE;
	}
	CStringArray arrStrQueId;//�ɴ�������Լ����ȼ�
	SLZWindow Window;
	BOOL flag = m_rWindowTable.QueryWindowById(iWinId,Window);
	if(!flag)return FALSE;
	BOOL bIsUserPower = Window.GetIsUsePower();//�Ƿ�ʹ�����ȼ�
	Window.GetArrayQueId(arrStrQueId);//��ȡ�ɴ�����м����ȼ�
	CString staffID = theApp.m_Controller.m_mapLoginList[Window.GetWindowId()];//��ȡ��¼STAFFID
	if(staffID.IsEmpty())return FALSE;
	if(arrStrQueId.GetCount() < 1)
	{
		return FALSE;
	}
	///////////////////////
	BOOL bFind = FALSE;
	m_mtInlineQue.Lock();
	if(bIsUserPower)//ʹ�����ȼ�
	{
		int count = arrStrQueId.GetCount();
		for(int i = 0; i < count; i++)
		{
			POSITION pos = m_lstInlineQue.GetHeadPosition();
			while(pos)
			{
				SLZData data;
				POSITION posLast = pos;
				data = m_lstInlineQue.GetNext(pos);
				if(data.GetWindowId()==0)//���û����ָ������
				{
					if(arrStrQueId[i] == data.GetBussinessType())
					{
						bFind = TRUE;
						rdata = data;
						m_lstInlineQue.RemoveAt(posLast);
						break;
					}
				}
				else//������ָ���������Ⱥ���
				{
					if(iWinId == data.GetWindowId())
					{
						rdata = data;
						m_lstInlineQue.RemoveAt(posLast);
						bFind = TRUE;
						break;
					}
				}
			}
			if(bFind)
			{
				break;
			}
		}
	}
	else//��ʹ�����ȼ�
	{
		/*
		/////////////////////////////////////////�����ҵ��ɴ���������ļ�������������
		CStringArray haveDataArray;
		GetCandoQueHaveData(haveDataArray,arrStrQueId,iWinId);
		if(haveDataArray.GetCount() == 0)
		{
			return FALSE;
		}
		/////////////////////////////////////////���ȡ��
		int count = haveDataArray.GetCount();
		time_t t;	
		srand((unsigned) time(&t));	
		int nRand = rand() % count;
		CString strQueID = haveDataArray[nRand];
		POSITION pos = m_lstInlineQue.GetHeadPosition();
		while(pos)
		{
			SLZData data;
			POSITION posLast = pos;
			data = m_lstInlineQue.GetNext(pos);
			if(data.GetWindowId()==0)//���û����ָ������
			{
				if(strQueID.Compare(data.GetBussinessType()) == 0)
				{
					rdata = data;
					m_lstInlineQue.RemoveAt(posLast);
					bFind = TRUE;
					break;
				}
			}
			else//ָ���˴���
			{
				if(iWinId == data.GetWindowId())
				{
					rdata = data;
					m_lstInlineQue.RemoveAt(posLast);
					bFind = TRUE;
					break;
				}
			}
		}
		*/
		bFind = GetFirstTakeNumData(rdata,arrStrQueId);
		if(bFind)
			bFind = RemoveFirstTakeNumData(rdata);
	}
	CTime currTime = CTime::GetCurrentTime();
	rdata.SetCallTime(currTime);//���ú���ʱ��
	rdata.SetStaffId(staffID);//����Ա��ID,�ĸ�Ա������
	rdata.SetWindowId(iWinId);//�����ĸ����ں���
	rdata.SetWindowShowId(Window.GetShowWndId());//���ô��ڱ��
	rdata.SetWndLefNum(GetCandoQueCount(iWinId));
	m_mtInlineQue.Unlock();
	return bFind;
}

void CInlineQueData::GetCandoQueHaveData(CStringArray& queIDArray,const CStringArray& canDoQueIDArray,UINT uWndID)
{
	int count = canDoQueIDArray.GetCount();
	for(int i = 0; i < count; i++)
	{
		CString candoQueID = canDoQueIDArray[i];
		POSITION pos = m_lstInlineQue.GetHeadPosition();
		while(pos)
		{
			SLZData data;
			POSITION posLast = pos;
			data = m_lstInlineQue.GetNext(pos);
			if(data.GetWindowId()==0)//���û����ָ������
			{
				if(candoQueID.Compare(data.GetBussinessType()) == 0)
				{
					queIDArray.Add(candoQueID);
					break;
				}
			}
			else//������ָ������
			{
				if(uWndID == data.GetWindowId())
				{
					queIDArray.Add(data.GetBussinessType());
					break;
				}
			}
		}
	}
}


void CInlineQueData::Add(SLZData& data)
{
	m_mtInlineQue.Lock();
	m_lstInlineQue.AddTail(data);
	m_mtInlineQue.Unlock();
}

void CInlineQueData::AddHeadData(const SLZData& data)
{
	m_mtInlineQue.Lock();
	m_lstInlineQue.AddHead(data);
	m_mtInlineQue.Unlock();
}

UINT CInlineQueData::GetCount()
{
	return m_lstInlineQue.GetCount();
}

UINT CInlineQueData::GetBussCount(const CString& strBussId)
{
	m_mtInlineQue.Lock();
	UINT iCount = 0;
	POSITION pos = m_lstInlineQue.GetHeadPosition();
	SLZData data;
	for(; pos; )
	{
		data = m_lstInlineQue.GetNext(pos);
		CString queID = data.GetBussinessType();
		if(queID == strBussId)
		{
			//ֻ���ر���������
// 			if(data.GetIsClientData() && theApp.m_logicVariables.IsOpenInterNum)
// 			{
// 				if(theApp.m_logicVariables.strInterIP[0] == '\0')//����
// 					continue;
// 			}
			iCount++;
		}
	}
	m_mtInlineQue.Unlock();
	return iCount;
}

// CInlineQueData ��Ա����

// BOOL CInlineQueData::GetInlineQueData(SLZData& rdata)
// {
// 	m_mtInlineQue.Lock();
// 	if(!m_lstInlineQue.GetCount())
// 	{
// 		m_mtInlineQue.Unlock();
// 		return FALSE;
// 	}
// 	if(theApp.m_logicVariables.IsOpenInterNum && 
// 		theApp.m_logicVariables.strInterIP[0] == '\0')//����
// 	{
// 		do
// 		{
// 			rdata = m_lstInlineQue.GetHead();
// 			m_lstInlineQue.RemoveHead();
// 		}while(rdata.GetIsClientData());
// 	}
// 	else if(theApp.m_logicVariables.IsOpenInterNum && 
// 		theApp.m_logicVariables.strInterIP[0] != '\0')//�ͻ���
// 	{
// 		do
// 		{
// 			rdata = m_lstInlineQue.GetHead();
// 			m_lstInlineQue.RemoveHead();
// 		}while(!rdata.GetIsClientData());
// 	}
// 	else
// 	{
// 		rdata = m_lstInlineQue.GetHead();
// 		m_lstInlineQue.RemoveHead();
// 	}
// 	m_mtInlineQue.Unlock();
// 	return TRUE;
// }

void CInlineQueData::RemoveAllData()
{
	m_mtInlineQue.Lock();
	m_lstInlineQue.RemoveAll();
	m_mtInlineQue.Unlock();
}

int CInlineQueData::GetMaxQueNum(const CString queID)
{
	m_mtInlineQue.Lock();
	int maxNum = 0;
	POSITION pos = m_lstInlineQue.GetHeadPosition();
	while(pos)
	{
		SLZData data = m_lstInlineQue.GetNext(pos);
		if(data.GetBussinessType() == queID)
		{
			int queNum = data.GetIntQueNum();
			maxNum = maxNum > queNum ? maxNum : queNum;
		}
	}
// 	CString num;
// 	if(!maxNum.IsEmpty())
// 	{
// 		for(int i=0;i<maxNum.GetLength();i++)
// 		{
// 			WCHAR w = maxNum.GetAt(i);
// 			if(w>'0'&&w<'9')
// 			{
// 				num+=w;
// 			}
// 		}
// 	}
	m_mtInlineQue.Unlock();
// 	int iNum = 0;
// 	CCommonConvert convert;
// 	convert.CStringToint(iNum,num);
	return maxNum;
}

UINT CInlineQueData::GetCandoQueCount(UINT iWinID)
{
	UINT uWaitCount = 0;
	SLZWindow Window;
	BOOL flag = m_rWindowTable.QueryWindowById(iWinID,Window);
	ASSERT(flag != FALSE);
	CStringArray arrayQueID;
	Window.GetArrayQueId(arrayQueID);
	for(int i=0;i<arrayQueID.GetCount();i++)
	{
		CString queID = arrayQueID.GetAt(i);
		ASSERT(!queID.IsEmpty());
		uWaitCount += GetBussCount(queID);
	}
	return uWaitCount;
}

BOOL CInlineQueData::AlterData(const CString QueNum,const CString strPhoneNum,const CString strMsg)
{
	m_mtInlineQue.Lock();
	int iCount = m_lstInlineQue.GetCount();
	for (int i = 0;i<iCount;i++)
	{
		SLZData data;
		POSITION pos = m_lstInlineQue.FindIndex(i);
		if (pos)
		{
			data = m_lstInlineQue.GetAt(pos);
			if (data.GetQueueNumber()==QueNum)
			{
				data.SetPhoneNum(strPhoneNum);
				data.SetSendMsg(strMsg);
				m_lstInlineQue.SetAt(pos,data);
				m_mtInlineQue.Unlock();
				return TRUE;
			}
		}
	}
	m_mtInlineQue.Unlock();
	return FALSE;
}

BOOL CInlineQueData::GetInlineQueData(int i,SLZData& data)
{
	if(!GetCount() || i<0)return FALSE;
	m_mtInlineQue.Lock();
	POSITION pos = m_lstInlineQue.FindIndex(i);
	if (pos)
	{
		data = m_lstInlineQue.GetAt(pos);
		m_mtInlineQue.Unlock();
		return TRUE;
	}
	m_mtInlineQue.Unlock();
	return FALSE;
}

// BOOL CInlineQueData::GetLastInlineQueWaitNum(const CString& strBussid,UINT* pWaitNum)
// {
// 	BOOL flag = FALSE;
// 	m_mtInlineQue.Lock();
// 	POSITION pos = m_lstInlineQue.GetTailPosition();
// 	for(;pos;)
// 	{
// 		SLZData data = m_lstInlineQue.GetPrev(pos);
// 		if(data.GetBussinessType() == strBussid)
// 		{
// 			*pWaitNum = data.GetCurrWaitNum();
// 			flag = TRUE;
// 			break;
// 		}
// 	}
// 	m_mtInlineQue.Unlock();
// 	return flag;
// }

// BOOL CInlineQueData::GetLastInlineQueWaitNumSub(const CString& strBussid,UINT* pWaitNum)
// {
// 	BOOL flag = FALSE;
// 	m_mtInlineQue.Lock();
// 	POSITION pos = m_lstInlineQue.GetTailPosition();
// 	POSITION pospre;
// 	for(;pos;)
// 	{
// 		pospre = pos;
// 		SLZData data = m_lstInlineQue.GetPrev(pos);
// 		if(data.GetBussinessType() == strBussid)
// 		{
// 			*pWaitNum = data.GetCurrWaitNum();
// 			*pWaitNum--;
// 			data.SetCurrWaitNum(*pWaitNum);
// 			m_lstInlineQue.SetAt(pospre,data);
// 			flag = TRUE;
// 			break;
// 		}
// 	}
// 	m_mtInlineQue.Unlock();
// 	return flag;
// }

void CInlineQueData::GetAllBussCount(const CString& strBussid,UINT* pWaitNum)
{
	m_mtInlineQue.Lock();
	UINT iCount = 0;
	POSITION pos = m_lstInlineQue.GetHeadPosition();
	SLZData data;
	for(; pos; )
	{
		data = m_lstInlineQue.GetNext(pos);
		CString queID = data.GetBussinessType();

		if(queID == strBussid)
		{
			//���ض���������
			iCount++;
		}
	}
	m_mtInlineQue.Unlock();
	*pWaitNum = iCount;
}

BOOL CInlineQueData::DeleteInlineClientData(BOOL bIsUsePower,const CStringArray& queIDArray,const CString& organId,SLZData* pData)
{
	m_mtInlineQue.Lock();
	BOOL flag = FALSE;
	if(bIsUsePower)
	{
		if(queIDArray.GetCount() < 1)
		{
			return FALSE;
		}
		
		int count = queIDArray.GetCount();
		for(int i = 0; i < count; i++)
		{
			POSITION pos = m_lstInlineQue.GetHeadPosition();
			while(pos)
			{
				SLZData data;
				POSITION posLast = pos;
				data = m_lstInlineQue.GetNext(pos);
				if(data.GetWindowId()==0)//���û����ָ������
				{
					if(queIDArray[i] == data.GetBussinessType())
					{
						flag = TRUE;
						*pData = data;
						m_lstInlineQue.RemoveAt(posLast);
						break;
					}
				}
			}
			if(flag)
			{
				break;
			}
		}
	}
	else
	{
		flag = GetFirstTakeNumData(*pData,queIDArray);
		if(flag)
			flag = RemoveFirstTakeNumData(*pData);
	}
	m_mtInlineQue.Unlock();
	return flag;
}

BOOL CInlineQueData::GetWindowCanDoQue(UINT nWindowID,CStringArray& queerial_id_array,CString& callStaffID,BOOL* pIsUsePower)
{
// 	if(m_lstInlineQue.GetCount() < 1)
// 	{
// 		return FALSE;
// 	}
	CStringArray arrStrQueId;//�ɴ�������Լ����ȼ�
	SLZWindow Window;
	BOOL flag = m_rWindowTable.QueryWindowById(nWindowID,Window);
	if(!flag)return FALSE;
	Window.GetArrayQueId(arrStrQueId);//��ȡ�ɴ�����м����ȼ�
	CString staffID = theApp.m_Controller.m_mapLoginList[Window.GetWindowId()];//��ȡ��¼STAFFID
	if(staffID.IsEmpty())return FALSE;
	if(arrStrQueId.GetCount() < 1)
	{
		return FALSE;
	}
	queerial_id_array.Copy(arrStrQueId);
	callStaffID = staffID;
	*pIsUsePower = Window.GetIsUsePower();
	return TRUE;
}

BOOL CInlineQueData::GetFirstTakeNumData(SLZData& data,const CStringArray& arrStrQueId)
{
	BOOL canDo = FALSE;
	
	POSITION pos = m_lstInlineQue.GetHeadPosition();
	SLZData tempdata,lastdata;
	while(pos)
	{
		tempdata = m_lstInlineQue.GetNext(pos);
		for(int i=0;i<arrStrQueId.GetCount();i++)
		{
			if(arrStrQueId[i] == tempdata.GetBussinessType())
			{
				canDo = TRUE;
				break;
			}
		}
		if(canDo)
		{
	
			lastdata = data;
			data = tempdata;
			
		
			if(!lastdata.GetBussinessType().IsEmpty())
				data = data.GetTakingNumTime() < lastdata.GetTakingNumTime() ? data : lastdata;
		}
	}
	return canDo;
}

BOOL CInlineQueData::RemoveFirstTakeNumData(const SLZData& data)
{
	BOOL flag = FALSE;
	POSITION pos = m_lstInlineQue.GetHeadPosition();
	POSITION poslast;
	SLZData tempdata;
	while(pos)
	{
		poslast = pos;
		tempdata = m_lstInlineQue.GetNext(pos);
		if(tempdata.GetSerialId() == data.GetSerialId())
		{
			flag = TRUE;
			m_lstInlineQue.RemoveAt(poslast);
			break;
		}
	}
	return flag;
}