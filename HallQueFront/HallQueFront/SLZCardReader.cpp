#include "StdAfx.h"
#include "SLZCardReader.h"
#include "CommonConvert.h"
#include "ComInit.h"
#include "DoFile.h"
#include "HallQueFront.h"
#include "TCPConnect.h"
#include "ComputeFuncationTime.h"
#include "mtx_32.h"

#pragma comment(lib,"Mtx_32.lib")

extern  void MyWriteConsole(CString str); 

SLZCardReader::SLZCardReader(void) : m_hReadTread(NULL),
m_hReadCard(NULL)
,m_hReadICCard(NULL)
//,m_hICCardDev(NULL)
//,m_hReadIDCard(NULL)
{
	OpenReadCard();
	CDoFile doFile;
	////////��ʶ����Ϣ�ļ�·��
	m_cardcofinfo_path = doFile.GetExeFullFilePath();
	m_cardcofinfo_path += _T("\\CardConfigInfo\\CardConfigInfo.dat");
	////////���Խ���Ϣ�ļ�·��
	m_connect_path = doFile.GetExeFullFilePath();
	m_connect_path += _T("\\CardConfigInfo\\CardConnectInfo.dat");
	////////���ȼ���Ϣ�ļ�·��
	m_cardLev_path = doFile.GetExeFullFilePath();
	m_cardLev_path += _T("\\CardConfigInfo\\CardLevInfo.dat");
}

SLZCardReader::~SLZCardReader(void)
{
	TerminateThread(m_hReadTread,0);
	TerminateThread(m_hReadCard,0);
	TerminateThread(m_hReadICCard,0);
//	TerminateThread(m_hReadIDCard,0);

	if(m_hReadICCard)
	{
		CloseHandle(m_hReadICCard);
		m_hReadICCard = NULL;
	}

// 	if(m_hReadIDCard)
// 	{
// 		CloseHandle(m_hReadIDCard);
// 		m_hReadIDCard = NULL;
// 	}

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
// 	if(m_hICCardDev)
// 	{
// 		CloseICCard();
// 		m_hICCardDev = NULL;
// 	}
	CComInit* pComInit = CComInit::GetInstance();
	if(pComInit->m_hComReadCard != INVALID_HANDLE_VALUE)
	{
		CloseHandle(pComInit->m_hComReadCard);
		pComInit->m_hComReadCard = INVALID_HANDLE_VALUE;
	}
	ClearSwingCardInfo();
	ClearCardLevInfo();
}


SLZCardReader* SLZCardReader::GetInstance()
{
	static SLZCardReader Instance;
	return &Instance;
}

void SLZCardReader::Run()
{
	//��ʼ���� ���������߳�
	if(Init_Library())
	{
		m_hReadTread=CreateThread(NULL,0,ReadThread,this,0,0);
		if (m_hReadTread==0)
		{
			AfxMessageBox(_T("���֤ʶ������ʧ�ܣ�"));
		}
	}

	
// 	CComInit* pComInit = CComInit::GetInstance();
// 	CString strComm = pComInit->GetNewCardComm();
// 	OpenICCard(strComm);//���豸�˿�
	//оƬ��ˢ������ʼ��
	if(Init_ICLibrary())
	{
		m_hReadICCard = CreateThread(NULL,0,ReadICCard,this,0,0);
	}
//	m_hReadIDCard = CreateThread(NULL,0,ReadIDCard,this,0,0);

	m_hReadCard=CreateThread(NULL,0,ReadCard,this,0,NULL);
	if (m_hReadCard==0)
	{
		AfxMessageBox(_T("���п�ʶ������ʧ�ܣ�"));
	}
}

BOOL SLZCardReader::HasData() 
{
	return !m_CardInfoList.IsEmpty(); //�ж��Ƿ�Ϊ��
}

BOOL SLZCardReader::Init_Library()
{
	HINSTANCE h_instance = NULL;
	h_instance=LoadLibrary(_T("sdtapi.dll")); //���ض�̬��
	if(h_instance)
	{
		SDT_StartFindIDCard=(int(_stdcall*)(int,unsigned char*,int))GetProcAddress(h_instance,"SDT_StartFindIDCard");
		SDT_SelectIDCard=(int(_stdcall*)(int,unsigned char*,int))GetProcAddress(h_instance,"SDT_SelectIDCard");
		SDT_ReadBaseMsg=(int(_stdcall*)(int,unsigned char*,unsigned int*,unsigned char*,unsigned int*,int))GetProcAddress(h_instance,"SDT_ReadBaseMsg");
	}
	else
	{
		AfxMessageBox(_T("���ض���֤��̬��ʧ��"));
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
		AfxMessageBox(_T("��ȡ��ַʧ��"));
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
		AfxMessageBox(_T("��ȡ����"));
		return FALSE;
	}
	// ��ʽת��
	memcpy(pName,&pucCHMsg[0],30);
	CString name(pName);
	memcpy(pNU,&pucCHMsg[122],36);
	CString num(pNU);  
	CARDINFO cardinfo;
	cardinfo.iCardType=cardIDCard;
	cardinfo.strCardNumber=num;//����
	cardinfo.strCustName=name;//����
	
	if(m_cardConnectInfo.IsConnect)//�Խ�
	{
		///�Խӻ�ȡ�����֤��Ӧ�ļ����Ӧ�����ĸ�����
		
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
		//	m_CardInfoList.AddTail(cardinfo); //���뻺����
		//	m_CardReaderMutex.Unlock();
		//}
		
	}
	else//���Խ�
	{
		cardinfo.iCustLevel=0;
		CString queID(m_cardConnectInfo.RegAttchQueID);//���֤��������ID
		if (!queID.IsEmpty())
		{
			cardinfo.strAttchQueID = queID;
			//û�г�������ʱ��
			if(!theApp.m_Controller.JudgeWorkTimeOut(queID))
			{
				m_CardReaderMutex.Lock();
				m_CardInfoList.AddTail(cardinfo); //���뻺����
				m_CardReaderMutex.Unlock();
			}
		}
		///���ӽ���
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
		DWORD dwReaded=0;//���˶���
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
			bres=ReadFile(pComInit->m_hComReadCard,buf,MAX_CARD_NO,&dwReaded,&osRead); //���豸�ж�����
			std::string strbuf((char*)buf);
			if(!strbuf.empty())
			{
				CARDINFO cardinfo;
				//�жϿ����Ƿ������
				std::string::size_type pos = strbuf.find('?');
				if(pos == strbuf.npos){//û����continu
					pCard->m_strCardNum += strbuf;
					continue;
				}
				//�����ˣ�+=��ֵ
				pCard->m_strCardNum += strbuf;
				//��ȡ����
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
				//�����ʿ�
				pCard->m_strCardNum = "";
#ifdef _DEBUG
				MyWriteConsole(cardinfo.strCardNumber);
#endif
				if(cardinfo.strCardNumber.IsEmpty()) continue;
				cardinfo.iCardType=cardMagCard;
				pCard->DealCardInfo(&cardinfo);
			}
		}
		Sleep(20);
	}
	return 0;
}

int SLZCardReader::GetCustLev(const CString& strCardNum)
{
	CString strPort;
	strPort.Format(_T("%d"),m_cardConnectInfo.ServerPort);
	CTCPConnect connect;
	int recvValue = connect.GetCustLevel(strCardNum,m_cardConnectInfo.ServerIP,
		strPort,m_cardConnectInfo.OverTime);
	return recvValue;
}

CARDINFO SLZCardReader::GetData()
{
	CARDINFO cardInfo;
	m_CardReaderMutex.Lock();
	cardInfo = m_CardInfoList.GetHead();
	m_CardInfoList.RemoveHead(); //��������Ϣɾ��
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
		::MessageBox((HWND)AfxGetMainWnd(),_T("ˢ�������ڴ�ʧ�ܻ�ռ��"),
			_T("ע��"),MB_OK|MB_ICONINFORMATION);
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
		////ע�����л�������ָ������Ѿ��������ڴ�
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
			///�ж�λ��
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
	ClearCardLevInfo();//��տ��ȼ���Ϣ
	CFile file;
	CFileException e;
	if (file.Open(m_cardLev_path,CFile::modeRead,&e))
	{
		////ע�����л�������ָ������Ѿ��������ڴ�
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
			///�ж�λ��
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


// BOOL SLZCardReader::MultiReadCard(int nPort,int nWaitTime,char* pErrInfo,CString& cCardNum)
// {
// 	
// 	return FALSE;
// }

// BOOL SLZCardReader::Init_ICLibrary()
// {
// 	HINSTANCE hInstance = NULL;
// 	hInstance=LoadLibrary(_T("Lib_MsrIc.dll"));
// 	if (hInstance==NULL)
// 	{
// 		//		AfxMessageBox(_T("��ȡ��̬��ʧ��"));
// 		return FALSE;
// 	}
// 	else
// 	{
// 		
// 	}
// 	return TRUE;
// }
/*
BOOL SLZCardReader::OpenICCard(CString ICCardComm)
{
	if(m_hICCardDev)
	{
		CloseICCard();
	}
	if(ICCardComm == _T("USB"))
	{
		m_hICCardDev = device_open(36,57600);//HIDͨ��ʱport��baud����Ϊ����ֵ
		if((int)m_hICCardDev > 0)
		{
			return TRUE;
		}
		else
		{
			m_hICCardDev = NULL;
		}
	}
	else
	{
		int nCom = 0;
		CCommonConvert::CStringToint(nCom,ICCardComm);
		if(nCom>0)
		{
			m_hICCardDev = device_open(nCom-1,9600);
			if(m_hICCardDev)
				return TRUE;
		}
		else
		{
			CloseICCard();
		}
	}
	return FALSE;
}
*/
/*
BOOL SLZCardReader::CloseICCard()
{
	if(!device_close(m_hICCardDev))
	{
		m_hICCardDev = NULL;
		return TRUE;
	}
	return FALSE;
}
*/
// CString SLZCardReader::GetCardNum(const char* buf)
// {
// 	ASSERT(buf != NULL);
// 	CString strCardNum;
// 	char strLen[4] = {0};
// 	strncat_s(strLen,4,buf + 1,3);
// 	int nLen = atoi(strLen);
// 
// 	char* destBuf = new char[nLen + 1];
// 	memset(destBuf,0,nLen + 1);
// 	strncat_s(destBuf,nLen+1,buf + 4,nLen);
// 	strCardNum = destBuf;
// 	delete [] destBuf;
// 	return strCardNum;
// }

DWORD WINAPI SLZCardReader::ReadICCard(LPVOID pParam)
{
	SLZCardReader* pThis = (SLZCardReader*)pParam;
	
	
	char buff[256] = {0};
	char tmpBuff[256] = {0};
	while(1)
	{
		memset(buff,0,256);
		memset(tmpBuff,0,256);
		int iRet = pThis->m_fLibIC_GetCardNumber(buff,tmpBuff);
		if(iRet == 0)
		{
			CString wStrCardNum(buff);

			int index = wStrCardNum.Find(_T("="));
			if(index != -1)
			{
				wStrCardNum = wStrCardNum.Left(index);
			}
#ifdef _DEBUG
			MyWriteConsole(wStrCardNum);
#endif
			if(wStrCardNum != pThis->m_strCurrentCardNum)
			{
				pThis->m_strCurrentCardNum = wStrCardNum;
				if(!wStrCardNum.IsEmpty())
				{
					CARDINFO cardinfo;
					cardinfo.iCardType = cardMagCard;
					cardinfo.strCardNumber = wStrCardNum;
					pThis->DealCardInfo(&cardinfo);
				}
			}
		}
		else
		{
			pThis->m_strCurrentCardNum.Empty();
		}
		Sleep(200);
	}
	return 0;
}

void SLZCardReader::DealCardInfo(CARDINFO* pCardinfo)
{
	if(!m_cardConnectInfo.IsConnect)
	{
		pCardinfo->strAttchQueID = JudgeCardAttchQue(pCardinfo->strCardNumber);
		pCardinfo->nAttchPageID = JudgeCardAttchPageID(pCardinfo->strCardNumber);
	}
	else
	{
		//����Խ�
		/*int nLev = pCard->GetCustLev(cardinfo.strCardNumber);
		if(nLev!=-1)
		{
		CString queID = pCard->JudgeCardAttchQue(nLev);
		cardinfo.strAttchQueID = queID;
		cardinfo.iCustLevel = nLev;
		}*/
	}
	//�ж��Ƿ񳬳�����ʱ��
	if(!theApp.m_Controller.JudgeWorkTimeOut(pCardinfo->strAttchQueID)&&!pCardinfo->strAttchQueID.IsEmpty())
	{
		m_CardReaderMutex.Lock();
		m_CardInfoList.AddTail(*pCardinfo); //���뻺����
		m_CardReaderMutex.Unlock();
	}
	//				theApp.m_pView->ShowPage(cardinfo.nAttchPageID);
	SendMessage(theApp.m_pView->m_hWnd,WM_SHOWPAGE,(WPARAM)pCardinfo->nAttchPageID,NULL);
}

void SLZCardReader::DealIDCardInof(CARDINFO* pCardinfo)
{
	if(m_cardConnectInfo.IsConnect)//�Խ�
	{
		///�Խӻ�ȡ�����֤��Ӧ�ļ����Ӧ�����ĸ�����

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
		//	m_CardInfoList.AddTail(cardinfo); //���뻺����
		//	m_CardReaderMutex.Unlock();
		//}

	}
	else//���Խ�
	{
		pCardinfo->iCustLevel=0;
		CString queID(m_cardConnectInfo.RegAttchQueID);//���֤��������ID
		if (!queID.IsEmpty())
		{
			pCardinfo->strAttchQueID = queID;
			//û�г�������ʱ��
			if(!theApp.m_Controller.JudgeWorkTimeOut(queID))
			{
				m_CardReaderMutex.Lock();
				m_CardInfoList.AddTail(*pCardinfo); //���뻺����
				m_CardReaderMutex.Unlock();
			}
		}
		///���ӽ���
		pCardinfo->nAttchPageID = m_cardConnectInfo.RegAttchPageID;
		SendMessage(theApp.m_pView->m_hWnd,WM_SHOWPAGE,(WPARAM)pCardinfo->nAttchPageID,NULL);
	}
}


// DWORD WINAPI SLZCardReader::ReadIDCard(LPVOID pParam)
// {
// 	SLZCardReader* pThis = (SLZCardReader*)pParam;
// 	CComInit* pComInit = CComInit::GetInstance();
// 	char cUnicodeCardNum[31]={0};
// 	char cUnicodeCardName[37]={0};
// 	unsigned char cMsg[1024]={0};
// 	while(1)
// 	{
// 		memset(cUnicodeCardNum,0,sizeof(cUnicodeCardNum));
// 		memset(cUnicodeCardName,0,sizeof(cUnicodeCardName));
// 		memset(cMsg,0,sizeof(cMsg));
// 		int len = 0;
// 		if(pThis->m_hICCardDev)
// 		{
// 			pThis->m_mtReadICLock.Lock();
// 			if( IDCard_Read(pThis->m_hICCardDev,&len,cMsg) == 0)
// 			{
// 				memcpy(cUnicodeCardName,cMsg+9,30);
// 				memcpy(cUnicodeCardNum,cMsg+131,36);
// 				char cName[128],cNum[128];
// 				cName[0]='\0';
// 				cNum[0]='\0';
// 				if( !IDCard_Name(cUnicodeCardName,cName)
// 					&& !IDCard_IDNumber(cUnicodeCardNum,cNum))
// 				{
// 					CString wStrCardName(cName);
// 					CString wStrCardNum(cNum);
// 					if(!wStrCardName.IsEmpty() && !wStrCardNum.IsEmpty())
// 					{
// 						device_beep(pThis->m_hICCardDev,773,1);//��������
// 						CARDINFO cardinfo;
// 						cardinfo.iCardType = cardIDCard;
// 						cardinfo.strCustName = wStrCardName;
// 						cardinfo.strCardNumber = wStrCardNum;
// 
// 						pThis->DealIDCardInof(&cardinfo);
// 					}
// 				}
// 			}
// 			pThis->m_mtReadICLock.Unlock();
// 		}
// 		Sleep(300);
// 	}
// 	return 0;
// }

BOOL SLZCardReader::Init_ICLibrary()
{
	HINSTANCE hInstance = NULL;
	hInstance=LoadLibrary(_T("ccid_lib.dll"));
	if (hInstance==NULL)
	{
		return FALSE;
	}
	else
	{
		m_fLibPowerOn = (fpPowerOn)GetProcAddress(hInstance,"PowerOn");
		m_fLibPowerOnAscii = (fpPowerOnAscii)GetProcAddress(hInstance, "PowerOnAscii");
		m_fLibApduAscii = (fpApduAscii)GetProcAddress(hInstance, "ApduAscii");
		m_fLibApdu = (fpApdu)GetProcAddress(hInstance, "Apdu");
		m_fLibPowerOff = (fpPowerOff)GetProcAddress(hInstance, "PowerOff");
		m_fLibIC_GetCardNumber = (fpIC_GetCardNumber)GetProcAddress(hInstance, "IC_GetCardNumber");
	}
	return TRUE;
}