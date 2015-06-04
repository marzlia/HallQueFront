#include "StdAfx.h"
#include "DealInterMsg.h"
#include "SLZData.h"
#include "HallQueFront.h"

CDealInterMsg::CDealInterMsg(void)
{
}

CDealInterMsg::~CDealInterMsg(void)
{
}

void CDealInterMsg::ProduceRetInterMsg(const SLZData* pData, UINT inlineNum,string& retMsg)
{
	if(!pData)return;
	CString msg = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\">");
	msg.Append(_T("<headCode>retInterMsg</headCode>"));
	//数据基本信息
	msg.AppendFormat(_T("<serialId>%s</serialId>"),pData->GetSerialId());
	msg.AppendFormat(_T("<bussName>%s</bussName>"),pData->GetBussName());
	msg.AppendFormat(_T("<queId>%s</queId>"),pData->GetBussinessType());
	msg.AppendFormat(_T("<queManNum>%s</queManNum>"),pData->GetQueSerialID());
	msg.AppendFormat(_T("<queNum>%s</queNum>"),pData->GetQueueNumber());
	msg.AppendFormat(_T("<intQueNum>%d</intQueNum>"),pData->GetIntQueNum());
	msg.AppendFormat(_T("<cardType>%d</cardType>"),pData->GetCardType());
	msg.AppendFormat(_T("<cardNum>%s</cardNum>"),pData->GetCardNumber());
	msg.AppendFormat(_T("<custName>%s</custName>"),pData->GetCustName());
	msg.AppendFormat(_T("<custLevel>%d</custLevel>"),pData->GetCustomerLevel());
	
	CTime takeNumTime = pData->GetTakingNumTime();
	CString strTime;
	strTime.Format(_T("%d-%d-%d %d:%d:%d"),takeNumTime.GetYear(),takeNumTime.GetMonth(),takeNumTime.GetDay(),
		takeNumTime.GetHour(),takeNumTime.GetMinute(),takeNumTime.GetSecond());

	msg.AppendFormat(_T("<takeNumTime>%s</takeNumTime>"),strTime);
	msg.AppendFormat(_T("<staffId>%s</staffId>"),pData->GetStaffId());
	msg.AppendFormat(_T("<phoneNum>%s</phoneNum>"),pData->GetPhoneNum());
	msg.AppendFormat(_T("<sendMsg>%s</sendMsg>"),pData->GetSendMsg());

	msg.AppendFormat(_T("<inlineNum>%d</inlineNum>"),inlineNum);
	msg.Append(_T("</dataPacket>"));

	CStringA a_retPacket(msg.GetBuffer(0));
	msg.ReleaseBuffer(0);
	retMsg = a_retPacket.GetBuffer(0);
	a_retPacket.ReleaseBuffer(0);
}

void CDealInterMsg::ProduceSendInNumMsg(const CString& queManNum,string& retInlineNumMsg)
{
	CString msg = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\">");
	msg.Append(_T("<headCode>sendInnumMsg</headCode>"));
	msg.AppendFormat(_T("<queManNum>%s</queManNum>"),queManNum);
	msg.Append(_T("</dataPacket>"));

	CStringA a_retPacket(msg.GetBuffer(0));
	msg.ReleaseBuffer(0);
	retInlineNumMsg = a_retPacket.GetBuffer(0);
	a_retPacket.ReleaseBuffer(0);
}

void CDealInterMsg::ProduceRetInNumMsg(UINT inlineNum,string& retInlineMsg)
{
	CString msg = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\">");
	msg.Append(_T("<headCode>retInnumMsg</headCode>"));
	msg.AppendFormat(_T("<inlineNum>%d</inlineNum>"),inlineNum);
	msg.Append(_T("</dataPacket>"));

	CStringA a_retPacket(msg.GetBuffer(0));
	msg.ReleaseBuffer(0);
	retInlineMsg = a_retPacket.GetBuffer(0);
	a_retPacket.ReleaseBuffer(0);
}

void CDealInterMsg::ProduceSendInterMsg(const CString& queManNum,string& retSendMsg)
{
	CString msg = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\">");
	msg.Append(_T("<headCode>sendInterMsg</headCode>"));
	msg.AppendFormat(_T("<queManNum>%s</queManNum>"),queManNum);
	msg.AppendFormat(_T("<organId>%s</organId>"),theApp.m_logicVariables.strOrganID);
	msg.AppendFormat(_T("<organName>%s</organName>"),theApp.m_logicVariables.strOrganNmae);
	msg.Append(_T("</dataPacket>"));

	CStringA a_retPacket(msg.GetBuffer(0));
	msg.ReleaseBuffer(0);
	retSendMsg = a_retPacket.GetBuffer(0);
	a_retPacket.ReleaseBuffer(0);
}

BOOL CDealInterMsg::AnaRetInNumMsg(const string& retMsg,UINT* pInlineNum)
{
	string::size_type pos1 = retMsg.find("<headCode>");
	string::size_type pos2 = retMsg.find("</headCode>");
	if(pos1 == retMsg.npos || pos2 == retMsg.npos)
		return FALSE;
	string headCode = retMsg.substr(pos1 + strlen("<headCode>"),pos2 - pos1 - strlen("<headCode>"));
	if(headCode != "retInnumMsg")
		return FALSE;
	pos1 = retMsg.find("<inlineNum>");
	pos2 = retMsg.find("</inlineNum>");
	if(pos1 == retMsg.npos || pos2 == retMsg.npos)
		return FALSE;
	string inlineNum = retMsg.substr(pos1 + strlen("<inlineNum>"),pos2 - pos1 - strlen("<inlineNum>"));
	*pInlineNum = atoi(const_cast<char*>(inlineNum.c_str()));
	return TRUE;
}

BOOL CDealInterMsg::AnaRetInterMsg(const string& retMsg,SLZData* pData,UINT* pInLineNum)
{
	string::size_type pos1 = retMsg.find("<headCode>");
	string::size_type pos2 = retMsg.find("</headCode>");
	if(pos1 == retMsg.npos || pos2 == retMsg.npos)
		return FALSE;
	string headCode = retMsg.substr(pos1 + strlen("<headCode>"),pos2 - pos1 - strlen("<headCode>"));
	if(headCode != "retInterMsg")
		return FALSE;

	
	string serialId;
	GetMsgData(retMsg,enumSlzDataSerialid,serialId);
	CString wStrSerialId(serialId.c_str());
	pData->SetSerialId(wStrSerialId);


	

	string bussName;
	GetMsgData(retMsg,enumSlzDatabussName,bussName);
	CString wBussName(bussName.c_str());
	pData->SetBussName(wBussName);


	string queId;
	GetMsgData(retMsg,enumSlzDataqueId,queId);
	CString wQueId(queId.c_str());
	pData->SetBussinessType(wQueId);


	string queManNum;
	GetMsgData(retMsg,enumSlzDataqueManNum,queManNum);
	CString wQueManNum(queManNum.c_str());
	pData->SetQueSerialID(wQueManNum);


	string queNum;
	GetMsgData(retMsg,enumSlzDataqueNum,queNum);
	CString wQueNum(queNum.c_str());
	pData->SetQueueNumber(wQueNum);


	string intQueNum;
	GetMsgData(retMsg,enumSlzDataintQueNum,intQueNum);
	pData->SetIntQueNum(atoi(const_cast<char*>(intQueNum.c_str())));
	

	string cardType;
	GetMsgData(retMsg,enumSlzDatacardType,cardType);
	pData->SetCardType((CardType)atoi(const_cast<char*>(cardType.c_str())));



	string cardNum;
	GetMsgData(retMsg,enumSlzDatacardNum,cardNum);
	CString wCardNum(cardNum.c_str());
	pData->SetCardNumber(wCardNum);


	string custName;
	GetMsgData(retMsg,enumSlzDatacustName,custName);
	CString wCustName(custName.c_str());
	pData->SetCustName(wCustName);



	string custLevel;
	GetMsgData(retMsg,enumSlzDatacustLevel,custLevel);
	pData->SetCustomerLevel(atoi(const_cast<char*>(custLevel.c_str())));


	string takeNumTime;
	GetMsgData(retMsg,enumSlzDatatakeNumTime,takeNumTime);
	CTime time;
	StringToTime(takeNumTime,time);
	pData->SetTakingNumTime(time);

	string staffId;
	GetMsgData(retMsg,enumSlzDatastaffId,staffId);
	CString wStaffId(staffId.c_str());
	pData->SetStaffId(wStaffId);

	string phoneNum;
	GetMsgData(retMsg,enumSlzDataphoneNum,phoneNum);
	CString wPhoneNum(phoneNum.c_str());
	pData->SetPhoneNum(wPhoneNum);


	string sendMsg;
	GetMsgData(retMsg,enumSlzDatasendMsg,phoneNum);
	CString wSendMsg(sendMsg.c_str());
	pData->SetSendMsg(wSendMsg);

	if(pInLineNum != NULL)
	{
		pos1 = retMsg.find("<inlineNum>");
		pos2 = retMsg.find("</inlineNum>");
		if(pos1 == retMsg.npos || pos2 == retMsg.npos)
			return FALSE;

		string inlineNum = retMsg.substr(pos1 + strlen("<inlineNum>"),pos2 - pos1 - strlen("<inlineNum>"));
		*pInLineNum = atoi(const_cast<char*>(inlineNum.c_str()));
	}
	return TRUE;
}

BOOL CDealInterMsg::AnaSendInNumMsg(const string& retSendMsg,CString& queManNum)
{
	string::size_type pos1 = retSendMsg.find("<headCode>");
	string::size_type pos2 = retSendMsg.find("</headCode>");
	if(pos1 == retSendMsg.npos || pos2 == retSendMsg.npos)
		return FALSE;
	
	string headCode = retSendMsg.substr(pos1 + strlen("<headCode>"),pos2 - pos1 - strlen("<headCode>"));
	if(headCode != "sendInnumMsg")
		return FALSE;

	pos1 = retSendMsg.find("<queManNum>");
	pos2 = retSendMsg.find("</queManNum>");
	if(pos1 == retSendMsg.npos || pos2 == retSendMsg.npos)
		return FALSE;

	string aQueManNum = retSendMsg.substr(pos1 + strlen("<queManNum>"),pos2 - pos1 - strlen("<queManNum>"));
	queManNum = aQueManNum.c_str();
	return TRUE;
}

BOOL CDealInterMsg::AnaSendInterMsg(const string& retSendMsg,CString& queManNum,CString& organId,CString& organName)
{
	string::size_type pos1 = retSendMsg.find("<headCode>");
	string::size_type pos2 = retSendMsg.find("</headCode>");
	if(pos1 == retSendMsg.npos || pos2 == retSendMsg.npos)
		return FALSE;

	string headCode = retSendMsg.substr(pos1 + strlen("<headCode>"),pos2 - pos1 - strlen("<headCode>"));
	if(headCode != "sendInterMsg")
		return FALSE;

	pos1 = retSendMsg.find("<queManNum>");
	pos2 = retSendMsg.find("</queManNum>");
	if(pos1 == retSendMsg.npos || pos2 == retSendMsg.npos)
		return FALSE;

	string aQueManNum = retSendMsg.substr(pos1 + strlen("<queManNum>"),pos2 - pos1 - strlen("<queManNum>"));
	queManNum = aQueManNum.c_str();

	pos1 = retSendMsg.find("<organId>");
	pos2 = retSendMsg.find("</organId>");
	if(pos1 == retSendMsg.npos || pos2 == retSendMsg.npos)
		return FALSE;

	string aOrganId = retSendMsg.substr(pos1 + strlen("<organId>"),pos2 - pos1 - strlen("<organId>"));
	organId = aOrganId.c_str();

	pos1 = retSendMsg.find("<organName>");
	pos2 = retSendMsg.find("</organName>");
	if(pos1 == retSendMsg.npos || pos2 == retSendMsg.npos)
		return FALSE;

	string aOrganName = retSendMsg.substr(pos1 + strlen("<organName>"),pos2 - pos1 - strlen("<organName>"));
	organName = aOrganName.c_str();
	return TRUE;
}

void CDealInterMsg::ProduceSendCallMsg(const CStringArray& queManNumArray,string& retCallMsg,const CString& organId,BOOL bIsUsePower)
{
	CString msg = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\">");
	msg.Append(_T("<headCode>sendCallMsg</headCode>"));
	msg.Append(_T("<queManNum>"));
	int count = queManNumArray.GetCount();
	for(int i=0;i<count;i++)
	{
		msg.AppendFormat(_T("%s"),queManNumArray.GetAt(i));
		if(i != count - 1)
		{
			msg.Append(_T(","));
		}
	}
	msg.Append(_T("</queManNum>"));
	msg.AppendFormat(_T("<usePower>%d</usePower>"),bIsUsePower);
	msg.AppendFormat(_T("<organId>%s</organId>"),organId);
	msg.Append(_T("</dataPacket>"));

	CStringA a_retPacket(msg.GetBuffer(0));
	msg.ReleaseBuffer(0);
	retCallMsg = a_retPacket.GetBuffer(0);
	a_retPacket.ReleaseBuffer(0);
}

BOOL CDealInterMsg::AnaSendCallMsg(const string& retSendMsg,CStringArray& queManNumArray,CString& organId,BOOL* pIsUsePower)
{
	string::size_type pos1 = retSendMsg.find("<headCode>");
	string::size_type pos2 = retSendMsg.find("</headCode>");
	if(pos1 == retSendMsg.npos || pos2 == retSendMsg.npos)
		return FALSE;

	string headCode = retSendMsg.substr(pos1 + strlen("<headCode>"),pos2 - pos1 - strlen("<headCode>"));
	if(headCode != "sendCallMsg")
		return FALSE;

	pos1 = retSendMsg.find("<queManNum>");
	pos2 = retSendMsg.find("</queManNum>");
	if(pos1 == retSendMsg.npos || pos2 == retSendMsg.npos)
		return FALSE;

	string aQueManNumArray = retSendMsg.substr(pos1 + strlen("<queManNum>"),pos2 - pos1 - strlen("<queManNum>"));
	string::size_type posTemp = 0;
	string::size_type posLast;
	for(posTemp;posTemp != aQueManNumArray.npos;)
	{
		posLast = posTemp;
		string aQueManNum;
		if(!posTemp)
		{
			posTemp = aQueManNumArray.find(",",posTemp);
			aQueManNum = aQueManNumArray.substr(posLast,posTemp);
		}
		else
		{
			posTemp = aQueManNumArray.find(",",posTemp + 1);
			aQueManNum = aQueManNumArray.substr(posLast + 1,posTemp);
		}
		CString wQueManNum(aQueManNum.c_str());
		queManNumArray.Add(wQueManNum);
	}
// 	string aQueManNum = aQueManNumArray.substr(posTemp,aQueManNumArray.npos);
// 	CString wQueManNum(aQueManNum.c_str());
// 	queManNumArray.Add(wQueManNum);

	
	pos1 = retSendMsg.find("<organId>");
	pos2 = retSendMsg.find("</organId>");
	if(pos1 == retSendMsg.npos || pos2 == retSendMsg.npos)
		return FALSE;

	string aOrganId = retSendMsg.substr(pos1 + strlen("<organId>"),pos2 - pos1 - strlen("<organId>"));
	organId = aOrganId.c_str();

	pos1 = retSendMsg.find("<usePower>");
	pos2 = retSendMsg.find("</usePower>");
	if(pos1 == retSendMsg.npos || pos2 == retSendMsg.npos)
		return FALSE;

	string aUsePower = retSendMsg.substr(pos1 + strlen("<usePower>"),pos2 - pos1 - strlen("<usePower>"));
	if(aUsePower == "0")
	{
		*pIsUsePower = FALSE;
	}
	else
	{
		*pIsUsePower = TRUE;
	}
	return TRUE;
}

void CDealInterMsg::ProduceRetCallMsg(BOOL isSucced,string& retMsg,const SLZData* pData)
{
	CString msg = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\">");
	msg.Append(_T("<headCode>retCallMsg</headCode>"));

	msg.AppendFormat(_T("<serialId>%s</serialId>"),pData->GetSerialId());
	msg.AppendFormat(_T("<bussName>%s</bussName>"),pData->GetBussName());
	msg.AppendFormat(_T("<queId>%s</queId>"),pData->GetBussinessType());
	msg.AppendFormat(_T("<queManNum>%s</queManNum>"),pData->GetQueSerialID());
	msg.AppendFormat(_T("<queNum>%s</queNum>"),pData->GetQueueNumber());
	msg.AppendFormat(_T("<intQueNum>%d</intQueNum>"),pData->GetIntQueNum());
	msg.AppendFormat(_T("<cardType>%d</cardType>"),pData->GetCardType());
	msg.AppendFormat(_T("<cardNum>%s</cardNum>"),pData->GetCardNumber());
	msg.AppendFormat(_T("<custName>%s</custName>"),pData->GetCustName());
	msg.AppendFormat(_T("<custLevel>%d</custLevel>"),pData->GetCustomerLevel());

	CTime takeNumTime = pData->GetTakingNumTime();
	CString strTime;
	strTime.Format(_T("%d-%d-%d %d:%d:%d"),takeNumTime.GetYear(),takeNumTime.GetMonth(),takeNumTime.GetDay(),
		takeNumTime.GetHour(),takeNumTime.GetMinute(),takeNumTime.GetSecond());

	msg.AppendFormat(_T("<takeNumTime>%s</takeNumTime>"),strTime);
	msg.AppendFormat(_T("<staffId>%s</staffId>"),pData->GetStaffId());
	msg.AppendFormat(_T("<phoneNum>%s</phoneNum>"),pData->GetPhoneNum());
	msg.AppendFormat(_T("<sendMsg>%s</sendMsg>"),pData->GetSendMsg());


	msg.AppendFormat(_T("<retCode>%d</retCode>"),isSucced);
	msg.Append(_T("</dataPacket>"));

	CStringA a_retPacket(msg.GetBuffer(0));
	msg.ReleaseBuffer(0);
	retMsg = a_retPacket.GetBuffer(0);
	a_retPacket.ReleaseBuffer(0);
}

BOOL CDealInterMsg::AnaRetCallMsg(const string& retMsg,BOOL* pIsSucced,SLZData* pData)
{
	string::size_type pos1 = retMsg.find("<headCode>");
	string::size_type pos2 = retMsg.find("</headCode>");
	if(pos1 == retMsg.npos || pos2 == retMsg.npos)
		return FALSE;

	string headCode = retMsg.substr(pos1 + strlen("<headCode>"),pos2 - pos1 - strlen("<headCode>"));
	if(headCode != "retCallMsg")
		return FALSE;


	string serialId;
	GetMsgData(retMsg,enumSlzDataSerialid,serialId);
	CString wStrSerialId(serialId.c_str());
	pData->SetSerialId(wStrSerialId);




	string bussName;
	GetMsgData(retMsg,enumSlzDatabussName,bussName);
	CString wBussName(bussName.c_str());
	pData->SetBussName(wBussName);


	string queId;
	GetMsgData(retMsg,enumSlzDataqueId,queId);
	CString wQueId(queId.c_str());
	pData->SetBussinessType(wQueId);


	string queManNum;
	GetMsgData(retMsg,enumSlzDataqueManNum,queManNum);
	CString wQueManNum(queManNum.c_str());
	pData->SetQueSerialID(wQueManNum);


	string queNum;
	GetMsgData(retMsg,enumSlzDataqueNum,queNum);
	CString wQueNum(queNum.c_str());
	pData->SetQueueNumber(wQueNum);


	string intQueNum;
	GetMsgData(retMsg,enumSlzDataintQueNum,intQueNum);
	pData->SetIntQueNum(atoi(const_cast<char*>(intQueNum.c_str())));


	string cardType;
	GetMsgData(retMsg,enumSlzDatacardType,cardType);
	pData->SetCardType((CardType)atoi(const_cast<char*>(cardType.c_str())));



	string cardNum;
	GetMsgData(retMsg,enumSlzDatacardNum,cardNum);
	CString wCardNum(cardNum.c_str());
	pData->SetCardNumber(wCardNum);


	string custName;
	GetMsgData(retMsg,enumSlzDatacustName,custName);
	CString wCustName(custName.c_str());
	pData->SetCustName(wCustName);



	string custLevel;
	GetMsgData(retMsg,enumSlzDatacustLevel,custLevel);
	pData->SetCustomerLevel(atoi(const_cast<char*>(custLevel.c_str())));


	string takeNumTime;
	GetMsgData(retMsg,enumSlzDatatakeNumTime,takeNumTime);
	CTime time;
	StringToTime(takeNumTime,time);
	pData->SetTakingNumTime(time);

	string staffId;
	GetMsgData(retMsg,enumSlzDatastaffId,staffId);
	CString wStaffId(staffId.c_str());
	pData->SetStaffId(wStaffId);

	string phoneNum;
	GetMsgData(retMsg,enumSlzDataphoneNum,phoneNum);
	CString wPhoneNum(phoneNum.c_str());
	pData->SetPhoneNum(wPhoneNum);


	string sendMsg;
	GetMsgData(retMsg,enumSlzDatasendMsg,phoneNum);
	CString wSendMsg(sendMsg.c_str());
	pData->SetSendMsg(wSendMsg);



	pos1 = retMsg.find("<retCode>");
	pos2 = retMsg.find("</retCode>");
	if(pos1 == retMsg.npos || pos2 == retMsg.npos)
		return FALSE;

	string aRetCode = retMsg.substr(pos1 + strlen("<queManNum>"),pos2 - pos1 - strlen("<queManNum>"));
	if(aRetCode == "0")
	{
		*pIsSucced = FALSE;
	}
	else
	{
		*pIsSucced = TRUE;
	}
	return TRUE;
}

BOOL CDealInterMsg::GetMsgData(const string& msg,SlzDataType dataType,string& data)
{
	if(msg.empty())return FALSE;
	string sample1,sample2;
	switch(dataType)
	{
	case enumSlzDataSerialid:
		sample1 = "<serialId>";
		sample2 = "</serialId>";
		break;
	case enumSlzDatabussName:
		sample1 = "<bussName>";
		sample2 = "</bussName>";
		break;
	case enumSlzDataqueId:
		sample1 = "<queId>";
		sample2 = "</queId>";
		break;
	case enumSlzDataqueManNum:
		sample1 = "<queManNum>";
		sample2 = "</queManNum>";
		break;
	case enumSlzDataqueNum:
		sample1 = "<queNum>";
		sample2 = "</queNum>";
		break;
	case enumSlzDataintQueNum:
		sample1 = "<intQueNum>";
		sample2 = "</intQueNum>";
		break;
	case enumSlzDatacardType:
		sample1 = "<cardType>";
		sample2 = "</cardType>";
		break;
	case enumSlzDatacardNum:
		sample1 = "<cardNum>";
		sample2 = "</cardNum>";
		break;
	case enumSlzDatacustName:
		sample1 = "<custName>";
		sample2 = "</custName>";
		break;
	case enumSlzDatacustLevel:
		sample1 = "<custLevel>";
		sample2 = "</custLevel>";
		break;
	case enumSlzDatatakeNumTime:
		sample1 = "<takeNumTime>";
		sample2 = "</takeNumTime>";
		break;
	case enumSlzDatastaffId:
		sample1 = "<staffId>";
		sample2 = "</staffId>";
		break;
	case enumSlzDataphoneNum:
		sample1 = "<phoneNum>";
		sample2 = "</phoneNum>";
		break;
	case enumSlzDatasendMsg:
		sample1 = "<sendMsg>";
		sample2 = "</sendMsg>";
		break;
	default:
		return FALSE;
	}
	
	string::size_type pos1,pos2;
	pos1 = msg.find(sample1);
	pos2 = msg.find(sample2);
	if(pos1 == msg.npos || pos2 == msg.npos)
		return FALSE;

	data = msg.substr(pos1 + strlen(sample1.c_str()),pos2 - pos1 - strlen(sample1.c_str()));
	return TRUE;
}

void CDealInterMsg::StringToTime(const string& strTime,CTime& time)
{
	string::size_type pos1,pos2,pos3,pos4,pos5;
	pos1 = strTime.find("-");if(pos1 == strTime.npos)return;
	string strYear = strTime.substr(0,pos1);
	int nYear = atoi(const_cast<char*>(strYear.c_str()));
	
	pos2 = strTime.find("-",pos1+1);if(pos2 == strTime.npos)return;
	string strMonth = strTime.substr(pos1+1,pos2 - pos1 -1);
	int nMonth = atoi(const_cast<char*>(strMonth.c_str()));

	pos3 = strTime.find(" ",pos2 + 1);if(pos3 == strTime.npos)return;
	string strDay = strTime.substr(pos2 + 1,pos3 - pos2 - 1);
	int nDay = atoi(const_cast<char*>(strDay.c_str()));

	pos4 = strTime.find(":",pos3+1);if(pos4 == strTime.npos)return;
	string strHour = strTime.substr(pos3 + 1,pos4 - pos3 - 1);
	int nHour = atoi(const_cast<char*>(strHour.c_str()));

	pos5 = strTime.find(":",pos4+1);if(pos5 == strTime.npos)return;
	string strMin = strTime.substr(pos4 + 1,pos5 - pos4 - 1);
	int nMin = atoi(const_cast<char*>(strMin.c_str()));

	string strSec = strTime.substr(pos5 + 1,2);
	int nSec = atoi(const_cast<char*>(strSec.c_str()));

	CTime temptime(nYear,nMonth,nDay,nHour,nMin,nSec);
	time = temptime;
}


void CDealInterMsg::ProduceRetAlertCallerMsg(const CString& queManNum,string& retAlertMsg)
{
	CString msg = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\">");
	msg.Append(_T("<headCode>retAlertMsg</headCode>"));
	msg.AppendFormat(_T("<queManNum>%s</queManNum>"),queManNum);

	CStringA a_retPacket(msg.GetBuffer(0));
	msg.ReleaseBuffer(0);
	retAlertMsg = a_retPacket.GetBuffer(0);
	a_retPacket.ReleaseBuffer(0);
}


BOOL CDealInterMsg::AnaRetAlterCallerMsg(const string& retAlterMsg,CString& queManNum)
{
	string::size_type pos1 = retAlterMsg.find("<headCode>");
	string::size_type pos2 = retAlterMsg.find("</headCode>");
	if(pos1 == retAlterMsg.npos || pos2 == retAlterMsg.npos)
		return FALSE;

	string headCode = retAlterMsg.substr(pos1 + strlen("<headCode>"),pos2 - pos1 - strlen("<headCode>"));
	if(headCode != "retAlertMsg")
		return FALSE;

	string aQueManNum;
	BOOL flag = GetMsgData(retAlterMsg,enumSlzDataqueManNum,aQueManNum);
	CString wQueManNum(aQueManNum.c_str());
	queManNum = wQueManNum;
	return flag;
}

void CDealInterMsg::ProduceBrodcastRetInNumMsg(const CString& queManNum,UINT inlineNum,string& retBrodcastNumMsg)
{
	CString msg = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\">");
	msg.Append(_T("<headCode>retBrodcastNumMsg</headCode>"));
	msg.AppendFormat(_T("<queManNum>%s</queManNum>"),queManNum);
	msg.AppendFormat(_T("<inlineNum>%d</inlineNum>"),inlineNum);
	msg.Append(_T("</dataPacket>"));

	CStringA a_retPacket(msg.GetBuffer(0));
	msg.ReleaseBuffer(0);
	retBrodcastNumMsg = a_retPacket.GetBuffer(0);
	a_retPacket.ReleaseBuffer(0);
}

BOOL CDealInterMsg::AnaRetBrodcastNumMsg(CString& queManNum,UINT* pInlineNum,string& retBrodcastNumMsg)
{
	string::size_type pos1 = retBrodcastNumMsg.find("<headCode>");
	string::size_type pos2 = retBrodcastNumMsg.find("</headCode>");
	if(pos1 == retBrodcastNumMsg.npos || pos2 == retBrodcastNumMsg.npos)
		return FALSE;

	string headCode = retBrodcastNumMsg.substr(pos1 + strlen("<headCode>"),pos2 - pos1 - strlen("<headCode>"));
	if(headCode != "retBrodcastNumMsg")
		return FALSE;
	
	string aQueManNum;
	BOOL flag = GetMsgData(retBrodcastNumMsg,enumSlzDataqueManNum,aQueManNum);
	CString wQueManNum(aQueManNum.c_str());
	queManNum = wQueManNum;

	if(pInlineNum != NULL)
	{
		pos1 = retBrodcastNumMsg.find("<inlineNum>");
		pos2 = retBrodcastNumMsg.find("</inlineNum>");
		if(pos1 == retBrodcastNumMsg.npos || pos2 == retBrodcastNumMsg.npos)
			return FALSE;

		string inlineNum = retBrodcastNumMsg.substr(pos1 + strlen("<inlineNum>"),pos2 - pos1 - strlen("<inlineNum>"));
		*pInlineNum = atoi(const_cast<char*>(inlineNum.c_str()));
	}
	return flag;
}