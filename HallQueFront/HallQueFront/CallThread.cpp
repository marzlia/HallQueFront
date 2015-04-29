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


extern void MyWriteConsole(CString str);

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
}

CCallThread::~CCallThread(void)
{
	
}

BOOL CCallThread::OpenMsgQueue()
{
//	PeekMessage(&m_msg, NULL, EVA_MSG, EVA_MSG, PM_NOREMOVE);
	return TRUE;
}

void CCallThread::Run()
{
	//接收评价消息
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
		//找出正在呼叫的那条数据
		if(m_rCalledQueData.GetCalledQueData(Window.GetWindowId(),data))
		{
			data.SetEvaluateLevel(pEvaData->GetEvaLevel());
			data.SetIsFinshEva(TRUE);
			CTime curTime = CTime::GetCurrentTime();
			data.SetFinishTime(curTime);
			if(m_rCalledQueData.DeleteCalledQueData(data))//删除正在呼叫的那条数据
			{
				m_rFinshQueData.Add(data);//加入完成队列
			}
		}
		else//不呼叫只评价
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
			处理业务ID
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
			m_rFinshQueData.Add(data);//加入完成队列
		}
	}
	if(pEvaData)//释放资源
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
	case callerCmdShowAdd:
		break;
	default:
		{
			return;
		}
		break;
	}
	///处理完后返回
	ReturnToCaller(callerCmd);
}

void CCallThread::OnLogin(CallerCmd& callerCmd)
{
	if(!TheStaffIDIsHave(callerCmd.GetCarriedData()))//判断登录的员工工号是不是已经设置
	{
		callerCmd.SetSuccess(FALSE);
		return;
	}
	BOOL flag = FALSE;
	std::map<UINT,CString>::const_iterator itera = m_map_login.begin();
	for(itera;itera!=m_map_login.end();itera++)
	{
		CString strVaule = itera->second;
		if(callerCmd.GetCarriedData() == strVaule)//在别的窗口已经登录
		{
			flag = TRUE;
			m_map_login.erase(itera);//删除原来窗口的登录
			///在新窗口登录
			m_map_login[callerCmd.GetWindowId()] = callerCmd.GetCarriedData();
			SetLoginStaffID(callerCmd.GetWindowId(),callerCmd.GetCarriedData());
			callerCmd.SetSuccess(TRUE);//成功
			break;
		}
	}
	if(!flag)//没有登录
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
		//删除原来窗口的登录信息,把原来的窗口的登录staffid置空
		//删除之前把当前呼叫的那条数据删除并添加到完成队列
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
	SLZData callingData;//正在呼叫的data
	if(m_rCalledQueData.GetCalledQueData(callerCmd.GetWindowId(),callingData))//有一条数据在正在呼叫队列没有处理完（包括评价）
	{
		if(callingData.GetIsOpenEva()==TRUE &&  callingData.GetIsFinshEva()==FALSE)
			//打开了评价但还没有评价，流程没完等待用户评价，如15秒后没有评价自动发送未评价消息
		{
			return;//评价没有结束不能呼叫
		}
		else if(!callingData.GetIsFinshEva())//没有开启评价
		{
			m_rCalledQueData.DeleteCalledQueData(callingData);//删除正在呼叫的那条数据
			///加入完成队列
			CTime curTime = CTime::GetCurrentTime();
			callingData.SetFinishTime(curTime);//设置完成时间
			m_rFinshQueData.Add(callingData);//添加到完成队列
		}
	}
	///////////////////////呼叫
	//首先判断该窗口下有没有等候的人,如果有呼叫等候队列的那条数据
	//没有从排队队列中取出新的数据
	SLZData data;
	if(m_rWaitQueData.IsHaveWaitQueData(callerCmd.GetWindowId()))
	{
		if(m_rWaitQueData.GetWaitQueData(callerCmd.GetWindowId(),data))
		{
			//加入正在呼叫队列
			m_rCalledQueData.Add(data);
		}
	}
	else
	{
		///得到排队队列的下一个数据
		if(!theApp.IsLocal())//客户机
		{
			/////如果是客户机呼叫则需要向服务端发送一条请求删除队列中的一条客户机数据
			CComplSocketClient client;
			CString queSerialID;
			m_rInlineQueData.GetWindowCanDoQue(callerCmd.GetWindowId(),queSerialID);
			CString queManNum;
			theApp.m_Controller.GetManQueNumByQueSerialID(queSerialID,queManNum);
			string sendMsg,recvMsg;
			int actRecvSize = 0;
			CDealInterMsg::ProduceSendCallMsg(queManNum,sendMsg,theApp.m_logicVariables.strOrganID);
			if(client.SendData(INTERPORT,theApp.m_logicVariables.strInterIP,
				sendMsg,sendMsg.size(),recvMsg,actRecvSize) && actRecvSize)
			{
				BOOL isSucced;
				CDealInterMsg::AnaRetCallMsg(recvMsg,&isSucced,&data);
			}
		}
		else
		{
			if(m_rInlineQueData.GetInlineQueData(callerCmd.GetWindowId(),data))
			{
				m_rCalledQueData.Add(data);//添加到正在呼叫队列
			}
		}
	}
	if(!data.GetBussinessType().IsEmpty())
	{
		
		//返回，写剩余人数
//		CString carriedData = data.GetQueueNumber() + _T(" ") + GetQueInlineCount(data.GetBussinessType());
		CString carriedData = data.GetQueueNumber() + _T(" ") + GetCandoQueInlineCount(callerCmd.GetWindowId());
		callerCmd.SetCarriedData(carriedData);
		//界面剩余人数更新
		if(theApp.m_pView)
		{
			//theApp.m_pView->ShowWaitNum(data.GetBussinessType(),m_rInlineQueData.GetBussCount(data.GetBussinessType()));
			ShowViewWaitNum(data.GetBussinessType());
		}
		//playsound,显示
		theApp.m_Controller.m_pPlaySound->DataPlay(data);

		//发短信
 		if (!data.GetPhoneNum().IsEmpty())
 		{
 			m_pShortMsg->ClearSendBox();
 			m_pShortMsg->SendMsg(data.GetPhoneNum(),data.GetSendMsg());
 		}
	}
	///重新写file，保存没处理（呼叫）的数据
	theApp.m_Controller.WriteInlineDataToFile();
}

void CCallThread::OnRecall(CallerCmd& callerCmd)
{
	//重呼
	SLZData data;
	UINT uWindID = callerCmd.GetWindowId();
	if(m_rCalledQueData.GetCalledQueData(uWindID,data))
	{
		//返回，写剩余人数
//		CString carriedData = data.GetQueueNumber() + _T(" ") + GetQueInlineCount(data.GetBussinessType());
		CString carriedData = data.GetQueueNumber() + _T(" ") + GetCandoQueInlineCount(callerCmd.GetWindowId());
		callerCmd.SetCarriedData(carriedData);
		//playsound,display
		theApp.m_Controller.m_pPlaySound->DataPlay(data);
	}
}

void CCallThread::OnDiscard(CallerCmd& callerCmd)
{
	//过号
	SLZData data;
	UINT uWindID = callerCmd.GetWindowId();
	if(m_rCalledQueData.GetCalledQueData(uWindID,data))
	{
		//加入到过号队列
		m_rDiscardQueData.Add(data);
		m_rCalledQueData.DeleteCalledQueData(data);
		//返回，写剩余人数
//		CString carriedData = data.GetQueueNumber() + _T(" ") + GetQueInlineCount(data.GetBussinessType());
		CString carriedData = data.GetQueueNumber() + _T(" ") + GetCandoQueInlineCount(callerCmd.GetWindowId());
		callerCmd.SetCarriedData(carriedData);
		//界面剩余人数更新
		if(theApp.m_pView)
		{
//			theApp.m_pView->ShowWaitNum(data.GetBussinessType(),m_rInlineQueData.GetBussCount(data.GetBussinessType()));
			ShowViewWaitNum(data.GetBussinessType());
		}
		//playsound,display
//		theApp.m_Controller.m_pPlaySound->DataPlay(data);
	}
}

void CCallThread::OnWait(CallerCmd& callerCmd)
{
	if(m_rWaitQueData.IsHaveWaitQueData(callerCmd.GetWindowId()))
	{
		return;//只允许每个窗口等候一个人
	}
	else
	{
		SLZData data;
		//获取排队首个加入等候队列
		if(m_rInlineQueData.GetInlineQueData(callerCmd.GetWindowId(),data))
			m_rWaitQueData.Add(data);
		//返回，写剩余人数
//		CString carriedData = data.GetQueueNumber() + _T(" ") + GetQueInlineCount(data.GetBussinessType());
		CString carriedData = data.GetQueueNumber() + _T(" ") + GetCandoQueInlineCount(callerCmd.GetWindowId());
		callerCmd.SetCarriedData(carriedData);
		//界面剩余人数更新
// 		if(theApp.m_pView)
// 		{
// 			theApp.m_pView->ShowWaitNum(data.GetBussinessType(),m_rInlineQueData.GetBussCount(data.GetBussinessType()));
// 		}
		//playsound,display
		theApp.m_Controller.m_pPlaySound->DataPlay(data,TRUE);
	}
}
/*
接收到开启评价命令
*/
void CCallThread::OnEvaReq(CallerCmd& callerCmd)
{
	SLZData data;
	if(m_rCalledQueData.GetCalledQueData(callerCmd.GetWindowId(),data))
	{
		data.SetIsOpenEva(TRUE);//开启评价
		m_rCalledQueData.ModifyCalledQueData(data);
		callerCmd.SetSuccess(TRUE);
	}
}
/*
	暂停
*/
void CCallThread::OnPause(CallerCmd& callerCmd)
{
	UINT winID = callerCmd.GetWindowId();
	
	//发送暂停服务
	//playsound,display
	SLZCWndScreen* pWnd = SLZCWndScreen::GetInstance();
	CString msg = _T("暂停服务");
	SLZWindow Window;
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(winID,Window);
	if(flag)
	{
		pWnd->AddScreenMsg(msg,Window.GetWndScreenId());	
		callerCmd.SetSuccess(TRUE);
	}
}

void CCallThread::OnResume(CallerCmd& callerCmd)
{

}
/*
呼叫特定号码
*/
void CCallThread::OnCallNum(CallerCmd& callerCmd)
{
	CString callNum = callerCmd.GetCarriedData();//获取附加数据，就是呼叫的号码
	UINT winID = callerCmd.GetWindowId();//窗口ID
	callerCmd.SetSuccess(TRUE);
	//playsound,display
	SLZData data;
	data.SetWindowId(winID);
	data.SetQueueNumber(callNum);
	theApp.m_Controller.m_pPlaySound->DataPlay(data);
}
/*
呼叫保安
*/
void CCallThread::OnCallSec(CallerCmd& callerCmd)
{
	UINT winID = callerCmd.GetWindowId();//窗口ID
	callerCmd.SetSuccess(TRUE);
	//playsound,display
	SLZWindow Wnd; 
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(winID,Wnd);
	if(!flag)return;
	CString callName = Wnd.GetWindowCallName();
	CString strMsg;
	callName.Remove(_T('号'));
	callName.Remove(_T('窗'));
	callName.Remove(_T('口'));
	strMsg.Format(_T("#请#保安#到#%s#号窗口"),callName);
	theApp.m_Controller.m_pPlaySound->DataPlay(strMsg);
}
/*
呼叫大堂经理
*/
void CCallThread::OnCallMana(CallerCmd& callerCmd)
{
	UINT winID = callerCmd.GetWindowId();//窗口ID
	callerCmd.SetSuccess(TRUE);
	//playsound,display
	SLZWindow Wnd;
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(winID,Wnd);
	if(!flag)return;
	CString callName = Wnd.GetWindowCallName();
	CString strMsg;
	callName.Remove(_T('号'));
	callName.Remove(_T('窗'));
	callName.Remove(_T('口'));
	strMsg.Format(_T("#请#大堂经理#到#%s#号窗口"),callName);
	theApp.m_Controller.m_pPlaySound->DataPlay(strMsg);
}
/*
呼叫业务顾问
*/
void CCallThread::OnCallBusc(CallerCmd& callerCmd)
{
	UINT winID = callerCmd.GetWindowId();//窗口ID
	callerCmd.SetSuccess(TRUE);
	//playsound,display
	SLZWindow Window; 
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(winID,Window);
	if(!flag)return;
	CString callName = Window.GetWindowCallName();
	CString strMsg;
	callName.Remove(_T('号'));
	callName.Remove(_T('窗'));
	callName.Remove(_T('口'));
	strMsg.Format(_T("#请#业务顾问#到#%s#号窗口"),callName);
	theApp.m_Controller.m_pPlaySound->DataPlay(strMsg);
}
/*
转移队列/窗口
*/
void CCallThread::OnExChange(CallerCmd& callerCmd)
{
	SLZData data;
	if(m_logicVariables.ExchangeQue)
	{
		BOOL flag = FALSE;
		CString queID = callerCmd.GetCarriedData();//获取转移的队列
		if(queID.IsEmpty())return;
		for(int i=0;i<m_map_que.GetCount();i++)
		{
			if(m_map_que[i].GetQueID()==queID)//找到QUEID
			{
				flag = TRUE;
				break;
			}
		}
		//获取对应窗口正在呼叫的数据
		if(flag)
		{
			if(m_rCalledQueData.GetCalledQueData(callerCmd.GetWindowId(),data))
			{
				data.SetIsOpenEva(FALSE);data.SetIsFinshEva(FALSE);
				m_rCalledQueData.DeleteCalledQueData(data);//删除正在呼叫队列的数据
				data.SetBussinessType(queID);
				//设置队列名称
				data.SetBussName(GetQueNumFromID(queID));
				data.SetWindowId(0);//不设置指定窗口,返回初始状态
				m_rInlineQueData.AddHeadData(data);//添加到排队队列最前
				callerCmd.SetSuccess(TRUE);
			}
		}
	}
	if(m_logicVariables.ExchangeWindow)//转移窗口
	{
		CString winID = callerCmd.GetCarriedData();//获取转移的窗口
		if(winID.IsEmpty())return;
		CCommonConvert convert;
		int i_winID = -1;
		convert.CStringToint(i_winID,winID);
		SLZWindow Window;
		BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(i_winID,Window);
		if(flag)//找到了窗口
		{
			if(m_rCalledQueData.GetCalledQueData(callerCmd.GetWindowId(),data))
			{
				if(JudgeWindowHaveQue(i_winID))//判断窗口ID下是否有可处理队列
				{
					data.SetIsOpenEva(FALSE);data.SetIsFinshEva(FALSE);
					m_rCalledQueData.DeleteCalledQueData(data);//删除正在呼叫队列的数据
					data.SetWindowId(i_winID);
					m_rInlineQueData.AddHeadData(data);//添加到排队队列最前
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
	data.SetCmdType(callerCmd.GetCmdType());//命令类型
	data.SetAttchMsg(callerCmd.GetCarriedData());//附加信息
	
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
	case callerCmdShowAdd:
		data.SetCmdType(callerCmdShowAdd);
		break;
	default:
		break;
	}
	SLZWindow Window; 
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(callerCmd.GetWindowId(),Window);
	if(flag)
	{
		data.SetCallerId(Window.GetCallerId());//呼叫器地址
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
获取对应队列的剩余人数
*/
CString CCallThread::GetQueInlineCount(const CString& queID)
{
	UINT waitNum = m_rInlineQueData.GetBussCount(queID);//等待人数
	CString strWaitNum;
	strWaitNum.Format(_T("%d"),waitNum);
	return strWaitNum;
}
/*
获取该窗口能够处理的队列排队人数
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
	//得到窗口
	SLZWindow Window;
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(iWinID,Window);
	if(!flag)return FALSE;
	Window.SetLoginStaff(staffID);//设置登录员工工号
	return TRUE;
}

BOOL CCallThread::DelWindowLoginStaffID(const UINT iWinID)
{
	SLZWindow Window;
	BOOL flag = m_rInlineQueData.m_rWindowTable.QueryWindowById(iWinID,Window);
	if(!flag)return FALSE;
	CString content;
	Window.SetLoginStaff(content);//设置登录员工工号
	return TRUE;
}
/*
界面取票时，呼叫器刷新等待人数
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

BOOL CCallThread::ShowViewWaitNum(const CString& queserial_id)
{
	if(theApp.m_logicVariables.IsOpenInterNum)
	{
		if(theApp.m_logicVariables.strInterIP[0] == '\0')//主机
		{
			goto Normal;		
		}
		else//客户机
		{
			CComplSocketClient client;
			CString queManNum;
			theApp.m_Controller.GetManQueNumByQueSerialID(queserial_id,queManNum);
			string sendMsg,recvMsg;
			int actRecvSize = 0;
			CDealInterMsg::ProduceSendInNumMsg(queManNum,sendMsg);
			if(client.SendData(INTERPORT,theApp.m_logicVariables.strInterIP,
				sendMsg,sendMsg.size(),recvMsg,actRecvSize) && actRecvSize)
			{
				//CDealInterMsg::AnaRetInterMsg(recvMsg,&iQueNum,pInlineNum);
				UINT waitNum = 0;
				CDealInterMsg::AnaRetInNumMsg(recvMsg,&waitNum);

				theApp.m_pView->ShowWaitNum(queserial_id,waitNum);
				return TRUE;
			}
			else
			{
				goto Normal;
			}
		}
	}
	else
	{
Normal:
		UINT nWaitNum = 0;
		m_rInlineQueData.GetAllBussCount(queserial_id,&nWaitNum);//获取当前队列人数
		theApp.m_pView->ShowWaitNum(queserial_id,nWaitNum);//m_rInlineQueData.GetBussCount(queserial_id));
		return TRUE;
	}
	return FALSE;
}

