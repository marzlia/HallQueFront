#pragma once

#include <afxwin.h>
#include <afxmt.h>
#include "jTTS_ML.h"
#pragma comment(lib, "jTTS_ML.lib")
//#include "NurseDef.h"

class JttsAPI
{
private:
	JttsAPI();

public:
	~JttsAPI();
	static JttsAPI* GetInstance();
	BOOL SetPlay(
		UINT uDeviceID, //如果为WAVE_MAPPER(-1)，表示缺省的放音设备，否则为指定的放音设备。 
		HWND hwnd, //一个合法的窗口句柄。当系统阅读时和完毕后，会发送一些自定义消息到此窗口。如果为NULL，则不使用窗口消息的方式通知应用程序。
		JTTS_CALLBACKPROC lpfnCallback, //一个合法的回调函数地址。当系统阅读时和完毕后，会调用此函数。
		DWORD dwUserData //用户传入的自定义数据，回调时将回传给回调函数
		);
	//同步播放（播放时阻塞）
	static BOOL Play(CString strVoice);
	//void ErrorMessage(ERRCODE err);
private:
	BOOL InitTTS();
	BOOL EndTTS();
	BOOL WriteReg(void);
private:

	JTTS_CONFIG m_config;
	char* m_szInitPath;
	char* m_szSerialNo;
	char* m_szVoiceInfo;

	static JttsAPI* m_pThis;

};