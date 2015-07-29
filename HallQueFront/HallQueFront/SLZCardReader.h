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
public:
	SLZCardReader(void);
	~SLZCardReader(void);

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
	CList<CARDINFO,CARDINFO&> m_CardInfoList; //������
	//�����֤����
	int (_stdcall *SDT_StartFindIDCard)(int iPort,unsigned char* pucManaInfo,int iIfOpen);
	int (_stdcall *SDT_SelectIDCard)(int iPort,unsigned char* pucManaMsg,int iIfOpen);
	int (_stdcall *SDT_ReadBaseMsg)(int iPort,unsigned char* pucCHMsg,unsigned int* puiCHMsgLen,unsigned char* pucPHMsg,unsigned int* puiPHMsgLen,int iIfOpen);
	static DWORD WINAPI ReadThread(LPVOID pParam); //�����֤�߳�
	static DWORD WINAPI ReadCard(LPVOID pParam); //���ſ��߳�
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
	///////////////////////////////
};
