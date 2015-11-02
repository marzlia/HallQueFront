#include "StdAfx.h"
#include "CallThread.h"
#include "SLZStaff.h"
#include "SLZCCaller.h"
#include "DataDef.h"
#include "HallQueFront.h"
#include "SLZEvaData.h"
#include "SLZCWndScreen.h"
#include "ComputeFuncationTime.h"
#include "ComplSocketClient.h"
#include "DealInterMsg.h"
#include "UDPBrodcast.h"
#include "ProduceClientPacket.h"


class CYC_LOCK
{//�ޣ��Ҷ�����ٽ����࣬����ͬ���߳�
private:
	CRITICAL_SECTION g_criSection;//�����ٽ�������
public:
	void lock(){EnterCriticalSection(&g_criSection); };
	void unlock(){LeaveCriticalSection(&g_criSection);};

	CYC_LOCK(){InitializeCriticalSection(&g_criSection);  };
	~CYC_LOCK(){::DeleteCriticalSection(&g_criSection);};

} cyclock;


extern void CYC_WRITE_LOGFILE(CString ss);
extern void MyWriteConsole(CString str);

CCallThread* pCallThread;

CCallThread::CCallThread(CInlineQueData& rInlineQueData,
						 CCalledQueData& rCalledQueData,
						 CCallerCmdData& rCallerCmdData,
						 CFinshQueData& rFinshQueData,
						 CDiscardQueData& rDiscardQueData,
						 CWaitQueData& rWaitQueData,
						 LogicVariables& logicVariables,
						 std::map<UINT,CString>& login,
						 CMap<int,int,CQueueInfo,CQueueInfo&>& queInfo,
						 SLZStaffQueryView& staffQueryView )
 : m_rInlineQueData(rInlineQueData)
 , m_rCalledQueData(rCalledQueData)
 , m_rCallerCmdData(rCallerCmdData)
 , m_rFinshQueData(rFinshQueData)
 , m_rDiscardQueData(rDiscardQueData)
 , m_rWaitQueData(rWaitQueData)
 , m_logicVariables(logicVariables)
 , m_map_login(login)
 , m_map_que(queInfo)
 , m_staffQueryView(staffQueryView)
{
	m_pShortMsg = CShortMsgModem::GetInstance();
	pCallThread = this;

	SetTimer(NULL,0,2000,MyDoCountTimeMsg);
}

CCallThread::~CCallThread(void)
{
	ClearListCountTime();
}

BOOL CCallThread::OpenMsgQueue()
{
//	PeekMessage(&m_msg, NULL, EVA_MSG, EVA_MSG, PM_NOREMOVE);
	return TRUE;
}

void CCallThread::Run()
{
	//����������Ϣ
//	if(SetEvent(theApp.m_hStartEvent))
//	{
		if(PeekMessage(&m_msg, NULL, EVA_MSG, EVA_MSG, PM_REMOVE)) //get msg from message queue
		{
			switch(m_msg.message)
			{
			case EVA_MSG:
				DoEvaMsg(m_msg);
			break;
			}
		}
		else
		{
			Sleep(10);
		}
//	}
//	else
// 	{
// 		Sleep(10);
// 	}
	////////////////////////////////////
	CallerCmd cmd;
	if(m_rCallerCmdData.GetHead(cmd))
	{
		DispatchCallerCmd(cmd);
	}
	else
	{
		Sleep(10);
	}

	
}

void CCallThread::DoEvaMsg(const MSG& msg)
{
	SLZEvaData* pEvaData = (SLZEvaData*)msg.wParam;

	SLZWindow Window;
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowByEvaId(pEvaData->GetEvaluatorId(),Window);
	if(flag)
	{
		SLZData data;
		//�ҳ����ں��е���������
		if(m_rCalledQueData.GetCalledQueData(Window.GetWindowId(),data))
		{
			data.SetEvaluateLevel(pEvaData->GetEvaLevel());
			data.SetIsFinshEva(TRUE);
			CTime curTime = CTime::GetCurrentTime();
			data.SetFinishTime(curTime);
			if(m_rCalledQueData.DeleteCalledQueData(data))//ɾ�����ں��е���������
			{
				m_rFinshQueData.Add(data);//������ɶ���
			}
		}
		else//������ֻ����
		{
			data.SetEvaluateLevel(pEvaData->GetEvaLevel());
			data.SetIsFinshEva(TRUE);
			CTime curTime = CTime::GetCurrentTime();
			data.SetFinishTime(curTime);
			data.SetWindowId(Window.GetWindowId());
			data.SetWindowShowId(Window.GetShowWndId());
			CString staffID = m_map_login[Window.GetWindowId()];
			data.SetStaffId(staffID);
			CString bussinessType;
			/*
			����ҵ��ID
			*/
			SLZWindow window;
			m_rInlineQueData.m_rWindowTable.QueryWindowById(data.GetWindowId(),window);
			CStringArray arrayQueID;
			window.GetArrayQueId(arrayQueID);
			if(!arrayQueID.IsEmpty())
			{
				bussinessType = arrayQueID[0];
			}
			else
			{
				CMap<int,int,CQueueInfo,CQueueInfo&>::CPair* pCurEva;
				int count = m_map_que.GetCount();
				for(int i=0;i<count;i++)
				{
					pCurEva = m_map_que.PLookup(i);
					bussinessType = pCurEva->value.GetQueID();
					break;
				}
			}
			////////////////////////////////
			data.SetBussinessType(bussinessType);
			data.SetOrganId(theApp.m_logicVariables.strOrganID);
			data.SetOrganName(theApp.m_logicVariables.strOrganNmae);
//			data.SetQueSerialID(_T("1"));
			data.SetQueueNumber(_T("0"));
			data.SetCallTime(curTime);
			data.SetTakingNumTime(curTime);
			m_rFinshQueData.Add(data);//������ɶ���
		}
	}
	if(pEvaData)//�ͷ���Դ
		delete pEvaData;
}

void CCallThread::DispatchCallerCmd(CallerCmd& callerCmd)
{
	switch(callerCmd.GetCmdType())
	{
	case cmdLogin:
		{
			OnLogin(callerCmd);
		}
		break;
	case cmdQuit:
		{
			OnQuit(callerCmd);
		}
		break;
	case cmdCall:
		{
			OnCall(callerCmd);
		}
		break;
	case cmdRecall:
		{
			OnRecall(callerCmd);
		}
		break;
	case cmdDiscard:
		{
			OnDiscard(callerCmd);
		}
		break;
	case cmdWait:
		{
			OnWait(callerCmd);
		}
		break;
	case cmdEvaReq:
		{
			OnEvaReq(callerCmd);
		}
		break;
	case cmdPause:
		{
			OnPause(callerCmd);
		}
		break;
	case cmdResume:
		{
			OnResume(callerCmd);
		}
		break;
	case cmdCallNum:
		{
			OnCallNum(callerCmd);
		}
		break;
	case cmdCallSec:
		{
			OnCallSec(callerCmd);
		}
		break;
	case cmdCallMana:
		{
			OnCallMana(callerCmd);
		}
		break;
	case cmdCallBusc:
		{
			OnCallBusc(callerCmd);
		}
		break;
	case cmdExChange:
		{
			OnExChange(callerCmd);
		}
		break;
	case callerCmdCountTime:
		{
			OnCountTime(callerCmd);
		}
		break;
	case callerCmdShowAdd:
		break;
	default:
		{
			return;
		}
		break;
	}
	///������󷵻�
	ReturnToCaller(callerCmd);
}

void CCallThread::OnLogin(CallerCmd& callerCmd)
{
	if(!TheStaffIDIsHave(callerCmd.GetCarriedData()))//�жϵ�¼��Ա�������ǲ����Ѿ�����
	{
		callerCmd.SetSuccess(FALSE);
		return;
	}
	BOOL flag = FALSE;
	std::map<UINT,CString>::const_iterator itera = m_map_login.begin();
	for(itera;itera!=m_map_login.end();itera++)
	{
		CString strVaule = itera->second;
		if(callerCmd.GetCarriedData() == strVaule)//�ڱ�Ĵ����Ѿ���¼
		{
			flag = TRUE;
			m_map_login.erase(itera);//ɾ��ԭ�����ڵĵ�¼
			///���´��ڵ�¼
			m_map_login[callerCmd.GetWindowId()] = callerCmd.GetCarriedData();
			SetLoginStaffID(callerCmd.GetWindowId(),callerCmd.GetCarriedData());
			callerCmd.SetSuccess(TRUE);//�ɹ�
			break;
		}
	}
	if(!flag)//û�е�¼
	{
		m_map_login[callerCmd.GetWindowId()] = callerCmd.GetCarriedData();
		SetLoginStaffID(callerCmd.GetWindowId(),callerCmd.GetCarriedData());
		callerCmd.SetSuccess(TRUE);
	}
}

void CCallThread::OnQuit(CallerCmd& callerCmd)
{
	std::map<UINT,CString>::const_iterator itera = m_map_login.find(callerCmd.GetWindowId());
	if(itera!=m_map_login.end())
	{
		//ɾ��ԭ�����ڵĵ�¼��Ϣ,��ԭ���Ĵ��ڵĵ�¼staffid�ÿ�
		//ɾ��֮ǰ�ѵ�ǰ���е���������ɾ������ӵ���ɶ���
		SLZData data;
		m_rCalledQueData.GetCalledQueData(callerCmd.GetWindowId(),data);
		m_rFinshQueData.Add(data);
		m_rCalledQueData.DeleteCalledQueData(data);
		//////////////////////////////
		m_map_login.erase(itera);
		callerCmd.SetSuccess(TRUE);
	}
	callerCmd.SetSuccess(FALSE);
}

void CCallThread::OnCall(CallerCmd& callerCmd)
{
	SLZData callingData;//���ں��е�data
	if(m_rCalledQueData.GetCalledQueData(callerCmd.GetWindowId(),callingData))//��һ�����������ں��ж���û�д����꣨�������ۣ�
	{

		CYC_WRITE_LOGFILE(CString(_T("�����ں��ж������ڴ����ŶӺ����ǣ�") + callingData.GetQueueNumber()));//�ޣ��Ҽӵĵ���

		if(callingData.GetIsOpenEva()==TRUE &&  callingData.GetIsFinshEva()==FALSE)
			//�������۵���û�����ۣ�����û��ȴ��û����ۣ���15���û�������Զ�����δ������Ϣ
		{
			return;//����û�н������ܺ���
		}
		else if(!callingData.GetIsFinshEva())//û�п�������
		{
			m_rCalledQueData.DeleteCalledQueData(callingData);//ɾ�����ں��е���������
			///������ɶ���
			CTime curTime = CTime::GetCurrentTime();
			callingData.SetFinishTime(curTime);//�������ʱ��
			m_rFinshQueData.Add(callingData);//��ӵ���ɶ���

			CYC_WRITE_LOGFILE(CString(_T("��һ���Ŷ��߽��뵽��ɶ��У��ŶӺ����ǣ�") + callingData.GetQueueNumber()));//�ޣ��Ҽӵĵ���
		}
	}
	///////////////////////����
	//�����жϸô�������û�еȺ����,����к��еȺ���е���������
	//û�д��ŶӶ�����ȡ���µ�����
	SLZData data;
	BOOL bFind = FALSE;
	if(m_rWaitQueData.IsHaveWaitQueData(callerCmd.GetWindowId()))
	{
		if(m_rWaitQueData.GetWaitQueData(callerCmd.GetWindowId(),data))
		{
			cyclock.lock();//�ޣ��Ҽӵ�

			//�������ں��ж���
			m_rCalledQueData.Add(data);

			cyclock.unlock();//�ޣ��Ҽӵ�
		}
	}
	else
	{
		///�õ��ŶӶ��е���һ������
		if(!theApp.IsLocal())//�ͻ���
		{
			/////����ǿͻ�����������Ҫ�����˷���һ������ɾ�������е�һ���ͻ�������
			CComplSocketClient client;
			CStringArray queSerialIDArray,queManNumArray;
			CString callStaffID,queManNum;
			BOOL bIsUsePower = FALSE;
			m_rInlineQueData.GetWindowCanDoQue(callerCmd.GetWindowId(),queSerialIDArray,callStaffID,&bIsUsePower);
			
			for(int i=0;i<queSerialIDArray.GetCount();i++)
			{
				theApp.m_Controller.GetManQueNumByQueSerialID(queSerialIDArray.GetAt(i),queManNum);
				if(!queManNum.IsEmpty())
					queManNumArray.Add(queManNum);
			}

			string sendMsg,recvMsg;
			int actRecvSize = 0;
			CDealInterMsg::ProduceSendCallMsg(queManNumArray,sendMsg,theApp.m_logicVariables.strOrganID,bIsUsePower);
			if(client.SendData(INTERPORT,theApp.m_logicVariables.strInterIP,
				sendMsg,sendMsg.size(),recvMsg,actRecvSize) && actRecvSize)
			{
				BOOL isSucced;
				CDealInterMsg::AnaRetCallMsg(recvMsg,&isSucced,&data);
				if(isSucced)
				{
					data.SetCallTime(CTime::GetCurrentTime());
					data.SetStaffId(callStaffID);
					data.SetWindowId(callerCmd.GetWindowId());
					CString queSerialID;
					theApp.m_Controller.GetQueSerialIDByManQueNum(queSerialID,data.GetQueSerialID());
#ifdef _DEBUG
					MyWriteConsole(_T("�ͻ������յ��ı���:"));
					CString wRecvMsg(recvMsg.c_str());
					MyWriteConsole(wRecvMsg);
					MyWriteConsole(_T("--------------------"));
					MyWriteConsole(_T("�ͻ������������Ķ���id:"));
					MyWriteConsole(queSerialID);
					MyWriteConsole(_T("---------------------"));
#endif
					data.SetBussinessType(queSerialID);
					bFind = TRUE;
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			bFind = m_rInlineQueData.GetInlineQueData(callerCmd.GetWindowId(),data);
		}
	}
	if(!data.GetBussinessType().IsEmpty() && bFind)
	{
		cyclock.lock();//�ޣ��Ҽӵ�

		m_rCalledQueData.Add(data);//��ӵ����ں��ж���

		cyclock.lock();//�ޣ��Ҽӵ�
		
		//����ʣ����������
			//theApp.m_pView->ShowWaitNum(data.GetBussinessType(),m_rInlineQueData.GetBussCount(data.GetBussinessType()));
		ShowViewWaitNum(data.GetBussinessType(),data,callerCmd);
		
		//playsound,��ʾ
		theApp.m_Controller.m_pPlaySound->DataPlay(data);

		//������
 		if (!data.GetPhoneNum().IsEmpty())
 		{
 			m_pShortMsg->ClearSendBox();
 			m_pShortMsg->SendMsg(data.GetPhoneNum(),data.GetSendMsg());
 		}

		
	}
	if(theApp.IsLocal())
		///����дfile������û�������У�������
		theApp.m_Controller.WriteInlineDataToFile();

	DeleteCountTimeWindow(callerCmd.GetWindowId());
}

void CCallThread::OnRecall(CallerCmd& callerCmd)
{
	//�غ�
	SLZData data;
	UINT uWindID = callerCmd.GetWindowId();
	if(m_rCalledQueData.GetCalledQueData(uWindID,data))
	{
		//���أ�дʣ������
//		CString carriedData = data.GetQueueNumber() + _T(" ") + GetQueInlineCount(data.GetBussinessType());
		CString carriedData = data.GetQueueNumber() + _T(" ") + GetCandoQueInlineCount(callerCmd.GetWindowId());
		callerCmd.SetCarriedData(carriedData);
		//playsound,display
		theApp.m_Controller.m_pPlaySound->DataPlay(data);
	}
	DeleteCountTimeWindow(callerCmd.GetWindowId());
}

void CCallThread::OnDiscard(CallerCmd& callerCmd)
{
	//����
	SLZData data;
	UINT uWindID = callerCmd.GetWindowId();
	if(m_rCalledQueData.GetCalledQueData(uWindID,data))
	{
		//���뵽���Ŷ���
		m_rDiscardQueData.Add(data);
		m_rCalledQueData.DeleteCalledQueData(data);
		//���أ�дʣ������
//		CString carriedData = data.GetQueueNumber() + _T(" ") + GetQueInlineCount(data.GetBussinessType());
		CString carriedData = data.GetQueueNumber() + _T(" ") + GetCandoQueInlineCount(callerCmd.GetWindowId());
		callerCmd.SetCarriedData(carriedData);
		//����ʣ����������
		if(theApp.m_pView)
		{
//			theApp.m_pView->ShowWaitNum(data.GetBussinessType(),m_rInlineQueData.GetBussCount(data.GetBussinessType()));
			ShowViewWaitNum(data.GetBussinessType(),data,callerCmd);
		}
		//playsound,display
//		theApp.m_Controller.m_pPlaySound->DataPlay(data);
	}
}

void CCallThread::OnWait(CallerCmd& callerCmd)
{
	if(m_rWaitQueData.IsHaveWaitQueData(callerCmd.GetWindowId()))
	{
		return;//ֻ����ÿ�����ڵȺ�һ����
	}
	else
	{
		SLZData data;
		//��ȡ�Ŷ��׸�����Ⱥ����
		if(m_rInlineQueData.GetInlineQueData(callerCmd.GetWindowId(),data))
			m_rWaitQueData.Add(data);
		//���أ�дʣ������
//		CString carriedData = data.GetQueueNumber() + _T(" ") + GetQueInlineCount(data.GetBussinessType());
		CString carriedData = data.GetQueueNumber() + _T(" ") + GetCandoQueInlineCount(callerCmd.GetWindowId());
		callerCmd.SetCarriedData(carriedData);
		//����ʣ����������
// 		if(theApp.m_pView)
// 		{
// 			theApp.m_pView->ShowWaitNum(data.GetBussinessType(),m_rInlineQueData.GetBussCount(data.GetBussinessType()));
// 		}
		//playsound,display
		theApp.m_Controller.m_pPlaySound->DataPlay(data,TRUE);
	}
	DeleteCountTimeWindow(callerCmd.GetWindowId());
}
/*
���յ�������������
*/
void CCallThread::OnEvaReq(CallerCmd& callerCmd)
{
	SLZData data;
	if(m_rCalledQueData.GetCalledQueData(callerCmd.GetWindowId(),data))
	{
		data.SetIsOpenEva(TRUE);//��������
		m_rCalledQueData.ModifyCalledQueData(data);
		callerCmd.SetSuccess(TRUE);
	}
}
/*
	��ͣ
*/
void CCallThread::OnPause(CallerCmd& callerCmd)
{
	UINT winID = callerCmd.GetWindowId();
	
	//������ͣ����
	//playsound,display
	SLZCWndScreen* pWnd = SLZCWndScreen::GetInstance();
	CString msg = _T("��ͣ����");
	SLZWindow Window;
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(winID,Window);
	if(flag)
	{
		
		CThroughWndScreenInfo wndScreenInfo;
		for(int i=0;i<Window.m_throughscreen_array.GetCount();i++)
		{
			
			wndScreenInfo = Window.m_throughscreen_array.GetAt(i);
					
			pWnd->AddScreenMsg(msg,wndScreenInfo.GetWndScreenId());
			pWnd->AddScreenMsg(msg,wndScreenInfo.GetComScreenId());
			pWnd->AddThroughScreenMsg(msg,wndScreenInfo.GetPhyId(),wndScreenInfo.GetPipeId(),wndScreenInfo.GetLocalIp());
		}
		
		///////��ͣ����ʱ
		if(theApp.m_logicVariables.IsOpenCountTime)
		{
			CountTime* pTime = new CountTime;
			pTime->nTimeSec = theApp.m_logicVariables.nTimeMintue * 60;
			pTime->window = Window;
			pTime->startTime = CTime::GetCurrentTime();
			pTime->bState = true;//��ͣ
			AddCountTime(pTime);
		}
		callerCmd.SetSuccess(TRUE);
	}
}


void CCallThread::OnCountTime(CallerCmd& callerCmd)
{
	UINT winID = callerCmd.GetWindowId();

	//������ͣ����
	//playsound,display
	SLZCWndScreen* pWnd = SLZCWndScreen::GetInstance();
	SLZWindow Window;
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(winID,Window);
	if(flag)
	{
		///////����ʱ
		if(theApp.m_logicVariables.IsOpenCountTime)
		{
			CountTime* pTime = new CountTime;
			pTime->nTimeSec = theApp.m_logicVariables.nTimeMintue * 60;
			pTime->window = Window;
			pTime->startTime = CTime::GetCurrentTime();
			pTime->bState = false;//�뿪
			AddCountTime(pTime);
			callerCmd.SetSuccess(TRUE);
		}
	}
}

void CCallThread::AddCountTime(CountTime* pTime)
{
	if(pTime == NULL)return;

	if(ModifyCountTimeWindow(pTime))
		return;
	m_mtCountTime.Lock();
	m_list_CountTime.push_back(pTime);
	m_mtCountTime.Unlock();
}

void CCallThread::OnResume(CallerCmd& callerCmd)
{
	UINT winID = callerCmd.GetWindowId();

	//������ͣ����
	//playsound,display
	SLZCWndScreen* pWnd = SLZCWndScreen::GetInstance();
	CString msg = _T("                   ");
	SLZWindow Window;
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(winID,Window);
	if(flag)
	{
		CThroughWndScreenInfo wndScreenInfo;
		for(int i=0;i<Window.m_throughscreen_array.GetCount();i++)
		{

			wndScreenInfo = Window.m_throughscreen_array.GetAt(i);

			pWnd->AddScreenMsg(msg,wndScreenInfo.GetWndScreenId());
			pWnd->AddScreenMsg(msg,wndScreenInfo.GetComScreenId());
			pWnd->AddThroughScreenMsg(msg,wndScreenInfo.GetPhyId(),wndScreenInfo.GetPipeId(),wndScreenInfo.GetLocalIp());
		}
		callerCmd.SetSuccess(TRUE);
	}

	
	DeleteCountTimeWindow(callerCmd.GetWindowId());
}
/*
�����ض�����
*/
void CCallThread::OnCallNum(CallerCmd& callerCmd)
{
	CString callNum = callerCmd.GetCarriedData();//��ȡ�������ݣ����Ǻ��еĺ���
	UINT winID = callerCmd.GetWindowId();//����ID
	callerCmd.SetSuccess(TRUE);
	//playsound,display
	SLZData data;
	data.SetWindowId(winID);
	data.SetQueueNumber(callNum);
	theApp.m_Controller.m_pPlaySound->DataPlay(data);

	DeleteCountTimeWindow(callerCmd.GetWindowId());
}
/*
���б���
*/
void CCallThread::OnCallSec(CallerCmd& callerCmd)
{
	UINT winID = callerCmd.GetWindowId();//����ID
	callerCmd.SetSuccess(TRUE);
	//playsound,display
	SLZWindow Wnd; 
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(winID,Wnd);
	if(!flag)return;
	CString callName = Wnd.GetWindowCallName();
	CString strMsg;
	callName.Remove(_T('��'));
	callName.Remove(_T('��'));
	callName.Remove(_T('��'));
	strMsg.Format(_T("#��#����#��#%s#�Ŵ���"),callName);
	theApp.m_Controller.m_pPlaySound->DataPlay(strMsg);

	DeleteCountTimeWindow(callerCmd.GetWindowId());
}
/*
���д��þ���
*/
void CCallThread::OnCallMana(CallerCmd& callerCmd)
{
	UINT winID = callerCmd.GetWindowId();//����ID
	callerCmd.SetSuccess(TRUE);
	//playsound,display
	SLZWindow Wnd;
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(winID,Wnd);
	if(!flag)return;
	CString callName = Wnd.GetWindowCallName();
	CString strMsg;
	callName.Remove(_T('��'));
	callName.Remove(_T('��'));
	callName.Remove(_T('��'));
	strMsg.Format(_T("#��#���þ���#��#%s#�Ŵ���"),callName);
	theApp.m_Controller.m_pPlaySound->DataPlay(strMsg);

	DeleteCountTimeWindow(callerCmd.GetWindowId());
}
/*
����ҵ�����
*/
void CCallThread::OnCallBusc(CallerCmd& callerCmd)
{
	UINT winID = callerCmd.GetWindowId();//����ID
	callerCmd.SetSuccess(TRUE);
	//playsound,display
	SLZWindow Window; 
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(winID,Window);
	if(!flag)return;
	CString callName = Window.GetWindowCallName();
	CString strMsg;
	callName.Remove(_T('��'));
	callName.Remove(_T('��'));
	callName.Remove(_T('��'));
	strMsg.Format(_T("#��#ҵ�����#��#%s#�Ŵ���"),callName);
	theApp.m_Controller.m_pPlaySound->DataPlay(strMsg);

	DeleteCountTimeWindow(callerCmd.GetWindowId());
}
/*
ת�ƶ���/����
*/
void CCallThread::OnExChange(CallerCmd& callerCmd)
{
	SLZData data;
	if(m_logicVariables.ExchangeQue)
	{
		BOOL flag = FALSE;
		CString queID = callerCmd.GetCarriedData();//��ȡת�ƵĶ���
		if(queID.IsEmpty())return;
		CQueueInfo queinfo;
		for(int i=0;i<m_map_que.GetCount();i++)
		{
			if(m_map_que[i].GetQueManNum()==queID)//�ҵ�QUEID
			{
				flag = TRUE;
				queinfo = m_map_que[i];
				break;
			}
		}
		//��ȡ��Ӧ�������ں��е�����
		if(flag)
		{
			if(m_rCalledQueData.GetCalledQueData(callerCmd.GetWindowId(),data))
			{
				data.SetIsOpenEva(FALSE);data.SetIsFinshEva(FALSE);
				m_rCalledQueData.DeleteCalledQueData(data);//ɾ�����ں��ж��е�����
				data.SetBussinessType(queinfo.GetQueID());
				//���ö�������
				data.SetBussName(GetQueNumFromID(queinfo.GetQueID()));
				data.SetWindowId(0);//������ָ������,���س�ʼ״̬
				m_rInlineQueData.AddHeadData(data);//��ӵ��ŶӶ�����ǰ
				callerCmd.SetSuccess(TRUE);
			}
		}
	}
	if(m_logicVariables.ExchangeWindow)//ת�ƴ���
	{
		CString winID = callerCmd.GetCarriedData();//��ȡת�ƵĴ���
		if(winID.IsEmpty())return;
		CCommonConvert convert;
		int i_winID = -1;
		convert.CStringToint(i_winID,winID);
		SLZWindow Window;
		BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowBySerialID(i_winID,Window);
		if(flag)//�ҵ��˴���
		{
			if(m_rCalledQueData.GetCalledQueData(callerCmd.GetWindowId(),data))
			{
				if(JudgeWindowHaveQue(Window.GetWindowId()))//�жϴ���ID���Ƿ��пɴ������
				{
					data.SetIsOpenEva(FALSE);data.SetIsFinshEva(FALSE);
					m_rCalledQueData.DeleteCalledQueData(data);//ɾ�����ں��ж��е�����
					data.SetWindowId(Window.GetWindowId());
					m_rInlineQueData.AddHeadData(data);//��ӵ��ŶӶ�����ǰ
					callerCmd.SetSuccess(TRUE);
				}
				else
				{
					callerCmd.SetSuccess(FALSE);
				}
			}
		}
		/////////////////////////////////////
	}
	///////////////////////////////////
}

void CCallThread::ReturnToCaller(CallerCmd& callerCmd)
{
	SLZCCaller* pSLZCCaller = SLZCCaller::GetInstance();
	SLZCallerData data;
	data.SetCmdType(callerCmd.GetCmdType());//��������
	data.SetAttchMsg(callerCmd.GetCarriedData());//������Ϣ
	
	switch(data.GetCmdType())
	{
	case cmdLogin:
		data.SetCmdType(callerCmd.GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case	cmdQuit:
		data.SetCmdType(callerCmd.GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdCall:
		data.SetCmdType(callerCmdShowNum);
		break;
	case cmdRecall:
		data.SetCmdType(callerCmdShowNum);
		break;
	case cmdDiscard:
		data.SetCmdType(callerCmdShowNum);
		break;
	case cmdWait:
		data.SetCmdType(callerCmdShowNum);
		break;
	case cmdEvaReq:
//		data.SetCmdType(callerCmd.GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case 	cmdPause:
		data.SetCmdType(callerCmd.GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdCallNum:
		data.SetCmdType(callerCmd.GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdCallSec:
		data.SetCmdType(callerCmd.GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdCallMana:
		data.SetCmdType(callerCmd.GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdCallBusc:
		data.SetCmdType(callerCmd.GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdExChange:
		data.SetCmdType(callerCmd.GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdResume:
		data.SetCmdType(callerCmd.GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case callerCmdShowAdd:
		data.SetCmdType(callerCmdShowAdd);
		break;
	case callerCmdCountTime:
		data.SetCmdType(callerCmd.GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	default:
		break;
	}
	SLZWindow Window; 
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(callerCmd.GetWindowId(),Window);
	if(flag)
	{
		data.SetCallerId(Window.GetCallerId());//��������ַ
		pSLZCCaller->AddWriteCallerData(data);
	}
}

BOOL CCallThread::TheStaffIDIsHave(const CString& staffID)
{
	SLZStaff* pStaff = m_staffQueryView.QueryStaffById(staffID);
	if(pStaff == NULL)
	{
		return FALSE;
	}
	return TRUE;
}
/*
��ȡ��Ӧ���е�ʣ������
*/
CString CCallThread::GetQueInlineCount(const CString& queID)
{
	UINT waitNum = m_rInlineQueData.GetBussCount(queID);//�ȴ�����
	CString strWaitNum;
	strWaitNum.Format(_T("%d"),waitNum);
	return strWaitNum;
}
/*
��ȡ�ô����ܹ�����Ķ����Ŷ�����
*/
CString CCallThread::GetCandoQueInlineCount(UINT iWinID)
{
	UINT uWaitNum = m_rInlineQueData.GetCandoQueCount(iWinID);
	CString strWaitNum;
	strWaitNum.Format(_T("%d"),uWaitNum);
	return strWaitNum;
}
CString CCallThread::GetQueNumFromID(const CString& queID)
{
	if(queID.IsEmpty()) return _T("");
	CString bussName;
	for (int i=0;i<m_map_que.GetCount();i++)
	{
		CQueueInfo queinfo;
		m_map_que.Lookup(i,queinfo);
		if (queID == queinfo.GetQueID())
		{
			bussName = queinfo.GetBussName();
		}
	}
	return bussName;
}

BOOL CCallThread::JudgeWindowHaveQue(const UINT iWindow)
{
	CStringArray arrStrQueId;
	SLZWindow Window;
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(iWindow,Window);
	if(!flag)return FALSE;
	Window.GetArrayQueId(arrStrQueId);
	if(arrStrQueId.GetCount() < 1)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CCallThread::SetLoginStaffID(const UINT iWinID,const CString& staffID)
{
	//�õ�����
	SLZWindow Window;
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(iWinID,Window);
	if(!flag)return FALSE;
	Window.SetLoginStaff(staffID);//���õ�¼Ա������
	return TRUE;
}

BOOL CCallThread::DelWindowLoginStaffID(const UINT iWinID)
{
	SLZWindow Window;
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(iWinID,Window);
	if(!flag)return FALSE;
	CString content;
	Window.SetLoginStaff(content);//���õ�¼Ա������
	return TRUE;
}
/*
����ȡƱʱ��������ˢ�µȴ�����
*/
BOOL CCallThread::ShowCallerWaitNum(const CString& queID)
{
	if(queID.IsEmpty())return FALSE;
	std::map<UINT,SLZWindow>::const_iterator itera = m_rInlineQueData.m_rWindowTable.m_mapIdWindow.begin();
	for(itera;itera!=m_rInlineQueData.m_rWindowTable.m_mapIdWindow.end();itera++)
	{
		SLZWindow Window = itera->second;
		CStringArray ArrayQueID;
		Window.GetArrayQueId(ArrayQueID);
		for(int i=0;i<ArrayQueID.GetCount();i++)
		{
			CString wStrQueID = ArrayQueID.GetAt(i);
			if(wStrQueID == queID)
			{
				SLZData data;
				m_rCalledQueData.GetCalledQueData(Window.GetWindowId(),data);
				CString carriedData = data.GetQueueNumber() + _T(" ") + GetCandoQueInlineCount(Window.GetWindowId());
				CallerCmd callerCmd;
				callerCmd.SetCmdType(callerCmdShowNum);
				callerCmd.SetWindowId(Window.GetWindowId());
				callerCmd.SetCarriedData(carriedData);
				ReturnToCaller(callerCmd);
				break;
			}
		}
	}
	return TRUE;
}

BOOL CCallThread::ShowCallerWaitNum(const CString& queID,int nWaitNum)
{
	if(queID.IsEmpty())return FALSE;
	std::map<UINT,SLZWindow>::const_iterator itera = m_rInlineQueData.m_rWindowTable.m_mapIdWindow.begin();
	for(itera;itera!=m_rInlineQueData.m_rWindowTable.m_mapIdWindow.end();itera++)
	{
		SLZWindow Window = itera->second;
		CStringArray ArrayQueID;
		Window.GetArrayQueId(ArrayQueID);
		for(int i=0;i<ArrayQueID.GetCount();i++)
		{
			CString wStrQueID = ArrayQueID.GetAt(i);
			if(wStrQueID == queID)
			{
				SLZData data;
				m_rCalledQueData.GetCalledQueData(Window.GetWindowId(),data);
				CString wStrWaitNum;
				wStrWaitNum.Format(_T("%d"),nWaitNum);
				CString carriedData = data.GetQueueNumber() + _T(" ") + wStrWaitNum;
				CallerCmd callerCmd;
				callerCmd.SetCmdType(callerCmdShowNum);
				callerCmd.SetWindowId(Window.GetWindowId());
				callerCmd.SetCarriedData(carriedData);
				ReturnToCaller(callerCmd);
				break;
			}
		}
	}
	return TRUE;
}

BOOL CCallThread::ShowViewWaitNum(const CString& queserial_id,const SLZData& data,CallerCmd& callerCmd)
{
	CString queManNum;
	theApp.m_Controller.GetManQueNumByQueSerialID(queserial_id,queManNum);
	if(theApp.m_logicVariables.IsOpenInterNum)
	{
		if(theApp.m_logicVariables.strInterIP[0] == '\0')//����
		{
			goto Normal;		
		}
		else//�ͻ���
		{
			CComplSocketClient client;
			
			string sendMsg,recvMsg;
			int actRecvSize = 0;
			CDealInterMsg::ProduceSendInNumMsg(queManNum,sendMsg);
			if(client.SendData(INTERPORT,theApp.m_logicVariables.strInterIP,
				sendMsg,sendMsg.size(),recvMsg,actRecvSize) && actRecvSize)
			{
				//CDealInterMsg::AnaRetInterMsg(recvMsg,&iQueNum,pInlineNum);
#ifdef _DEBUG
				MyWriteConsole(_T("�ͻ������Ͷ�����������:"));
				CString wSendMsg(sendMsg.c_str());
				MyWriteConsole(wSendMsg);
				MyWriteConsole(_T("-----------------------------"));
				MyWriteConsole(_T("�ͻ������յ��Ķ����������ģ�"));
				CString wRecvMsg(recvMsg.c_str());
				MyWriteConsole(wRecvMsg);
#endif
				UINT waitNum = 0;
				CDealInterMsg::AnaRetInNumMsg(recvMsg,&waitNum);

				theApp.m_pView->ShowWaitNum(queserial_id,waitNum);

				//���أ�дʣ������
				CString wStrWaitNum;
				wStrWaitNum.Format(_T("%d"),waitNum);
				CString carriedData = data.GetQueueNumber() + _T(" ") + wStrWaitNum;
				callerCmd.SetCarriedData(carriedData);
				return TRUE;
			}
			else
			{
#ifdef _DEBUG
				MyWriteConsole(_T("�ͻ������Ͷ�����������ʧ��"));
#endif
				return FALSE;
			}
		}
	}
	else
	{
Normal:
		UINT nWaitNum = 0;
		m_rInlineQueData.GetAllBussCount(queserial_id,&nWaitNum);//��ȡ��ǰ��������
		theApp.m_pView->ShowWaitNum(queserial_id,nWaitNum);//m_rInlineQueData.GetBussCount(queserial_id));
		
		//���أ�дʣ������
		CString wStrWaitNum;
		wStrWaitNum.Format(_T("%d"),nWaitNum);
		CString carriedData = data.GetQueueNumber() + _T(" ") + wStrWaitNum;
		callerCmd.SetCarriedData(carriedData);

		//�㲥����
		CUDPBrodcast brodcast;
		string retMsg;
		CDealInterMsg::ProduceBrodcastRetInNumMsg(queManNum,nWaitNum,retMsg);
		CString wRetMsg(retMsg.c_str());
		brodcast.BroadCast(wRetMsg);
		
		return TRUE;
	}
	return FALSE;
}

void CALLBACK CCallThread::MyDoCountTimeMsg( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
	SLZCWndScreen* pWnd = SLZCWndScreen::GetInstance();
	list<CountTime*>::const_iterator itera = pCallThread->m_list_CountTime.begin();
	for(itera;itera != pCallThread->m_list_CountTime.end();++itera)
	{
		CountTime* pTime = *itera;
		pTime->nTimeSec -= 2;
		if(!pTime->bState)
		{
			if(pTime->nTimeSec <= 0)
			{
				CThroughWndScreenInfo wndScreenInfo;
				int nOverTimeSec = abs(pTime->nTimeSec);
			 
				CString strMsg = pCallThread->ChangeTimeToCstring(nOverTimeSec);
				CString strTimeMsg = _T("--")+strMsg;
				for(int i=0;i<pTime->window.m_throughscreen_array.GetCount();i++)
				{
					wndScreenInfo = pTime->window.m_throughscreen_array.GetAt(i);

					pWnd->AddScreenMsg(strTimeMsg,wndScreenInfo.GetWndScreenId());
					pWnd->AddScreenMsg(strTimeMsg,wndScreenInfo.GetComScreenId());
					pWnd->AddThroughScreenMsg(strTimeMsg,wndScreenInfo.GetPhyId(),wndScreenInfo.GetPipeId(),wndScreenInfo.GetLocalIp());
				}

// 				delete pTime;
// 				pTime = NULL;
// 				pCallThread->m_list_CountTime.erase(itera);
				break;
			}
		
			CString strTime = pCallThread->ChangeTimeToCstring(pTime->nTimeSec);
			CString strMsg = _T("���Ժ� ") + strTime;
			CThroughWndScreenInfo wndScreenInfo;
			for(int i=0;i<pTime->window.m_throughscreen_array.GetCount();i++)
			{
				wndScreenInfo = pTime->window.m_throughscreen_array.GetAt(i);

				pWnd->AddScreenMsg(strMsg,wndScreenInfo.GetWndScreenId());
				pWnd->AddScreenMsg(strMsg,wndScreenInfo.GetComScreenId());
				pWnd->AddThroughScreenMsg(strMsg,wndScreenInfo.GetPhyId(),wndScreenInfo.GetPipeId(),wndScreenInfo.GetLocalIp());
			}
		}
	}
}


CString CCallThread::ChangeTimeToCstring(int nTimeSec)
{
	CString retStrTime;

	int nMintue = nTimeSec / 60;
	int nSec = nTimeSec % 60;

	if(nMintue != 0)
		retStrTime.Format(_T("%d��"),nMintue);
	if(nSec != 0)
		retStrTime.AppendFormat(_T("%d��"),nSec);

	return retStrTime;
}

void CCallThread::ClearListCountTime()
{
	list<CountTime*>::const_iterator itera = m_list_CountTime.begin();
	for(itera;itera != m_list_CountTime.end();++itera)
	{
		CountTime* pTime = *itera;
		delete pTime;
		pTime = NULL;
	}

	m_list_CountTime.clear();
}

void CCallThread::DeleteCountTimeWindow(UINT uWindowID)
{
	list<CountTime*>::const_iterator itera = m_list_CountTime.begin();
	for(itera;itera != m_list_CountTime.end();++itera)
	{
		CountTime* pCountTime = *itera;
		if(pCountTime->window.GetWindowId() == uWindowID)
		{
			m_mtCountTime.Lock();
			m_list_CountTime.erase(itera);
			m_mtCountTime.Unlock();
			
			pCountTime->endTime = CTime::GetCurrentTime();

			CTimeSpan dealTime = pCountTime->endTime - pCountTime->startTime;
			pCountTime->nTimeSec = (int)(dealTime.GetTotalSeconds() - theApp.m_logicVariables.nTimeMintue * 60);
			if(pCountTime->nTimeSec<0)
				pCountTime->nTimeSec = 0;

			///���͵������
			if(theApp.m_logicVariables.IsAutoSendToServer)
			{
				CString staffID = theApp.m_Controller.m_mapLoginList[pCountTime->window.GetWindowId()];//��ȡ��¼STAFFID
				CProduceClientPacket packet;
				CString wRetMsg = packet.ProducePauseTime(theApp.m_logicVariables.strOrganID,staffID,pCountTime->window.GetWindowId(),
					pCountTime->startTime,pCountTime->endTime,pCountTime->nTimeSec,pCountTime->bState);
			
				CComplSocketClient client;
				std::string recvMsg;int actSize = 0;
				client.SendData(wRetMsg,recvMsg,actSize);
			}
			//////////////////
			delete pCountTime;
			pCountTime = NULL;
			break;
		}
	}
}

BOOL CCallThread::ModifyCountTimeWindow(CountTime* pTime)
{
	if(pTime == NULL) return FALSE;
	CountTime* pCountTime = NULL;
	list<CountTime*>::const_iterator itera = m_list_CountTime.begin();
	for(itera;itera != m_list_CountTime.end();++itera)
	{
		pCountTime = *itera;
		if(pCountTime->window.GetWindowId() == pTime->window.GetWindowId())
		{
			m_mtCountTime.Lock();
			pCountTime->nTimeSec = pTime->nTimeSec;
			m_mtCountTime.Unlock();
			return TRUE;
		}
	}
	return FALSE;
}