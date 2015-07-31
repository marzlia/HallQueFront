// SLZCCaller.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "SLZCCaller.h"
#include "HardWareDef.h"
#include "SLZCEvaluator.h"
#include "DoComInOut.h"
#include "DataDef.h"

//SLZCCaller* SLZCCaller::m_pInstance=NULL;//new SLZCCaller;
// SLZCCaller
extern void MyWriteConsole(CString str); 

SLZCCaller::SLZCCaller() :
m_hDoCallerMsgThread(NULL)
,m_hWriteCallerThread(NULL)
{
	InitHardWareCaller();
}

SLZCCaller::~SLZCCaller()
{
	if(m_hDoCallerMsgThread)
	{
		TerminateThread(m_hDoCallerMsgThread,0);
		CloseHandle(m_hDoCallerMsgThread);
		m_hDoCallerMsgThread = NULL;
	}
	if(m_hWriteCallerThread)
	{
		TerminateThread(m_hWriteCallerThread,0);
		CloseHandle(m_hWriteCallerThread);
		m_hWriteCallerThread = NULL;
	}
}

SLZCCaller* SLZCCaller::GetInstance()
{
	static SLZCCaller instance;   //�ֲ���̬����  
	return &instance;  
}
// SLZCCaller ��Ա����
/*
�ṩ���ⲿ�ӿڣ�����������������д��Ϣ
*/
BOOL SLZCCaller::AddWriteCallerData(SLZCallerData callerData)
{
	m_writeCallerLock.Lock();
	m_writeCallerData.AddTail(callerData);
	m_writeCallerLock.Unlock();
	return TRUE;
}
/*
�ṩ�ⲿ�ӿ����ڻ�ȡ��ȡ�ĺ���������
*/
BOOL SLZCCaller::HasData()
{
	return !m_callDataList.IsEmpty();
}
/*
�ṩ�ⲿ�ӿ����ڻ�ȡ��ȡ�ĺ���������
*/
SLZCallerData SLZCCaller::GetData()
{
	m_callerLock.Lock();
	SLZCallerData callerData;
	callerData = m_callDataList.GetHead();
	m_callDataList.RemoveHead();
	m_callerLock.Unlock();
	return callerData;
}

DWORD WINAPI SLZCCaller::DoCallerMsgThread(LPVOID pParam)
{
	SLZCCaller* pThis=(SLZCCaller*)pParam;
	while(1)
	{
		if(pThis->m_listCallerMsg.GetCount()==0)
		{
			Sleep(100);
		}
		else
		{
			POSITION pos=pThis->m_listCallerMsg.GetHeadPosition();
			while(pos!=NULL)
			{
				pThis->m_callerReadMutex.Lock();
				CString msg=pThis->m_listCallerMsg.GetNext(pos);
// #ifdef _DEBUG
// 				MyWriteConsole(msg);
// #endif

				char temp[1024]={0};
				int size=msg.GetLength();
				if(size%10==0)
				{
					for(int i=0;i<size/10;i++)
					{
						CString c_temp=msg.Left(10);
						temp[i]=wcstol(c_temp,NULL,16);
						msg=msg.Mid(10);
					}
				}
				pThis->m_listCallerMsg.RemoveHead();
				pThis->DoCallerMsg(temp,size/10);
				pThis->m_callerReadMutex.Unlock();
			}
		}
	}
	return 0;
}

void SLZCCaller::DoCallerMsg(char *buf,int size)
{
	int CallerAddress=buf[5];//��������ַ
	DWORD dwWrited=0;
	int waitCount=0;

	int address=buf[5];
	SLZCallerData callerData;
	callerData.SetCallerId(address);
	if(buf[size-3]==HARDWARE_CALLER_CALL && size==10)
	{
		//������һλ
		callerData.SetCmdType(cmdCall);
	}
	if(buf[size-3]==HARDWARE_CALLER_CALL && size>=11)
	{
		//�����ض�����
		char resultbuf[50]={0};
		int length=SepaMsgNum(buf,resultbuf,size);
		CString str;
		for(int i=0;i<length;i++)
		{
			CString temp;
			temp.Format(_T("%x"),resultbuf[i]);
			str+=temp;
		}
		callerData.SetAttchMsg(str);
		callerData.SetCmdType(cmdCallNum);
	}
	if(buf[size-3]==HARDWARE_CALLER_WAIT)
	{
		
		//�������۳�ʱ
		//m_hardEvaltor.StartEvaltor(buf);
		if(size == 10)
		{
			SLZCEvaluator::GetInstance()->StartEvaltor(buf);
			callerData.SetCmdType(cmdEvaReq);
		}
		if(size == 11 && buf[size-4] == 0x00)//�Ⱥ�0+�Ⱥ�
		{
			callerData.SetCmdType(cmdWait);
		}
	}
	if(buf[size-3]==HARDWARE_CALLER_RECALL && size==10)
	{
		//�غ�
		callerData.SetCmdType(cmdRecall);
	}

	if(buf[size-3]==HARDWARE_CALLER_FUNCATION)
	{
		//����
		//����+����-----��Ա�ŵ�¼
		if(size>11)
		{
			callerData.SetCmdType(cmdLogin);
			char resultbuf[50]={0};
			int length=SepaMsgNum(buf,resultbuf,size);
			CString str;
			for(int i=0;i<length;i++)
			{
				CString temp;
				temp.Format(_T("%x"),resultbuf[i]);
				str+=temp;
			}
			callerData.SetAttchMsg(str);
		}
		//8+����-----��ѯ��ַ
		if(size==11&&buf[size-4]==0x08)
		{
			callerData.SetCmdType(callerCmdShowAdd);
		}
		//5+����-----����
		if(size==11 && buf[size-4]==0x05)
		{
			callerData.SetCmdType(cmdDiscard);
		}
		//6+���ܺ��б���
		if(size==11 && buf[size-4]==0x06)
		{
			callerData.SetCmdType(cmdCallSec);
		}
		//7+����------���д��þ���
		if(size==11 && buf[size-4]==0x07)
		{
			callerData.SetCmdType(cmdCallMana);
		}
		//9+����------����ҵ�����
		if(size==11 && buf[size-4]==0x09)
		{
			callerData.SetCmdType(cmdCallBusc);
		}
		//4+����-----�˳���¼
		if(size==11 && buf[size-4]==0x04)
		{
			callerData.SetCmdType(cmdQuit);
		}
		//0+����------��ͣ����
		if(size==11 && buf[size-4]==0x00)
		{
			callerData.SetCmdType(cmdPause);
		}
		//2+����------�ָ�����
		if(size==11 && buf[size-4]==0x02)
		{
			callerData.SetCmdType(cmdResume);
		}
	}
	if(buf[size-3]==HARDWARE_CALLER_START && size>=11)//����+��ʼ��ת�ƴ��ڻ����
	{
		callerData.SetCmdType(cmdExChange);
		char resultbuf[50]={0};
		int length=SepaMsgNum(buf,resultbuf,size);
		CString str;
		for(int i=0;i<length;i++)
		{
			CString temp;
			temp.Format(_T("%x"),resultbuf[i]);
			str+=temp;
		}
		callerData.SetAttchMsg(str);
	}

	////////////////////��������Ϣ�Ժ���뻺����
	m_callerLock.Lock();
	m_callDataList.AddTail(callerData);
	m_callerLock.Unlock();
	///////////////////////////////////////////////
}

int SLZCCaller::AlarmToSystem(const int CallerAddress,char* addbuf)
{
	addbuf[0]=(char)0xFF;
	addbuf[1]=0x68;
	addbuf[2]=0x02;
	addbuf[3]=0x02;
	addbuf[4]=0x68;
	addbuf[5]=CallerAddress;//��ַ
	addbuf[6]=HARDWARE_CALLER_SYSTEM;
	addbuf[7]=addbuf[5]+addbuf[6];
	addbuf[8]=0x16;
	addbuf[9]='\0';
	return 10;
}

int SLZCCaller::ShowSuccFail(const int address,char* buf,const int conntext)
{
	buf[0]=(char)0xFF;
	buf[1]=0x68;
	buf[2]=0x03;
	buf[3]=0x03;
	buf[4]=0x68;
	buf[5]=address;//��ַ
	buf[6]=HARDWARE_CALLER_SHOW;
	buf[7]=conntext;
	buf[8]=buf[5]+buf[6]+buf[7];
	buf[9]=0x16;
	buf[10]='\0';
	return 11;
}

int SLZCCaller::ShowLastPaintCount(const int CallerAddress,int WaitCount,char *addbuf)
{
	addbuf[0]=(char)0xFF;
	addbuf[1]=0x68;

	addbuf[4]=0x68;
	addbuf[5]=CallerAddress;//��ַ
	addbuf[6]=HARDWARE_CALLER_SHOW;
	int i=7;

	int divisor = 1;
	int temp = 0;
	int length=0;
	////////////////////////////////
	while(WaitCount/divisor)
	{
		length++;
		divisor*=10;
	}
	////////////////////////////////
	divisor=10;
	if(length==0)return 0;
	int *num = new int[length];
	int k = length-1;
	memset(num,0,length*sizeof(int));
	do
	{
		int temp1=temp;
		temp = (WaitCount % divisor );
		if(temp>=0)
		{
			if(temp>=10)
			{
				int result=(temp - temp1)/(divisor/10); 
				num[k--] = result;
			}
			else
			{
				num[k--] = temp-temp1;
			}
		}
		divisor*=10;
	}while(temp != WaitCount);
	////////////////////////////////////
	for(int m=0;m<length;m++)
	{
		addbuf[i++] = num[m];
	}
	delete [] num;
	//////////////////////////////////
	for(int j=5;j<i;j++)
	{
		addbuf[i]+=addbuf[j];
	}
	addbuf[i+1]='\0';

	addbuf[2]=i-5;
	addbuf[3]=i-5;
	return i+2;
}

int SLZCCaller::SepaMsgNum(const char* buf,char* result,int size)
{
	int num=7;
	int i=0;
	for(num;num<size-3;num++)
	{
		result[i]=buf[num];
		i++;
	}
	return i;
}

void SLZCCaller::DoReadMsg(const DWORD dwReaded,char* buf)
{
	DWORD j=0;
	BOOL f=FALSE;
	CString first_str;
	for(j=0;j<dwReaded&&j<1024;j++)
	{
		CString temp;
		temp.Format(_T("%08x"),buf[j]);
		if(temp==_T("ffffffff") && j!=0)
		{
			f=TRUE;
		}
		if(f)
		{
			m_ReciveMsg.AddTail(first_str);
			first_str=_T("0x");
			first_str+=temp;
			f=FALSE;
		}
		else
		{
			first_str+=_T("0x");
			first_str+=temp;
		}
	}
	////////////////////////////////////
	if(!first_str.IsEmpty() && 
		first_str.Right(10)==_T("0x00000016")
		&& first_str.Left(10)==_T("0xffffffff")
		)
	{
		m_ReciveMsg.AddTail(first_str);
	}
	else if(!first_str.IsEmpty())
	{
		m_lastMsg+=first_str;
		if(!m_lastMsg.IsEmpty() && 
			m_lastMsg.Right(10)==_T("0x00000016")
			&& m_lastMsg.Left(10)==_T("0xffffffff")
			)
		{
			int flag=0;
			int count=m_lastMsg.GetLength();
			for(int i=0;i<count;i+=10)
			{
				CString mid=m_lastMsg.Mid(i,10);
				if(mid==_T("0xffffffff"))
				{
					flag++;
				}
			}
			if(flag==1)
			{
				m_ReciveMsg.AddTail(m_lastMsg);
			}
			m_lastMsg=_T("");
		}
	}
	///////////////////////////////////
	ExTransmission();
}

void SLZCCaller::ExTransmission()
{
	POSITION posRe=m_ReciveMsg.GetHeadPosition();
	while(posRe)
	{
		POSITION posDel=posRe;
		CString msg=m_ReciveMsg.GetNext(posRe);
		if(msg!=_T("") && msg.Left(10)==_T("0xffffffff")
			&& msg.Right(10)==_T("0x00000016"))
		{
			int length=msg.GetLength();
			CString str=msg.Mid(70,length-90);
			length=str.GetLength();
			if(str.Right(20)!=_T("0x0000000c0x0000000c"))
				m_listCallerMsg.AddTail(msg);
		}
		m_ReciveMsg.RemoveAt(posDel);
	}
}

int SLZCCaller::ShowSystemNum(int CallerAddress,char* buf, const int num[],const int count)
{
	buf[0]=(char)0xFF;
	buf[1]=0x68;

	buf[4]=0x68;
	buf[5]=CallerAddress;//��ַ
	buf[6]=HARDWARE_CALLER_SHOW;
	int j=7;
	int i=0;

	for(int i=0;i<count;i++)
	{
		buf[j]=num[i];
		j++;
	}
	for(int k=5;k<j;k++)
	{
		buf[j]+=buf[k];
	}

	buf[j+1]=0x16;
	buf[2]=j-5;
	buf[3]=j-5;
	return j+2;
}

DWORD WINAPI SLZCCaller::WriteCallerThread(LPVOID pParam)
{
	SLZCCaller* pThis = (SLZCCaller*)pParam;
	while(TRUE)
	{
		if(pThis->m_writeCallerData.IsEmpty())
		{
			Sleep(10);
		}
		else
		{
			char buf[256]={0};int length = 0;DWORD dwWrited = 0;
			pThis->m_writeCallerLock.Lock();
			SLZCallerData callerData = pThis->m_writeCallerData.GetHead();
			pThis->m_writeCallerData.RemoveHead();
			switch(callerData.GetCmdType())
			{
			case callerCmdShowAdd:
				length = pThis->ShowLastPaintCount(callerData.GetCallerId(),
					callerData.GetCallerId(),buf);
				break;
			case callerCmdShowSuc:
				length = pThis->ShowSuccFail(callerData.GetCallerId(),buf,1);
				break;
			case callerCmdShowFail:
				length = pThis->ShowSuccFail(callerData.GetCallerId(),buf,0);
				break;
			case callerCmdShowNum:
				{
					int nWaitNum = 0;
					CString attchMsg = callerData.GetAttchMsg();
					int pos = attchMsg.Find(_T(" "));
					CString callNum = attchMsg.Left(pos);//���к���
					CString wStrWaitNum = attchMsg.Mid(pos+1);
					CCommonConvert::CStringToint(nWaitNum,wStrWaitNum);
					length = pThis->ShowSysCallLeftNum(callerData.GetCallerId(),buf,
						nWaitNum,callNum);
				}
				break;
			case callerCmdShowAlarm:
				length = pThis->AlarmToSystem(callerData.GetCallerId(),buf);
				break;
			}
// #ifdef _DEBUG
// 			CString firstMsg = _T("firstmsg:");
// 			for(int i=0;i<length;i++)
// 			{
// 				firstMsg.AppendFormat(_T("%x "),buf[i]);
// 			}
// 			firstMsg.AppendFormat(_T("count:%d"),length);
// 			MyWriteConsole(firstMsg);
// #endif
			
			WriteComMsg* pMsg = new WriteComMsg;
			memset(pMsg->buf,0,textNum);
			memcpy(pMsg->buf,buf,length);
			pMsg->length = length;
			CDoComInOut* pDoComInOut = CDoComInOut::GetInstance();
			pDoComInOut->AddWriteComMsg(pMsg);
  			pThis->m_writeCallerLock.Unlock();
		}
	}
	return 0;
}

void SLZCCaller::InitHardWareCaller()
{
	/////////////////////////////////////////�򿪴���
	CComInit* pComInit = CComInit::GetInstance();
	CString wndCom = pComInit->GetWndComm();
	int i_wndCom = 0;
	CCommonConvert convert;
	convert.CStringToint(i_wndCom,wndCom);
	int re = pComInit->OpenWndScreen(i_wndCom);
	if(re == -1)
	{
		::MessageBox((HWND)AfxGetMainWnd(),_T("�����������ڴ�ʧ�ܻ�ռ��"),
			_T("ע��"),MB_OK|MB_ICONINFORMATION);
	}
	/////////////////////////////////////////////////////
	///////////////////���������ڶ�д���߳�
	CDoComInOut* pDoComInOut = CDoComInOut::GetInstance();
	///////////////////////////////////////////////////
	if(m_hDoCallerMsgThread==NULL)
	{
		m_hDoCallerMsgThread=CreateThread(NULL,0,
			DoCallerMsgThread,this,NULL,0);//���崦��������߳�
	}
	if(m_hWriteCallerThread == NULL)
	{
		///////////////д�������߳�
		m_hWriteCallerThread = CreateThread(NULL,0,
			WriteCallerThread,this,NULL,0);
	}
}

int SLZCCaller::ShowSysCallLeftNum(int callerAddress,char* buf,int leftNum,const CString& callNum)
{
	buf[0]=(char)0xFF;
	buf[1]=0x68;
	buf[4]=0x68;
	buf[5]=callerAddress;//��ַ
	buf[6]=HARDWARE_CALLER_SHOW;
	int len = CCommonConvert::CStringToChar(callNum,NULL);
	char* tempMsg = new char[len+1];
	memset(tempMsg,0,len+1);
	CCommonConvert::CStringToChar(callNum,tempMsg);
	int i = 7;
	for(int j=0;j<len;j++)
	{
		if(tempMsg[j]>='0' && tempMsg[j]<='9')
		{
			int i_msg = tempMsg[j]-48;
			buf[i++] = i_msg;
		}
	}
	delete [] tempMsg;
	buf[i++]=(char)0x0A;//0A��ʾ_
	///////////////////����ʣ������
	int divisor = 1;
	int temp = 0;
	int length=0;
	////////////////////////////////
	while(leftNum/divisor)
	{
		length++;
		divisor*=10;
	}
	////////////////////////////////
	divisor=10;
	if(length!=0)
	{
		int *num = new int[length];
		int k = length-1;
		memset(num,0,length*sizeof(int));
		do
		{
			int temp1=temp;
			temp = (leftNum % divisor );
			if(temp>=0)
			{
				if(temp>=10)
				{
					int result=(temp - temp1)/(divisor/10); 
					num[k--] = result;
				}
				else
				{
					num[k--] = temp-temp1;
				}
			}
			divisor*=10;
		}while(temp != leftNum);
		////////////////////////////////////
		for(int m=0;m<length;m++)
		{
			buf[i++] = num[m];
		}
		delete [] num;
	}
	else
	{
		buf[i++]=0;
	}
	//////////////////////////////////
	for(int j=5;j<i;j++)
	{
		buf[i]+=buf[j];
	}
	buf[i+1]='\0';
	buf[2]=i-5;
	buf[3]=i-5;
	return i+2;
}