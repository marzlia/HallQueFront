#include "StdAfx.h"
#include "SLZCWndScreen.h"
#include "ComInit.h"
#include "CommonConvert.h"
#include "ComplSocketClient.h"
#include "ComputeFuncationTime.h"
#include "DoComInOut.h"
//#include "ComplSocketClient.h"
#include "MySocketUDP.h"
#include "StbContent.h"
#include "StbDisplay.h"
#include "HallQueFront.h"
#include "DealInterMsg.h"

//SLZCWndScreen* SLZCWndScreen::m_pInstance=NULL;//new SLZCWndScreen;
extern void MyWriteConsole(CString str);

SLZCWndScreen::SLZCWndScreen(void) : 
m_hDoWndScreenMsgThread(NULL)
, m_ThrWndMutex(NULL)
, m_hDoStbScreenThread(NULL)
, m_pStbDisplay(NULL)
{
	m_pStbDisplay = new StbDisplay;
	StartHardScreen();
	////////////////////////窗口屏
	///初始化通屏通道，获取通道的信息最大255块通屏
	
//   	for(int i=0;i<255;i++)
//   	{
//  		InitThroughScreen(1);
//  	}
//	InitThroughScreen(_T("192.168.1.8"),1024);
	////////////////////////////////////////////
}

SLZCWndScreen::~SLZCWndScreen(void)
{
	TerminateThread(m_hDoWndScreenMsgThread,0);
	if(m_hDoWndScreenMsgThread)
	{
		CloseHandle(m_hDoWndScreenMsgThread);
		m_hDoWndScreenMsgThread = NULL;
	}
	TerminateThread(m_hDoThrWndMsgThread,0);
	if(m_hDoThrWndMsgThread)
	{
		CloseHandle(m_hDoThrWndMsgThread);
		m_hDoThrWndMsgThread = NULL;
	}
	TerminateThread(m_hDoStbScreenThread,0);
	if(m_hDoStbScreenThread)
	{
		CloseHandle(m_hDoStbScreenThread);
		m_hDoStbScreenThread = NULL;
	}
	ClearStbContentInfo();

	if(m_pStbDisplay)
	{
		delete m_pStbDisplay;
		m_pStbDisplay = NULL;
	}
	ClearMapStbcallMsg();
}

int SLZCWndScreen::DoScreenMsg(CString& msg,
								 int address, char* buf)
{
	char h,l;//高位，低位
	char color=0;//颜色默认为0红色
	int count=msg.GetLength();
	if(count==0)
	{
		return 0;
	}

	buf[0]=(char)WNDSCREEN_HEAD_FIRST;
	buf[1]=(char)WNDSCREEN_HEAD_SCEND;
	/*屏地址*/
	buf[2]=address;
	buf[3]=(char)WNDSCREEN_SHOW_INFOR;

	int m=6;
	unsigned char mydatacount=0;
	for(int i=0;i<count;i++)
	{
		BOOL flag=FALSE;
		WCHAR w=msg.GetAt(i);
		if(w<=255)
		{
			char ch=w;
			if(ch=='#')
			{
				char mark[4]={0};
				mark[0]=ch;
				int temp=i;
				WCHAR secd=msg.GetAt(temp+1);
				if(secd<=255 && secd>='0' && secd<='2')
				{
					mark[1]=secd;
					WCHAR three=msg.GetAt(temp+2);
					if(three<=255 && three=='#')
					{
						flag=TRUE;
						mark[2]=three;

						mark[3]='\0';
						if(mark[1]=='0')
						{
							color=0;
						}
						else if(mark[1]=='1')
						{
							color=1;
						}
						else if(mark[1]=='2')
						{
							color=2;
						}
						i+=2;
					}
				}
			}
			if(!flag)
			{
				buf[m]=w;
				m++;
				buf[m]=color;//颜色
				m++;
				mydatacount+=2;//数据大小
			}
		}
		else
		{	
			//第一个字节
			char c_w[2]={0};
			WideCharToMultiByte(CP_ACP,0,&w,-1,c_w,2,NULL,NULL);
			h=(c_w[0]&0xf0)>>4;if(h>9) h+=0x57;else h+=0x30;
			l=(c_w[0]&0x0f);if(l>9)  l+=0x57;else l+=0x30;

			char arr[2];
			arr[0]=h;
			arr[1]=l;

			StrToHexes(arr,2,&buf[m]);//数据
			m++;

			//第二个字节
			h=(c_w[1]&0xf0)>>4;if(h>9) h+=0x57;else h+=0x30;
			l=(c_w[1]&0x0f);if(l>9)  l+=0x57;else l+=0x30;

			arr[0]=h;
			arr[1]=l;

			StrToHexes(arr,2,&buf[m]);//数据
			m++;
			buf[m]=color;//颜色
			m++;
			mydatacount+=3;
		}
	}
	buf[4]=mydatacount/255;//数据大小高位
	buf[5]=mydatacount%255;//数据大小低位
	for(int i=2;i<m;i++)
	{
		buf[m]^=buf[i];
	}
	buf[m+1]='\0';

	return m+1;
}

void SLZCWndScreen::AssicToHex(char *param)
{
	if(*param >= '0' && *param <= '9')
		*param -= '0';
	else if(*param >= 'a' && *param <= 'f')
		*param -= ('a'-10);
	else if(*param >= 'A' && *param <= 'F')
		*param -= ('A'-10);
}

void SLZCWndScreen::StrToHexes(char *pc,int nsize,char *puc)
{
	int i=0;
	char uctemp=0;
	for(;i<nsize;i+=2)
	{
		*puc=*pc++;
		AssicToHex(puc);
		*puc <<= 4;//高位
		uctemp=*pc++;//低位
		AssicToHex(&uctemp);
		*puc+=uctemp;
		puc++;
	}
}

int SLZCWndScreen::DoSingleColorMsg(CString msg,int address,char* buf)
{
	msg.Replace(_T("#0#"),_T(""));
	msg.Replace(_T("#1#"),_T(""));
	msg.Replace(_T("#2#"),_T(""));
	char h,l;
	int count = msg.GetLength();
	if (count==0)
	{
		return 0;
	}
	int h2 = address&0xc0;
	h2>>=2;
	buf[0]=0x80+address;
	buf[1]=0xc0+h2;
	buf[2]='D';
	int index = 3;
	for (int i=0;i<count;i++)
	{
		WCHAR w=msg.GetAt(i);
		if (w<=255)
		{
			char ch = w;
			buf[index] = ch;
			index++;
		}
		else
		{
			//第一个字节
			char c_w[2]={0};
			WideCharToMultiByte(CP_ACP,0,&w,-1,c_w,2,NULL,NULL);
			h=(c_w[0]&0xf0)>>4;if(h>9) h+=0x57;else h+=0x30;
			l=(c_w[0]&0x0f);if(l>9)  l+=0x57;else l+=0x30;

			char arr[2];
			arr[0]=h;
			arr[1]=l;

			StrToHexes(arr,2,&buf[index]);//数据


			//第二个字节
			h=(c_w[1]&0xf0)>>4;if(h>9) h+=0x57;else h+=0x30;
			l=(c_w[1]&0x0f);if(l>9)  l+=0x57;else l+=0x30;

			arr[0]=h;
			arr[1]=l;

			StrToHexes(arr,2,&buf[index+1]);//数据
			index+=2;
		}
	}
	buf[index]=0x1a;
	return index+1;
}

int SLZCWndScreen::InitScreen(char *buf,const int address)
{
	buf[0]=(char)WNDSCREEN_HEAD_FIRST;
	buf[1]=(char)WNDSCREEN_HEAD_SCEND;
	buf[2]=address;
	buf[3]=(char)WNDSCREEN_SHOW_ROM;
	buf[4]=0x00;
	buf[5]=0x00;
	buf[6]=buf[2]^buf[3]^buf[4]^buf[5];
	buf[7]='\0';
	return 8;
}

BOOL SLZCWndScreen::IsResaved(const char* buf)
{
	int size=strlen(buf);
	if(buf[size]==0xAA)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

DWORD WINAPI SLZCWndScreen::DoWndScreenMsgThread(LPVOID pParm)
{
	while(1)
	{
		SLZCWndScreen* pDlg=(SLZCWndScreen*)pParm;
		if(pDlg->m_listScreenMsg.GetCount()==0)
		{
			Sleep(100);
		}
		else
		{
			BOOL res = FALSE;
//			DWORD dwWrite;
			POSITION pos=pDlg->m_listScreenMsg.GetHeadPosition();
			while(pos!=NULL)
			{
				pDlg->m_WndScreenMutex.Lock();
				WNDSCREENMSG wndmsg=pDlg->m_listScreenMsg.GetNext(pos);
				pDlg->m_listScreenMsg.RemoveHead();
				pDlg->m_WndScreenMutex.Unlock();
				char buf[1024]={0};

				int size = pDlg->DoScreenMsg(wndmsg.strmsg,wndmsg.address,
					buf);
				char Singlebuf[1024]={0};
				int length = pDlg->DoSingleColorMsg(wndmsg.strmsg,wndmsg.address,Singlebuf);
				
				WriteComMsg *pMsg = new WriteComMsg;
				memset(pMsg->buf,0,DATABUFLEN);
				memcpy(pMsg->buf,buf,size);
				pMsg->length = size;
				WriteComMsg *pSingleMsg = new WriteComMsg;
				memset(pSingleMsg->buf,0,DATABUFLEN);
				memcpy(pSingleMsg->buf,Singlebuf,length);
				pSingleMsg->length = length;
				CDoComInOut* pComInOut = CDoComInOut::GetInstance();
				pComInOut->AddWriteComMsg(pSingleMsg);
				pComInOut->AddWriteComMsg(pMsg);
			}
		}
	}
	return 0;
}

void SLZCWndScreen::AddScreenMsg(CString& msg,const int address)
{
	if(msg!=_T(""))
	{
		WNDSCREENMSG wndmsg;
		wndmsg.address=address;
		wndmsg.strmsg=msg;
		wndmsg.color=0;
		m_WndScreenMutex.Lock();
		m_listScreenMsg.AddTail(wndmsg);
		m_WndScreenMutex.Unlock();
	}
}

void SLZCWndScreen::StartHardScreen()
{
	if(m_hDoWndScreenMsgThread == NULL)
	{
		//缓冲处理窗口屏线程
		m_hDoWndScreenMsgThread=CreateThread(NULL,0,
			DoWndScreenMsgThread,this,NULL,0);
	}
	if(m_hDoThrWndMsgThread == NULL)
	{
		m_hDoThrWndMsgThread = CreateThread(NULL,0,DoThrWndMsgThread,
			this,NULL,0);
	}

	if(!m_hDoStbScreenThread)
	{
		m_hDoStbScreenThread = CreateThread(NULL,0,DoStbScreenMsgThread,this,NULL,0);
	}

	CDoFile doFile;
	m_pStbDisplay->Start();
	ReadStbContentInfo();
	CString strtitle,strstbid,strnotice,strtitlepic,strnewfile;
	strnewfile = doFile.GetExeFullFilePath();
	strnewfile += _T("\\webRoot\\img\\hspt.jpg");
	list<CStbContent*>::const_iterator itera = m_list_stbcontent.begin();
	for(itera;itera!=m_list_stbcontent.end();++itera)
	{
		strtitle = (*itera)->GetStbTitle();
		strnotice = (*itera)->GetStbNotice();
		strstbid = (*itera)->GetStbNum();
		strtitlepic = (*itera)->GetStbTitlePicPath();
// 		if(!doFile.IsFileExit(strtitlepic) && !strtitlepic.IsEmpty())
// 		{
			::CopyFile(strtitlepic,strnewfile,FALSE);
//		}
		m_pStbDisplay->StbUpdateTitleHtml(strtitle,strstbid);
		m_pStbDisplay->StbUpdateNoticeHtml(strnotice,strstbid);
	}

}
/*
void SLZCWndScreen::InitThroughScreen(const int address)
{
	ThroughScreenHead head;
	memset(&head,0,sizeof(head));
	head.flagFirst = HARDWARE_THROUGH_FLAG1;
	head.flagSecond = HARDWARE_THROUGH_FLAG2;
	head.address = address;
	char cmd[15]="get qtlist.lst";

	head.length = 14;
	unsigned char headAddCmd[22]={0};
	memcpy(headAddCmd,&head,sizeof(head));
	memcpy(&headAddCmd[7],cmd,14);
	headAddCmd[21]='\0';
	DWORD dwWrite = 0;
	CComInit* pComInit = CComInit::GetInstance();
// 	Sleep(1);
// 	int res=WriteFile(pComInit->m_hComWndScreen,
// 		headAddCmd,22,&dwWrite,NULL);
	WriteComMsg *pMsg = new WriteComMsg;
//	pMsg->buf = new char(22);
	memset(pMsg->buf,0,DATABUFLEN);
	memcpy(pMsg->buf,headAddCmd,22);
	pMsg->length = 22;
	CDoComInOut* pComInOut = CDoComInOut::GetInstance();
	pComInOut->AddWriteComMsg(pMsg);
}
*/
/*
void SLZCWndScreen::InitThroughScreen(const CString IP,USHORT port)
{
	ThroughScreenHead head;
	memset(&head,0,sizeof(head));
	head.flagFirst = HARDWARE_THROUGH_FLAG1;
	head.flagSecond = HARDWARE_THROUGH_FLAG2;
	head.address = 255;
	char cmd[15]="get qtlist.lst";

	head.length = 14;
	unsigned char headAddCmd[22]={0};
	memcpy(headAddCmd,&head,sizeof(head));
	memcpy(&headAddCmd[7],cmd,14);
	headAddCmd[21]='\0';

// 	char recvBuf[2*1024]={0};
// 	CComplSocketClient client;
// 	int actRecvSize = 0;
// 	if(client.SendData(port,IP,(char*)headAddCmd,21,recvBuf,2*1024,actRecvSize))
// 	{
// 		if(actRecvSize>0)
// 			AddThroughInitStr(recvBuf,actRecvSize);
// 	}
}
*/
CString SLZCWndScreen::FlushCstringToFitWndScreen(const CString& str,const int length,int height)
{
	if(height==1)return str;
	int num=0;
	CString temp=_T("");
	int count=str.GetLength();
	for(int i=0;i<count;i++)
	{
		BOOL flag=FALSE;
		WCHAR w=str.GetAt(i);
		WCHAR secd;WCHAR three;
		if(w<=255)
		{
			if(w=='#')
			{
				int i_temp=i;
				secd=str.GetAt(i_temp+1);
				if(secd<=255 && secd>='0' && secd<='2')
				{
					three=str.GetAt(i_temp+2);
					if(three<=255 && three=='#')
					{
						flag=TRUE;
						i+=2;
					}
				}
			}
			else
			{
				num++;
			}
		}
		else if(w>255)
		{
			num+=2;
		}
		
		if(num<length)
		{
			temp+=w;
			if(flag)
			{
				temp+=secd;
				temp+=three;
			}
		}
		else if(num>length)
		{
			temp+=_T(" ");
			temp+=w;
			if(w>255)
			{
				num=2;
			}
			else
			{
				num=1;
			}
		}
		else if(num==length)
		{
			temp+=w;
			num = 0;
		}
	}

	if(num<length && num!=0)
	{
		for(int j=0;j<length-num;j++)
		{
			temp+=_T(" ");
		}
	}
	return temp;
}


// void SLZCWndScreen::AddThroughInitStr(const char* buf,const DWORD count)
// {
// 	m_recvThroughInitStr.append(buf,count);
// }

// BOOL SLZCWndScreen::DoThroughInitMsg()
// {
// 	CDoComInOut* pComInOut = CDoComInOut::GetInstance();
// 	pComInOut->SetThroughInitDone(TRUE);
// 	if(m_recvThroughInitStr.empty())return FALSE;
// 	string::size_type position,oldPosition;
// 	position = m_recvThroughInitStr.find("Get_Data");
// 	while(position!=m_recvThroughInitStr.npos)
// 	{
// 		oldPosition = position;
// 		position = m_recvThroughInitStr.find("Get_Data",position+1);
// 		string temp = m_recvThroughInitStr.substr(oldPosition,position-oldPosition-1);
// 		m_list_recvString.push_back(temp);
// 	}
// 	string recvMsg;
// 	if(!m_list_recvString.empty())
// 	{
// 		std::list<string>::const_iterator itera = m_list_recvString.begin();
// 		for(itera;itera!=m_list_recvString.end();itera++)
// 		{
// 			recvMsg= *itera;
// 			if(!recvMsg.empty())
// 			{
// 				string::size_type first_pos = GetIpPos(recvMsg);
// 				while(first_pos!=recvMsg.npos)//找到了
// 				{
// 					/////////////////////////////通屏基本信息如:屏地址
// 					string::size_type last_Pos = first_pos;
// 					ThrScreenBasicMsg ThrScreenMsg;
// 					memset(&ThrScreenMsg,0,sizeof(ThrScreenMsg));
// 					string basicInfo = recvMsg.substr(first_pos,10);
// 					ThrScreenMsg.address = basicInfo[7];
// 					
// 					recvMsg = recvMsg.substr(first_pos+10);
// 					first_pos = GetIpPos(recvMsg);
// 					string temp = recvMsg.substr(0,first_pos);
// 					
// 					for(int i=0;i<(int)temp.size();i+=34)
// 					{
// 						string channel = temp.substr(i,34);
// 						ThrScreenMsg.channel = channel[0];//通道号
// 						ThrScreenMsg.fone = channel[8];//字体
// 						memcpy(&ThrScreenMsg.width,&channel[4],2);
// 						memcpy(&ThrScreenMsg.height,&channel[6],2);
// 						AddThrBasicMsg(ThrScreenMsg);
// 					}
// 					
// 					/*
// 					string throughMsg = recvMsg.substr(first_pos+1,32);
// 					ThrScreenMsg.address = throughMsg[29];
// 					///////////////////////////////////////////
// 					string temp = recvMsg.substr(first_pos+33);
// 					for(int i=0;i<(int)temp.size();i+=34)
// 					{
// 						/////////////////通道信息
// 						string channel = temp.substr(i,34);
// 						ThrScreenMsg.channel = channel[0];//通道号
// 						ThrScreenMsg.fone = channel[8];//字体
// 						memcpy(&ThrScreenMsg.width,&channel[4],2);
// 						memcpy(&ThrScreenMsg.height,&channel[6],2);
// 						/////////////////////
// 						AddThrBasicMsg(ThrScreenMsg);
// 					}
// 					*/
// 					////////////
// 				}
// 			}
// //			m_list_recvString.pop_front();
// 		}
// 		m_list_recvString.clear();//删除
// 	}
// 	m_recvThroughInitStr.clear();
// 	return TRUE;
// }

/*
void SLZCWndScreen::AddThroughInitStr(const char* buf,const DWORD count)
{
	m_recvThroughInitStr.append(buf,count);
}
*/

/*
BOOL SLZCWndScreen::DoThroughInitMsg()
{
	CDoComInOut* pComInOut = CDoComInOut::GetInstance();
	pComInOut->SetThroughInitDone(TRUE);
	if(m_recvThroughInitStr.empty())return FALSE;
	string::size_type position,oldPosition;
	position = m_recvThroughInitStr.find("Get_Data");
	while(position!=m_recvThroughInitStr.npos)
	{
		oldPosition = position;
		position = m_recvThroughInitStr.find("Get_Data",position+1);
		string temp = m_recvThroughInitStr.substr(oldPosition,position-oldPosition-1);
		m_list_recvString.push_back(temp);
	}
	string recvMsg;
	if(!m_list_recvString.empty())
	{
		std::list<string>::const_iterator itera = m_list_recvString.begin();
		for(itera;itera!=m_list_recvString.end();itera++)
		{
			recvMsg= *itera;
			if(!recvMsg.empty())
			{
				string::size_type first_pos = GetIpPos(recvMsg);
				while(first_pos!=recvMsg.npos)//找到了
				{
					/////////////////////////////通屏基本信息如:屏地址
					string::size_type last_Pos = first_pos;
					ThrScreenBasicMsg ThrScreenMsg;
					memset(&ThrScreenMsg,0,sizeof(ThrScreenMsg));
					string basicInfo = recvMsg.substr(first_pos,10);
					ThrScreenMsg.address = basicInfo[7];
					
					recvMsg = recvMsg.substr(first_pos+10);
					first_pos = GetIpPos(recvMsg);
					string temp = recvMsg.substr(0,first_pos);
					
					for(int i=0;i<(int)temp.size();i+=34)
					{
						string channel = temp.substr(i,34);
						ThrScreenMsg.channel = channel[0];//通道号
						ThrScreenMsg.fone = channel[8];//字体
						memcpy(&ThrScreenMsg.width,&channel[4],2);
						memcpy(&ThrScreenMsg.height,&channel[6],2);
						AddThrBasicMsg(ThrScreenMsg);
					}
					
					
				}
			}
		}
		m_list_recvString.clear();//删除
	}
	m_recvThroughInitStr.clear();
	return TRUE;
}
*/

void SLZCWndScreen::AddThrBasicMsg(ThrScreenBasicMsg msg)
{
	BOOL flag = FALSE;
	std::list<ThrScreenBasicMsg>::iterator itera = m_list_thrBasicMsg.begin();
	for(itera;itera!=m_list_thrBasicMsg.end();itera++)
	{
		if(msg.address == itera->address && msg.channel==itera->channel)
		{
			flag = TRUE;
			itera->fone = msg.fone;
			itera->height = msg.height;
			itera->width = msg.width;
			break;
		}
	}
	if(!flag)
		m_list_thrBasicMsg.push_back(msg);
}

BOOL SLZCWndScreen::SendDataToThroughScreen(const CString& str,int address,int channel,const CString& localIp)
{
	///自己公司协议
// 	int height = 0;
// 	int width = FindChannelWidth(address,channel,height);
	CString msg = str;
// #ifdef _DEBUG
//  	CString test;
// 	test.Format(_T("through wnd:address:%d,channel:%d,width:%d"),address,channel,width);
//  	MyWriteConsole(test);
// 	MyWriteConsole(str);
// #endif
//  	if(width>0 && width<=128)//注意小于128是因为同屏卡最多显示128个汉字
//  	{
//  		msg = FlushCstringToFitWndScreen(msg,width,height);
//  	}
	char buf[512]={0};
	int length = DoScreenMsg(msg,address+channel,buf);
	WriteComMsg *pMsg = new WriteComMsg;
	memset(pMsg->buf,0,DATABUFLEN);
	memcpy(pMsg->buf,buf,length);
	pMsg->length = length;
	CDoComInOut* pComInOut = CDoComInOut::GetInstance();
	pComInOut->AddWriteComMsg(pMsg);

	BOOL flag = FALSE;
	if(!localIp.IsEmpty())//UDP发送
	{
		MySocketUDP Client;
		Client.StartSocket(1024);
		flag = Client.SendTo(buf,length,localIp,1024);
		
#ifdef _DEBUG
		if(flag)
		{
			MyWriteConsole(_T("发送通屏消息成功"));
		}
		else
		{
			MyWriteConsole(_T("发送同频消息失败"));
		}
#endif
	}
	return flag;
}

int SLZCWndScreen::FindChannelWidth(int address,int channel,int& height)
{
	int width = -1;
	std::list<ThrScreenBasicMsg>::const_iterator itera = m_list_thrBasicMsg.begin();
	for(itera;itera!=m_list_thrBasicMsg.end();itera++)
	{
		ThrScreenBasicMsg msg = *itera;
		if(address == msg.address && channel == msg.channel)
		{
			width = msg.width/msg.fone*2;
			if(msg.width%msg.fone>=msg.fone/2)
			{
				width++;
			}
			height = msg.height/msg.fone;
			break;
		}
	}
	return width;
}

void SLZCWndScreen::AddThroughScreenMsg(const CString& msg,int address,int channel,const CString& localIp)
{
	if(address<=0||msg.IsEmpty())return;
	SendThrScreenMsg sendMsg;
	sendMsg.msg = msg;
	sendMsg.address = address;
	sendMsg.channel = channel-1;
	sendMsg.localIp = localIp;
	m_ThrWndMutex.Lock();
	m_list_sendThrMsg.push_back(sendMsg);
	m_ThrWndMutex.Unlock();
}

DWORD WINAPI SLZCWndScreen::DoThrWndMsgThread(LPVOID pParam)
{
	SLZCWndScreen* pThis = (SLZCWndScreen*)pParam;
	while(TRUE)
	{
		if(pThis->m_list_sendThrMsg.empty())
		{
			Sleep(5);
		}
		else
		{
			SendThrScreenMsg msg;
			pThis->m_ThrWndMutex.Lock();
			std::list<SendThrScreenMsg>::const_iterator itera = pThis->m_list_sendThrMsg.begin();
			msg = *itera;
			pThis->m_list_sendThrMsg.pop_front();
			pThis->m_ThrWndMutex.Unlock();
//			WaitForSingleObject(pThis->m_hDoWndScreenMsgThread,3);
#ifdef _DEBUG
			CString strAddress;
			strAddress.Format(_T("%d"),msg.address);
			CString strChannel;
			strChannel.Format(_T("%d"),msg.channel);


			MyWriteConsole(_T("通屏信息:") + msg.msg + _T("地址:") + strAddress + _T("通道:") + strChannel + _T("ip:") + msg.localIp );
#endif
			pThis->SendDataToThroughScreen(msg.msg,msg.address,msg.channel,msg.localIp);
		}
	}
	return 0;
}

string::size_type SLZCWndScreen::GetIpPos(const string& msg)
{
	string::size_type resultPos = msg.npos;
	string::size_type firstPoint = msg.find(".");
	if(firstPoint == msg.npos)
		return msg.npos;
	string::size_type seconedPoint = msg.find(".",firstPoint+1);
	if(seconedPoint == msg.npos)
		return msg.npos;
	string ip2 = msg.substr(firstPoint+1,seconedPoint-firstPoint-1);
	int i_ip2 = atoi(ip2.c_str());
	if(i_ip2<0 || i_ip2>255)
		return msg.npos;
	string::size_type thirdPoint = msg.find(".",seconedPoint+1);
	if(thirdPoint == msg.npos)
		return msg.npos;
	string next1 = msg.substr(thirdPoint+1,1);
	int i_next1 = atoi(next1.c_str());
	if(i_next1>=0 && i_next1<=9)
		resultPos = thirdPoint+1;
	string next2 = msg.substr(thirdPoint+2,1);
	int i_next2 = atoi(next2.c_str());
	if(i_next2>=0 && i_next2<=9)
		resultPos = thirdPoint+2;
	string next3 = msg.substr(thirdPoint+3,1);
	int i_next3 = atoi(next3.c_str());
	if(i_next3>=0 && i_next3<=9)
		resultPos = thirdPoint+3;
	return resultPos+1;
}

DWORD WINAPI SLZCWndScreen::DoStbScreenMsgThread(LPVOID pParam)
{
	SLZCWndScreen* pThis = (SLZCWndScreen*)pParam;
	while(TRUE)
	{
		if(pThis->m_list_stdscreenmsg.empty())
		{
			Sleep(5);
		}
		else
		{
			StbScreenMsg stbScreenMsg;
			pThis->m_mtStbScreenMsg.Lock();
			list<StbScreenMsg>::const_iterator itera = pThis->m_list_stdscreenmsg.begin();
			stbScreenMsg = *itera;
			pThis->m_list_stdscreenmsg.pop_front();
			pThis->m_mtStbScreenMsg.Unlock();

			CString strStbNum;
			CString strRetMsg = pThis->ProduceStbMsg(strStbNum,stbScreenMsg.uStbID);

			if(pThis->m_pStbDisplay && !strRetMsg.IsEmpty())
			{
				pThis->m_pStbDisplay->StbUpdateCallMsg(strRetMsg,strStbNum);
			}

			if(!theApp.IsLocal())//开启了联机取号,客户机发送机顶盒信息到服务端
			{
				string aRetSendStbMsg;
				CDealInterMsg::ProduceSendSTBShowMsg(stbScreenMsg.strMsg,strStbNum,aRetSendStbMsg);
				CComplSocketClient client;
				int actRecvSize = 0;
				string recvMsg;
				if(client.SendData(INTERPORT,theApp.m_logicVariables.strInterIP,
					aRetSendStbMsg,aRetSendStbMsg.size(),recvMsg,actRecvSize) && actRecvSize)
				{
					BOOL bSucced = FALSE;
					CDealInterMsg::AnaRetStbMsg(recvMsg,&bSucced);
#ifdef _DEBUG
					if(bSucced)
						MyWriteConsole(_T("stbmsg client send succed"));
					else
						MyWriteConsole(_T("stbmsg client send failed"));
#endif
				}
			}
#ifdef _DEBUG
			MyWriteConsole(_T("retmsg:") + strRetMsg);
#endif
		}
	}
	return 0;
}

void SLZCWndScreen::AddStbScreenMsg(const CString& msg, UINT uStbID)
{
	if(msg.IsEmpty() || uStbID == 0)
		return;
	StbScreenMsg stb;
	stb.strMsg = msg;
	stb.uStbID = uStbID;

	m_mtStbmapcallmsg.Lock();
	map<UINT,CStringArray*>::const_iterator itera;
	itera = m_map_stbcallMsg.find(uStbID);
	if(itera != m_map_stbcallMsg.end())
	{
		if(itera->second->GetCount() > 50)
		{
			itera->second->RemoveAt(0,10);
		}
		itera->second->Add(msg);
	}
	else
	{
		CStringArray* pStrMsgArray = new CStringArray;
		pStrMsgArray->Add(msg);
		m_map_stbcallMsg[uStbID] = pStrMsgArray;
	}
	m_mtStbmapcallmsg.Unlock();


	m_mtStbScreenMsg.Lock();
	m_list_stdscreenmsg.push_back(stb);
	m_mtStbScreenMsg.Unlock();
}

BOOL SLZCWndScreen::ReadStbContentInfo()
{
	CDoFile doFile;
	CString strExePath = doFile.GetExeFullFilePath();
	strExePath += _T("\\StbBasicInfo\\StbInfo.dat");
	CFile file;
	CFileException e;
	if (file.Open(strExePath,CFile::modeRead,&e))
	{
		CStbContent* pStb=NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>pStb;
				if (pStb)
				{
					m_list_stbcontent.push_back(pStb);
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
			return TRUE;
	}
	return FALSE;
}

void SLZCWndScreen::ClearStbContentInfo()
{
	CStbContent* pStb = NULL;
	list<CStbContent*>::const_iterator itera = m_list_stbcontent.begin();
	for(itera;itera!=m_list_stbcontent.end();++itera)
	{
		pStb = *itera;
		if(pStb)
			delete pStb;
	}

	m_list_stbcontent.clear();
}

BOOL SLZCWndScreen::ReFlushStbContentInfo()
{
	ClearStbContentInfo();
	return ReadStbContentInfo();
}

void SLZCWndScreen::UpdateStbTitleAndNotice()
{
	CDoFile doFile;
	CString strtitle,strstbid,strnotice,strtitlepic,strnewfile;
	strnewfile = doFile.GetExeFullFilePath();
	strnewfile += _T("\\webRoot\\img\\hspt.jpg");

	list<CStbContent*>::const_iterator itera = m_list_stbcontent.begin();
	for(itera;itera!=m_list_stbcontent.end();++itera)
	{
		strtitle = (*itera)->GetStbTitle();
		strnotice = (*itera)->GetStbNotice();
		strstbid = (*itera)->GetStbNum();
		strtitlepic = (*itera)->GetStbTitlePicPath();
//		if(doFile.IsFileExit() && !strtitlepic.IsEmpty())
//		{
		::CopyFile(strtitlepic,strnewfile,FALSE);
//		}
		m_pStbDisplay->StbUpdateTitleHtml(strtitle,strstbid);
		m_pStbDisplay->StbUpdateNoticeHtml(strnotice,strstbid);
	}
}

void SLZCWndScreen::ClearMapStbcallMsg()
{
	map<UINT,CStringArray*>::const_iterator itera = m_map_stbcallMsg.begin();
	for(itera;itera!=m_map_stbcallMsg.end();++itera)
	{
		if(itera->second)
		{
			delete itera->second;
		}
	}
	m_map_stbcallMsg.clear();
}

CString SLZCWndScreen::ProduceStbMsg(CString& strStbNum,UINT uStbID)
{
	CString strRetMsg;
	CStbContent* pStb = NULL;
	list<CStbContent*>::const_iterator itera = m_list_stbcontent.begin();
	for(itera;itera!=m_list_stbcontent.end();++itera)
	{
		if((*itera)->GetSerialID() == uStbID)
		{
			pStb = (*itera);
			strStbNum = pStb->GetStbNum();
			break;
		}
	}

	if(pStb)
	{
		int nLineNum = pStb->GetStbLineNum();
		CStringArray* pStrArray = NULL;
		m_mtStbmapcallmsg.Lock();
		map<UINT,CStringArray*>::const_iterator itera;
		itera = m_map_stbcallMsg.find(uStbID);
		if(itera != m_map_stbcallMsg.end())
		{
			pStrArray = itera->second;
		}
		m_mtStbmapcallmsg.Unlock();
		if(pStrArray)
		{
			int count = pStrArray->GetCount();
			if(nLineNum <= count)
			{
				int temp = count - nLineNum;
				for(temp;temp<count;temp++)
				{
					strRetMsg += pStrArray->GetAt(temp);
					strRetMsg += _T("<br/> ");
				}
			}
			else
			{
				for(int i=0;i<pStrArray->GetCount();i++)
				{
					strRetMsg += pStrArray->GetAt(i);
					strRetMsg += _T("<br/> ");
				}
			}
		}
	}
	return strRetMsg;
}

BOOL SLZCWndScreen::GetStbIDByStbNum(const CString& strStbNum,UINT* pStbID)
{
	BOOL flag = FALSE;
	CStbContent* pStb = NULL;
	list<CStbContent*>::const_iterator itera = m_list_stbcontent.begin();
	for(itera;itera!=m_list_stbcontent.end();++itera)
	{
		if((*itera)->GetStbNum() == strStbNum)
		{
			pStb = (*itera);
			*pStbID = pStb->GetSerialID();
			flag = TRUE;
			break;
		}
	}
	return flag;
}