#include "StdAfx.h"
#include "ProducePacket.h"
#include "../HallQueFront/CommonConvert.h"
#include "DbaConfig.h"
#include "HallQueFrontServer.h"

extern void MyWriteConsole(CString str);

CProducePacket::CProducePacket(void)
{
// 	if(!m_mySql.IsConnect())
// 	{
// 		CDbaConfig dbaConfig;
// 		CString account = dbaConfig.GetServerAcount();
// 		CString ip = dbaConfig.GetServerIP();
// 		CString pass = dbaConfig.GetServerPassword();
// 		CString port = dbaConfig.GetDBPort();
// 		int i_port = 0;
// 		CCommonConvert::CStringToint(i_port,port);
// 		m_mySql.ConnectToDB(ip,i_port,account,pass);
// 	}
}

CProducePacket::~CProducePacket(void)
{
}

int CProducePacket::JudgeSendPacket(const std::string strPacket)
{
	if(strPacket.empty())return 0;//报文为空,返回报文错误
	std::string::size_type firstIndex = strPacket.find("<headCode>");
	if(firstIndex == strPacket.npos)return 11;//头错误
	std::string::size_type lastIndex = strPacket.find("</headCode>");
	if(lastIndex == strPacket.npos)return 11;//头错误
	std::string head = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	if(head=="queInfo")
	{
		firstIndex = strPacket.find("<queNum>");
		if(firstIndex == strPacket.npos)return -26;//报文错误
		lastIndex = strPacket.find("</queNum>");
		if(lastIndex == strPacket.npos)return -26;//报文错误
		std::string strQueNum = strPacket.substr(firstIndex+8,lastIndex-firstIndex-8);
		if(strQueNum.empty())return -26;
		//////////////////////////////////
		firstIndex = strPacket.find("<queName>");
		if(firstIndex == strPacket.npos)return -26;
		lastIndex = strPacket.find("</queName>");
		if(lastIndex == strPacket.npos)return -26;
		std::string queName = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
		if(queName.empty())return -26;
		////////////////////////////////
		firstIndex = strPacket.find("<orgID>");
		if(firstIndex == strPacket.npos)return -26;
		lastIndex = strPacket.find("</orgID>");
		if(lastIndex == strPacket.npos)return -26;
		std::string strOrgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
		if(strOrgID.empty())return -26;
// 		firstIndex = strPacket.find("<onlyID>");
// 		if(firstIndex == strPacket.npos)return -26;
// 		lastIndex = strPacket.find("</onlyID>");
// 		if(lastIndex == strPacket.npos)return -26;
//		std::string strOnlyID = strPacket.substr(firstIndex+8,lastIndex-firstIndex-8);
		return 26;
	}
	else if(head=="staffInfo")
	{
		firstIndex = strPacket.find("<staffID>");
		if(firstIndex==strPacket.npos)return -28;
		lastIndex = strPacket.find("</staffID>");
		if(lastIndex==strPacket.npos)return -28;
		std::string staffID = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
		if(staffID.empty())return -28;
		//////////////////////////////
		firstIndex = strPacket.find("<staffName>");
		if(firstIndex==strPacket.npos)return -28;
		lastIndex = strPacket.find("</staffName>");
		if(lastIndex==strPacket.npos)return -28;
		std::string staffName = strPacket.substr(firstIndex+11,lastIndex-firstIndex-11);
		if(staffName.empty())return -28;
		//////////////////////////////
		firstIndex = strPacket.find("<orgID>");
		if(firstIndex == strPacket.npos)return -28;
		lastIndex = strPacket.find("</orgID>");
		if(lastIndex == strPacket.npos)return -28;
		std::string strOrgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
		if(strOrgID.empty())return -28;
// 		firstIndex = strPacket.find("<onlyID>");
// 		if(firstIndex == strPacket.npos)return -26;
// 		lastIndex = strPacket.find("</onlyID>");
// 		if(lastIndex == strPacket.npos)return -26;
		return 28;
	}
	else if(head=="wndInfo")
	{
		firstIndex = strPacket.find("<wndID>");
		if(firstIndex==strPacket.npos)return -27;
		lastIndex = strPacket.find("</wndID>");
		if(lastIndex==strPacket.npos)return -27;
		std::string wndID = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
		if(wndID.empty())return -27;
		//////////////////////////////
		firstIndex = strPacket.find("<wndName>");
		if(firstIndex==strPacket.npos)return -27;
		lastIndex = strPacket.find("</wndName>");
		if(lastIndex==strPacket.npos)return -27;
		std::string wndName = strPacket.substr(firstIndex+11,lastIndex-firstIndex-11);
		if(wndName.empty())return -27;
		//////////////////////////////
		firstIndex = strPacket.find("<orgID>");
		if(firstIndex == strPacket.npos)return -27;
		lastIndex = strPacket.find("</orgID>");
		if(lastIndex == strPacket.npos)return -27;
		std::string strOrgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
		if(strOrgID.empty())return -27;
// 		firstIndex = strPacket.find("<onlyID>");
// 		if(firstIndex == strPacket.npos)return -26;
// 		lastIndex = strPacket.find("</onlyID>");
// 		if(lastIndex == strPacket.npos)return -26;
		return 27;
	}
	else if(head=="orgInfo")
	{
		return 25;
	}
	else if(head == "hallQue")
	{
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
	}
	else if(head == "searQue")
	{
		firstIndex = strPacket.find("<orgID>");
		if(firstIndex==strPacket.npos)return -29;
		lastIndex = strPacket.find("</orgID>");
		if(lastIndex==strPacket.npos)return -29;
		std::string orgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
		if(orgID.empty())return -29;
		return 29;
	}
	else if(head == "searWnd")
	{
		firstIndex = strPacket.find("<orgID>");
		if(firstIndex==strPacket.npos)return -30;
		lastIndex = strPacket.find("</orgID>");
		if(lastIndex==strPacket.npos)return -30;
		std::string orgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
		if(orgID.empty())return -30;
		return 30;
	}
	else if(head == "searStaff")
	{
		firstIndex = strPacket.find("<orgID>");
		if(firstIndex==strPacket.npos)return -31;
		lastIndex = strPacket.find("</orgID>");
		if(lastIndex==strPacket.npos)return -31;
		std::string orgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
		if(orgID.empty())return -31;
		return 31;
	}
	else if(head == "sendOrg")//发送机构数据
	{
		firstIndex = strPacket.find("<parOrgID>");
		if(firstIndex==strPacket.npos)return -32;
		lastIndex = strPacket.find("</parOrgID>");
		if(lastIndex==strPacket.npos)return -32;
		std::string strParOrgID = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
		if(strParOrgID.empty())return -32;
		firstIndex = strPacket.find("<curOrgID>");
		if(firstIndex==strPacket.npos)return -32;
		lastIndex = strPacket.find("</curOrgID>");
		if(lastIndex==strPacket.npos)return -32;
		std::string strCurOrgID = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
		if(strCurOrgID.empty())return -32;
		firstIndex = strPacket.find("<parOrgName>");
		if(firstIndex == strPacket.npos)return -32;
		lastIndex = strPacket.find("</parOrgName>");
		if(lastIndex==strPacket.npos)return -32;
		std::string strParOrgName = strPacket.substr(firstIndex+12,lastIndex-firstIndex-12);
		if(strParOrgName.empty())return -32;
		firstIndex = strPacket.find("<curOrgName>");
		if(firstIndex==strPacket.npos)return -32;
		lastIndex = strPacket.find("</curOrgName>");
		if(lastIndex==strPacket.npos)return -32;
		std::string strCurOrgName = strPacket.substr(firstIndex+12,lastIndex-firstIndex-12);
		if(strCurOrgName.empty())return -32;
// 		firstIndex = strPacket.find("<onlyID>");
// 		if(firstIndex == strPacket.npos)return -32;
// 		lastIndex = strPacket.find("</onlyID>");
// 		if(lastIndex==strPacket.npos)return -32;
		return 32;
	}
	else if(head == "delQue")
	{
		firstIndex = strPacket.find("<queNum>");
		if(firstIndex == strPacket.npos)return -33;
		lastIndex = strPacket.find("</queNum>");
		if(lastIndex == strPacket.npos)return -33;
		std::string queNum = strPacket.substr(firstIndex+8,lastIndex-firstIndex-8);
		if(queNum.empty())return -33;
		firstIndex = strPacket.find("<orgID>");
		if(firstIndex==strPacket.npos)return -33;
		lastIndex = strPacket.find("</orgID>");
		if(lastIndex==strPacket.npos)return -33;
		std::string orgID=strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
		if(orgID.empty())return -33;
		return 33;
	}
	else if(head == "delStaff")
	{
		firstIndex = strPacket.find("<staffID>");
		if(firstIndex==strPacket.npos)return -34;
		lastIndex = strPacket.find("</staffID>");
		if(lastIndex==strPacket.npos)return -34;
		std::string staffID=strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
		if(staffID.empty())return -34;
		firstIndex = strPacket.find("<orgID>");
		if(firstIndex==strPacket.npos)return -34;
		lastIndex = strPacket.find("</orgID>");
		if(lastIndex==strPacket.npos)return -34;
		std::string orgID=strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
		if(orgID.empty())return -34;
		return 34;
	}
	else if(head=="delWnd")
	{
		firstIndex = strPacket.find("<wndID>");
		if(firstIndex==strPacket.npos)return -35;
		lastIndex = strPacket.find("</wndID>");
		if(lastIndex==strPacket.npos)return -35;
		std::string wndID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
		if(wndID.empty())return -35;
		firstIndex = strPacket.find("<orgID>");
		if(firstIndex==strPacket.npos)return -35;
		lastIndex = strPacket.find("</orgID>");
		if(lastIndex==strPacket.npos)return -35;
		std::string orgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
		if(orgID.empty())return -35;
		return 35;
	}
	else
	{
		return 11;
	}
	return 1;
}

std::string CProducePacket::ProduceSendRet(int errCode,const std::string strPacket)
{
	if(!m_mySql.IsConnect())
	{
		CDbaConfig dbaConfig;
		CString account = dbaConfig.GetServerAcount();
		CString ip = dbaConfig.GetServerIP();
		CString pass = dbaConfig.GetServerPassword();
		CString port = dbaConfig.GetDBPort();
		int i_port = 0;
		CCommonConvert::CStringToint(i_port,port);
		m_mySql.ConnectToDB(ip,i_port,account,pass);
	}
	switch(errCode)
	{
	case 25://查询机构
		{
			std::auto_ptr<sql::ResultSet> res(NULL);
			if(m_mySql.IsConnect())
			{
				CString sql = _T("select orID,orOrganId,orName,orParentName,orParent from organ where orParent='0'");
				res =m_mySql.ExecuteQuery(sql);
			}
			CString retPacket = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>orgInfo</headCode>");
			if(res.get())
			{
				while(res->next())
				{
					std::string parentOrgID = res->getString("orParent");
					std::string curOrgID = res->getString("orOrganId");
					std::string parOrgName = res->getString("orParentName");
					std::string curOrgName = res->getString("orName");
//					int ID = res->getInt("ID");
					/////////////////////////////////////////////
					CString wParentOrgID(parentOrgID.c_str());
					CString wCurOrgID(curOrgID.c_str());
					CString wParOrgName(parOrgName.c_str());
					CString wCurOrgName(curOrgName.c_str());
					/*CString wOnlyID;wOnlyID.Format(_T("%d"),ID);*/
					/////////////////////////////////////////////
					retPacket.AppendFormat(_T("<parOrgID>%s</parOrgID>"),wParentOrgID);
					retPacket.AppendFormat(_T("<curOrgID>%s</curOrgID>"),wCurOrgID);
					retPacket.AppendFormat(_T("<parOrgName>%s</parOrgName>"),wParOrgName);
					retPacket.AppendFormat(_T("<curOrgName>%s</curOrgName>"),wCurOrgName);
//					retPacket.AppendFormat(_T("<onlyID>%s</onlyID>"),wOnlyID);
					retPacket+=_T("<end>");
				}
			}
			////////////////////////////
			std::string strRetPacket;
			CStringA a_retPacket(retPacket.GetBuffer(0));
			retPacket.ReleaseBuffer(0);
			strRetPacket = a_retPacket.GetBuffer(0);
			a_retPacket.ReleaseBuffer(0);
			////////////////////////////
			return strRetPacket;
		}
		break;
	case 26:
	case -26:
		{
			CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>queInfo</headCode>");
			packet.AppendFormat(_T("<retCode>%d</retCode>"),errCode);
			int len = CCommonConvert::CStringToChar(packet,NULL);
			char* strPacket = new char[len+1];
			memset(strPacket,0,len+1);
			CCommonConvert::CStringToChar(packet,strPacket);
			std::string stdStrPacket(strPacket);
			delete [] strPacket;
			return stdStrPacket;
		}
		break;
	case 27:
	case -27:
		{
			CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>wndInfo</headCode>");
			packet.AppendFormat(_T("<retCode>%d</retCode>"),errCode);
			int len = CCommonConvert::CStringToChar(packet,NULL);
			char* strPacket = new char[len+1];
			memset(strPacket,0,len+1);
			CCommonConvert::CStringToChar(packet,strPacket);
			std::string stdStrPacket(strPacket);
			delete [] strPacket;
			return stdStrPacket;
		}
		break;
	case 28:
	case -28:
		{
			CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>staffInfo</headCode>");
			packet.AppendFormat(_T("<retCode>%d</retCode>"),errCode);
			int len = CCommonConvert::CStringToChar(packet,NULL);
			char* strPacket = new char[len+1];
			memset(strPacket,0,len+1);
			CCommonConvert::CStringToChar(packet,strPacket);
			std::string stdStrPacket(strPacket);
			delete [] strPacket;
			return stdStrPacket;
		}
		break;
	case 29://队列查询
		{
			std::string::size_type firstIndex = strPacket.find("<orgID>");
			std::string::size_type lastIndex = strPacket.find("</orgID>");
			std::string orgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
			std::auto_ptr<sql::ResultSet> res;
			if(m_mySql.IsConnect())
			{
				CString wOrgID(orgID.c_str());
				CString sql;
				sql.Format(_T("select qQueueID,qOrganID,qName,qID from queue where qOrganID='%s'"),wOrgID);
				res = m_mySql.ExecuteQuery(sql);
			}
			CString retPacket = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>searQue</headCode>");
			if(res.get())
			{
				while (res->next())
				{
					std::string queID = res->getString("qQueueID");
					std::string orgID = res->getString("qOrganID");
					std::string queName = res->getString("qName");
					
					CString wQueID(queID.c_str());
					CString wOrgID(orgID.c_str());
					CString wQueName(queName.c_str());
					retPacket.AppendFormat(_T("<queID>%s</queID>"),wQueID);
					retPacket.AppendFormat(_T("<orgID>%s</orgID>"),wOrgID);
					retPacket.AppendFormat(_T("<queName>%s</queName>"),wQueName);
					retPacket+=_T("<end>");
				}
			}
			////////////////////////////
			std::string strRetPacket;
			CStringA a_retPacket(retPacket.GetBuffer(0));
			retPacket.ReleaseBuffer(0);
			strRetPacket = a_retPacket.GetBuffer(0);
			a_retPacket.ReleaseBuffer(0);
			////////////////////////////
			return strRetPacket;
		}
		break;
	case 30://查询窗口
		{
			std::string::size_type firstIndex = strPacket.find("<orgID>");
			std::string::size_type lastIndex = strPacket.find("</orgID>");
			std::string orgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
			std::auto_ptr<sql::ResultSet> res;
			if(m_mySql.IsConnect())
			{
				CString wOrgID(orgID.c_str());
				CString sql;
				sql.Format(_T("select wWindowID,wOrganID,wName,wID from window where wOrganID='%s'"),wOrgID);
				res =m_mySql.ExecuteQuery(sql);
			}
			CString retPacket = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>searWnd</headCode>");
			if(res.get())
			{
				while (res->next())
				{
					std::string wndID = res->getString("wWindowID");
					std::string orgID = res->getString("wOrganID");
					std::string wndName = res->getString("wName");
					
					CString wWndID(wndID.c_str());
					CString wOrgID(orgID.c_str());
					CString wWndName(wndName.c_str());
					
					retPacket.AppendFormat(_T("<wndID>%s</wndID>"),wWndID);
					retPacket.AppendFormat(_T("<orgID>%s</orgID>"),wOrgID);
					retPacket.AppendFormat(_T("<wndName>%s</wndName>"),wWndName);
					
					retPacket+=_T("<end>");
				}
			}
			////////////////////////////
			std::string strRetPacket;
			CStringA a_retPacket(retPacket.GetBuffer(0));
			retPacket.ReleaseBuffer(0);
			strRetPacket = a_retPacket.GetBuffer(0);
			a_retPacket.ReleaseBuffer(0);
			////////////////////////////
			return strRetPacket;
		}
		break;
	case 31://查询员工
		{
			std::string::size_type firstIndex = strPacket.find("<orgID>");
			std::string::size_type lastIndex = strPacket.find("</orgID>");
			std::string orgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
			std::auto_ptr<sql::ResultSet> res;
			if(m_mySql.IsConnect())
			{
				CString wOrgID(orgID.c_str());
				CString sql;
				sql.Format(_T("select wWorkid,wOrganID,wName,wID from worker where wOrganID='%s'"),wOrgID);
				res =m_mySql.ExecuteQuery(sql);
			}
			CString retPacket = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>searStaff</headCode>");
			if(res.get())
			{
				while (res->next())
				{
					std::string staffID = res->getString("wWorkid");
					std::string orgID = res->getString("wOrganID");
					std::string staffName = res->getString("wName");
					
					CString wStaffID(staffID.c_str());
					CString wOrgID(orgID.c_str());
					CString wStaffName(staffName.c_str());
					
					retPacket.AppendFormat(_T("<staffID>%s</staffID>"),wStaffID);
					retPacket.AppendFormat(_T("<orgID>%s</orgID>"),wOrgID);
					retPacket.AppendFormat(_T("<staffName>%s</staffName>"),wStaffName);
					
					retPacket+=_T("<end>");
				}
			}
			////////////////////////////
			std::string strRetPacket;
			CStringA a_retPacket(retPacket.GetBuffer(0));
			retPacket.ReleaseBuffer(0);
			strRetPacket = a_retPacket.GetBuffer(0);
			a_retPacket.ReleaseBuffer(0);
			////////////////////////////
			return strRetPacket;
		}
		break;
	case 32:
	case -32:
		{
			CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>sendOrg</headCode>");
			packet.AppendFormat(_T("<retCode>%d</retCode>"),errCode);
			int len = CCommonConvert::CStringToChar(packet,NULL);
			char* strPacket = new char[len+1];
			memset(strPacket,0,len+1);
			CCommonConvert::CStringToChar(packet,strPacket);
			std::string stdStrPacket(strPacket);
			delete [] strPacket;
			return stdStrPacket;
		}
		break;
	default:
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
		break;
	}
	return "";
}
