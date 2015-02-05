#ifndef CONFIGCARDDEF_H
#define CONFIGCARDDEF_H
#include "TrackDef.h"
////////////识别卡信息
typedef struct _tagDistinguishCard
{
	UINT postion;//位置
	UINT toValue;//对应的值
}DISTINGUISHCARD,*LPDISTINGUISHCARD;
////////////////////////////
/////////////卡对接链接信息
typedef struct _tagCardConnectInfo
{
public:
	_tagCardConnectInfo(){
		IsConnect = FALSE;
		memset(ServerIP,0,addNum);
		ServerPort = 0;
		OverTime = 0;
		memset(RegAttchQueID,0,addNum);
		RegAttchPageID = -1;
	}
	~_tagCardConnectInfo(){}
public:
	BOOL IsConnect;//是否和服务器对接
	WCHAR ServerIP[addNum];//ip
	UINT ServerPort;//端口
	UINT OverTime;//超时时间
	WCHAR RegAttchQueID[addNum];//身份证关联ID
	int RegAttchPageID;//身份证关联界面ID
}CARDCONNECTINFO,*LPCARDCONNECTINFO;
/////////////////////////////////
#endif