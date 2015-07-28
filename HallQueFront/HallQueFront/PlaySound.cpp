#include "stdafx.h"
#include "PlaySound.h"
#include "HallQueFront.h"
#include "CommonStrMethod.h"
#include "SLZCWndScreen.h"
#include "DataDef.h"
#include "CommonConvert.h"
#include <Mmsystem.h>
#include <Shlwapi.h>
#include "SLZWindow.h"

#pragma comment(lib, "WINMM.LIB")
extern void MyWriteConsole(CString str); 
SoundPlay* playsound;
SoundPlay::SoundPlay(SLZWindowQueryView& windowTable)
:m_iSoundReplayTimes(1)
,m_pPlayVoiceThread(NULL)
,m_windowTable(windowTable)
,iSpeed(5)
{
	GetReplayTimes();
	SetTimer(NULL,0,1000,MyDoOutTimerMsg);
	playsound = this;//SoundPlay::GetInstance(windowTable);
	m_strWavLibPath = CommonStrMethod::GetModuleDir() + _T("wavLib\\");
	m_WindowPath = CommonStrMethod::GetModuleDir() + _T("Window.dat");
	if (Init())
	{
		ShowAdven();				//������ʾ���
	}
}

SoundPlay::~SoundPlay()
{
	if(m_pPlayVoiceThread)
	{
		WaitForSingleObject(m_pPlayVoiceThread->m_hThread,20);
		TerminateThread(m_pPlayVoiceThread->m_hThread,0);
		delete m_pPlayVoiceThread;
		m_pPlayVoiceThread = NULL;
	}
	//jTTS_End();
}

SoundPlay* SoundPlay::GetInstance(SLZWindowQueryView& windowTable)
{
	static SoundPlay Instance(windowTable);
	return &Instance;
}

BOOL SoundPlay::Init()
{
	WriteReg();
	ERRCODE err;
	err = jTTS_Init(NULL,NULL);
	if (err != ERR_NONE && err != ERR_ALREADYINIT)
	{
		AfxMessageBox(_T("jTTS������ʼ������"));
		return FALSE; // ��ʼ��ʧ��
	}
	BOOL bRet = ::PathFileExists(m_strWavLibPath);
	if (!bRet)
	{
		CString str;
		str.Format(_T("������·��������:%s"), m_strWavLibPath);
		AfxMessageBox(str);

		return FALSE;
	}
	//
	
	CFileFind finder;
	BOOL bFound = finder.FindFile(CommonStrMethod::GetModuleDir() + _T("wavLib\\*.wav"));
	while(bFound)
	{
		bFound = finder.FindNextFile();
		m_WavList.AddTail(finder.GetFileTitle());
	}
	finder.Close();
	m_pPlayVoiceThread = AfxBeginThread(PlayVoiceThread, this);

	return TRUE;
}

BOOL SoundPlay::ShowAdven()
{
	if (ReadWindowInfo())			//��������Ϣ
	{
		std::list<SLZWindow>::iterator itera = m_list_WindowInfo.begin();
		for (itera;itera!=m_list_WindowInfo.end();itera++)
		{
			CString strAdmsg = itera->GetAdMsg();
			if (!strAdmsg.IsEmpty())
			{

				SLZCWndScreen* WndScreen = SLZCWndScreen::GetInstance();

				int count = itera->m_throughscreen_array.GetCount();
				
				int nLedPhyId = 0;
				int nLedPipeId = 0;
				int nWndScreenId = 0;
				int nComScreenId = 0;
				for(int i=0;i<count;i++)
				{
					nLedPhyId = itera->m_throughscreen_array.GetAt(i).GetPhyId();
					nLedPipeId = itera->m_throughscreen_array.GetAt(i).GetPipeId();
					nWndScreenId = itera->m_throughscreen_array.GetAt(i).GetWndScreenId();
					nComScreenId = itera->m_throughscreen_array.GetAt(i).GetComScreenId();

					if (nLedPhyId != 0 && nLedPipeId != 0)
					{
						WndScreen->AddThroughScreenMsg(strAdmsg,nLedPhyId,nLedPipeId,itera->m_throughscreen_array.GetAt(i).GetLocalIp());
					}

					if(nWndScreenId != 0)
					{
						WndScreen->AddScreenMsg(strAdmsg,nWndScreenId);
					}

					if(nComScreenId != 0)
					{
						WndScreen->AddScreenMsg(strAdmsg,nComScreenId);
					}
				}
			}
		}
	}
	else return FALSE;
	return TRUE;
}

BOOL SoundPlay::ReadWindowInfo()
{
	m_list_WindowInfo.clear();
	CFile file;
	CFileException e;
	if (file.Open(m_WindowPath,CFile::modeRead,&e))
	{
		SLZWindow* pWindow = NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength())
		{
			do 
			{
				ar>>pWindow;
				if (pWindow)
				{
					m_list_WindowInfo.push_back(*pWindow);
					delete pWindow;
					pWindow = NULL;
				}
			} while (!ar.IsBufferEmpty());
		}
	}
	else return FALSE;
	return TRUE;
}

BOOL SoundPlay::DataPlay(const SLZData& Data,BOOL bWait)
{
	VOICEDISPLAYSTR vst;
	UINT uWindowId = Data.GetWindowId();    //��ȡ����ID
	vst.iWndid = uWindowId;
	SLZWindow WindowInfo;
	BOOL flag = m_windowTable.QueryWindowById(uWindowId,WindowInfo);
	if(flag)
	{
		if (bWait)
		{
			vst.strVoiceStr = ChangeWaitCallStr(Data,&WindowInfo);
			vst.strDisplayStr = ChangeWaitShowStr(Data,&WindowInfo);
		}
		else
		{
			vst.strVoiceStr = ChangeCallStr(Data,&WindowInfo);
			vst.strDisplayStr = ChangeShowStr(Data,&WindowInfo);
		}
		/////��ȡ��ͬ��Ļ��ַ////
//		vst.iLEDPhyId = WindowInfo.GetLEDPhyId();
//		vst.iLEDPipeId = WindowInfo.GetLEDPipeId();
//		vst.iWndScreenId = WindowInfo.GetWndScreenId();
//		vst.iWndComId = WindowInfo.GetComScreenId();
		vst.strAd = WindowInfo.GetAdMsg();
		vst.iShowTime = WindowInfo.GetMsgShowTime();

		int count = WindowInfo.m_throughscreen_array.GetCount();
		for(int i=0;i<count;i++)
		{
			vst.ScreenWndInfoArray.Add(WindowInfo.m_throughscreen_array.GetAt(i));
//			AddPlayText(vst);
		}
		
		AddPlayText(vst);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL SoundPlay::DataPlay(const CString strPlay)
{ 

	if (!strPlay.IsEmpty())
	{
		VOICEDISPLAYSTR vst;
		vst.strVoiceStr = strPlay;
		AddPlayText(vst);
		return TRUE;
	}
	else return FALSE;
}

CString SoundPlay::ChangeCallStr(const SLZData& data,SLZWindow* WindowInfo)
{
	CString strPlaySound = WindowInfo->GetCallMsg();
	CString strQueNum = data.GetQueueNumber();
	CString strCallName = WindowInfo->GetWindowCallName();
	CString strBussName = data.GetBussName();
	if (theApp.m_logicVariables.IsUseJtts)						/////ʹ��JTTS������
	{
		if (theApp.m_logicVariables.iNumberCallType==2)   ////////һ����ģʽֱ���滻
		{
			CString strChange = strQueNum;
			InsertSplit(strChange,L"#");	
			strPlaySound.Replace(_T("[�ŶӺ���]"),strChange);
			strChange = strCallName;
			InsertSplit(strChange,L"#");	
			strPlaySound.Replace(_T("[��������]"),strChange);
		}
		if (theApp.m_logicVariables.iNumberCallType==1)
		{
				CStringArray strarr;
				SplitCusNumToStrArr(strQueNum,strarr);    ////ת���ŶӺ���
				if (strarr.GetCount()>0)
				{
					CString strQuenumChange = strarr[0];
					for (int i=1;i<strarr.GetCount();i++)
					{
						strQuenumChange += _T("#")+strarr[i];
					}
					strPlaySound.Replace(_T("[�ŶӺ���]"),strQuenumChange);
				}
				CStringArray strarr2;
				SplitCusNumToStrArr(strCallName,strarr2);   /////////ת����������
				if (strarr2.GetCount()>0)
				{
					CString strWndNameChange = strarr2[0];
					for (int i=1;i<strarr2.GetCount();i++)
					{
						strWndNameChange += _T("#")+strarr2[i];
					}
					strPlaySound.Replace(_T("[��������]"),strWndNameChange);
				}
		}
	}
	else
	{			
		CStringArray strArr;
		SplitStrToNumArray(strCallName,strArr);   ///�ָ�������ƺ�����Ǻ���
		
		if (theApp.m_logicVariables.iNumberCallType==2)   ///////////////һ����ģʽ
		{
			InsertSplit(strQueNum,_T("#"));
			strPlaySound.Replace(_T("[�ŶӺ���]"),strQueNum);
			for (int i=0;i<strArr.GetCount();i++)
			{
				CString strOrigin = strArr.GetAt(i);
				if (StrHasNum(strOrigin))							/////////ֻת�����������е�����
				{
					InsertSplit(strOrigin,L"#");
				}
				strArr.SetAt(i,strOrigin);
			}
			CString strChange = InsertSplitToArray(strArr,L"#");
			strPlaySound.Replace(_T("[��������]"),strChange);
		}
		if (theApp.m_logicVariables.iNumberCallType==1)         /////һ�ٶ�ʮ��ģʽ
		{
			CStringArray strarr;
			SplitCusNumToStrArr(strQueNum,strarr);
			CString strChangeNum;
			strChangeNum = InsertSplitToArray(strarr,L"#");                      ////�ֺõ������в���#��
			strPlaySound.Replace(_T("[�ŶӺ���]"),strChangeNum);

			for (int i=0;i<strArr.GetCount();i++)
			{
				CString strOrigin = strArr.GetAt(i);
				if (StrHasNum(strOrigin))
				{
					CStringArray strarr;
					SplitCusNumToStrArr(strOrigin,strarr);
					strArr.RemoveAt(i);
					strArr.InsertAt(i,&strarr);
					i += strarr.GetCount()-1;				
				}
			}
			CString strChangeCall = InsertSplitToArray(strArr,L"#");
			strPlaySound.Replace(_T("[��������]"),strChangeCall);
		}
	}
	strPlaySound.Replace(_T("[ҵ������]"),strBussName);
	CString strQueNumEng;
	CString strWndNameEng;
	CString strWndNameNum = RemoveChinese(strCallName);
	strQueNum = data.GetQueueNumber();
	InsertSplit(strQueNum,L"#");
	InsertSplit(strWndNameNum,L"#");
	if (theApp.m_logicVariables.IsUseJtts)
	{
		strQueNumEng= InsertJttsEng(strQueNum);
		strWndNameEng = InsertJttsEng(strWndNameNum);
	}
	else
	{
		strQueNumEng = InsertSplitEng(strQueNum);
		strWndNameEng = InsertSplitEng(strWndNameNum);
	}
	strPlaySound.Replace(_T("[�ŶӺ���(Ӣ)]"),strQueNumEng);
	strPlaySound.Replace(_T("[��������(Ӣ)]"),_T("number#")+strWndNameEng+_T("#window"));
	return strPlaySound;
}

CString SoundPlay::ChangeShowStr(const SLZData& data,SLZWindow* WindowInfo)
{
	CString strPlaySound = WindowInfo->GetShowMsg();
	strPlaySound.Replace(_T("[�ŶӺ���]"),data.GetQueueNumber());
	strPlaySound.Replace(_T("[�ͻ�����]"),data.GetCustName());
	strPlaySound.Replace(_T("[��������]"),WindowInfo->GetWindowName());
	strPlaySound.Replace(_T("[ҵ������]"),data.GetBussName());
	CString strwndname = WindowInfo->GetWindowName();
	strwndname.Remove(_T('��'));
	strwndname.Remove(_T('��'));
	strwndname.Remove(_T('��'));
	strPlaySound.Replace(_T("[�ŶӺ���(Ӣ)]"),data.GetQueueNumber());
	strPlaySound.Replace(_T("[��������(Ӣ)]"),_T("No.")+strwndname+_T(" window"));
	return strPlaySound;
}

CString SoundPlay::ChangeWaitCallStr(const SLZData& data,SLZWindow* WindowInfo)
{
	CString strPlaySound = WindowInfo->GetWaitCallMsg();
	CString strQueNum = data.GetQueueNumber();
	CString strCallName = WindowInfo->GetWindowCallName();
	CString strBussName = data.GetBussName();
	if (theApp.m_logicVariables.IsUseJtts)						/////ʹ��JTTS������
	{
		if (theApp.m_logicVariables.iNumberCallType==2)   ////////һ����ģʽֱ���滻
		{
			strPlaySound.Replace(_T("[�ŶӺ���]"),strQueNum);
			strPlaySound.Replace(_T("[��������]"),strCallName);
		}
		if (theApp.m_logicVariables.iNumberCallType==1)
		{
			CStringArray strarr;
			SplitCusNumToStrArr(strQueNum,strarr);    ////ת���ŶӺ���
			if (strarr.GetCount()>0)
			{
				CString strQuenumChange = strarr[0];
				for (int i=1;i<strarr.GetCount();i++)
				{
					strQuenumChange += _T("#")+strarr[i];
				}
				strPlaySound.Replace(_T("[�ŶӺ���]"),strQuenumChange);
			}
			CStringArray strarr2;
			SplitCusNumToStrArr(strCallName,strarr2);   /////////ת����������
			if (strarr2.GetCount()>0)
			{
				CString strWndNameChange = strarr2[0];
				for (int i=1;i<strarr2.GetCount();i++)
				{
					strWndNameChange += _T("#")+strarr2[i];
				}
				strPlaySound.Replace(_T("[��������]"),strWndNameChange);
			}
		}
	}
	else
	{			
		CStringArray strArr;
		SplitStrToNumArray(strCallName,strArr);   ///�ָ�������ƺ�����Ǻ���

		if (theApp.m_logicVariables.iNumberCallType==2)   ///////////////һ����ģʽ
		{
			InsertSplit(strQueNum,_T("#"));
			strPlaySound.Replace(_T("[�ŶӺ���]"),strQueNum);
			for (int i=0;i<strArr.GetCount();i++)
			{
				CString strOrigin = strArr.GetAt(i);
				if (StrHasNum(strOrigin))							/////////ֻת�����������е�����
				{
					InsertSplit(strOrigin,L"#");
				}
				strArr.SetAt(i,strOrigin);
			}
			CString strChange = InsertSplitToArray(strArr,L"#");
			strPlaySound.Replace(_T("[��������]"),strChange);
		}
		if (theApp.m_logicVariables.iNumberCallType==1)         /////һ�ٶ�ʮ��ģʽ
		{
			CStringArray strarr;
			SplitCusNumToStrArr(strQueNum,strarr);
			CString strChangeNum;
			strChangeNum = InsertSplitToArray(strarr,L"#");                      ////�ֺõ������в���#��
			strPlaySound.Replace(_T("[�ŶӺ���]"),strChangeNum);

			for (int i=0;i<strArr.GetCount();i++)
			{
				CString strOrigin = strArr.GetAt(i);
				if (StrHasNum(strOrigin))
				{
					CStringArray strarr;
					SplitCusNumToStrArr(strOrigin,strarr);
					strArr.RemoveAt(i);
					strArr.InsertAt(i,&strarr);
					i += strarr.GetCount()-1;

				}
			}
			CString strChangeCall = InsertSplitToArray(strArr,L"#");
			strPlaySound.Replace(_T("[��������]"),strChangeCall);
		}
	}
	strPlaySound.Replace(_T("[ҵ������]"),strBussName);
	CString strQueNumEng;
	CString strWndNameEng;
	CString strWndNameNum = RemoveChinese(strCallName);
	strQueNum = data.GetQueueNumber();
	InsertSplit(strQueNum,L"#");
	InsertSplit(strWndNameNum,L"#");
	if (theApp.m_logicVariables.IsUseJtts)
	{
		strQueNumEng= InsertJttsEng(strQueNum);
		strWndNameEng = InsertJttsEng(strWndNameNum);
	}
	else
	{
		strQueNumEng = InsertSplitEng(strQueNum);
		strWndNameEng = InsertSplitEng(strWndNameNum);
	}
	strPlaySound.Replace(_T("[�ŶӺ���(Ӣ)]"),strQueNumEng);
	strPlaySound.Replace(_T("[��������(Ӣ)]"),_T("number#")+strWndNameEng+_T("#window"));
	return strPlaySound;
}

CString SoundPlay::ChangeWaitShowStr(const SLZData& data,SLZWindow* WindowInfo)
{
	CString strPlaySound = WindowInfo->GetWaitShowMsg();
	strPlaySound.Replace(_T("[�ŶӺ���]"),data.GetQueueNumber());
	strPlaySound.Replace(_T("[�ͻ�����]"),data.GetCustName());
	strPlaySound.Replace(_T("[��������]"),WindowInfo->GetWindowName());
	strPlaySound.Replace(_T("[ҵ������]"),data.GetBussName());
	CString strwndname = WindowInfo->GetWindowName();
	strwndname.Remove(_T('��'));
	strwndname.Remove(_T('��'));
	strwndname.Remove(_T('��'));
	strPlaySound.Replace(_T("[�ŶӺ���(Ӣ)]"),data.GetQueueNumber());
	strPlaySound.Replace(_T("[��������(Ӣ)]"),_T("No.")+strwndname+_T(" window"));
	return strPlaySound;
}

/////////ת���ŶӺ�/////


void SoundPlay::InsertSplit(CString &strinsert,CString strsplt)
{
	CString s1;
	int len = (strinsert.GetLength() -1);
	for(int i=len;i>0;i--)
	{
		s1 = strinsert.Right(strinsert.GetLength()-i);
		strinsert = strinsert.Left(i) + strsplt;
		strinsert += s1;
	}
}

CString SoundPlay::InsertSplitEng(CString strinsert)
{
	strinsert.Replace(_T("0"),_T("e0"));
	strinsert.Replace(_T("1"),_T("e1"));
	strinsert.Replace(_T("2"),_T("e2"));
	strinsert.Replace(_T("3"),_T("e3"));
	strinsert.Replace(_T("4"),_T("e4"));
	strinsert.Replace(_T("5"),_T("e5"));
	strinsert.Replace(_T("6"),_T("e6"));
	strinsert.Replace(_T("7"),_T("e7"));
	strinsert.Replace(_T("8"),_T("e8"));
	strinsert.Replace(_T("9"),_T("e9"));
	return strinsert;
}

CString SoundPlay::InsertJttsEng(CString strinsert)
{
	strinsert.Replace(_T("0"),_T("zero"));
	strinsert.Replace(_T("1"),_T("one"));
	strinsert.Replace(_T("2"),_T("two"));
	strinsert.Replace(_T("3"),_T("three"));
	strinsert.Replace(_T("4"),_T("four"));
	strinsert.Replace(_T("5"),_T("five"));
	strinsert.Replace(_T("6"),_T("six"));
	strinsert.Replace(_T("7"),_T("seven"));
	strinsert.Replace(_T("8"),_T("eight"));
	strinsert.Replace(_T("9"),_T("nine"));
	return strinsert;
}

BOOL SoundPlay::End()
{
	if(m_pPlayVoiceThread)
	{
		m_bPlayVoiceThread = FALSE;
		DWORD dwRet = ::WaitForSingleObject(m_pPlayVoiceThread->m_hThread, 30);
		if(dwRet == WAIT_OBJECT_0)
		{
			//AfxMessageBox(_T("Jtts�̰߳�ȫ�˳�"));
		}

		else
		{
			//::TerminateThread(m_pPlayVoiceThread->m_hThread, 0);
		}
	}
	//JttsAPI::EndTTS();

	return TRUE;
}

BOOL SoundPlay::AddPlayText(VOICEDISPLAYSTR& vdstr)
{
	if(vdstr.strVoiceStr.IsEmpty())
	{
		return FALSE;
	}
	m_mtPlayStrList.Lock();
	if(!m_PlayDataList.IsEmpty())
	{
		if(vdstr.strVoiceStr==m_PlayDataList.GetTail().strVoiceStr)
		{
			m_mtPlayStrList.Unlock();
			return FALSE;
		}
	}
	m_PlayDataList.AddTail(vdstr);
	m_mtPlayStrList.Unlock();
	return TRUE;
}

void CALLBACK SoundPlay::MyDoOutTimerMsg(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	POSITION pos;
	int count = playsound->m_MsgShowList.GetCount();
	for (int i=0;i<count;i++)
	{
		ShowMsgTime ShowTime;
		pos = playsound->m_MsgShowList.FindIndex(i);
		if(!pos) break;
		ShowTime = playsound->m_MsgShowList.GetAt(pos);
		SLZWindow Window;
		BOOL flag = playsound->m_windowTable.QueryWindowById(ShowTime.iWndid,Window);
		if(!flag)//û���ҵ���Ӧ�Ĵ���
		{
			playsound->m_MsgShowList.RemoveAt(pos);//ɾ��
			break;
		}
		if (ShowTime.time<Window.GetMsgShowTime())
		{
			ShowTime.time++;
			playsound->m_MsgShowList.SetAt(pos,ShowTime);
// #ifdef _DEBUG
// 			CString strErrcode;
// 			strErrcode.Format(_T("wndid:%d,wndAdd:%d,ComAdd:%d,TroughAdd:%d,TroughPipe:%d,time:%d"),
// 				ShowTime.iWndid,ShowTime.address,ShowTime.ComId,ShowTime.LedAddress,ShowTime.LedPipe,ShowTime.time);
// 			MyWriteConsole(strErrcode);
// #endif
		}
		if (ShowTime.time>=Window.GetMsgShowTime())
		{
			//ShowMsgTime ShowOutTime = playsound->m_MsgShowList.GetAt(pos);
// #ifdef _DEBUG
// 			CString strErrcode;
// 			strErrcode.Format(_T("wndid:%d,wndAdd:%d,ComAdd:%d,TroughAdd:%d,TroughPipe:%d,time:%d"),
// 				ShowTime.iWndid,ShowTime.address,ShowTime.ComId,ShowTime.LedAddress,ShowTime.LedPipe,ShowTime.time);
// 			MyWriteConsole(strErrcode);
// #endif
			SLZCWndScreen* pWndScreen = SLZCWndScreen::GetInstance();
			if (ShowTime.address!=0)
			{
				pWndScreen->AddScreenMsg(ShowTime.strAd,ShowTime.address);
			}
			if (ShowTime.ComId!=0)
			{
				pWndScreen->AddScreenMsg(ShowTime.strAd,ShowTime.ComId);
			}
			if (ShowTime.LedAddress!=0&&ShowTime.LedPipe!=0)
			{
				pWndScreen->AddThroughScreenMsg(ShowTime.strAd,ShowTime.LedAddress,ShowTime.LedPipe,ShowTime.localIP);
			}
			playsound->m_MsgShowList.RemoveAt(pos);
		}
	}
}

UINT SoundPlay::PlayVoiceThread(LPVOID pParam)
{
	SoundPlay* pThis = (SoundPlay*)pParam;
	pThis->m_bPlayVoiceThread = TRUE;

	while(pThis->m_bPlayVoiceThread)
	{
		//if(!pThis->m_mtThreadPlayVoice.Lock(50))
		//{
		//	break;
		//}
		if(pThis->m_PlayDataList.GetCount())
		{
			pThis->m_mtPlayStrList.Lock();
			VOICEDISPLAYSTR PlayStr = pThis->m_PlayDataList.GetHead();
			pThis->m_PlayDataList.RemoveHead();
			pThis->m_mtPlayStrList.Unlock();
			

			SLZCWndScreen* pWndScreen = SLZCWndScreen::GetInstance();
			int count = PlayStr.ScreenWndInfoArray.GetCount();
			CThroughWndScreenInfo WndScreenInfo;
			for(int i=0;i<count;i++)
			{
				WndScreenInfo = PlayStr.ScreenWndInfoArray.GetAt(i);
				if (WndScreenInfo.GetWndScreenId() != 0 || (WndScreenInfo.GetPhyId() != 0 && WndScreenInfo.GetPipeId() !=0 ) 
					|| WndScreenInfo.GetComScreenId() != 0)
				{
					ShowMsgTime showtime={0};   //��ʼ����ʱ�ṹ��
					showtime.iWndid = PlayStr.iWndid;
					showtime.strAd = PlayStr.strAd;
					if (WndScreenInfo.GetWndScreenId() != 0)
					{
					
						pWndScreen->AddScreenMsg(PlayStr.strDisplayStr,WndScreenInfo.GetWndScreenId());
						showtime.address = WndScreenInfo.GetWndScreenId();

					}
					if (WndScreenInfo.GetPhyId() != 0 && WndScreenInfo.GetPipeId() != 0)
					{
						
						pWndScreen->AddThroughScreenMsg(PlayStr.strDisplayStr,WndScreenInfo.GetPhyId(),WndScreenInfo.GetPipeId(),WndScreenInfo.GetLocalIp());
						showtime.LedAddress = WndScreenInfo.GetPhyId();
						showtime.LedPipe = WndScreenInfo.GetPipeId();
						showtime.localIP = WndScreenInfo.GetLocalIp();
					}
					if (WndScreenInfo.GetComScreenId() != 0)
					{
						
						pWndScreen->AddScreenMsg(PlayStr.strDisplayStr,WndScreenInfo.GetComScreenId());
						showtime.ComId = WndScreenInfo.GetComScreenId();
					}
					ShowMsgTime checkshow={0};		//���ṹ��
					pThis->m_mtShowMsgList.Lock();
					int length = pThis->m_MsgShowList.GetCount();
					for (int i =0;i<length;i++)
					{
						POSITION posFind = pThis->m_MsgShowList.FindIndex(i);
						if(!posFind)break;
						checkshow = pThis->m_MsgShowList.GetAt(posFind);
						if (checkshow.iWndid==showtime.iWndid)
						{
							pThis->m_MsgShowList.RemoveAt(posFind);			//���
						}
					}
					pThis->m_MsgShowList.AddTail(showtime);
					pThis->m_mtShowMsgList.Unlock();
				}
			}
//////////////////////��������//////////////////////////
			for(int i=0;i<pThis->m_iSoundReplayTimes;i++)
			{
				if (!PlayStr.strVoiceStr.IsEmpty())
				{
					if (theApp.m_logicVariables.IsUseJtts)
					{
						CStringArray soundarray;
						CommonStrMethod::StrSplit(PlayStr.strVoiceStr,soundarray,_T("#"));
						for (int i=0;i< soundarray.GetCount();i++)
						{
							pThis->PlayJtts(soundarray.GetAt(i));
						}
					}
					else
					{
						CStringArray soundarray;
						CommonStrMethod::StrSplit(PlayStr.strVoiceStr,soundarray,_T("#"));
						for (int j=0;j<soundarray.GetCount();j++)
						{
							CString strjts = soundarray.GetAt(j);
							pThis->PlayTheVoice(strjts,pThis->m_WavList);
						}
					}
				}
			}
		}
		//pThis->m_mtThreadPlayVoice.Unlock();
		else
		{
			Sleep(20);
		}
	}
	::ExitThread(0);

	return 0;
}

void SoundPlay::PlayJtts(CString strChinese)
{
	int nLength = strChinese.GetLength() * 2 + 1;
	char* buf = new char[(nLength+1)*sizeof(char)];
	memset(buf, 0, nLength);
	CommonStrMethod::WChar2Char(buf, nLength, strChinese.GetBuffer());
	buf[nLength]='\0';
	JTTS_CONFIG config={0};
	int sounderr = jTTS_Get(&config);
	config.nSpeed=iSpeed;
	sounderr = jTTS_Set(&config);
	UINT err= jTTS_Play(buf, PLAYMODE_SYNC);
	delete [] buf;
	buf = NULL;
}



int SoundPlay::GetReplayTimes()
{
	CString strExePath = CommonStrMethod::GetModuleDir();
	strExePath += _T("\\SYS\\sysconfig.ini");
	CFile file;
	if(!CommonStrMethod::PathFileExist(strExePath))
	{
		return 1;
	}
	WCHAR wszTimes[11] = {0};
	GetPrivateProfileString(_T("sys"),_T("SOUND_REPLAY_TIMES"), NULL, wszTimes, 11, strExePath);
	CHAR szTimes[11];
	memset(szTimes, 0, 11);
	CommonStrMethod::WChar2Char(szTimes, 11, wszTimes);
	int iTimes = atoi((char*)szTimes);
	if(iTimes < 1)
	{
		iTimes = 1;
	}
	m_iSoundReplayTimes = iTimes;
	return iTimes ;
}

void SoundPlay::SetSoundReplayTimes(int iTimes)
{
	m_iSoundReplayTimes = iTimes;
}

/*
BOOL TTSAPI JTTSCallBackProc(WORD wParam, LONG lParam, DWORD dwUserData)
{
CString str;
CMainFrame * pFrame  = (CMainFrame *) AfxGetMainWnd();
CStatusBar * pBar = &pFrame->m_wndStatusBar;
switch(wParam)
{
case NOTIFY_BEGIN:
{
g_lPrevPos = 0;
str.Format("Notify - Begin - Total Size: %ld", lParam);
break;
}
case NOTIFY_END:
{
str.Format("Notify - End - Return Code: %ld", lParam);
break;
}
case NOTIFY_SENTEND:
{
g_lPrevPos = lParam;	// ��Ϊһ��Ľ�������һ�俪ʼ����ͬʱ������Ϊ��ʾ�������ȱ�����ֵ���ȵ������¾俪ʼʱ��ͳһ��ʾ
break;
}
case NOTIFY_SENTBEGIN:
{
str.Format("Notify - Reading From %ld To %ld", g_lPrevPos, lParam);
break;
}
case NOTIFY_MARK:
{
DWORD dwOffset = GETMARKOFFSET(lParam);
WORD wLen = GETMARKLEN(lParam);
str.Format("Notify - Mark Offset: %ld Len: %d", dwOffset, wLen);
break;
}
default:
str.Format("Notify - Unknow, lParam: %ld", lParam);
break;
}

SendMessage(pBar->m_hWnd, (UINT) SB_SETTEXT, 0, (LPARAM)(LPCTSTR)str);

return TRUE;
}
*/

BOOL SoundPlay::WriteReg(void)
{
	CString strModulePath = CommonStrMethod::GetModuleDir();
	//CString strSerialNo = GetProfileString(_T("Config"), _T("SerialNo"), _T("4C3E77924B936234"));
	CString strJttsSerialNo(_T("4C3E77924B936234"));

	HKEY hKey;
	LONG ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		_T("Software\\SinoVoice\\jTTS4_Professional\\"), 0, KEY_WRITE, &hKey);
	if(ERROR_SUCCESS != ret)
	{

		LPCTSTR lpSubKey = _T("Software\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegCreateKey:HKEY_LOCAL_MACHINE\\Software\\"));
			return FALSE;
		}
		lpSubKey = _T("SinoVoice\\jTTS4_Professional\\");
		ret = ::RegCreateKey(hKey, lpSubKey, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegCreateKey:SinoVoice\\jTTS4_Professional\\"));
			return FALSE;
		}
		ret = ::RegOpenKeyEx(hKey, NULL, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegOpenKeyEx:HKEY_LOCAL_MACHINE\\Software\\SinoVoice\\jTTS4_Professional\\"));
			return FALSE;
		}
	}

	ret = ::RegSetValueEx(hKey, _T("LibPath4"), 0, REG_SZ,
		(const BYTE*)strModulePath.GetBuffer(), strModulePath.GetLength()*2+1);
	if(ERROR_SUCCESS != ret)
	{
		//AfxMessageBox(_T("Error:RegSetValue:Write LibPath4"));
		return FALSE;
	}
	ret = ::RegSetValueEx(hKey, _T("SerialNo"), 0, REG_SZ,
		(const BYTE*)strJttsSerialNo.GetBuffer(), strJttsSerialNo.GetLength()*2);
	if(ERROR_SUCCESS != ret)
	{
		//AfxMessageBox(_T("Error:RegSetValue:Write SerialNo"));
		return FALSE;
	}
	return TRUE;
}

BOOL SoundPlay::StrHasNum(CString strOrigin)
{
	int iCount = strOrigin.GetLength();
	for (int i=0;i<iCount;i++)
	{
		if (strOrigin[i]>'0'&&strOrigin[i]<'9')
		{
			return TRUE;
		}		
	}
	return FALSE;
}

CString SoundPlay::RemoveChinese(CString strOrigin)
{
	for (int i=0;i<strOrigin.GetLength();i++)
	{
		if ((strOrigin[i]>='0'&&strOrigin[i]<='9')||(strOrigin[i]>='a'&&strOrigin[i]<='z')||(strOrigin[i]>='A'&&strOrigin[i]<='Z'))
		{
			continue;
		}
		else
		{
			int iChange = strOrigin.Remove(strOrigin[i]);
			i--;
		}
	}
	return strOrigin;
}

int SoundPlay::SplitStrToCharArray(const CString& strSrc, CStringArray& strarrDest)
{
	int i = 0;
	for(; i < strSrc.GetLength(); i++)
	{
		CString strChar;
		strChar.Format(_T("%c"), strSrc.GetAt(i));
		strarrDest.Add(strChar);
	}
	return i;
}

int SoundPlay::SplitCusNumToStrArr(const CString& strSrc, CStringArray& strarrDest)
{
	int i = 0;
	CString strNumTemp;
	for(; i < strSrc.GetLength(); i++)
	{
		if(strSrc.GetAt(i) < '0'  || strSrc.GetAt(i) > '9')
		{
			if(!strNumTemp.IsEmpty())
			{
				if(strNumTemp.GetLength() > 1)
				{
					CStringArray strarr;
					SplitNumStrToNumbers(strNumTemp, strarr);
					for(int j = 0; j < strarr.GetCount(); j++)
					{
						strarrDest.Add(strarr[j]);
					}
				}
				else
				{
					strarrDest.Add(strNumTemp);
				}
				strNumTemp.Empty();
			}
			CString strChar;
			strChar.Format(_T("%c"), strSrc.GetAt(i));
			strarrDest.Add(strChar);
		}
		else
		{
			strNumTemp += strSrc.GetAt(i);
		}
	}
	if(!strNumTemp.IsEmpty())
	{
		if(strNumTemp.GetLength() > 1)
		{
			CStringArray strarr;
			SplitNumStrToNumbers(strNumTemp, strarr);
			for(int j = 0; j < strarr.GetCount(); j++)
			{
				strarrDest.Add(strarr[j]);
			}
		}
		else
		{
			strarrDest.Add(strNumTemp);
		}
		strNumTemp.Empty();
	}

	return i;
}

int SoundPlay::SplitNumStrToNumbers(const CString& strNumSrc, CStringArray& strarrDest)
{
	if(strNumSrc.IsEmpty())
	{
		return -1;
	}
	for(int i = 0; i < strNumSrc.GetLength(); i++)
	{
		if( strNumSrc.GetAt(i) < '0' || strNumSrc.GetAt(i) > '9')
		{
			return -1;
		}
	}
	CString strTemp = strNumSrc;
	int nCount = 0;
	for(; !strTemp.IsEmpty(); nCount++)
	{
		if(strTemp.GetAt(0) == '0')
		{
			strTemp = strTemp.Mid(1);
			//strarrDest.Add(_T("0"));
			continue;
		}
		CString strNumPart;
		strNumPart.Format(_T("%c"), strTemp.GetAt(0));
		for(int i = 0; i < strTemp.GetLength()-1; i++)
		{
			strNumPart += _T("0");
		}
		strarrDest.Add(strNumPart);
		if(strNumPart == strTemp)
		{
			break;
		}
		strTemp = strTemp.Mid(1);
		if(strTemp.GetAt(0) == '0' && strarrDest[nCount] != _T("0"))
		{
			strarrDest.Add(_T("0"));
		}
	}

	return nCount;
}

CString SoundPlay::InsertSplitToArray(CStringArray&strarrOrigin,CString strSplit)
{
	CString strChange;
	for (int i=0;i<strarrOrigin.GetCount();i++)
	{
		if (i!=strarrOrigin.GetCount()-1)
		{
			strChange +=strarrOrigin.GetAt(i)+L"#";
		}
		else strChange +=strarrOrigin.GetAt(i);
	}
	return strChange;
}

int SoundPlay::SplitStrToNumArray(CString strSrc,CStringArray&strarrDest)
{
	while (strSrc.GetLength())
	{
		CString element = Getthesame(strSrc);
		strSrc = strSrc.Right(strSrc.GetLength()-element.GetLength());
		strarrDest.Add(element);
	}
	return strarrDest.GetCount();
}

CString SoundPlay::Getthesame(CString strSrc)
{
	CString strSame;
	if (strSrc[0]>=255)
	{
		while (strSrc.GetLength())
		{
			if (strSrc[0]>=255)
			{
				strSame += strSrc[0];
				strSrc = strSrc.Right(strSrc.GetLength()-1);
			}
			else return strSame;
		}
		return strSame;
	}
	else
	{
		while (strSrc.GetLength())
		{
			if (strSrc[0]<255)
			{
				strSame += strSrc[0];
				strSrc = strSrc.Right(strSrc.GetLength()-1);
			}
			else return strSame;
		}
		return strSame;
	}
}

BOOL SoundPlay::PlayTheVoice(CString strVoice,const CStringList& WavList)
{
	/*m_mtPlayStrList.Lock();*/
	for (int i=0;i<WavList.GetCount();i++)
	{
		POSITION pos = WavList.FindIndex(i);
		if (pos!=NULL)
		{
			if (WavList.GetAt(pos)==strVoice)
			{
				//m_mtPlayStrList.Unlock();
				CString sound = m_strWavLibPath+strVoice+_T(".wav");
				return ::sndPlaySound(sound,SND_NODEFAULT | SND_SYNC);				
			}
		}
	}
	if (strVoice.GetLength()>1)
	{
		while (strVoice.GetLength())
		{
			CString strPlayResult = PlayTheVoiceFromRight(strVoice,m_WavList);
			if (strPlayResult.IsEmpty())
			{
				strVoice = strVoice.Right(strVoice.GetLength()-1);
			}
			else
			{
				int iChange = strPlayResult.GetLength();
				strVoice = strVoice.Right(strVoice.GetLength()-iChange);
			}
		}
	}
	return TRUE;
}

CString SoundPlay::PlayTheVoiceFromRight(CString strVoice,const CStringList& WavList)
{
	while (strVoice.GetLength())
	{
			for (int i=0;i<WavList.GetCount();i++)
			{
				POSITION pos = WavList.FindIndex(i);
				if (pos!=NULL)
				{
					if (WavList.GetAt(pos)==strVoice)
					{
						CString sound = m_strWavLibPath+strVoice+_T(".wav");
						::sndPlaySound(sound,SND_NODEFAULT | SND_SYNC);				
						return strVoice;
					}
				}			
			}
			strVoice = strVoice.Left(strVoice.GetLength()-1);
	}
	return L"";
}