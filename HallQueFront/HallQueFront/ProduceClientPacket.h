#pragma once
#include <string>
#include "SocketBufDef.h"
#include "SLZData.h"
using namespace std;
class CProduceClientPacket
{
public:
	CProduceClientPacket(void);
	~CProduceClientPacket(void);
	CString ProduceSearchOrgInfo(); 
	CString ProduceSearchQueInfo(const CString& orgID);
	CString ProduceSearchWndInfo(const CString& orgID);
	CString ProduceSearchStaffInfo(const CString& orgID);
	CString ProduceSendPacket(const SLZData& data,int leftNum,SendDataType dataType,CString cardNum=_T(""),CardType cardType=cardNotDefine);
	int JudgePacketRet(const std::string strPacket);
	CString ProduceQuePacket(const CString& queNum,const CString& queName,const CString& orgID);
	CString ProduceStaffPacket(const CString& staffID,const CString& staffName,const CString& orgID);
	CString ProduceWndPacket(const CString& wndID,const CString& wndName,const CString& orgID);
	CString ProduceOrgPacket(const CString& curOrgID,const CString& curOrgName,const CString& parOrgID,const CString& parOrgName);
	CString ProduceDelQue(const CString& queID,const CString& orgID);
	CString ProduceDelWnd(const CString& wndID,const CString& orgID);
	CString ProduceDelStaff(const CString& staffID,const CString& orgID);
	CString ProducePauseTime(const CString& organID,const CString& staffID,int windowID,const CTime& startTime,const CTime& endTime,int nSec);
private:
	CString ExChangeTime(const CTime& time);
};
