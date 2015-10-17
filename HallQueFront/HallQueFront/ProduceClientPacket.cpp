#include "StdAfx.h"
#include "ProduceClientPacket.h"
#include "WriteLogError.h"

CProduceClientPacket::CProduceClientPacket(void)
{
}

CProduceClientPacket::~CProduceClientPacket(void)
{
}

CString CProduceClientPacket::ProduceSendPacket(const SLZData& data,int leftNum,SendDataType dataType,CString cardNum/* =_T */,CardType cardType/*cardNotDefine*/)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>hallQue</headCode>");
	packet.AppendFormat(_T("<dataType>%d</dataType>"),dataType);
	packet.AppendFormat(_T("<orgName>%s</orgName>"),data.GetOrganName());
	packet.AppendFormat(_T("<orgID>%s</orgID>"),data.GetOrganId());
	packet.AppendFormat(_T("<cardType>%d</cardType>"),cardType);
	packet.AppendFormat(_T("<cardNum>%s</cardNum>"),cardNum);
	packet.AppendFormat(_T("<queID>%s</queID>"),data.GetQueSerialID());
	packet.AppendFormat(_T("<queNum>%s</queNum>"),data.GetQueueNumber());
	CString wStrtakeNumTime = ExChangeTime(data.GetTakingNumTime());
	packet.AppendFormat(_T("<enterTime>%s</enterTime>"),wStrtakeNumTime);
	CString wStrCallTime = ExChangeTime(data.GetCallTime());
	packet.AppendFormat(_T("<callTime>%s</callTime>"),wStrCallTime);
	packet.AppendFormat(_T("<workerID>%s</workerID>"),data.GetStaffId());
	packet.AppendFormat(_T("<windowNum>%d</windowNum>"),data.GetWindowShowId());
	CString wStrFinishTime = ExChangeTime(data.GetFinishTime());
	packet.AppendFormat(_T("<scoreTime>%s</scoreTime>"),wStrFinishTime);
	packet.AppendFormat(_T("<scoreLev>%d</scoreLev>"),data.GetEvaluateLevel());
	
	return packet;
}

int CProduceClientPacket::JudgePacketRet(const std::string strPacket)
{
	CWriteLogError writeLogErr;
	if(strPacket.empty())
	{
		writeLogErr.WriteErrLog(_T("返回报文为空"));
		return -1;
	}
	CString wRecvMsg(strPacket.c_str());
	CString wFormatRecvMsg = _T("recvSwingCardMsg:") + wRecvMsg;

	std::string::size_type firstIndex = strPacket.find("<retCode>");
	if(firstIndex == strPacket.npos)
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到<retCode>"));
		return -1;
	}
	std::string::size_type lastIndex = strPacket.find("</retCode>");
	if(lastIndex == strPacket.npos)
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到</retCode>"));
		return -1;
	}
	std::string errCode = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
	if(errCode=="1")
	{
		return 1;//成功
	}
	return 0;
}

CString CProduceClientPacket::ExChangeTime(const CTime& time)
{
	CString wStrTime;
	wStrTime.Format(_T("%d-%d-%d %d:%d:%d"),time.GetYear(),time.GetMonth(),
		time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
	return wStrTime;
}

CString CProduceClientPacket::ProduceSearchOrgInfo()
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>orgInfo</headCode>");
	return packet;
}

CString CProduceClientPacket::ProduceQuePacket(const CString& queNum,const CString& queName,const CString& orgID)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>queInfo</headCode>");
	packet.AppendFormat(_T("<queNum>%s</queNum>"),queNum);
	packet.AppendFormat(_T("<queName>%s</queName>"),queName);
	packet.AppendFormat(_T("<orgID>%s</orgID>"),orgID);
	return packet;
}

CString CProduceClientPacket::ProduceStaffPacket(const CString& staffID,const CString& staffName,const CString& orgID)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>staffInfo</headCode>");
	packet.AppendFormat(_T("<staffID>%s</staffID>"),staffID);
	packet.AppendFormat(_T("<staffName>%s</staffName>"),staffName);
	packet.AppendFormat(_T("<orgID>%s</orgID>"),orgID);
	return packet;
}

CString CProduceClientPacket::ProduceWndPacket(const CString& wndID,const CString& wndName,const CString& orgID)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>wndInfo</headCode>");
	packet.AppendFormat(_T("<wndID>%s</wndID>"),wndID);
	packet.AppendFormat(_T("<wndName>%s</wndName>"),wndName);
	packet.AppendFormat(_T("<orgID>%s</orgID>"),orgID);
	return packet;
}

CString CProduceClientPacket::ProduceSearchQueInfo(const CString& orgID)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>searQue</headCode>");
	packet.AppendFormat(_T("<orgID>%s</orgID>"),orgID);
	return packet;
}

CString CProduceClientPacket::ProduceSearchWndInfo(const CString& orgID)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>searWnd</headCode>");
	packet.AppendFormat(_T("<orgID>%s</orgID>"),orgID);
	return packet;
}

CString CProduceClientPacket::ProduceSearchStaffInfo(const CString& orgID)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>searStaff</headCode>");
	packet.AppendFormat(_T("<orgID>%s</orgID>"),orgID);
	return packet;
}

CString CProduceClientPacket::ProduceOrgPacket(const CString& curOrgID,const CString& curOrgName,const CString& parOrgID,const CString& parOrgName)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>sendOrg</headCode>");
	if(!parOrgID.IsEmpty())
		packet.AppendFormat(_T("<parOrgID>%s</parOrgID>"),parOrgID);//父级结构
	else 
		packet += _T("<parOrgID>0</parOrgID>");//顶级机构
	packet.AppendFormat(_T("<curOrgID>%s</curOrgID>"),curOrgID);
	packet.AppendFormat(_T("<parOrgName>%s</parOrgName>"),parOrgName);
	packet.AppendFormat(_T("<curOrgName>%s</curOrgName>"),curOrgName);
	return packet;
}

CString CProduceClientPacket::ProduceDelQue(const CString& queID,const CString& orgID)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>delQue</headCode>");
	packet.AppendFormat(_T("<queNum>%s</queNum>"),queID);
	packet.AppendFormat(_T("<orgID>%s</orgID>"),orgID);
	return packet;
}

CString CProduceClientPacket::ProduceDelWnd(const CString& wndID,const CString& orgID)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>delWnd</headCode>");
	packet.AppendFormat(_T("<wndID>%s</wndID>"),wndID);
	packet.AppendFormat(_T("<orgID>%s</orgID>"),orgID);
	return packet;
}

CString CProduceClientPacket::ProduceDelStaff(const CString& staffID,const CString& orgID)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>delStaff</headCode>");
	packet.AppendFormat(_T("<staffID>%s</staffID>"),staffID);
	packet.AppendFormat(_T("<orgID>%s</orgID>"),orgID);
	return packet;
}

CString CProduceClientPacket::ProducePauseTime(const CString& organID,const CString& staffID,int windowID,const CTime& startTime,const CTime& endTime,int nSec,bool bState)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>pauseTime</headCode>");
	packet.AppendFormat(_T("<organID>%s</organID>"),organID);
	packet.AppendFormat(_T("<staffID>%s</staffID>"),staffID);
	packet.AppendFormat(_T("<windowID>%d</windowID>"),windowID);
	CString strStartTime,strEndTime;
	strStartTime.Format(_T("%d-%d-%d %d:%d:%d"),startTime.GetYear(),startTime.GetMonth(),startTime.GetDay(),
		startTime.GetHour(),startTime.GetMinute(),startTime.GetSecond());
	strEndTime.Format(_T("%d-%d-%d %d:%d:%d"),endTime.GetYear(),endTime.GetMonth(),endTime.GetDay(),
		endTime.GetHour(),endTime.GetMinute(),endTime.GetSecond());

	packet.AppendFormat(_T("<startTime>%s</startTime>"),strStartTime);
	packet.AppendFormat(_T("<endTime>%s</endTime>"),strEndTime);

	packet.AppendFormat(_T("<overTime>%d</overTime>"),nSec);
	packet.AppendFormat(_T("<state>%d</state>"),bState);//0代表离开1代表暂停
	return packet;
}