#pragma once
#include "DataDef.h"
//#include "ConfigCardDef.h"
#include "SwingCard.h"
#include "CardLevel.h"
#include <string>
#include <afxmt.h>
#include <string>
using namespace std;
#define MAX_CARD_NO 255 //卡读取最大范围
//读到的卡的信息
class SLZCardReader
{
public:
	SLZCardReader(void);
	~SLZCardReader(void);

	BOOL HasData(); //判断缓冲区里是否有数据可以读
	CARDINFO GetData(); //获取缓冲区里读到的刷卡信息
	void Run(); //读卡类启动函数
private:
	BOOL Init_Library(); //初始化身份证库
	BOOL ReadMsg(); //读身份证卡信息
//	int GetStatus(); //获取优先级
	CMutex m_CardReaderMutex; //线程锁
	HANDLE  m_hReadTread;//线程句柄
	HANDLE m_hReadCard;//线程句柄
	CList<CARDINFO,CARDINFO&> m_CardInfoList; //缓冲区
	//读身份证函数
	int (_stdcall *SDT_StartFindIDCard)(int iPort,unsigned char* pucManaInfo,int iIfOpen);
	int (_stdcall *SDT_SelectIDCard)(int iPort,unsigned char* pucManaMsg,int iIfOpen);
	int (_stdcall *SDT_ReadBaseMsg)(int iPort,unsigned char* pucCHMsg,unsigned int* puiCHMsgLen,unsigned char* pucPHMsg,unsigned int* puiPHMsgLen,int iIfOpen);
	static DWORD WINAPI ReadThread(LPVOID pParam); //读身份证线程
	static DWORD WINAPI ReadCard(LPVOID pParam); //读磁卡线程
private:
	BOOL OpenReadCard();
	CString m_cardcofinfo_path;//卡识别信息文件路径
	CString m_connect_path;//卡连接信息文件路径
	CArray<CCardLevel*,CCardLevel*> m_list_cardLev;//卡的级别信息数组
	CString m_cardLev_path;//卡的等级信息文件路径
private:
	BOOL ReadCardConnectInfo();//读卡的链接信息
	BOOL ReadCardConnectLev();//读卡的链接信息级别对应的队列ID
	BOOL ReadCardConfigInfo();//读卡的识别信息
	void ClearSwingCardInfo();//删除数组里的变量CSwingCard
	CString JudgeCardAttchQue(const CString& cardNum);//判断刷卡的这张卡属于哪个队列,返回队列ID，用于不对接的情况
	int JudgeCardAttchPageID(const CString& cardNum);//判断刷卡的卡号链接到那个界面,用于不对接情况
	CARDCONNECTINFO m_cardConnectInfo;//卡连接远程服务器判断信息
	CArray<CSwingCard*,CSwingCard*> m_list_swingCard;//刷卡的链表
	void ClearCardLevInfo();//清空卡等级数组数组
	int GetCustLev(const CString& strCardNum,CustLev* pCustLev);//获取客户等级用于对接情况
	CString JudgeCardAttchQue(const int level);//判断卡对接，传入等级对应一个队列，返回队列，用于对接的情况
	int JudgeCardAttchPageID(int level);//判断卡对接,传入等级返回界面ID，用于对接情况
public:
	BOOL ReFlushCardConnectInfo();//接口,刷新卡的连接信息
	BOOL ReFlushCardConfigInof();//接口,刷新卡的识别信息
	BOOL ReFlushCardLevelInfo();//接口,刷新卡等级返回队列信息
private:
	string m_strCardNum;
};
