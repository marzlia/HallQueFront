#include "StdAfx.h"
#include "EvaCmdData.h"

CEvaCmdData::CEvaCmdData(void)
{
}

CEvaCmdData::~CEvaCmdData(void)
{
}

void CEvaCmdData::Add(const SLZEvaData& data)
{
	m_mtEvaData.Lock();
	m_list_evaData.push_back(data);
	m_mtEvaData.Unlock();
}

BOOL CEvaCmdData::GetHeadEvaData(SLZEvaData& data)
{
	if(m_list_evaData.size()==0)return FALSE;
	m_mtEvaData.Lock();
	data = *m_list_evaData.begin();
	m_list_evaData.pop_front();
//	m_list_evaData.erase(m_list_evaData.begin());
	m_mtEvaData.Unlock();
	return TRUE;
}

int CEvaCmdData::GetEvaListCount()
{
	return m_list_evaData.size();
}
