#include "StdAfx.h"
#include "ConnectToMySql.h"
#include "../HallQueFront/CommonConvert.h"
#include "../HallQueFront/WriteLogError.h"


CConnectToMySql::CConnectToMySql(void) : m_pSqlDriver(NULL)
,m_pSqlCon(NULL)
,m_pSqlState(NULL)
{
	
//	ReadDataBaseInfo();
//	CString c_ip = m_dataBaseInfo.GetIPAddress();
//	CString c_port = m_dataBaseInfo.GetPort();
//	convert.CStringToint(i_port,c_port);
//	ConnectToDB(c_ip,i_port,m_dataBaseInfo.GetUserName(),m_dataBaseInfo.GetPassWord());
}

CConnectToMySql::~CConnectToMySql(void)
{
	if(m_pSqlState)
	{
		m_pSqlState->close();
		delete m_pSqlState;
		m_pSqlState = NULL;
	}
	if(m_pSqlCon)
	{
		m_pSqlCon->close();
		delete m_pSqlCon;
		m_pSqlCon = NULL;
	}
}

BOOL CConnectToMySql::ConnectToDB(const CString& IP,const USHORT port,const CString& UserName, const CString& passWord)
{
	std::string connectIP,connectUserName,connectPassWord;
	CCommonConvert convert;
	CString c_port;
	convert.intToCString(port,c_port);
	CString c_ip = _T("tcp://")+IP+_T(":")+c_port;
	/*
	int len = convert.CStringToChar(c_ip,NULL);
	char* pConnectIP = new char[len+1];
	convert.CStringToChar(c_ip,pConnectIP);

	len = convert.CStringToChar(UserName,NULL);
	char* pUserName = new char[len+1];
	convert.CStringToChar(UserName,pUserName);

	len = convert.CStringToChar(passWord,NULL);
	char* pPassWord = new char[len+1];
	convert.CStringToChar(passWord,pPassWord);
	*/
	/*从CString到string的转换*/		
	CStringA a_ip(c_ip.GetBuffer(0));
	c_ip.ReleaseBuffer(0);
	connectIP = a_ip.GetBuffer(0);
	a_ip.ReleaseBuffer(0);
	CString c_usrName = UserName;
	CStringA a_UserName(c_usrName.GetBuffer(0));
	c_usrName.ReleaseBuffer(0);
	connectUserName = a_UserName.GetBuffer(0);
	a_UserName.ReleaseBuffer(0);
	CString c_passWord = passWord;
	CStringA a_passWord(c_passWord.GetBuffer(0));
	c_passWord.ReleaseBuffer(0);
	connectPassWord = a_passWord.GetBuffer(0);
	a_passWord.ReleaseBuffer(0);
		
	try
	{
		m_pSqlDriver = get_driver_instance();
		ASSERT(m_pSqlDriver!=NULL);
		m_pSqlCon = m_pSqlDriver->connect(connectIP,connectUserName,connectPassWord);
		ASSERT(m_pSqlCon!=NULL);
		m_pSqlCon->setSchema("newinfoman");
		//m_pSqlCon->setSchema("infoman");
		m_pSqlCon->setClientOption("CHARSET", "GB2312");
		m_pSqlState = m_pSqlCon->createStatement();
		ASSERT(m_pSqlState!=NULL);
	}
	catch(sql::SQLException &e)
	{
		CString errcode=_T("# ERR: ");
		std::string what = e.what();
		errcode += what.c_str();
		errcode += _T("");
		errcode+=_T("(Mysql error code:");
		errcode.AppendFormat(_T("%d"),e.getErrorCode());
		errcode+=_T(",SQLState:");
		errcode.AppendFormat(_T("%s"),e.getSQLState());
		errcode+=_T(")");
#ifdef _DEBUG
		AfxMessageBox(errcode);
#endif
		CWriteLogError log;
		log.WriteErrLog(errcode);
//		AfxMessageBox(_T("连接数据库失败"));
		return FALSE;
	}
	return TRUE;
}

BOOL CConnectToMySql::Execute(const CString& sql)
{
	if(m_pSqlCon==NULL || m_pSqlState==NULL || m_pSqlDriver==NULL)
		return FALSE;
	CString c_sql=sql;
	CStringA a_sql(c_sql.GetBuffer(0));
	c_sql.ReleaseBuffer(0);
	std::string executeSql = a_sql.GetBuffer(0);
	a_sql.ReleaseBuffer(0);
	try
	{
		/*m_sqlStatement->execute("SET CHARSET GB2312");*/
		m_pSqlState->execute(executeSql);
	}
	catch(sql::SQLException &e)
	{
		CString errcode=_T("# ERR: ");
		std::string what = e.what();
		errcode += what.c_str();
		errcode += _T("");
		errcode+=_T("(Mysql error code:");
		errcode.AppendFormat(_T("%d"),e.getErrorCode());
		errcode+=_T(",SQLState:");
		errcode.AppendFormat(_T("%s"),e.getSQLState());
		errcode+=_T(")");
#ifdef _DEBUG
		AfxMessageBox(errcode);
#endif
		CWriteLogError log;
		log.WriteErrLog(errcode);
		return FALSE;
	}
	return TRUE;
}

std::auto_ptr<sql::ResultSet> CConnectToMySql::ExecuteQuery(const CString& sql)
{
	CString c_sql=sql;
	CStringA a_sql(c_sql.GetBuffer(0));
	c_sql.ReleaseBuffer(0);
	std::string executeSql = a_sql.GetBuffer(0);
	a_sql.ReleaseBuffer(0);
 	
//		return (std::auto_ptr<sql::ResultSet> nullRes);
	std::auto_ptr<sql::ResultSet> sqlRes(NULL);
	
	if(m_pSqlCon==NULL || m_pSqlState==NULL || m_pSqlDriver==NULL)
	{
		return sqlRes;
	}
	try
	{
		m_pSqlState->execute("SET CHARSET GB2312");
		std::auto_ptr<sql::ResultSet> res(m_pSqlState->executeQuery(executeSql));
		sqlRes = res;
	}
	catch(sql::SQLException &e)
	{
		CString errcode=_T("# ERR: ");
		std::string what = e.what();
		errcode += what.c_str();
		errcode += _T("");
		errcode+=_T("(Mysql error code:");
		errcode.AppendFormat(_T("%d"),e.getErrorCode());
		errcode+=_T(",SQLState:");
		errcode.AppendFormat(_T("%s"),e.getSQLState());
		errcode+=_T(")");
#ifdef _DEBUG
		AfxMessageBox(errcode);
#endif
		CWriteLogError log;
		log.WriteErrLog(errcode);
	}
	return sqlRes;
}

// BOOL CConnectToMySql::ReadDataBaseInfo()
// {
// 	CFile file;
// 	CFileException e;
// 	if (file.Open(m_dataBasePath,CFile::modeRead,&e))
// 	{
// 		CDataBaseSetInfo* databaseinfo=NULL;
// 		CArchive ar(&file,CArchive::load);
// 		if (file.GetLength()) 
// 			do
// 			{
// 				ar>>databaseinfo;
// 				if (databaseinfo)
// 				{
// 					m_dataBaseInfo = *databaseinfo;
// 					delete databaseinfo;
// 					databaseinfo = NULL;
// 				}
// 			}while(!ar.IsBufferEmpty());
// 			ar.Close();
// 			file.Close();
// 	}
// 	else return FALSE;
// 	return TRUE;
// }

BOOL CConnectToMySql::IsConnect()
{
	if(m_pSqlCon==NULL || m_pSqlDriver==NULL || m_pSqlState==NULL)
	{
		return FALSE;
	}
	return !m_pSqlCon->isClosed();
}