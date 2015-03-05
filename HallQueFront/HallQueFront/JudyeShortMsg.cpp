#include "StdAfx.h"
#include "JudyeShortMsg.h"

UINT g_JudgeShortMsgSerialID = 0;
IMPLEMENT_SERIAL(CJudgeShortMsg,CObject,1);

CJudgeShortMsg::CJudgeShortMsg(void)
{
	g_JudgeShortMsgSerialID++;
	m_shortMsgSerialID = g_JudgeShortMsgSerialID;
}

CJudgeShortMsg::~CJudgeShortMsg(void)
{
}

CJudgeShortMsg::CJudgeShortMsg(const CJudgeShortMsg& obj)
{
	m_shortMsgSerialID = obj.m_shortMsgSerialID;
	m_strPhoneNum = obj.m_strPhoneNum;
	m_strShortMsg = obj.m_strShortMsg;
}

CJudgeShortMsg& CJudgeShortMsg::operator = (const CJudgeShortMsg& obj)
{
	if(this == &obj)
	{
		return *this;
	}
	m_shortMsgSerialID = obj.m_shortMsgSerialID;
	m_strPhoneNum = obj.m_strPhoneNum;
	m_strShortMsg = obj.m_strShortMsg;
	return *this;
}

void CJudgeShortMsg::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar<<m_shortMsgSerialID<<m_strPhoneNum<<m_strShortMsg;
	}
	else
	{
		ar>>m_shortMsgSerialID>>m_strPhoneNum>>m_strShortMsg;
	}
}
