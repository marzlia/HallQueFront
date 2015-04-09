#include "StdAfx.h"
#include "SLZCardReader.h"
#include "CommonConvert.h"
#include "ComInit.h"
#include "DoFile.h"
#include "HallQueFront.h"
#include "TCPConnect.h"
#include "福建\DoWebService.h"

extern  void MyWriteConsole(CString str); 

SLZCardReader::SLZCardReader(void) : m_hReadTread(NULL),
m_hReadCard(NULL)
{
	OpenReadCard();
	CDoFile doFile;
	////////卡识别信息文件路径
	m_cardcofinfo_path = doFile.GetExeFullFilePath();
	m_cardcofinfo_path += _T("\\CardConfigInfo\\CardConfigInfo.dat");
	////////卡对接信息文件路径
	m_connect_path = doFile.GetExeFullFilePath();
	m_connect_path += _T("\\CardConfigInfo\\CardConnectInfo.dat");
	////////卡等级信息文件路径
	m_cardLev_path = doFile.GetExeFullFilePath();
	m_cardLev_path += _T("\\CardConfigInfo\\CardLevInfo.dat");
}

SLZCardReader::~SLZCardReader(void)
{
	TerminateThread(m_hReadTread,0);
	TerminateThread(m_hReadCard,0);
	if(m_hReadTread)
	{
		CloseHandle(m_hReadTread);
		m_hReadTread = NULL;
	}
	if(m_hReadCard)
	{
		CloseHandle(m_hReadCard);
		m_hReadCard = NULL;
	}
	CComInit* pComInit = CComInit::GetInstance();
	if(pComInit->m_hComReadCard != INVALID_HANDLE_VALUE)
	{
		CloseHandle(pComInit->m_hComReadCard);
		pComInit->m_hComReadCard = INVALID_HANDLE_VALUE;
	}
	ClearSwingCardInfo();
	ClearCardLevInfo();
}

void SLZCardReader::Run()
{
	//初始化后 启动两个线程
	if(Init_Library())
	{
		m_hReadTread=CreateThread(NULL,0,ReadThread,this,0,0);
		if (m_hReadTread==0)
		{
			AfxMessageBox(_T("身份证识别启动失败！"));
		}
	}
	m_hReadCard=CreateThread(NULL,0,ReadCard,this,0,NULL);
	if (m_hReadCard==0)
	{
		AfxMessageBox(_T("银行卡识别启动失败！"));
	}
}

BOOL SLZCardReader::HasData() 
{
	return !m_CardInfoList.IsEmpty(); //判断是否为空
}

BOOL SLZCardReader::Init_Library()
{
	HINSTANCE h_instance = NULL;
	h_instance=LoadLibrary(_T("sdtapi.dll")); //加载动态库
	if(h_instance)
	{
		SDT_StartFindIDCard=(int(_stdcall*)(int,unsigned char*,int))GetProcAddress(h_instance,"SDT_StartFindIDCard");
		SDT_SelectIDCard=(int(_stdcall*)(int,unsigned char*,int))GetProcAddress(h_instance,"SDT_SelectIDCard");
		SDT_ReadBaseMsg=(int(_stdcall*)(int,unsigned char*,unsigned int*,unsigned char*,unsigned int*,int))GetProcAddress(h_instance,"SDT_ReadBaseMsg");
	}
	else
	{
		AfxMessageBox(_T("加载二代证动态库失败"));
		return FALSE;
	}
	
	return TRUE;
}


DWORD WINAPI SLZCardReader::ReadThread(LPVOID pParam)
{
	SLZCardReader* m_Read=(SLZCardReader*)pParam;
	while(1)
	{
		m_Read->ReadMsg();
		Sleep(200);
	}
	return 0;
}

BOOL SLZCardReader::ReadMsg()
{
	Sleep(10);
	int iPort=1001;
	unsigned char pucManaInfo[256]={0};
	unsigned char pucManaMsg[255]={0};
	unsigned char pucCHMsg[256]={0};
	unsigned char pucPHMsg[1024]={0};
	unsigned int puiCHMsgLen=0;
	unsigned int puiPHMsglen=0;
	int st=0;
	wchar_t pNU[36]={0};
	wchar_t pName[30]={0};
	if(!(SDT_ReadBaseMsg&&SDT_SelectIDCard&&SDT_StartFindIDCard))
	{
		AfxMessageBox(_T("获取地址失败"));
		return FALSE;
	}
	try
	{
		st=SDT_StartFindIDCard(iPort,pucManaInfo,1);
		if(st!=0x9f)
		{
 			return FALSE;
		}

		st=SDT_SelectIDCard(iPort,pucManaMsg,1);
		if(st!=0x90)
		{
			return FALSE;
		}
		st=SDT_ReadBaseMsg(iPort,pucCHMsg,&puiCHMsgLen,pucPHMsg,&puiPHMsglen,1);
		if(st!=0x90)
		{
			return FALSE;
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("读取错误"));
		return FALSE;
	}
	// 格式转换
	memcpy(pName,&pucCHMsg[0],30);
	CString name(pName);
	memcpy(pNU,&pucCHMsg[122],36);
	CString num(pNU);  
	CARDINFO cardinfo;
	cardinfo.iCardType=cardIDCard;
	cardinfo.strCardNumber=num;//卡号
	cardinfo.strCustName=name;//姓名
	
	if(m_cardConnectInfo.IsConnect)//对接
	{
		///对接获取该身份证对应的级别和应该排哪个队列
		
		//int nLev = GetCustLev(cardinfo.strCardNumber);
		//if(nLev!=-1)
		//{
		//	CString queID = JudgeCardAttchQue(nLev);
		//	cardinfo.strAttchQueID = queID;
		//	cardinfo.iCustLevel = nLev;
		//}
		//if(!theApp.m_Controller.JudgeWorkTimeOut(cardinfo.strAttchQueID)&&!cardinfo.strAttchQueID.IsEmpty())
		//{
		//	m_CardReaderMutex.Lock();
		//	m_CardInfoList.AddTail(cardinfo); //加入缓冲区
		//	m_CardReaderMutex.Unlock();
		//}
		
	}
	else//不对接
	{
		cardinfo.iCustLevel=0;
		CString queID(m_cardConnectInfo.RegAttchQueID);//身份证关联队列ID
		if (!queID.IsEmpty())
		{
			cardinfo.strAttchQueID = queID;
			//没有超出工作时间
			if(!theApp.m_Controller.JudgeWorkTimeOut(queID))
			{
				m_CardReaderMutex.Lock();
				m_CardInfoList.AddTail(cardinfo); //加入缓冲区
				m_CardReaderMutex.Unlock();
			}
		}
		///链接界面
		cardinfo.nAttchPageID = m_cardConnectInfo.RegAttchPageID;
//		theApp.m_pView->ShowPage(cardinfo.nAttchPageID);
		SendMessage(theApp.m_pView->m_hWnd,WM_SHOWPAGE,(WPARAM)cardinfo.nAttchPageID,NULL);
	}
	return TRUE;
}

DWORD WINAPI SLZCardReader::ReadCard(LPVOID pParam)
{
	SLZCardReader* pCard=(SLZCardReader*)pParam;
	while(1)
	{
		unsigned char buf[MAX_CARD_NO]={0};
		DWORD dwReaded=0;//读了多少
		BOOL bres=FALSE;
		DWORD dwErrorFlags;
		COMSTAT ComStat;
		OVERLAPPED osRead;
		memset(&osRead,0,sizeof(OVERLAPPED));
		CComInit* pComInit = CComInit::GetInstance();
		if(pComInit->m_hComReadCard!=INVALID_HANDLE_VALUE)
		{	
			ClearCommError(pComInit->m_hComWndScreen,&dwErrorFlags,&ComStat);
			Sleep(300);
			bres=ReadFile(pComInit->m_hComReadCard,buf,MAX_CARD_NO,&dwReaded,&osRead); //从设备中读数据
			std::string strbuf((char*)buf);
			if(!strbuf.empty())
			{
				CARDINFO cardinfo;
				//判断卡号是否读完了
				std::string::size_type pos = strbuf.find('?');
				if(pos == strbuf.npos){//没读完continu
					pCard->m_strCardNum += strbuf;
					continue;
				}
				//读完了，+=赋值
				pCard->m_strCardNum += strbuf;
				//提取卡号
				pos = pCard->m_strCardNum.find('=');
				if(pos!=pCard->m_strCardNum.npos)
				{
					std::string strCardNum = pCard->m_strCardNum.substr(1,pos-1);
					cardinfo.strCardNumber = strCardNum.c_str();
				}
				else
				{
					pos = pCard->m_strCardNum.find('?');
					if(pos!=pCard->m_strCardNum.npos)
					{
						std::string strCardNum = pCard->m_strCardNum.substr(1,pos-1);
						cardinfo.strCardNumber = strCardNum.c_str();
					}
				}
				//卡号质空
				pCard->m_strCardNum = "";
#ifdef _DEBUG
				MyWriteConsole(cardinfo.strCardNumber);
#endif
				if(cardinfo.strCardNumber.IsEmpty()) continue;
				cardinfo.iCardType=cardMagCard;
				if(!pCard->m_cardConnectInfo.IsConnect)
				{
					cardinfo.strAttchQueID = pCard->JudgeCardAttchQue(cardinfo.strCardNumber);
					cardinfo.nAttchPageID = pCard->JudgeCardAttchPageID(cardinfo.strCardNumber);
				}
				else
				{
					//这里对接,返回的是卡等级
					CustLev LevValue;
					int nCustLev = pCard->GetCustLev(cardinfo.strCardNumber,&LevValue);
					if(nCustLev != -1 && LevValue.isSucced){
						//以客户等级去判断
						cardinfo.custLev = LevValue;
						cardinfo.strAttchQueID = pCard->JudgeCardAttchQue(LevValue.custLev);
						cardinfo.nAttchPageID = pCard->JudgeCardAttchPageID(LevValue.custLev);
					}
					else{//以本地判断
						cardinfo.strAttchQueID = pCard->JudgeCardAttchQue(cardinfo.strCardNumber);
						cardinfo.nAttchPageID = pCard->JudgeCardAttchPageID(cardinfo.strCardNumber);
					}
				}
				//判断是否超出工作时间
				if(!theApp.m_Controller.JudgeWorkTimeOut(cardinfo.strAttchQueID)&&!cardinfo.strAttchQueID.IsEmpty())
				{
					pCard->m_CardReaderMutex.Lock();
					pCard->m_CardInfoList.AddTail(cardinfo); //加入缓冲区
					pCard->m_CardReaderMutex.Unlock();
				}
//				theApp.m_pView->ShowPage(cardinfo.nAttchPageID);
				SendMessage(theApp.m_pView->m_hWnd,WM_SHOWPAGE,(WPARAM)cardinfo.nAttchPageID,NULL);
			}
		}
		Sleep(20);
	}
	return 0;
}

int SLZCardReader::GetCustLev(const CString& strCardNum,CustLev* pCustLev)
{
	CDoWebService doWebService;
	int recvValue = doWebService.GetCardLevelFromServer(m_cardConnectInfo.ServerIP,strCardNum,m_cardConnectInfo.ServerPort,
		m_cardConnectInfo.OverTime,pCustLev);
	return recvValue;
}

CARDINFO SLZCardReader::GetData()
{
	CARDINFO cardInfo;
	m_CardReaderMutex.Lock();
	cardInfo = m_CardInfoList.GetHead();
	m_CardInfoList.RemoveHead(); //读过的信息删掉
	m_CardReaderMutex.Unlock();
	return cardInfo;
}

BOOL SLZCardReader::OpenReadCard()
{
	CComInit* pComInit = CComInit::GetInstance();
	CString c_CardCom = pComInit->GetCardComm();
	CCommonConvert convert;
	int i_CardCom = 0;
	convert.CStringToint(i_CardCom,c_CardCom);
	if(pComInit->OpenCardComm(i_CardCom)==-1)
	{
		::MessageBox((HWND)AfxGetMainWnd(),_T("刷卡器串口打开失败或被占用"),
			_T("注意"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	return TRUE;
}

BOOL SLZCardReader::ReadCardConfigInfo()
{
	ClearSwingCardInfo();
	CFile file;
	CFileException e;
	if (file.Open(m_cardcofinfo_path,CFile::modeRead,&e))
	{
		////注意序列化出来的指针变量已经分配了内存
		CSwingCard* cardConfInfo = NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
		{
			do
			{
				ar>>cardConfInfo;
				if (cardConfInfo)
				{
					m_list_swingCard.Add( cardConfInfo);
				}
			}while(!ar.IsBufferEmpty());
		}
		ar.Close();
		file.Close();
	}
	else return FALSE;
	return TRUE;
}

BOOL SLZCardReader::ReadCardConnectInfo()
{
	memset(&m_cardConnectInfo,0,sizeof(m_cardConnectInfo));
	CFile file;
	CFileException e;
	if (file.Open(m_connect_path,CFile::modeRead,&e))
	{
		file.Read(&m_cardConnectInfo,sizeof(CARDCONNECTINFO));
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void SLZCardReader::ClearSwingCardInfo()
{
	for(int i=0;i<m_list_swingCard.GetCount();i++)
	{
		delete m_list_swingCard[i];
		m_list_swingCard[i] = NULL;
	}
	m_list_swingCard.RemoveAll();
}

BOOL SLZCardReader::ReFlushCardConnectInfo()
{
	return ReadCardConnectInfo();
}

BOOL SLZCardReader::ReFlushCardConfigInof()
{
	return ReadCardConfigInfo();
}

CString SLZCardReader::JudgeCardAttchQue(const CString& cardNum)
{
	if(cardNum.IsEmpty())return _T("");

	int count = m_list_swingCard.GetCount();
	CString queID;
	for(int i=0;i<count;i++)
	{
		BOOL flag = FALSE;
		for(int j=0;j<4;j++)
		{
			UINT postion = m_list_swingCard[i]->m_disTinguishCard[j].postion;
			if(postion==0)
			{
				continue;
			}
			UINT toValue = m_list_swingCard[i]->m_disTinguishCard[j].toValue;
			CString wStrValue;
			wStrValue.Format(_T("%d"),toValue);
			///判断位置
			if((int)postion-1>cardNum.GetLength() || (int)postion-1<0)
			{
				return _T("");
			}
			if(wStrValue == cardNum.GetAt(postion-1))
			{
				flag = TRUE;
			}
			else
			{
				flag = FALSE;
				break;
			}
		}
		if(flag)
		{
			queID = m_list_swingCard[i]->GetAttchQueID();
			break;
		}
	}
	return queID;
}

BOOL SLZCardReader::ReadCardConnectLev()
{
	ClearCardLevInfo();//清空卡等级信息
	CFile file;
	CFileException e;
	if (file.Open(m_cardLev_path,CFile::modeRead,&e))
	{
		////注意序列化出来的指针变量已经分配了内存
		CCardLevel* pCardLevel = NULL;
		UINT cardLevelSerialID = 0;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
		{
			do
			{
				ar>>pCardLevel;
				if (pCardLevel)
				{
					m_list_cardLev.Add(pCardLevel);
				}
			}while(!ar.IsBufferEmpty());
		}
		ar.Close();
		file.Close();
	}
	else return FALSE;
	return TRUE;
}

void SLZCardReader::ClearCardLevInfo()
{
	for(int i=0;i<m_list_cardLev.GetCount();i++)
	{
		delete m_list_cardLev[i];
		m_list_cardLev[i]=NULL;
	}
	m_list_cardLev.RemoveAll();
}

BOOL SLZCardReader::ReFlushCardLevelInfo()
{
	return ReadCardConnectLev();
}

CString SLZCardReader::JudgeCardAttchQue(const int level)
{
	CString queID;
	for(int i=0;i<m_list_cardLev.GetCount();i++)
	{
		if(m_list_cardLev[i]->GetCardLevel()==level)
		{
			queID = m_list_cardLev[i]->GetCardLevAttchToQue();
			break;
		}
	}
	return queID;
}

int SLZCardReader::JudgeCardAttchPageID(const CString& cardNum)
{
	int nPageID = -1;
	if(cardNum.IsEmpty())return -1;

	int count = m_list_swingCard.GetCount();
	CString queID;
	for(int i=0;i<count;i++)
	{
		BOOL flag = FALSE;
		for(int j=0;j<4;j++)
		{
			UINT postion = m_list_swingCard[i]->m_disTinguishCard[j].postion;
			if(postion==0)
			{
				continue;
			}
			UINT toValue = m_list_swingCard[i]->m_disTinguishCard[j].toValue;
			CString wStrValue;
			wStrValue.Format(_T("%d"),toValue);
			///判断位置
			if((int)postion-1>cardNum.GetLength() || (int)postion-1<0)
			{
				return -1;
			}
			if(wStrValue == cardNum.GetAt(postion-1))
			{
				flag = TRUE;
			}
			else
			{
				flag = FALSE;
				break;
			}
		}
		if(flag)
		{
			nPageID = m_list_swingCard[i]->GetAttchPageID();
			break;
		}
	}
	return nPageID;
}

int SLZCardReader::JudgeCardAttchPageID(int level)
{
	int nPageID = -1;
	for(int i=0;i<m_list_cardLev.GetCount();i++)
	{
		if(m_list_cardLev[i]->GetCardLevel()==level)
		{
			nPageID = m_list_cardLev[i]->GetCardLevAttchToPageID();
			break;
		}
	}
	return nPageID;
}