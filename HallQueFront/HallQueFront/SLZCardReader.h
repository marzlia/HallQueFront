#pragma once
#include "DataDef.h"
//#include "ConfigCardDef.h"
#include "SwingCard.h"
#include "CardLevel.h"
#include <string>
#include <afxmt.h>
#include <string>
using namespace std;
#define MAX_CARD_NO 255 //����ȡ���Χ
//�����Ŀ�����Ϣ
class SLZCardReader
{
private:
	SLZCardReader(void);
public:
	~SLZCardReader(void);

	static SLZCardReader* GetInstance();

	BOOL HasData(); //�жϻ��������Ƿ������ݿ��Զ�
	CARDINFO GetData(); //��ȡ�������������ˢ����Ϣ
	void Run(); //��������������
private:
	BOOL Init_Library(); //��ʼ�����֤��
	BOOL ReadMsg(); //�����֤����Ϣ
//	int GetStatus(); //��ȡ���ȼ�
	CMutex m_CardReaderMutex; //�߳���
	HANDLE  m_hReadTread;//�߳̾��
	HANDLE m_hReadCard;//�߳̾��

	
//	HANDLE m_hReadNewCard;//оƬ���߳̾��
	
	HANDLE m_hReadICCard;//���ǽӴ�ʽоƬ������һ�߳̾��
//	HANDLE m_hReadIDCard;//�����֤�߳�(����һˢ����)
//	HANDLE m_hICCardDev;//�����֤�ͷǽӴ�ʽоƬ������һ�豸���

	CList<CARDINFO,CARDINFO&> m_CardInfoList; //������
	//�����֤����
	int (_stdcall *SDT_StartFindIDCard)(int iPort,unsigned char* pucManaInfo,int iIfOpen);
	int (_stdcall *SDT_SelectIDCard)(int iPort,unsigned char* pucManaMsg,int iIfOpen);
	int (_stdcall *SDT_ReadBaseMsg)(int iPort,unsigned char* pucCHMsg,unsigned int* puiCHMsgLen,unsigned char* pucPHMsg,unsigned int* puiPHMsgLen,int iIfOpen);
	static DWORD WINAPI ReadThread(LPVOID pParam); //�����֤�߳�
	static DWORD WINAPI ReadCard(LPVOID pParam); //���ſ��߳�
//	static DWORD WINAPI ReadNewCard(LPVOID pParam);//оƬ���߳�
	static DWORD WINAPI ReadICCard(LPVOID pParam);//�ǽӴ�ʽоƬ��
//	static DWORD WINAPI ReadIDCard(LPVOID pParam);//���֤оƬ������һ
private:
	BOOL OpenReadCard();
	CString m_cardcofinfo_path;//��ʶ����Ϣ�ļ�·��
	CString m_connect_path;//��������Ϣ�ļ�·��
	CArray<CCardLevel*,CCardLevel*> m_list_cardLev;//���ļ�����Ϣ����
	CString m_cardLev_path;//���ĵȼ���Ϣ�ļ�·��
private:
	BOOL ReadCardConnectInfo();//������������Ϣ
	BOOL ReadCardConnectLev();//������������Ϣ�����Ӧ�Ķ���ID
	BOOL ReadCardConfigInfo();//������ʶ����Ϣ
	void ClearSwingCardInfo();//ɾ��������ı���CSwingCard
	CString JudgeCardAttchQue(const CString& cardNum);//�ж�ˢ�������ſ������ĸ�����,���ض���ID�����ڲ��Խӵ����
	int JudgeCardAttchPageID(const CString& cardNum);//�ж�ˢ���Ŀ������ӵ��Ǹ�����,���ڲ��Խ����
	CARDCONNECTINFO m_cardConnectInfo;//������Զ�̷������ж���Ϣ
	CArray<CSwingCard*,CSwingCard*> m_list_swingCard;//ˢ��������
	void ClearCardLevInfo();//��տ��ȼ���������
	int GetCustLev(const CString& strCardNum);//��ȡ�ͻ��ȼ����ڶԽ����
	CString JudgeCardAttchQue(const int level);//�жϿ��Խӣ�����ȼ���Ӧһ�����У����ض��У����ڶԽӵ����
	int JudgeCardAttchPageID(int level);//�жϿ��Խ�,����ȼ����ؽ���ID�����ڶԽ����
public:
	BOOL ReFlushCardConnectInfo();//�ӿ�,ˢ�¿���������Ϣ
	BOOL ReFlushCardConfigInof();//�ӿ�,ˢ�¿���ʶ����Ϣ
	BOOL ReFlushCardLevelInfo();//�ӿ�,ˢ�¿��ȼ����ض�����Ϣ
private:
	string m_strCardNum;

private:
	/**************************��ˢ����(оƬ��)***************************/
	typedef int (__stdcall *fpPowerOn)(byte slot, byte* atr, int* atrlen);
	typedef int (__stdcall *fpPowerOnAscii)(byte slot, char* atrBuff);
	typedef int (__stdcall *fpApduAscii)(byte slot, const char* sendAscii, char* recvAscii);
	typedef int (__stdcall *fpPowerOff)(byte slot);
	typedef int (__stdcall *fpIC_GetCardNumber)(char* cardNumber, char* tr2Buff);
	typedef int (__stdcall *fpApdu)(byte slot, const byte* sendBcd, int sendlen, byte* recvBcd, int* recvlen);

	fpPowerOn m_fLibPowerOn;
	fpPowerOnAscii m_fLibPowerOnAscii;
	fpApduAscii m_fLibApduAscii;
	fpApdu m_fLibApdu;
	fpPowerOff m_fLibPowerOff;
	fpIC_GetCardNumber m_fLibIC_GetCardNumber;

	//��ˢ��(оƬ��)��ʼ����̬��
	BOOL Init_ICLibrary();

	void DealCardInfo(CARDINFO* pCardinfo);

	void DealIDCardInof(CARDINFO* pCardinfo);

//	CString GetCardNum(const char* buf);//�õ�����
private:
	CString m_strCurrentCardNum;//�����жϵ�ǰ�Ӵ�ʽ�������Ƿ��ظ�ˢ
//	CMutex m_mtReadICLock;
public:
// 	BOOL OpenICCard(CString ICCardComm);//��оƬ��
// 	BOOL CloseICCard();//�ر�оƬ���˿�
};
