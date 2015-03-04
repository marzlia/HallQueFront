#include "StdAfx.h"
#include "DealData.h"
#include "DbaConfig.h"
#include "../HallQueFront/CommonConvert.h"
CDealData::CDealData(void) : m_pAnaPecket(NULL)
{
// 	if(!m_conMySql.IsConnect())
// 	{
// 		CDbaConfig dbaConfig;
// 		CString account = dbaConfig.GetServerAcount();
// 		CString ip = dbaConfig.GetServerIP();
// 		CString pass = dbaConfig.GetServerPassword();
// 		CString port = dbaConfig.GetDBPort();
// 		int i_port = 0;
// 		CCommonConvert::CStringToint(i_port,port);
// 		m_conMySql.ConnectToDB(ip,i_port,account,pass);
// 	}
// 	Start();
}

CDealData* CDealData::GetInstance()
{
	static CDealData dealData;
	return &dealData;
}

CDealData::~CDealData(void)
{
	if(m_pAnaPecket)
	{
		WaitForSingleObject(m_pAnaPecket->m_hThread,100);
		TerminateThread(m_pAnaPecket->m_hThread,0);
		m_pAnaPecket->ExitInstance();
		delete m_pAnaPecket;
	}
}

void CDealData::AddPacket(const std::string packet)
{
	m_mtRecvLock.Lock();
	m_list_recvPacket.push_back(packet);
	m_mtRecvLock.Unlock();
}

UINT CDealData::AnaPacket(LPVOID lpParam)
{
	CDealData* pThis = (CDealData*)lpParam;
	while (TRUE)
	{
		if(pThis->m_list_recvPacket.empty())
		{
			Sleep(10);
		}
		else
		{
			pThis->m_mtRecvLock.Lock();
			std::list<std::string>::const_iterator itera = pThis->m_list_recvPacket.begin();
			std::string strPacket = *itera;
			pThis->m_list_recvPacket.pop_front();
			pThis->m_mtRecvLock.Unlock();
			std::string::size_type firstIndex = strPacket.find("<headCode>");
			std::string::size_type lastIndex = strPacket.find("</headCode>");
			std::string head = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
			if(head=="hallQue")
			{
				CommDao commDao;
				pThis->TranslatePacket(strPacket,commDao);
				pThis->WriteDataToDB(commDao);
			}
			else if(head=="queInfo")
			{
				CommDaoQue commDaoQue;
				pThis->TranslatePacket(strPacket,commDaoQue);
				pThis->WriteDataToDB(commDaoQue);
			}
			else if(head=="wndInfo")
			{
				CommDaoWnd commDaoWnd;
				pThis->TranslatePacket(strPacket,commDaoWnd);
				pThis->WriteDataToDB(commDaoWnd);
			}
			else if(head=="staffInfo")
			{
				CommDaoStaff commDaoStaff;
				pThis->TranslatePacket(strPacket,commDaoStaff);
				pThis->WriteDataToDB(commDaoStaff);
			}
			else if(head=="sendOrg")
			{
				CommDaoOrg commDaoOrg;
				pThis->TranslatePacket(strPacket,commDaoOrg);
				pThis->WriteDataToDB(commDaoOrg);
			}
			else if(head=="delQue")
			{
				CommDaoQue commDaoQue;
				pThis->TranslateDelQue(strPacket,commDaoQue);
				pThis->WriteDelQue(commDaoQue);
			}
			else if(head=="delStaff")
			{
				CommDaoStaff commDaoStaff;
				pThis->TranslateDelStaff(strPacket,commDaoStaff);
				pThis->WriteDelStaff(commDaoStaff);
			}
			else if(head=="delWnd")
			{
				CommDaoWnd commDaoWnd;
				pThis->TranslateDelWnd(strPacket,commDaoWnd);
				pThis->WriteDelWnd(commDaoWnd);
			}
		}
	}
	return 0;
}

void CDealData::TranslatePacket(const std::string strPacket,CommDao& commDao)
{
	/////数据类型
	std::string::size_type firstIndex = strPacket.find("<dataType>");
	std::string::size_type lastIndex = strPacket.find("</dataType>");
	std::string dataType = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	commDao.dataType = dataType.c_str();
	////机构名称
	firstIndex = strPacket.find("<orgName>");
	lastIndex = strPacket.find("</orgName>");
	std::string orgName = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
	commDao.organName = orgName.c_str();
	/////机构ID
	firstIndex = strPacket.find("<orgID>");
	lastIndex = strPacket.find("</orgID>");
	std::string orgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
	commDao.organID = orgID.c_str();
	/////卡类型
	firstIndex = strPacket.find("<cardType>");
	lastIndex = strPacket.find("</cardType>");
	std::string cardType = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	if(cardType=="0")commDao.cardType = cardNotDefine;
	else if(cardType=="1")commDao.cardType = cardIDCard;
	else if(cardType=="2")commDao.cardType = cardMagCard;
	/////卡号
	firstIndex = strPacket.find("<cardNum>");
	lastIndex = strPacket.find("</cardNum>");
	std::string cardNum = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
	commDao.cardNum = cardNum.c_str();
	/////队列编号
	firstIndex = strPacket.find("<queID>");//队列编号可以为空
	lastIndex = strPacket.find("</queID>");
	std::string queID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
	commDao.queueID = queID.c_str();
	/////排队号码
	firstIndex = strPacket.find("<queNum>");
	lastIndex = strPacket.find("</queNum>");
	std::string queNum = strPacket.substr(firstIndex+8,lastIndex-firstIndex-8);
	commDao.clientID = queNum.c_str();
	//////取号时间
	firstIndex = strPacket.find("<enterTime>");
	lastIndex = strPacket.find("</enterTime>");
	std::string enterTime = strPacket.substr(firstIndex+11,lastIndex-firstIndex-11);
	commDao.timeEnter = enterTime.c_str();
	//////呼叫时间
	firstIndex = strPacket.find("<callTime>");
	lastIndex = strPacket.find("</callTime>");
	std::string callTime = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	commDao.timeCall = callTime.c_str();
	//////员工ID
	firstIndex = strPacket.find("<workerID>");
	lastIndex = strPacket.find("</workerID>");
	std::string workerID = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	commDao.workerID = workerID.c_str();
	//////窗口编号
	firstIndex = strPacket.find("<windowNum>");
	lastIndex = strPacket.find("</windowNum>");
	std::string windowNum = strPacket.substr(firstIndex+11,lastIndex-firstIndex-11);
	commDao.windowID = windowNum.c_str();
	//////评价时间
	firstIndex = strPacket.find("<scoreTime>");
	lastIndex = strPacket.find("</scoreTime>");
	std::string scoreTime = strPacket.substr(firstIndex+11,lastIndex-firstIndex-11);
	commDao.timeScore = scoreTime.c_str();
	commDao.timeLeave = commDao.timeScore;
	/////评价等级
	firstIndex = strPacket.find("<scoreLev>");
	lastIndex = strPacket.find("</scoreLev>");
	std::string scoreLev = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	if(scoreLev == "-1")commDao.evalLev = evaNone;
	else if(scoreLev == "0")commDao.evalLev = evaBad;
	else if(scoreLev == "1")commDao.evalLev = evaNormal;
	else if(scoreLev == "2")commDao.evalLev = evaGood;
	else if(scoreLev == "3")commDao.evalLev = evaPefect;
}

void CDealData::TranslatePacket(const std::string strPacket,CommDaoQue& commDaoQue)
{
	std::string::size_type firstIndex = strPacket.find("<queNum>");
	std::string::size_type lastIndex = strPacket.find("</queNum>");
	std::string queNum = strPacket.substr(firstIndex+8,lastIndex-firstIndex-8);
	commDaoQue.queID = queNum.c_str();
	/////////////////////////////////////////////
	firstIndex = strPacket.find("<queName>");
	lastIndex = strPacket.find("</queName>");
	std::string queName = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
	commDaoQue.queName = queName.c_str();
	/////////////////////////////////////////////
	firstIndex = strPacket.find("<orgID>");
	lastIndex = strPacket.find("</orgID>");
	std::string orgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
	commDaoQue.orgID = orgID.c_str();
	////////////////////////////////////////
// 	firstIndex = strPacket.find("<onlyID>");
// 	lastIndex = strPacket.find("</onlyID>");
// 	std::string onlyID = strPacket.substr(firstIndex+8,lastIndex-firstIndex-8);
// 	commDaoQue.onlyID = onlyID.c_str();
}

void CDealData::TranslatePacket(const std::string strPacket,CommDaoStaff& commDaoStaff)
{
	std::string::size_type firstIndex = strPacket.find("<staffID>");
	std::string::size_type lastIndex = strPacket.find("</staffID>");
	std::string staffID = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
	commDaoStaff.staffID = staffID.c_str();
	/////////////////////////////////////////////
	firstIndex = strPacket.find("<staffName>");
	lastIndex = strPacket.find("</staffName>");
	std::string staffName = strPacket.substr(firstIndex+11,lastIndex-firstIndex-11);
	commDaoStaff.staffName = staffName.c_str();
	/////////////////////////////////////////////
	firstIndex = strPacket.find("<orgID>");
	lastIndex = strPacket.find("</orgID>");
	std::string orgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
	commDaoStaff.orgID = orgID.c_str();
	////////////////////////////////////////
// 	firstIndex = strPacket.find("<onlyID>");
// 	lastIndex = strPacket.find("</onlyID>");
// 	std::string onlyID = strPacket.substr(firstIndex+8,lastIndex-firstIndex-8);
// 	commDaoStaff.onlyID = onlyID.c_str();
}

void CDealData::TranslatePacket(const std::string strPacket,CommDaoWnd& commDaoWnd)
{
	std::string::size_type firstIndex = strPacket.find("<wndID>");
	std::string::size_type lastIndex = strPacket.find("</wndID>");
	std::string wndID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
	commDaoWnd.wndID = wndID.c_str();
	/////////////////////////////////////////////
	firstIndex = strPacket.find("<wndName>");
	lastIndex = strPacket.find("</wndName>");
	std::string wndName = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
	commDaoWnd.wndName = wndName.c_str();
	/////////////////////////////////////////////
	firstIndex = strPacket.find("<orgID>");
	lastIndex = strPacket.find("</orgID>");
	std::string orgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
	commDaoWnd.orgID = orgID.c_str();
	////////////////////////////////////////
// 	firstIndex = strPacket.find("<onlyID>");
// 	lastIndex = strPacket.find("</onlyID>");
// 	std::string onlyID = strPacket.substr(firstIndex+8,lastIndex-firstIndex-8);
// 	commDaoWnd.onlyID = onlyID.c_str();
}

BOOL CDealData::Start()
{
	m_pAnaPecket = AfxBeginThread(AnaPacket,this);
	if(!m_pAnaPecket)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CDealData::WriteDataToDB(const CommDao& commDao)
{
	if(!m_conMySql.IsConnect())
	{
		CDbaConfig dbaConfig;
		CString account = dbaConfig.GetServerAcount();
		CString ip = dbaConfig.GetServerIP();
		CString pass = dbaConfig.GetServerPassword();
		CString port = dbaConfig.GetDBPort();
		int i_port = 0;
		CCommonConvert::CStringToint(i_port,port);
		m_conMySql.ConnectToDB(ip,i_port,account,pass);
	}
	if(m_conMySql.IsConnect()){
		CString sql;
		sql.Format(_T("select * from eva where eValue='%d'"),commDao.evalLev);
		std::auto_ptr<sql::ResultSet> res = m_conMySql.ExecuteQuery(sql);
		if(res.get()){
			if(res->next()){//找到了
				sql.Format(_T("insert into zbstat (organID,client_id,time_enter,queueID,time_call,time_leave,workerID,windowID,time_score,score) values ('%s','%s','%s','%s','%s','%s','%s','%s','%s',%d)"),
					commDao.organID,commDao.clientID,commDao.timeEnter,commDao.queueID,commDao.timeCall,commDao.timeLeave,commDao.workerID,commDao.windowID,commDao.timeScore,res->getInt("eID"));
				return m_conMySql.Execute(sql);
			}
		}
	}
	return FALSE;
}

BOOL CDealData::WriteDataToDB(const CommDaoQue& commDaoQue)
{
	if(!m_conMySql.IsConnect())
	{
		CDbaConfig dbaConfig;
		CString account = dbaConfig.GetServerAcount();
		CString ip = dbaConfig.GetServerIP();
		CString pass = dbaConfig.GetServerPassword();
		CString port = dbaConfig.GetDBPort();
		int i_port = 0;
		CCommonConvert::CStringToint(i_port,port);
		m_conMySql.ConnectToDB(ip,i_port,account,pass);
	}
	if(m_conMySql.IsConnect())
	{
// 		int onlyID = 0;
// 		CCommonConvert::CStringToint(onlyID,commDaoQue.onlyID);
		CString sql;
		sql.Format(_T("select * from queue where qQueueID='%s' and qOrganID='%s'"),commDaoQue.queID
			,commDaoQue.orgID);
		std::auto_ptr<sql::ResultSet> res = m_conMySql.ExecuteQuery(sql);
		if(res.get())
		{
			if(res->next())//找到了更新
			{
				std::string strQueID = res->getString("qQueueID");
				std::string name = res->getString("qName");
				std::string organID = res->getString("qOrganID");
				CString wStrQueID(strQueID.c_str());CString wQueName(name.c_str());
				CString wOrganID(organID.c_str());
				if(commDaoQue.orgID!=wOrganID || wStrQueID!=commDaoQue.queID
					|| commDaoQue.queName!=wQueName)
				{
					sql.Format(_T("update queue set qName='%s' where qQueueID='%s' and qOrganID='%s'"),
						commDaoQue.queName,commDaoQue.queID,commDaoQue.orgID);
					return m_conMySql.Execute(sql);
				}
			}
			else//数据库没有插入
			{
				sql.Format(_T("insert into queue(qQueueID,qName,qOrganID) values ('%s','%s','%s')"),
					commDaoQue.queID,commDaoQue.queName,commDaoQue.orgID);
				return m_conMySql.Execute(sql);
			}
		}
	}
	return FALSE;
}

BOOL CDealData::WriteDataToDB(const CommDaoStaff& commDaoStaff)
{
	if(!m_conMySql.IsConnect())
	{
		CDbaConfig dbaConfig;
		CString account = dbaConfig.GetServerAcount();
		CString ip = dbaConfig.GetServerIP();
		CString pass = dbaConfig.GetServerPassword();
		CString port = dbaConfig.GetDBPort();
		int i_port = 0;
		CCommonConvert::CStringToint(i_port,port);
		m_conMySql.ConnectToDB(ip,i_port,account,pass);
	}
	if(m_conMySql.IsConnect())
	{
// 		int onlyID = 0;
// 		CCommonConvert::CStringToint(onlyID,commDaoStaff.onlyID);
		CString sql;
		sql.Format(_T("select * from worker where wWorkid='%s' and wOrganid='%s'"),
			commDaoStaff.staffID,commDaoStaff.orgID);
		std::auto_ptr<sql::ResultSet> res = m_conMySql.ExecuteQuery(sql);
		if(res.get())
		{
			if(res->next())//找到了更新
			{
				std::string strStaffID = res->getString("wWorkid");
				std::string name = res->getString("wName");
				std::string strOrgID = res->getString("wOrganID");
				CString wStaffID(strStaffID.c_str());CString wName(name.c_str());
				CString wOrgID(strOrgID.c_str());
				if(wStaffID!=commDaoStaff.staffID||wOrgID!=commDaoStaff.orgID||wName!=commDaoStaff.staffName)
				{
					sql.Format(_T("update worker set wName='%s' where wOrganID='%s' and wWorkid='%s'"),
						commDaoStaff.staffName,commDaoStaff.orgID,commDaoStaff.staffID);
					return m_conMySql.Execute(sql);
				}
			}
			else//数据库没有插入
			{
				sql.Format(_T("insert into worker(wWorkid,wName,wOrganID) values ('%s','%s','%s')"),
					commDaoStaff.staffID,commDaoStaff.staffName,commDaoStaff.orgID);
				return m_conMySql.Execute(sql);
			}
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}

BOOL CDealData::WriteDataToDB(const CommDaoWnd& commDaoWnd)
{
	if(!m_conMySql.IsConnect())
	{
		CDbaConfig dbaConfig;
		CString account = dbaConfig.GetServerAcount();
		CString ip = dbaConfig.GetServerIP();
		CString pass = dbaConfig.GetServerPassword();
		CString port = dbaConfig.GetDBPort();
		int i_port = 0;
		CCommonConvert::CStringToint(i_port,port);
		m_conMySql.ConnectToDB(ip,i_port,account,pass);
	}
	if(m_conMySql.IsConnect())
	{
// 		int onlyID = 0;
// 		CCommonConvert::CStringToint(onlyID,commDaoWnd.onlyID);
		CString sql;
		sql.Format(_T("select * from window where wWindowid='%s' and wOrganid='%s'"),
			commDaoWnd.wndID,commDaoWnd.orgID);
		std::auto_ptr<sql::ResultSet> res = m_conMySql.ExecuteQuery(sql);
		if(res.get())
		{
			if(res->next())//找到了更新
			{
				std::string strWndID = res->getString("wWindowid");
				std::string name = res->getString("wName");
				std::string strOrgID = res->getString("wOrganID");
				CString wStrWndID(strWndID.c_str());CString wName(name.c_str());
				CString wStrOrgID(strOrgID.c_str());
				if(wStrWndID!=commDaoWnd.wndID||wStrOrgID!=commDaoWnd.orgID||wName!=commDaoWnd.wndName)
				{
					sql.Format(_T("update window set wName='%s' where wOrganID='%s' and wWindowid='%s'"),
						commDaoWnd.wndName,commDaoWnd.orgID,commDaoWnd.wndID);
					return m_conMySql.Execute(sql);
				}
			}
			else//数据库没有插入
			{
				sql.Format(_T("insert into window(wWindowid,wName,wOrganID) values ('%s','%s','%s')"),
					commDaoWnd.wndID,commDaoWnd.wndName,commDaoWnd.orgID);
				return m_conMySql.Execute(sql);
			}
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}

void CDealData::TranslatePacket(const std::string strPacket,CommDaoOrg& commDaoOrg)
{
	std::string::size_type firstIndex = strPacket.find("<parOrgID>");
	std::string::size_type lastIndex = strPacket.find("</parOrgID>");
	std::string strParOrgID = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	commDaoOrg.parOrgID = strParOrgID.c_str();
	///////////////////////////////////////////////
	firstIndex = strPacket.find("<curOrgID>");
	lastIndex = strPacket.find("</curOrgID>");
	std::string strCurOrgID = strPacket.substr(firstIndex+10,lastIndex-firstIndex-10);
	commDaoOrg.curOrgID = strCurOrgID.c_str();
	///////////////////////////////////////////////
	firstIndex = strPacket.find("<parOrgName>");
	lastIndex = strPacket.find("</parOrgName>");
	std::string strParOrgName = strPacket.substr(firstIndex+12,lastIndex-firstIndex-12);
	commDaoOrg.parOrgName = strParOrgName.c_str();
	////////////////////////////////////////////////
	firstIndex = strPacket.find("<curOrgName>");
	lastIndex = strPacket.find("</curOrgName>");
	std::string strCurOrgName = strPacket.substr(firstIndex+12,lastIndex-firstIndex-12);
	commDaoOrg.curOrgName = strCurOrgName.c_str();
	///////////////////////////////////////////////
// 	firstIndex = strPacket.find("<onlyID>");
// 	lastIndex = strPacket.find("</onlyID>");
// 	std::string strOnlyID = strPacket.substr(firstIndex+8,lastIndex-firstIndex-8);
// 	commDaoOrg.onlyID = strOnlyID.c_str();
}

BOOL CDealData::WriteDataToDB(const CommDaoOrg& commDaoOrg)
{
	if(!m_conMySql.IsConnect())
	{
		CDbaConfig dbaConfig;
		CString account = dbaConfig.GetServerAcount();
		CString ip = dbaConfig.GetServerIP();
		CString pass = dbaConfig.GetServerPassword();
		CString port = dbaConfig.GetDBPort();
		int i_port = 0;
		CCommonConvert::CStringToint(i_port,port);
		m_conMySql.ConnectToDB(ip,i_port,account,pass);
	}
	if(m_conMySql.IsConnect())
	{
// 		int onlyID = 0;
// 		CCommonConvert::CStringToint(onlyID,commDaoOrg.onlyID);
		CString sql;
		sql.Format(_T("select * from organ where orOrganid='%s'"),commDaoOrg.curOrgID);
		std::auto_ptr<sql::ResultSet> res = m_conMySql.ExecuteQuery(sql);
		if(res.get())
		{
			if(res->next())//找到了更新
			{
				std::string curOrgID = res->getString("orOrganid");
				std::string curName = res->getString("orName");
				std::string parOrgID = res->getString("orParent");
				std::string parName = res->getString("orParentName");
				CString wCurOrgID(curOrgID.c_str());CString wCurName(curName.c_str());
				CString wParOrgID(parOrgID.c_str());CString wParName(parName.c_str());
				if(commDaoOrg.curOrgName!=wCurName || commDaoOrg.parOrgID!=wParOrgID 
					|| commDaoOrg.parOrgName!=wParName)
				{
					if(!commDaoOrg.parOrgID.IsEmpty())
					{
						sql.Format(_T("update organ set orName='%s',orParent='%s',orParentName='%s' where orOrganid='%s'"),
						commDaoOrg.curOrgName,commDaoOrg.parOrgID,commDaoOrg.parOrgName,commDaoOrg.curOrgID);
						return m_conMySql.Execute(sql);
					}
					else
					{
						sql.Format(_T("update organ set orName='%s',orParent=NULL,orParentName='顶级机构' where orOrganid='%s'"),
							commDaoOrg.curOrgName,commDaoOrg.curOrgID);
						return m_conMySql.Execute(sql);
					}
				}
			}
			else//数据库没有插入
			{
				if(!commDaoOrg.parOrgID.IsEmpty())
				{
 					sql.Format(_T("insert into organ(orOrganid,orName,orParent,orParentname) values ('%s','%s','%s','%s')"),
 						commDaoOrg.curOrgID,commDaoOrg.curOrgName,commDaoOrg.parOrgID,commDaoOrg.parOrgName);
 					return m_conMySql.Execute(sql);
				}
				else
				{
					sql.Format(_T("insert into organ(orOrganid,orName) values ('%s','%s')"),
						commDaoOrg.curOrgID,commDaoOrg.curOrgName);
					return m_conMySql.Execute(sql);
				}
			}
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}

void CDealData::TranslateDelQue(const std::string strPacket,CommDaoQue& commDaoQue)
{
	std::string::size_type firstIndex = strPacket.find("<queNum>");
	std::string::size_type lastIndex = strPacket.find("</queNum>");
	std::string strQueID = strPacket.substr(firstIndex+8,lastIndex-firstIndex-8);
	commDaoQue.queID = strQueID.c_str();
	firstIndex = strPacket.find("<orgID>");
	lastIndex = strPacket.find("</orgID>");
	std::string strOrgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
	commDaoQue.orgID = strOrgID.c_str();
}

void CDealData::TranslateDelWnd(const std::string strPacket,CommDaoWnd& commDaoWnd)
{
	std::string::size_type firstIndex = strPacket.find("<wndID>");
	std::string::size_type lastIndex = strPacket.find("</wndID>");
	std::string strWndID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
	commDaoWnd.wndID=strWndID.c_str();
	firstIndex = strPacket.find("<orgID>");
	lastIndex = strPacket.find("</orgID>");
	std::string strOrgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
	commDaoWnd.orgID=strOrgID.c_str();
}

void CDealData::TranslateDelStaff(const std::string strPacket,CommDaoStaff& commDaoStaff)
{
	std::string::size_type firstIndex = strPacket.find("<staffID>");
	std::string::size_type lastIndex = strPacket.find("</staffID>");
	std::string strStaffID = strPacket.substr(firstIndex+9,lastIndex-firstIndex-9);
	commDaoStaff.staffID=strStaffID.c_str();
	firstIndex = strPacket.find("<orgID>");
	lastIndex = strPacket.find("</orgID>");
	std::string strOrgID = strPacket.substr(firstIndex+7,lastIndex-firstIndex-7);
	commDaoStaff.orgID=strOrgID.c_str();
}

BOOL CDealData::WriteDelQue(const CommDaoQue& commDaoQue)
{
	if(!m_conMySql.IsConnect())
	{
		CDbaConfig dbaConfig;
		CString account = dbaConfig.GetServerAcount();
		CString ip = dbaConfig.GetServerIP();
		CString pass = dbaConfig.GetServerPassword();
		CString port = dbaConfig.GetDBPort();
		int i_port = 0;
		CCommonConvert::CStringToint(i_port,port);
		m_conMySql.ConnectToDB(ip,i_port,account,pass);
	}
	if(m_conMySql.IsConnect())
	{
		CString sql;
		sql.Format(_T("delete from queue where qQueueid='%s' and qOrganid='%s'"),
			commDaoQue.queID,commDaoQue.orgID);
		return m_conMySql.Execute(sql);
	}
	return FALSE;
}

BOOL CDealData::WriteDelWnd(const CommDaoWnd& commDaoWnd)
{
	if(!m_conMySql.IsConnect())
	{
		CDbaConfig dbaConfig;
		CString account = dbaConfig.GetServerAcount();
		CString ip = dbaConfig.GetServerIP();
		CString pass = dbaConfig.GetServerPassword();
		CString port = dbaConfig.GetDBPort();
		int i_port = 0;
		CCommonConvert::CStringToint(i_port,port);
		m_conMySql.ConnectToDB(ip,i_port,account,pass);
	}
	if(m_conMySql.IsConnect())
	{
		CString sql;
		sql.Format(_T("delete from window where wWindowid='%s' and wOrganid='%s'"),
			commDaoWnd.wndID,commDaoWnd.orgID);
		return m_conMySql.Execute(sql);
	}
	return FALSE;
}

BOOL CDealData::WriteDelStaff(const CommDaoStaff& commDaoStaff)
{
	if(!m_conMySql.IsConnect())
	{
		CDbaConfig dbaConfig;
		CString account = dbaConfig.GetServerAcount();
		CString ip = dbaConfig.GetServerIP();
		CString pass = dbaConfig.GetServerPassword();
		CString port = dbaConfig.GetDBPort();
		int i_port = 0;
		CCommonConvert::CStringToint(i_port,port);
		m_conMySql.ConnectToDB(ip,i_port,account,pass);
	}
	if(m_conMySql.IsConnect())
	{
		CString sql;
		sql.Format(_T("delete from worker where wWorkid='%s' and wOrganid='%s'"),
			commDaoStaff.staffID,commDaoStaff.orgID);
		return m_conMySql.Execute(sql);
	}
	return FALSE;
}