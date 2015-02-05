#pragma once

#include <afxwin.h>
#include <afxmt.h>
#include "jTTS_ML.h"
#include "DataDef.h"
#include "JttsAPI.h"
#include "SLZWindowQueryView.h"
#include "SLZData.h"
#include "PlaySoundThread.h"
#include <list>
#pragma comment(lib, "jTTS_ML.lib")
///////信息中转结构体/////////
typedef struct _tagVoiceAndDisplayStr
{
	CString		strAd;		//广告
	CString		strVoiceStr;	//声音内容
	CString		strDisplayStr;	//显示内容
	int			iWndScreenId;		//窗口屏地址
	CString		strConsultStbId;		
	int			iLEDPhyId;	//物理地址
	int			iLEDPipeId;		//通道
	int			iWndComId;		//综合屏地址
	UINT		iShowTime;	
	int			iWndid;     //窗口ID
} VOICEDISPLAYSTR, *LPVOICEDISPLAYSTR;

////////显示计时结构体///////
typedef struct _tagShowMsgTime
{
	CString strAd;	//广告
	UINT address;	//窗口屏地址
	UINT LedAddress;	//通屏物理地址
	UINT LedPipe;	//通屏通道
	UINT ComId;	//综合屏地址
	int	iWndid;	//窗口ID
	UINT time;
}ShowMsgTime,*LPSHOWMSGTIME;

class SoundPlay
{
public:
	~SoundPlay();

	BOOL Init();
	BOOL End();

/////外部调用显示发声接口///////////
	BOOL DataPlay(const SLZData& Data,BOOL bWait=FALSE);
	BOOL DataPlay(const CString strPlay);
///////////////////////
	static UINT PlayVoiceThread(LPVOID pParam);
private:

	BOOL AddPlayText(VOICEDISPLAYSTR& vdstr);
	CList<VOICEDISPLAYSTR> m_PlayDataList;    ////////中转结构体链表

	CWinThread* m_pPlayVoiceThread;
	BOOL m_bPlayVoiceThread;
	CMutex m_mtPlayStrList;
	CMutex m_mtShowMsgList;//LED定时显示广告语的线程锁
	int m_iSoundReplayTimes;		//声音播放次数		
	//CMutex m_mtThreadPlayVoice;
	//CEvent* m_pevtKillThread;
	SLZWindowQueryView& m_windowTable;
	////////////信息提取 字符串转换//////////
	CString ChangeCallStr(const SLZData& data,SLZWindow* WindowInfo);
	CString ChangeShowStr(const SLZData& data,SLZWindow* WindowInfo);
	CString ChangeWaitCallStr(const SLZData& data,SLZWindow* WindowInfo);
	CString ChangeWaitShowStr(const SLZData& data,SLZWindow* WindowInfo);
	CString m_strSoundPlay;
	//////////计时回调函数////////
	static void CALLBACK MyDoOutTimerMsg(
		HWND hwnd, // handle of window for timer messages
		UINT uMsg, // WM_TIMER message
		UINT idEvent, // timer identifier
		DWORD dwTime // current system time	
		);
	CList<ShowMsgTime,ShowMsgTime&> m_MsgShowList;
	BOOL WriteReg(void);
	UINT iSpeed;
	void PlayJtts(CString strChinese);
	void InsertSplit(CString& strinsert,CString strsplt);//添加分隔符
	///////转换英语//////
	CString InsertSplitEng(CString strinsert);
	CString InsertJttsEng(CString strinsert);
	CString m_strWavLibPath;
	CString ReplaceQueNum(CString strQueNum);
public:
	int GetReplayTimes();
	void SetSoundReplayTimes(int iTimes);
	void SetSoundSpeed(const UINT Speed)
	{
		iSpeed = Speed;
	}
	static SoundPlay* GetInstance(SLZWindowQueryView& windowTable);
	/*{

	static SoundPlay Instance(windowTable);
	return &Instance;
	}*/
private:
	SoundPlay(SLZWindowQueryView& windowTable);
	BOOL ShowAdven();//开机显示广告
	std::list<SLZWindow> m_list_WindowInfo; //窗口信息
	BOOL ReadWindowInfo();
	CString m_WindowPath;
	CStringList m_WavList;

	int SplitNumStrToNumbers(const CString& strNumSrc, CStringArray& strarrDest);			 //////转换数字格式到数组中
	int SplitCusNumToStrArr(const CString& strSrc, CStringArray& strarrDest);
	int SplitStrToCharArray(const CString& strSrc, CStringArray& strarrDest);
	int SplitStrToNumArray(CString strSrc,CStringArray&strarrDest);					/////分割汉字与非汉字
	CString InsertSplitToArray(CStringArray&strarrOrigin,CString strSplit);			///// 向数组中插入字符
	CString RemoveChinese(CString strOrigin);			////////删除字符串汇中的汉字
	BOOL StrHasNum(CString strOrigin);						//// 字符串中是否有数字
	CString Getthesame(CString strSrc);						//////提取字符串中和第一个字符类型相同的字符串（汉字或非汉字）
	BOOL PlayTheVoice(CString strVoice,const CStringList& WavList);			
	CString PlayTheVoiceFromRight(CString strVoice,const CStringList& WavList);			////////从字符串的右侧循环发声
};