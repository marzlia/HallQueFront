#include "StdAfx.h"
#include "DoWebService.h"
#include "..\HallQueFront.h"
#include "..\CommonConvert.h"
#include "..\WriteLogError.h"
#include "WinSock2.h"
#pragma comment(lib,"Ws2_32.lib")
extern void MyWriteConsole(CString str);
CDoWebService::CDoWebService(void) : m_nTimeOut(1000)
, m_uPort(0)
{
	InitClient();
	/*
	string msg = "&lt;?xml version=\"1.0\" encoding=\"UTF-8\"?&gt; \
		&lt;dataPacket version=\"1.0\"&gt; \
		&lt;retCode&gt;0000&lt;/retCode&gt; \
		&lt;highestCardLevel&gt;03&lt;/highestCardLevel&gt; \
		&lt;cardLevel&gt;02&lt;/cardLevel&gt; \
		&lt;custLevel&gt;08&lt;/custLevel&gt; \
		&lt;custLevelAll&gt;08&lt;/custLevelAll&gt; \
		&lt;highestCRCDLevel&gt;50&lt;/highestCRCDLevel&gt; \
		&lt;aubm&gt;1000&lt;/aubm&gt; \
		&lt;aubmMavg&gt;1200&lt;/aubmMavg&gt; \
		&lt;aubmQavg&gt;15000&lt;/aubmQavg&gt; \
		&lt;aubmYavg&gt;2500000&lt;/aubmYavg&gt; \
		&lt;proForCust&gt;1#2#0#1#3#1#2#1#2#2#3#1#2#0#1#0&lt;/proForCust&gt; \
		&lt;reserve1&gt;&lt;/reserve1&gt; \
		&lt;reserve2&gt;&lt;/reserve2&gt; \
		&lt;reserve3&gt;&lt;/reserve3&gt; \
		&lt;reserve4&gt;&lt;/reserve4&gt; \
		&lt;reserve5&gt;&lt;/reserve5&gt; \
		&lt;/dataPacket&gt;";
	CustLev exampleCustLev;memset(&exampleCustLev,0,sizeof(CustLev));
	AnaCustLevel(msg,&exampleCustLev);
	*/
	/*
	string msg = "<?xml version=\"1.0\" encoding=\"UTF-8\"?> \
	<dataPacket version=\"1.0\"> \
	<retCode>0000</retCode> \
	<highestCardLevel>03</highestCardLevel> \
	<cardLevel>02</cardLevel> \
	<custLevel>08</custLevel> \
	<custLevelAll>08</custLevelAll> \
	<highestCRCDLevel>50</highestCRCDLevel> \
	<aubm>1000</aubm> \
	<aubmMavg>1200</aubmMavg> \
	<aubmQavg>15000</aubmQavg> \
	<aubmYavg>2500000</aubmYavg> \
	<proForCust>1#2#0#1#3#1#2#1#2#2#3#1#2#0#1#0</proForCust> \
	<reserve1></reserve1> \
	<reserve2></reserve2> \
	<reserve3></reserve3> \
	<reserve4></reserve4> \
	<reserve5></reserve5> \
	</dataPacket>";
	*/
}

CDoWebService::~CDoWebService(void)
{
	StopClient();
}

std::string CDoWebService::ProduceSendCardMsg(const CString& cardNo,const CString& host,USHORT port)
{
	CCommonConvert convert;
	//http://192.168.1.100:8080/jaxwsserver/services/T000101/getData?transCode=T000101&orgCode=001&machineCode=79787145456&typeFlag=02&cardNo=123456&transTime=2014-10-08%202010:10:10&queueLeft=10&queueCode=45465
	
	CString packet = _T("GET /bmc/services/T000103/getData?transCode=T000103");
//	CString packet = _T("GET /jaxwsserver/services/T000103/getData?transCode=T000103");
	packet.AppendFormat(_T("&orgCode=%s"),theApp.m_logicVariables.strOrganID);//机构号
	packet.AppendFormat(_T("&machineCode=%s"),theApp.m_logicVariables.strOrganNmae);//设备号
	packet+=_T("&typeFlag=02");
	packet.AppendFormat(_T("&cardNo=%s"),cardNo);
	CTime currTime = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%d-%d-%d%%20%d:%d:%d"),currTime.GetYear(),currTime.GetMonth(),currTime.GetDay(),
		currTime.GetHour(),currTime.GetMinute(),currTime.GetSecond());
	packet.AppendFormat(_T("&transTime=%s"),strTime);
	packet+=_T("&queueLeft=0");
	packet+=_T("&queueCode=001");
	packet.AppendFormat(_T(" HTTP/1.0\r\nHost:%s:%d\r\n\r\n"),host,port);
//	packet.AppendFormat(_T(" HTTP/1.1\nHost:%s:%d\r\n"),host,port);
//	packet+=_T(" HTTP/1.1\r\n\r\n");
	/////////////////////////
	int length = convert.CStringToChar(packet,NULL);
	char* strPacket = new char[length+1];
	memset(strPacket,0,length+1);
	convert.CStringToChar(packet,strPacket);
	std::string stdStrPacket(strPacket);
	delete [] strPacket;
	return stdStrPacket;
}

int CDoWebService::AnaCustLevel(const std::string& recvMsg,CustLev* pCustLev)
{
	CWriteLogError writeLogErr;
	if(recvMsg.empty())
	{
		writeLogErr.WriteErrLog(_T("刷卡返回报文为空"));
		pCustLev->isSucced = FALSE;
		return -1;
	}
	CString wRecvMsg(recvMsg.c_str());
	CString wFormatRecvMsg = _T("recvSwingCardMsg:") + wRecvMsg;
#ifdef _DEBUG
	MyWriteConsole(wFormatRecvMsg);
#endif
	std::string::size_type firstIndex = recvMsg.find("&lt;retCode&gt;");
	if(firstIndex == recvMsg.npos)
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到<retCode>"));
		pCustLev->isSucced = FALSE;
		return -1;
	}
	std::string::size_type lastIndex = recvMsg.find("&lt;/retCode&gt;");
	if(lastIndex == recvMsg.npos)
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到</retCode>"));
		pCustLev->isSucced = FALSE;
		return -1;
	}
	std::string errCode = recvMsg.substr(firstIndex+15,lastIndex-firstIndex-15);
	if(errCode=="0000")
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡交易成功"));
		pCustLev->isSucced = TRUE;
	}
	else
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡交易失败"));
		pCustLev->isSucced = FALSE;
		return -1;
	}
	/////刷卡级别
	firstIndex = recvMsg.find("&lt;cardLevel&gt;");
	if(firstIndex == recvMsg.npos)
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到<cardLevel>"));
		return -1;
	}
	lastIndex = recvMsg.find("&lt;/cardLevel&gt;");
	if(lastIndex == recvMsg.npos)
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到</cardLevel>"));
		return -1;
	}
	std::string cardLevel = recvMsg.substr(firstIndex+17,lastIndex-firstIndex-17);
	int nCardLevel = -1;
	if(!cardLevel.empty())
	{
		nCardLevel = atoi(cardLevel.c_str());
		pCustLev->cardLev = nCardLevel;
	}
	////////////最高可级别
	firstIndex = recvMsg.find("&lt;highestCardLevel&gt;");
	if(firstIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到<highestCardLevel>"));
	}
	lastIndex = recvMsg.find("&lt;/highestCardLevel&gt;");
	if(lastIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到</highestCardLevel>"));
	}
	if(firstIndex != recvMsg.npos && lastIndex != recvMsg.npos){
		string highestCardLev = recvMsg.substr(firstIndex+24,lastIndex-firstIndex-24);
		if(!highestCardLev.empty()){
			int nHighestCardLev = atoi(highestCardLev.c_str());
			pCustLev->highestCardLev = nHighestCardLev;
		}
	}
	////////////////分行客户级别
	firstIndex = recvMsg.find("&lt;custLevel&gt;");
	if(firstIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到<custLevel>"));
	}
	lastIndex = recvMsg.find("&lt;/custLevel&gt;");
	if(lastIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到</custLevel>"));
	}
	if(firstIndex != recvMsg.npos && lastIndex != recvMsg.npos){
		string custLev = recvMsg.substr(firstIndex+17,lastIndex-firstIndex-17);
		if(!custLev.empty()){
			int nCustLev = atoi(custLev.c_str());
			pCustLev->custLev = nCustLev;
		}
	}
	///////客户级别总行
	firstIndex = recvMsg.find("&lt;custLevelAll&gt;");
	if(firstIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到<custLevelAll>"));
	}
	lastIndex = recvMsg.find("&lt;/custLevelAll&gt;");
	if(lastIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到</custLevelAll>"));
	}
	if(firstIndex != recvMsg.npos && lastIndex != recvMsg.npos){
		string custLevelAll = recvMsg.substr(firstIndex+20,lastIndex-firstIndex-20);
		int nCustLevelAll = atoi(custLevelAll.c_str());
		pCustLev->custLevAll = nCustLevelAll;
	} 
	//////////信用卡最高级别
	firstIndex = recvMsg.find("&lt;highestCRCDLevel&gt;");
	if(firstIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到<highestCRCDLevel>"));
	}
	lastIndex = recvMsg.find("&lt;/highestCRCDLevel&gt;");
	if(lastIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到</highestCRCDLevel>"));
	}
	if(firstIndex != recvMsg.npos && lastIndex != recvMsg.npos){
		string highestCRCDLevel = recvMsg.substr(firstIndex+24,lastIndex-firstIndex-24);
		int nHighestCRCDLevel = atoi(highestCRCDLevel.c_str());
		pCustLev->creditCardLev = nHighestCRCDLevel;
	}
	//////////////金融资产总额
	firstIndex = recvMsg.find("&lt;aubm&gt;");
	if(firstIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到<aubm>"));
	}
	lastIndex = recvMsg.find("&lt;/aubm&gt;");
	if(lastIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到</aubm>"));
	}
	if(firstIndex != recvMsg.npos && lastIndex != recvMsg.npos){
		string aubm = recvMsg.substr(firstIndex+12,lastIndex-firstIndex-12);
		pCustLev->strAubm = aubm;
	}
	///////////////////金融资产总额月日均
	firstIndex = recvMsg.find("&lt;aubmMavg&gt;");
	if(firstIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到<aubmMavg>"));
	}
	lastIndex = recvMsg.find("&lt;/aubmMavg&gt;");
	if(lastIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到</aubmMavg>"));
	}
	if(firstIndex != recvMsg.npos && lastIndex != recvMsg.npos)
	{
		string aubmMavg = recvMsg.substr(firstIndex+16,lastIndex-firstIndex-16);
		pCustLev->strAubmMavg = aubmMavg;
	}
	///////////////////金融资产季日均
	firstIndex = recvMsg.find("&lt;aubmQavg&gt;");
	if(firstIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到<aubmQavg>"));
	}
	lastIndex = recvMsg.find("&lt;/aubmQavg&gt;");
	if(lastIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到</aubmQavg>"));
	}
	if(firstIndex != recvMsg.npos && lastIndex != recvMsg.npos)
	{
		string aubmQavg = recvMsg.substr(firstIndex+16,lastIndex-firstIndex-16);
		pCustLev->strAubmQavg = aubmQavg;
	}
	//////////////////金融资产年日均
	firstIndex = recvMsg.find("&lt;aubmYavg&gt;");
	if(firstIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到<aubmYavg>"));
	}
	lastIndex = recvMsg.find("&lt;/aubmYavg&gt;");
	if(lastIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到</aubmYavg>"));
	}
	if(firstIndex != recvMsg.npos && lastIndex != recvMsg.npos){
		string aubmYavg = recvMsg.substr(firstIndex+16,lastIndex-firstIndex-16);
		pCustLev->strAubmYavg = aubmYavg;
	}
	/////////////////////客户所持金融产品
	GetCustProduct(recvMsg,pCustLev);
	return nCardLevel;
}

void CDoWebService::GetCustProduct(const string& recvMsg,CustLev* pCustLev)
{
	string::size_type firstIndex = recvMsg.find("&lt;proForCust&gt;");
	CWriteLogError writeLogErr;
	if(firstIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到<proForCust>"));
	}
	string::size_type lastIndex = recvMsg.find("&lt;/proForCust&gt;");
	if(lastIndex == recvMsg.npos){
		writeLogErr.WriteErrLog(_T("刷卡返回报文中找不到</proForCust>"));
	}
	if(firstIndex != recvMsg.npos && lastIndex != recvMsg.npos){
		string proForCust = recvMsg.substr(firstIndex+18,lastIndex-firstIndex-18);
		string::size_type nPos = proForCust.find("#");
		pCustLev->proForCust[0] = atoi(proForCust.substr(0,nPos).c_str());
		int i = 1;
		while(nPos != proForCust.npos){
			string::size_type nLastPos = nPos;
			nPos = proForCust.find("#",nPos+1);
			string strValue = proForCust.substr(nLastPos+1,nPos-nLastPos-1);
			if(!strValue.empty()){
				int nValue = atoi(strValue.c_str()); 
				pCustLev->proForCust[i] = nValue;
			}
			if(i==14)break;
			i++;
		}
	}
}

std::string CDoWebService::ProduceDealBusMsg(const SLZData& dealData,int leftNum,const CString& host,USHORT port,BOOL isEnd)
{
	CCommonConvert convert;
	
//	CString packet = _T("GET /bmc/services/T000102/getData?transCode=T000102");
	CString packet = _T("GET /bmc/services/T000104/getData?transCode=T000104");
	packet.AppendFormat(_T("&orgCode=%s"),theApp.m_logicVariables.strOrganID);//机构号
	packet.AppendFormat(_T("&machineCode=%s"),theApp.m_logicVariables.strOrganNmae);//设备号
	packet+=_T("&typeFlag=02");
	packet.AppendFormat(_T("&cardNo=%s"),dealData.GetCardNumber());
	CTime currTime = dealData.GetTakingNumTime();
	CString strTime;
	strTime.Format(_T("%d-%d-%d%%20%d:%d:%d"),currTime.GetYear(),currTime.GetMonth(),currTime.GetDay(),
		currTime.GetHour(),currTime.GetMinute(),currTime.GetSecond());
	packet.AppendFormat(_T("&transTime=%s"),strTime);
	packet.AppendFormat(_T("&queueLeft=%d"),leftNum);
	packet.AppendFormat(_T("&queueCode=%s"),dealData.GetQueueNumber());
	CTime callTime = dealData.GetCallTime();
	CString strCallTime;
	strCallTime.Format(_T("%d-%d-%d%%20%d:%d:%d"),callTime.GetYear(),callTime.GetMonth(),callTime.GetDay(),
		callTime.GetHour(),callTime.GetMinute(),callTime.GetSecond());
	packet.AppendFormat(_T("&processBeginTime=%s"),strCallTime);
	packet.AppendFormat(_T("&handTellerNo=%d"),dealData.GetWindowShowId());
	packet.AppendFormat(_T("&tellerNo=%s"),dealData.GetStaffId());

	if(isEnd)//处理结束,完成处理
	{
		CTime finshTime = dealData.GetFinishTime();
		CString strFinshTime;
		strFinshTime.Format(_T("%d-%d-%d%%20%d:%d:%d"),finshTime.GetYear(),finshTime.GetMonth(),finshTime.GetDay(),
			finshTime.GetHour(),finshTime.GetMinute(),finshTime.GetSecond());
		packet.AppendFormat(_T("&processEndTime=%s"),strFinshTime);
	}
	else//开始处理
	{
		packet += _T("&processEndTime=9999-99-99 99:99:99");
	}

	packet.AppendFormat(_T(" HTTP/1.0\r\nHost:%s:%d\r\n\r\n"),host,port);
//	packet+=_T(" HTTP/1.1\r\n\r\n");
	/////////////////////////
	int length = convert.CStringToChar(packet,NULL);
	char* strPacket = new char[length+1];
	memset(strPacket,0,length+1);
	convert.CStringToChar(packet,strPacket);
	std::string stdStrPacket(strPacket);
	delete [] strPacket;
	return stdStrPacket;

	/*
	CCommonConvert convert;
	//http://192.168.1.100:8080/jaxwsserver/services/T000101/getData?transCode=T000101&orgCode=001&machineCode=79787145456&typeFlag=02&cardNo=123456&transTime=2014-10-08%202010:10:10&queueLeft=10&queueCode=45465

	CString packet = _T("GET /bmc/services/T000103/getData?transCode=T000103");
	//	CString packet = _T("GET /jaxwsserver/services/T000103/getData?transCode=T000103");
	packet.AppendFormat(_T("&orgCode=%s"),theApp.m_logicVariables.strOrganID);//机构号
	packet.AppendFormat(_T("&machineCode=%s"),theApp.m_logicVariables.strOrganNmae);//设备号
	packet+=_T("&typeFlag=02");
	packet.AppendFormat(_T("&cardNo=%s"),cardNo);
	CTime currTime = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%d-%d-%d%%20%d:%d:%d"),currTime.GetYear(),currTime.GetMonth(),currTime.GetDay(),
	currTime.GetHour(),currTime.GetMinute(),currTime.GetSecond());
	packet.AppendFormat(_T("&transTime=%s"),strTime);
	packet+=_T("&queueLeft=0");
	packet+=_T("&queueCode=001");
	packet.AppendFormat(_T(" HTTP/1.0\r\nHost:%s:%d\r\n\r\n"),host,port);
	//	packet.AppendFormat(_T(" HTTP/1.1\nHost:%s:%d\r\n"),host,port);
	//	packet+=_T(" HTTP/1.1\r\n\r\n");
	/////////////////////////
	int length = convert.CStringToChar(packet,NULL);
	char* strPacket = new char[length+1];
	memset(strPacket,0,length+1);
	convert.CStringToChar(packet,strPacket);
	std::string stdStrPacket(strPacket);
	delete [] strPacket;
	return stdStrPacket;
	*/
}

int CDoWebService::AnaBusErrcode(const std::string& recvMsg)
{
	CWriteLogError writeLogErr;
	int nErrCode = 0;
	if(recvMsg.empty())
	{
		writeLogErr.WriteErrLog(_T("客户办理业务返回报文为空"));
		nErrCode = -1;
		return nErrCode;
	}

	CString wRecvMsg(recvMsg.c_str());
	CString wFormatRecvMsg = _T("recvDealBusMsg:") + wRecvMsg;

	std::string::size_type firstIndex = recvMsg.find("&lt;retCode&gt;");//("<retCode>");
	if(firstIndex == recvMsg.npos)
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("客户办理业务返回报文中找不到<retCode>"));
		nErrCode = -2;
		return nErrCode;
	}
	std::string::size_type lastIndex = recvMsg.find("&lt;/retCode&gt;");
	if(lastIndex == recvMsg.npos)
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("客户办理业务返回报文中找不到</retCode>"));
		nErrCode = -2;
		return nErrCode;
	}
	std::string errCode = recvMsg.substr(firstIndex+15,lastIndex-firstIndex-15);
	if(errCode == "0000")
	{
		nErrCode = 1;
	}
	else if(errCode == "1004")
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("客户办理业务交易号为空"));
		nErrCode = -3;
	}
	else if(errCode == "1028")
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("客户办理业务查询结果为空"));
		nErrCode = -4;
	}
	else if(errCode == "1029")
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("客户办理业务该支行不存在上级分行"));
		nErrCode = -5;
	}
	else if(errCode == "2015")
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("客户办理业务报文格式错误"));
		nErrCode = -6;
	}
	else if(errCode == "2016")
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("客户办理业务交易码错误"));
		nErrCode = -7;
	}
	else if(errCode == "3009")
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("客户办理业务数据库访问失败"));
		nErrCode = -8;
	}
	else if(errCode == "9999")
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		writeLogErr.WriteErrLog(_T("客户办理业务处理失败"));
		nErrCode = -9;
	}
	else
	{
		writeLogErr.WriteErrLog(wFormatRecvMsg);
		nErrCode = 0;
	}
	return nErrCode;
}
BOOL CDoWebService::ConnectToServer(SOCKET& cliSock,const CString& host,USHORT port,int waitSec)
{
	cliSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET == cliSock)
	{
		WSACleanup();
#ifdef _DEBUG
		MyWriteConsole(_T("socket error"));
#endif
		CWriteLogError log;
		log.WriteErrLog(_T("socket error"));
		return FALSE;
	}
	SOCKADDR_IN ServerAddr;
	ServerAddr.sin_port = htons(port);
	ServerAddr.sin_family = AF_INET;
	///转换
	CCommonConvert convert;
	int len = convert.CStringToChar(host,NULL);

	char* buf = new char[len+1]; 
	ZeroMemory(buf,len+1);
	convert.CStringToChar(host,buf);
	////////////////////////////////////////////
	LPHOSTENT lphost = gethostbyname(buf);
	if (lphost != NULL)
		ServerAddr.sin_addr.S_un.S_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
	delete [] buf;
	if(SOCKET_ERROR==connect(cliSock,(SOCKADDR*)&ServerAddr,
		sizeof(ServerAddr)))
	{
#ifdef _DEBUG
		MyWriteConsole(_T("connect error"));
#endif
		CWriteLogError log;
		log.WriteErrLog(_T("connect error"));
		closesocket(cliSock);
		return FALSE;
	}
	return TRUE;
}

int CDoWebService::GetCardLevelFromServer(const CString& host,const CString& cardNo,USHORT port,int waitTimeSec,CustLev *pCustLev)
{
	ASSERT(pCustLev != NULL);
	SOCKET cliSock;//客户端socket
	int nCardLev = -1;
	if(!ConnectToServer(cliSock,host,port,waitTimeSec))
	{
		pCustLev->isSucced = FALSE;
		return nCardLev;
	}
	std::string sendBuf = ProduceSendCardMsg(cardNo,host,port);
	sendBuf=CCommonConvert::string_To_UTF8(sendBuf);
	setsockopt(cliSock,SOL_SOCKET,SO_SNDTIMEO,(char *)&m_nTimeOut,sizeof(UINT));
	if(SOCKET_ERROR == send(cliSock,sendBuf.c_str(),sendBuf.size(),0))
	{
#ifdef _DEBUG
		MyWriteConsole(_T("send failed"));
#endif
		CWriteLogError log;
		CString strErrCode;
		strErrCode.Format(_T("send failed:%d"),WSAGetLastError());
		log.WriteErrLog(strErrCode);
		closesocket(cliSock);
		pCustLev->isSucced = FALSE;
		return nCardLev;
	}
	else
	{
		CWriteLogError log;
		CString strErrCode(sendBuf.c_str());
		strErrCode = _T("send succed:") + strErrCode;
		log.WriteErrLog(strErrCode);
#ifdef _DEBUG
		MyWriteConsole(strErrCode);
#endif
	}

	std::string strRecvBuf;
	char recvBuf[MAXRECVBUF+1]={0};
	setsockopt(cliSock,SOL_SOCKET,SO_RCVTIMEO,(char *)&m_nTimeOut,sizeof(UINT));
	while(1)
	{
		memset(recvBuf,0,MAXRECVBUF+1);
		int actRecvSize = recv(cliSock,recvBuf,MAXRECVBUF,0);
		if(actRecvSize == SOCKET_ERROR || actRecvSize == 0)
		{
#ifdef _DEBUG
			DWORD errCode = GetLastError();
			CString str = _T("recv error:");
			str.AppendFormat(_T("%d"),errCode);
			MyWriteConsole(str);
#endif
			CWriteLogError log;
			CString strErrCode;
			strErrCode.Format(_T("recv error:%d"),WSAGetLastError());
			log.WriteErrLog(strErrCode);
			closesocket(cliSock);
//			pCustLev->isSucced = FALSE;
			break;
		}
		else
		{
			std::string strTemp(recvBuf);
			strRecvBuf += strTemp;
			int pos = strRecvBuf.find("&lt;/dataPacket&gt;");
			if(pos != strRecvBuf.npos)
			{
				break;
			}
		}
	}
	/*
	int actRecvSize = recv(cliSock,recvBuf,MAXRECVBUF,0);
	if(actRecvSize==SOCKET_ERROR)
	{
#ifdef _DEBUG
		DWORD errCode = GetLastError();
		CString str = _T("recv error:");
		str.AppendFormat(_T("%d"),errCode);
		MyWriteConsole(str);
#endif
		CWriteLogError log;
		CString strErrCode;
		strErrCode.Format(_T("recv error:%d"),WSAGetLastError());
		log.WriteErrLog(strErrCode);
		closesocket(cliSock);
		pCustLev->isSucced = FALSE;
		return nCardLev;	
	}
	std::string strRecvBuf(recvBuf);
	while(actRecvSize!=0 && actRecvSize != SOCKET_ERROR)
	{
		memset(recvBuf,0,MAXRECVBUF+1);
		actRecvSize = recv(cliSock,recvBuf,MAXRECVBUF,0);
		if(actRecvSize>0)
		{
			std::string strTemp(recvBuf);
			strRecvBuf += strTemp;
		}
	}
	*/
	closesocket(cliSock);
	strRecvBuf = CCommonConvert::UTF8_To_string(strRecvBuf);
//	strRecvBuf = "&lt;?xml version=\"1.0\" encoding=\"UTF-8\"?&gt;&lt;dataPacket version=\"1.0\"&gt;&lt;retCode&gt;0000&lt;/retCode&gt;&lt;highestCardLevel&gt;03&lt;/highestCardLevel&gt;&lt;cardLevel&gt;02&lt;/cardLevel&gt;&lt;custLevel&gt;08&lt;/custLevel&gt;&lt;custLevelAll&gt;08&lt;/custLevelAll&gt;&lt;highestCRCDLevel&gt;50&lt;/highestCRCDLevel&gt;&lt;aubm&gt;1000&lt;/aubm&gt;&lt;aubmMavg&gt;1200&lt;/aubmMavg&gt;&lt;aubmQavg&gt;15000&lt;/aubmQavg&gt;&lt;aubmYavg&gt;2500000&lt;/aubmYavg&gt;&lt;proForCust&gt;1#2#0#1#3#1#2#1#2#2#3#1#2#0#1#0&lt;/proForCust&gt;&lt;reserve1&gt;&lt;/reserve1&gt;&lt;reserve2&gt;&lt;/reserve2&gt;&lt;reserve3&gt;&lt;/reserve3&gt;&lt;reserve4&gt;&lt;/reserve4&gt;&lt;reserve5&gt;&lt;/reserve5&gt;&lt;/dataPacket&gt;";
	nCardLev = AnaCustLevel(strRecvBuf,pCustLev);
	return nCardLev;
}

int CDoWebService::SendDealBusMsg(const CString& host,const SLZData& data,USHORT port,int waitTimeSec,int leftNum,BOOL isEnd)
{
	int nResult = -1;
	SOCKET cliSock;
	if(!ConnectToServer(cliSock,host,port,waitTimeSec))
	{
		return nResult;
	}
//	if(data.GetCardNumber().IsEmpty()){return nResult;}
	std::string sendBuf = ProduceDealBusMsg(data,leftNum,host,port,isEnd);
	sendBuf=CCommonConvert::string_To_UTF8(sendBuf);
	setsockopt(cliSock,SOL_SOCKET,SO_SNDTIMEO,(char *)&m_nTimeOut,sizeof(UINT));
	if(SOCKET_ERROR == send(cliSock,sendBuf.c_str(),sendBuf.size(),0))
	{
#ifdef _DEBUG
		MyWriteConsole(_T("send failed"));
#endif
		CWriteLogError log;
		CString strErrCode;
		strErrCode.Format(_T("send failed:%d"),WSAGetLastError());
		log.WriteErrLog(strErrCode);
		closesocket(cliSock);
		return nResult;
	}
	else
	{
		CWriteLogError log;
		CString strErrCode(sendBuf.c_str());
		strErrCode = _T("send succed:") + strErrCode;
		log.WriteErrLog(strErrCode);
#ifdef _DEBUG
		MyWriteConsole(strErrCode);
#endif
	}
	/*
	char recvBuf[MAXRECVBUF+1]={0};
	setsockopt(cliSock,SOL_SOCKET,SO_RCVTIMEO,(char *)&m_nTimeOut,sizeof(UINT));
	std::string strRecvBuf;
	while(1)
	{
		memset(recvBuf,0,MAXRECVBUF+1);
		int actRecvSize = recv(cliSock,recvBuf,MAXRECVBUF,0);
		if(actRecvSize == 0 || actRecvSize == SOCKET_ERROR)
		{
#ifdef _DEBUG
			DWORD errCode = GetLastError();
			CString str = _T("recv error:");
			str.AppendFormat(_T("%d"),errCode);
			MyWriteConsole(str);
#endif
			CWriteLogError log;
			CString strErrCode;
			strErrCode.Format(_T("recv error:%d"),WSAGetLastError());
			log.WriteErrLog(strErrCode);
			closesocket(cliSock);
			return nResult;
		}
		else
		{
			std::string strTemp(recvBuf);
			strRecvBuf += strTemp;
			int npos = strRecvBuf.find("&lt;/dataPacket&gt;");
			if(npos != strRecvBuf.npos)
			{
				break;
			}
		}
	}
	/*
	int actRecvSize = recv(cliSock,recvBuf,MAXRECVBUF,0);
	if(actRecvSize == 0 || actRecvSize == SOCKET_ERROR)
	{
#ifdef _DEBUG
		DWORD errCode = GetLastError();
		CString str = _T("recv error:");
		str.AppendFormat(_T("%d"),errCode);
		MyWriteConsole(str);
#endif
		CWriteLogError log;
		CString strErrCode;
		strErrCode.Format(_T("recv error:%d"),WSAGetLastError());
		log.WriteErrLog(strErrCode);
		closesocket(cliSock);
		return nResult;
	}
	std::string strRecvBuf(recvBuf);
	while(actRecvSize!=0 && actRecvSize != SOCKET_ERROR)
	{
		memset(recvBuf,0,MAXRECVBUF+1);
		actRecvSize = recv(cliSock,recvBuf,MAXRECVBUF,0);
		std::string strTemp(recvBuf);
		strRecvBuf += strTemp;
	}
	*/
	closesocket(cliSock);
	
//	strRecvBuf = CCommonConvert::UTF8_To_string(strRecvBuf);
//	nResult = AnaBusErrcode(strRecvBuf);
	return nResult;
}

BOOL CDoWebService::InitClient()
{
	WSADATA wsaData;
	//初始化
	if(SOCKET_ERROR==WSAStartup(MAKEWORD(2,2),&wsaData))
	{
#ifdef _DEBUG
		MyWriteConsole(_T("WSAStartup error"));
#endif
		CWriteLogError log;
		log.WriteErrLog(_T("WSAStartup error"));
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

void CDoWebService::StopClient()
{
#ifdef _DEBUG
	MyWriteConsole(_T("WSACleanup"));
#endif
	WSACleanup();
}