#pragma once

#include <afxwin.h>
#include <afxmt.h>
#include "jTTS_ML.h"
#include "DataDef.h"
#include "JttsAPI.h"
#include "SLZWindowQueryView.h"
#include "SLZData.h"
#include "PlaySoundThread.h"
#include "ThroughWndScreenInfo.h"
#include <list>
#pragma comment(lib, "jTTS_ML.lib")
///////��Ϣ��ת�ṹ��/////////
typedef struct _tagVoiceAndDisplayStr
{
public:
	_tagVoiceAndDisplayStr(void){
		iShowTime = 0;
		iWndid = 0;
	}
	~_tagVoiceAndDisplayStr(void){}
	_tagVoiceAndDisplayStr(const _tagVoiceAndDisplayStr& obj){
		strAd = obj.strAd;
		strVoiceStr = obj.strVoiceStr;
		strDisplayStr = obj.strDisplayStr;
		strConsultStbId = obj.strConsultStbId;
		iShowTime = obj.iShowTime;
		iWndid = obj.iWndid;

		ScreenWndInfoArray.RemoveAll();
		for(int i=0;i<obj.ScreenWndInfoArray.GetCount();i++)
		{
			ScreenWndInfoArray.Add(obj.ScreenWndInfoArray.GetAt(i));
		}
	}
	_tagVoiceAndDisplayStr& operator = (const _tagVoiceAndDisplayStr& obj){
		if(&obj == this)return *this;
		strAd = obj.strAd;
		strVoiceStr = obj.strVoiceStr;
		strDisplayStr = obj.strDisplayStr;
		strConsultStbId = obj.strConsultStbId;
		iShowTime = obj.iShowTime;
		iWndid = obj.iWndid;

		ScreenWndInfoArray.RemoveAll();
		for(int i=0;i<obj.ScreenWndInfoArray.GetCount();i++)
		{
			ScreenWndInfoArray.Add(obj.ScreenWndInfoArray.GetAt(i));
		}
		return *this;
	}
public:
	CString		strAd;		//���
	CString		strVoiceStr;	//��������
	CString		strDisplayStr;	//��ʾ����
	CString		strConsultStbId;		
	UINT		iShowTime;	
	int			iWndid;     //����ID
	CArray<CThroughWndScreenInfo> ScreenWndInfoArray;//������������
} VOICEDISPLAYSTR, *LPVOICEDISPLAYSTR;

////////��ʾ��ʱ�ṹ��///////
typedef struct _tagShowMsgTime
{
	CString strAd;	//���
	UINT address;	//��������ַ
	UINT LedAddress;	//ͨ�������ַ
	UINT LedPipe;	//ͨ��ͨ��
	UINT ComId;	//�ۺ�����ַ
	int	iWndid;	//����ID
	UINT time;
	CString localIP;//ͬƵIP��ַ
}ShowMsgTime,*LPSHOWMSGTIME;

class SoundPlay
{
public:
	~SoundPlay();

	BOOL Init();
	BOOL End();

/////�ⲿ������ʾ�����ӿ�///////////
	BOOL DataPlay(const SLZData& Data,BOOL bWait=FALSE);
	BOOL DataPlay(const CString strPlay);
///////////////////////
	static UINT PlayVoiceThread(LPVOID pParam);
private:

	BOOL AddPlayText(VOICEDISPLAYSTR& vdstr);
	CList<VOICEDISPLAYSTR> m_PlayDataList;    ////////��ת�ṹ������

	CWinThread* m_pPlayVoiceThread;
	BOOL m_bPlayVoiceThread;
	CMutex m_mtPlayStrList;
	CMutex m_mtShowMsgList;//LED��ʱ��ʾ�������߳���
	int m_iSoundReplayTimes;		//�������Ŵ���		
	//CMutex m_mtThreadPlayVoice;
	//CEvent* m_pevtKillThread;
	SLZWindowQueryView& m_windowTable;
	////////////��Ϣ��ȡ �ַ���ת��//////////
	CString ChangeCallStr(const SLZData& data,SLZWindow* WindowInfo);
	CString ChangeShowStr(const SLZData& data,SLZWindow* WindowInfo);
	CString ChangeWaitCallStr(const SLZData& data,SLZWindow* WindowInfo);
	CString ChangeWaitShowStr(const SLZData& data,SLZWindow* WindowInfo);
	CString m_strSoundPlay;
	//////////��ʱ�ص�����////////
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
	void InsertSplit(CString& strinsert,CString strsplt);//��ӷָ���
	///////ת��Ӣ��//////
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
	BOOL ShowAdven();//������ʾ���
	std::list<SLZWindow> m_list_WindowInfo; //������Ϣ
	BOOL ReadWindowInfo();
	CString m_WindowPath;
	CStringList m_WavList;

	int SplitNumStrToNumbers(const CString& strNumSrc, CStringArray& strarrDest);			 //////ת�����ָ�ʽ��������
	int SplitCusNumToStrArr(const CString& strSrc, CStringArray& strarrDest);
	int SplitStrToCharArray(const CString& strSrc, CStringArray& strarrDest);
	int SplitStrToNumArray(CString strSrc,CStringArray&strarrDest);					/////�ָ����Ǻ���
	CString InsertSplitToArray(CStringArray&strarrOrigin,CString strSplit);			///// �������в����ַ�
	CString RemoveChinese(CString strOrigin);			////////ɾ���ַ������еĺ���
	BOOL StrHasNum(CString strOrigin);						//// �ַ������Ƿ�������
	CString Getthesame(CString strSrc);						//////��ȡ�ַ����к͵�һ���ַ�������ͬ���ַ��������ֻ�Ǻ��֣�
	BOOL PlayTheVoice(CString strVoice,const CStringList& WavList);			
	CString PlayTheVoiceFromRight(CString strVoice,const CStringList& WavList);			////////���ַ������Ҳ�ѭ������
};