#include "StdAfx.h"
#include "ProducePacket.h"
#include "CommonConvert.h"
#include "HallQueFront.h"
#include "WriteLogError.h"

extern void MyWriteConsole(CString str);

CProducePacket::CProducePacket(void)
{
}

CProducePacket::~CProducePacket(void)
{
}

CString CProducePacket::ProduceSendPacket(const SLZData& data,int leftNum,SendDataType dataType,CString cardNum/* =_T */,CardType cardType/*cardNotDefine*/)
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
	
// 	int len = CCommonConvert::CStringToChar(packet,NULL);
// 	char* strPacket = new char[len+1];
// 	memset(strPacket,0,len+1);
// 	CCommonConvert::CStringToChar(packet,strPacket);
// 	std::string stdStrPacket(strPacket);
// 	return stdStrPacket;
	return packet;
}

int CProducePacket::JudgePacketRet(const std::string strPacket)
{
	CWriteLogError writeLogErr;
	if(strPacket.empty())
	{
		writeLogErr.WriteErrLog(_T("返回报文为空"));
		return -1;
	}
	CString wRecvMsg(strPacket.c_str());
	CString wFormatRecvMsg = _T("recvSwingCardMsg:") + wRecvMsg;
#ifdef _DEBUG
	MyWriteConsole(wRecvMsg);
#endif
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

int CProducePacket::JudgeSendPacket(const std::string strPacket)
{
	if(strPacket.empty())return 0;//报文为空,返回报文错误
	std::string::size_type firstIndex = strPacket.find("<headCode>");
	if(firstIndex == strPacket.npos)return 11;//头错误
	std::string::size_type lastIndex = strPacket.find("</headCode>");
	if(lastIndex == strPacket.npos)return 11;//头错误
	std::string head = strPacket.substr(firstIndex+10,7);
	if(head!="hallQue")return 11;//头错
	firstIndex = strPacket.find("<dataType>");
	if(firstIndex == strPacket.npos)return 12;//数据类型错误
	lastIndex = strPacket.find("</dataType>");
	if(lastIndex == strPacket.npos)return 12;//数据类型错误
	std::string dataType = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	if(!(dataType == "0" || dataType=="1" || dataType=="2" || dataType=="3"))
		return 12;//数据类型错误
	firstIndex = strPacket.find("<orgName>");
	if(firstIndex == strPacket.npos)return 13;
	lastIndex = strPacket.find("</orgName>");
	if(lastIndex == strPacket.npos)return 13;
	std::string orgName = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
	if(orgName.empty())return 13;//机构名称不能为空
	firstIndex = strPacket.find("<orgID>");
	if(firstIndex == strPacket.npos)return 14;
	lastIndex = strPacket.find("</orgID>");
	if(lastIndex == strPacket.npos)return 14;
	std::string orgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
	if(orgID.empty())return 14;
	firstIndex = strPacket.find("<cardType>");
	if(firstIndex == strPacket.npos)return 15;
	lastIndex = strPacket.find("</cardType>");
	if(lastIndex == strPacket.npos)return 15;
	std::string cardType = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	if(!(cardType=="0" || cardType=="1" || cardType=="2"))return 15;
	firstIndex = strPacket.find("<cardNum>");
	if(firstIndex == strPacket.npos)return 16;
	lastIndex = strPacket.find("</cardNum>");
	if(lastIndex == strPacket.npos)return 16;
	std::string cardNum = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
	if((cardType=="1" || cardType=="2"))
		if(cardNum.empty())return 16;
	firstIndex = strPacket.find("<queID>");//队列编号可以为空
	if(firstIndex == strPacket.npos)return 17;
	lastIndex = strPacket.find("</queID>");
	if(lastIndex == strPacket.npos)return 17;
	firstIndex = strPacket.find("<queNum>");
	if(firstIndex == strPacket.npos)return 18;
	lastIndex = strPacket.find("</queNum>");
	if(lastIndex == strPacket.npos)return 18;
	std::string queNum = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
	if(queNum.empty())return 18;
	firstIndex = strPacket.find("<enterTime>");
	if(firstIndex == strPacket.npos)return 19;
	lastIndex = strPacket.find("</enterTime>");
	if(lastIndex == strPacket.npos)return 19;
	std::string enterTime = strPacket.substr(firstIndex+11,lastIndex-firstIndex-11);
	if(enterTime.empty())return 19;
	firstIndex = strPacket.find("<callTime>");
	if(firstIndex == strPacket.npos)return 20;
	lastIndex = strPacket.find("</callTime>");
	if(lastIndex == strPacket.npos)return 20;
	std::string callTime = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	if(callTime.empty())return 20;
	firstIndex = strPacket.find("<workerID>");
	if(firstIndex == strPacket.npos)return 21;
	lastIndex = strPacket.find("</workerID>");
	if(firstIndex == strPacket.npos)return 21;
	std::string workerID = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	if(workerID.empty())return 21;
	firstIndex = strPacket.find("<windowNum>");
	if(firstIndex==strPacket.npos)return 22;
	lastIndex = strPacket.find("</windowNum>");
	if(lastIndex == strPacket.npos)return 22;
	firstIndex = strPacket.find("<scoreTime>");
	if(firstIndex == strPacket.npos)return 23;
	lastIndex = strPacket.find("</scoreTime>");
	if(lastIndex == strPacket.npos)return 23;
	std::string scoreTime = strPacket.substr(firstIndex+11,lastIndex-firstIndex-11);
	if(scoreTime.empty())return 23;
	firstIndex = strPacket.find("<scoreLev>");
	if(firstIndex == strPacket.npos)return 24;
	lastIndex = strPacket.find("</scoreLev>");
	if(lastIndex == strPacket.npos)return 24;
	std::string scoreLev = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	if(!(scoreLev=="-1" || scoreLev=="0" || scoreLev=="1" || scoreLev=="2" || scoreLev=="3"))
		return 24;
	return 1;
}

CString CProducePacket::ExChangeTime(const CTime& time)
{
	CString wStrTime;
	wStrTime.Format(_T("%d-%d-%d %d:%d:%d"),time.GetYear(),time.GetMonth(),
		time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
	return wStrTime;
}

std::string CProducePacket::ProduceSendRet(int errCode)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>hallQue</headCode>");
	packet.AppendFormat(_T("<retCode>%d</retCode>"),errCode);
	int len = CCommonConvert::CStringToChar(packet,NULL);
	char* strPacket = new char[len+1];
	memset(strPacket,0,len+1);
	CCommonConvert::CStringToChar(packet,strPacket);
	std::string stdStrPacket(strPacket);
	delete [] strPacket;
	return stdStrPacket;
}