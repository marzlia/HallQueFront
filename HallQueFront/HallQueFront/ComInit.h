#pragma once

class CComInit
{
private:
	CComInit(void);
public:
	~CComInit(void);
	//打开窗口屏、呼叫器串口
	int OpenWndScreen(int ncom);
	int m_canUse[10];//保存本机上所有的可用串口号
	HANDLE m_hComWndScreen;//条屏串口(呼叫器)
	HANDLE m_hComReadCard;//刷卡器串口
	HANDLE m_hComMsg;//短信猫串口
	HANDLE OpenComm(int ncom);//打开串口
	///try本机所有的可用的串口
	int TryCom( int nCom);
	//打开刷卡器串口
	int OpenCardComm( int ncom );
	//打开短信猫串口
	//保存到文件
	void SaveComm();
	//读文件
	CString GetCardComm();
	CString GetWndComm();
	CString GetMsgComm();
	void SetMsgComm(const CString strCom)
	{
		m_cMsgComm = strCom;
	}

	//芯片卡
//	CString GetNewCardComm();
//	void SetNewCardComm(const CString& strNewCardComm);
	//打开芯片卡com
//	int OpenNewCardComm(CString ICCardComm);
	//关闭芯片卡com
//	int CloseNewCardComm();
private:
	//初始化串口
	void InitCom();
	void ReadComInfo();
	CString m_cCardComm;
	CString m_cWndComm;
	CString m_cMsgComm;
	CString m_strPath;
//	CString m_cNewCardComm;
public:
	OVERLAPPED m_write_os,m_read_os;
public:
	//获取唯一实例
	static CComInit* GetInstance(){
		static CComInit instance;
		return &instance;
	}
};
