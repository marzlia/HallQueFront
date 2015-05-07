#include "StdAfx.h"
#include "ThroughWndScreenInfo.h"

UINT g_throughWndScreenId;
CThroughWndScreenInfo::CThroughWndScreenInfo(void) : m_nPhyId(0)
,m_nPipeId(0)
,m_nScreenId(0)
,m_nComScreenId(0)
{
	g_throughWndScreenId++;
	m_nThroughWndScreenId = g_throughWndScreenId;
}

CThroughWndScreenInfo::~CThroughWndScreenInfo(void)
{
}

CThroughWndScreenInfo::CThroughWndScreenInfo(const CThroughWndScreenInfo& obj)
{
	m_nThroughWndScreenId = obj.m_nThroughWndScreenId;
	m_strLocalIP = obj.m_strLocalIP;
	m_nPhyId = obj.m_nPhyId;
	m_nPipeId = obj.m_nPipeId;
	m_nComScreenId = obj.m_nComScreenId;
	m_nScreenId = obj.m_nScreenId;
}

CThroughWndScreenInfo& CThroughWndScreenInfo::operator =(const CThroughWndScreenInfo& obj)
{
	if(&obj == this)return *this;

	m_nThroughWndScreenId = obj.m_nThroughWndScreenId;
	m_strLocalIP = obj.m_strLocalIP;
	m_nPhyId = obj.m_nPhyId;
	m_nPipeId = obj.m_nPipeId;
	m_nComScreenId = obj.m_nComScreenId;
	m_nScreenId = obj.m_nScreenId;

	return *this;
}

void CThroughWndScreenInfo::SetLocalIP(const CString& ip)
{
	m_strLocalIP = ip;
}

CString CThroughWndScreenInfo::GetLocalIp() const
{
	return m_strLocalIP;
}

void CThroughWndScreenInfo::SetPhyId(int nPhyId)
{
	m_nPhyId = nPhyId;
}

int CThroughWndScreenInfo::GetPhyId() const
{
	return m_nPhyId;
}

void CThroughWndScreenInfo::SetPipeId(int nPipeId)
{
	m_nPipeId = nPipeId;
}

int CThroughWndScreenInfo::GetPipeId() const
{
	return m_nPipeId;
}

int CThroughWndScreenInfo::GetThroughWndScreenId() const
{
	return m_nThroughWndScreenId;
}

void CThroughWndScreenInfo::SetThroughWndScreenId(UINT nWndScreenId)
{
	m_nThroughWndScreenId = nWndScreenId;
}

int CThroughWndScreenInfo::GetWndScreenId() const
{
	return m_nScreenId;
}

void CThroughWndScreenInfo::SetWndScreenId(int nScreenId)
{
	m_nScreenId = nScreenId;
}

int CThroughWndScreenInfo::GetComScreenId() const
{
	return m_nComScreenId;
}

void CThroughWndScreenInfo::SetComScreenId(int nComScreenId)
{
	m_nComScreenId = nComScreenId;
}