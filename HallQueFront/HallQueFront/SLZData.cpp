#include "StdAfx.h"
#include "SLZData.h"


IMPLEMENT_SERIAL(SLZData,CObject,1);

SLZData::SLZData(void) : m_iCusLevel(0)
, m_iWindowId(0)
, m_bIsFinshEva(FALSE)
, m_bIsOpenEva(FALSE)
, m_EvaLevel(evaNone)
, m_CardType(cardNotDefine)
, m_iWindowShowId(0)
{
	m_strSerialId = GetOnlyId();
}

SLZData::~SLZData(void)
{
}

SLZData::SLZData(const SLZData& obj)
{
	m_bIsFinshEva = obj.m_bIsFinshEva;
	m_bIsOpenEva = obj.m_bIsOpenEva;
	m_EvaLevel = obj.m_EvaLevel;
	m_iCusLevel = obj.m_iCusLevel;
	m_iWindowId = obj.m_iWindowId;
	m_strSerialId = obj.m_strSerialId;
	m_strOrganId = obj.m_strOrganId;
	m_strOrganName = obj.m_strOrganName;
	m_strBType = obj.m_strBType;
	m_strBussName = obj.m_strBussName;
	m_strQueueNum = obj.m_strQueueNum;
	m_CardType = obj.m_CardType;
	m_strCardNum = obj.m_strCardNum;
	m_CustName = obj.m_CustName;
	m_timeTakingNum = obj.m_timeTakingNum;
	m_strStaffId = obj.m_strStaffId;
	m_timeCall = obj.m_timeCall;
	m_timeFinish = obj.m_timeFinish;
	m_iWindowShowId = obj.m_iWindowShowId;
	m_queSerialId = obj.m_queSerialId;
	m_iQueNum = obj.m_iQueNum;
	m_strPhoneNum = obj.m_strPhoneNum;
	m_strMsg = obj.m_strMsg;
	m_leftNum = obj.m_leftNum;
	m_custLev = obj.m_custLev;
}

SLZData& SLZData::operator=(const SLZData& obj)
{
	if(this==&obj)return *this;//×Ô¸³Öµ
	m_bIsFinshEva = obj.m_bIsFinshEva;
	m_bIsOpenEva = obj.m_bIsOpenEva;
	m_EvaLevel = obj.m_EvaLevel;
	m_iCusLevel = obj.m_iCusLevel;
	m_iWindowId = obj.m_iWindowId;
	m_strSerialId = obj.m_strSerialId;
	m_strOrganId = obj.m_strOrganId;
	m_strOrganName = obj.m_strOrganName;
	m_strBType = obj.m_strBType;
	m_strBussName = obj.m_strBussName;
	m_strQueueNum = obj.m_strQueueNum;
	m_CardType = obj.m_CardType;
	m_strCardNum = obj.m_strCardNum;
	m_CustName = obj.m_CustName;
	m_timeTakingNum = obj.m_timeTakingNum;
	m_strStaffId = obj.m_strStaffId;
	m_timeCall = obj.m_timeCall;
	m_timeFinish = obj.m_timeFinish;
	m_iWindowShowId = obj.m_iWindowShowId;
	m_queSerialId = obj.m_queSerialId;
	m_iQueNum = obj.m_iQueNum;
	m_strPhoneNum = obj.m_strPhoneNum;
	m_strMsg = obj.m_strMsg;
	m_leftNum = obj.m_leftNum;
	m_custLev = obj.m_custLev;
	return *this;
}

CString SLZData::GetOnlyId()
{
	CString m_id;
	CTime now=CTime::GetCurrentTime();
	static int nCount;
	m_id.Format(_T("%04d%02d%02d%02d%02d%02d%04d%04d"),
		now.GetYear(),now.GetMonth(),now.GetDay(),now.GetHour(),
		now.GetSecond(),now.GetMinute(),GetSrand(9999),++nCount);
	return m_id;
}

int SLZData::GetSrand(const int &max)
{
	time_t t;	
	srand((unsigned) time(&t));	
	return	rand() % max;
}

void SLZData::Serialize(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar<<m_strSerialId<<m_strOrganId<<m_strOrganName<<m_strBType
			<<m_strBussName<<m_strQueueNum<<(DWORD)m_CardType<<m_strCardNum<<m_CustName
			<<m_iCusLevel<<m_timeTakingNum<<m_iWindowId<<m_strStaffId<<m_timeCall<<m_timeFinish
			<<(DWORD)m_EvaLevel<<m_bIsOpenEva<<m_bIsFinshEva<<m_iWindowShowId<<m_queSerialId<<m_iQueNum
			<<m_strPhoneNum<<m_strMsg;
	}
	else
	{
		DWORD cardType=0,evaLevel=0;
		ar>>m_strSerialId>>m_strOrganId>>m_strOrganName>>m_strBType
			>>m_strBussName>>m_strQueueNum>>cardType>>m_strCardNum>>m_CustName
			>>m_iCusLevel>>m_timeTakingNum>>m_iWindowId>>m_strStaffId>>m_timeCall>>m_timeFinish
			>>evaLevel>>m_bIsOpenEva>>m_bIsFinshEva>>m_iWindowShowId>>m_queSerialId>>m_iQueNum>>m_strPhoneNum
			>>m_strMsg;
		m_EvaLevel = (EvaLevel)evaLevel;
		m_CardType = (CardType)cardType;
	}
}