#pragma once
#include <string>
#include "SLZData.h"
#include "SocketBufDef.h"
using namespace std;
class CProducePacket
{
public:
	CProducePacket(void);
	~CProducePacket(void);
	CString ProduceSendPacket(const SLZData& data,int leftNum,SendDataType dataType,CString cardNum=_T(""),CardType cardType=cardNotDefine);
	
	int JudgePacketRet(const std::string strPacket);
	int JudgeSendPacket(const std::string strPacket);
	std::string ProduceSendRet(int errCode);
private:
	CString ExChangeTime(const CTime& time);
};
