#include "StdAfx.h"
#include "HallQueFront.h"
#include "SLZWindow.h"
#include "SLZStaff.h"

unsigned int golbal_windowserial_id;
IMPLEMENT_SERIAL(SLZWindow,CObject,1)
SLZWindow::SLZWindow(void) : m_iCallerId(0)
, m_iEvaluatorId(0)
, m_iWndScreenId(0)
, m_iComScreenId(0)
, m_iMsgShowTime(30)
, m_iEvaTimeOut(15)
, m_iLEDPhyId(0)
, m_iShowWndId(0)
, m_iLEDPipeId(0)
{
	
	m_ShowMsg = _T("#0#请#2#[排队号码]#0#号#2#[客户姓名]#0#到#2#[窗口名称]#0#办理业务#0#   Please No.#2#[排队号码(英)]#0# to the #2#[窗口名称(英)]#0# for business.");
	m_CalledMsg = _T("#请#[排队号码]#号到#[呼叫名称]#办理业务#please#number#[排队号码(英)]#goto#[呼叫名称(英)]#for#business");
	m_WaitShowMsg = _T("#0#请#2#[排队号码]#0#号#2#[客户姓名]#0#做好准备#0#   Please No.#2#[排队号码]#0#  get ready.");
	m_WaitCalledMsg = _T("#请#[排队号码]#号#做好准备#please#number#[排队号码(英)]#get#ready");
}

SLZWindow::~SLZWindow()
{
}

SLZWindow::SLZWindow(const SLZWindow& obj)
{
	m_iShowWndId = obj.m_iShowWndId;
	m_iWindowId=obj.m_iWindowId;
	m_strWindowName=obj.m_strWindowName;
	m_strWindowCallName=obj.m_strWindowCallName;
	m_iCallerId=obj.m_iCallerId;
	m_iEvaluatorId=obj.m_iEvaluatorId;
	m_iWndScreenId=obj.m_iWndScreenId;
	m_iComScreenId=obj.m_iComScreenId;
	//m_StrCurStaffName=obj.m_StrCurStaffName;
	//m_StrDefStaffName=obj.m_StrDefStaffName;
	//m_staffLoginId=obj.m_staffLoginId;
	m_staffDefaultId=obj.m_staffDefaultId;
	//m_ArraySize=obj.m_ArraySize;
	//m_AbleQueName=obj.m_AbleQueName;
	//m_strAbleQueId=obj.m_strAbleQueId;
	m_arrBussId.Copy(obj.m_arrBussId);
	m_ArraySize = m_arrBussId.GetCount();
	m_CalledMsg=obj.m_CalledMsg;
	m_WaitCalledMsg=obj.m_WaitCalledMsg;
	m_ShowMsg=obj.m_ShowMsg;
	m_WaitShowMsg=obj.m_WaitShowMsg;
	//m_FlagMustEval=obj.m_FlagMustEval;
	m_iEvaTimeOut = obj.m_iEvaTimeOut;
	m_iMsgShowTime = obj.m_iMsgShowTime;
	m_iLEDPhyId=obj.m_iLEDPhyId;
	m_strLEDIPId=obj.m_strLEDIPId;
	m_iLEDPipeId = obj.m_iLEDPipeId;
	m_strAdMsg = obj.m_strAdMsg;
}

SLZWindow& SLZWindow::operator =(const SLZWindow& obj)
{
	m_iShowWndId = obj.m_iShowWndId;
	m_iWindowId=obj.m_iWindowId;
	m_strWindowName=obj.m_strWindowName;
	m_strWindowCallName=obj.m_strWindowCallName;
	m_iCallerId=obj.m_iCallerId;
	m_iEvaluatorId=obj.m_iEvaluatorId;
	m_iWndScreenId=obj.m_iWndScreenId;
	m_iComScreenId=obj.m_iComScreenId;
	//m_StrCurStaffName=obj.m_StrCurStaffName;
	//m_StrDefStaffName=obj.m_StrDefStaffName;
	//m_staffLoginId=obj.m_staffLoginId;
	m_staffDefaultId=obj.m_staffDefaultId;
	//m_ArraySize=obj.m_ArraySize;
	//m_AbleQueName=obj.m_AbleQueName;
	//m_strAbleQueId=obj.m_strAbleQueId;
	m_arrBussId.Copy(obj.m_arrBussId);
	m_ArraySize = m_arrBussId.GetCount();
	m_CalledMsg=obj.m_CalledMsg;
	m_WaitCalledMsg=obj.m_WaitCalledMsg;
	m_ShowMsg=obj.m_ShowMsg;
	m_WaitShowMsg=obj.m_WaitShowMsg;
	//m_FlagMustEval=obj.m_FlagMustEval;
	m_iEvaTimeOut = obj.m_iEvaTimeOut;
	m_iMsgShowTime = obj.m_iMsgShowTime;
	m_iLEDPhyId=obj.m_iLEDPhyId;
	m_strLEDIPId=obj.m_strLEDIPId;
	m_iLEDPipeId = obj.m_iLEDPipeId;
	m_strAdMsg = obj.m_strAdMsg;
	return *this;
}


void SLZWindow::Serialize( CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_iWindowId << m_strWindowName << m_strWindowCallName << m_iCallerId 
			<< m_iEvaluatorId << m_iWndScreenId << m_iComScreenId << m_strLEDIPId 
			<< m_iLEDPhyId << m_iLEDPipeId << m_strStbId << m_staffDefaultId << m_CalledMsg << m_WaitCalledMsg
			<< m_ShowMsg << m_WaitShowMsg << m_iMsgShowTime << m_strAdMsg 
			<< m_iEvaTimeOut<<m_iShowWndId;
		m_ArraySize = m_arrBussId.GetCount();
		ar << m_ArraySize;
		for(int i = 0; i < m_ArraySize; i++)
		{
			ar << m_arrBussId[i];
		}
	}
	else
	{
		ar >> m_iWindowId >> m_strWindowName >> m_strWindowCallName >> m_iCallerId 
			>> m_iEvaluatorId >> m_iWndScreenId >> m_iComScreenId >> m_strLEDIPId 
			>> m_iLEDPhyId >> m_iLEDPipeId >> m_strStbId >> m_staffDefaultId >> m_CalledMsg >> m_WaitCalledMsg
			>> m_ShowMsg >> m_WaitShowMsg >> m_iMsgShowTime >> m_strAdMsg 
			>> m_iEvaTimeOut>>m_iShowWndId;
		//m_ArraySize = m_arrBussId.GetCount();
		ar >> m_ArraySize;
		for(int i = 0; i < m_ArraySize; i++)
		{
			CString strQueId;
			ar >> strQueId;
			m_arrBussId.Add(strQueId);
		}
	}
}
