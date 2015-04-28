#pragma once
#include <string>
using namespace std;

class SLZData;

typedef enum SlzDataType{
	enumSlzDataSerialid,
	enumSlzDatabussName,
	enumSlzDataqueId,
	enumSlzDataqueManNum,
	enumSlzDataqueNum,
	enumSlzDataintQueNum,
	enumSlzDatacardType,
	enumSlzDatacardNum,
	enumSlzDatacustName,
	enumSlzDatacustLevel,
	enumSlzDatatakeNumTime,
	enumSlzDatastaffId,
	enumSlzDataphoneNum,
	enumSlzDatasendMsg
};

class CDealInterMsg
{
public:
	CDealInterMsg(void);
	~CDealInterMsg(void);
public:
	static void ProduceSendInterMsg(const CString& queManNum,string& retSendMsg);//产生发送联机取号的报文
	static void ProduceRetInterMsg(const SLZData* pData,
		UINT inlineNum/*队列当前人数*/,string& retMsg/*返回的msg*/);//产生联机取号，返回的报文
	static void ProduceSendInNumMsg(const CString& queManNum,string& retInlineNumMsg);//产生联机取号发送排队人数报文
	static void ProduceRetInNumMsg(UINT inlineNum,string& retInlineMsg);//产生联机取号返回排队人数报文
	static void ProduceSendCallMsg(const CString& queManNum,string& retCallMsg,const CString& organId);//呼叫时联机取号产生的消息
	static void ProduceRetCallMsg(BOOL isSucced,string& retMsg,const SLZData* pData);//呼叫联机取号信息返回
public:
	static BOOL AnaRetInterMsg(const string& retMsg,SLZData* pData,UINT* pInLineNum=NULL);//分析返回的联机取号报文，得到当前队列的最大号和排队人数
	static BOOL AnaRetInNumMsg(const string& retMsg,UINT* pInlineNum);//分析返回的当前排队人数的报文,得到当前队列排队人数

	static BOOL AnaSendInterMsg(const string& retSendMsg,CString& queManNum,CString& organId,CString& organName);//分析发送的联机取号报文，得到取号的队列ID
	static BOOL AnaSendInNumMsg(const string& retSendMsg,CString& queManNum);//分析发送的获取排队队列人数的报文,得到队列ID


	static BOOL AnaSendCallMsg(const string& retSendMsg,CString& queManNum,CString& organId);//分析发送的呼叫联机取号产生的消息

	static BOOL AnaRetCallMsg(const string& retMsg,BOOL* pIsSucced,SLZData* pData);//分析返回的联机取号呼叫产生的消息/报文
private:
	static BOOL GetMsgData(const string& msg,SlzDataType dataType,string& data);
	static void StringToTime(const string& strTime,CTime& time);
};
