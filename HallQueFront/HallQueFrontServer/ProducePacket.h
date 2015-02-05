#pragma once
#include <string>
#include "../HallQueFront/SocketBufDef.h"
#include "ConnectToMySql.h"
using namespace std;

class CProducePacket
{
public:
	CProducePacket(void);
	~CProducePacket(void);
	int JudgeSendPacket(const std::string strPacket);
	std::string ProduceSendRet(int errCode,const std::string strPacket);
//	CString ExChangeTime(const CTime& time);
private:
	CConnectToMySql m_mySql;
};
