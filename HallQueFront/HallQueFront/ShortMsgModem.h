#pragma once

class CShortMsgModem
{
private:
	CShortMsgModem(void);
public:
	static CShortMsgModem* GetInstance(){
		static CShortMsgModem instance;
		return &instance;
	}
	~CShortMsgModem(void);

	BOOL InitDemoCat();
	BOOL SendMsg(CString strPhoneNum,CString strMsg);
	BOOL ReadMsg();
	BOOL ClearSendBox();
	//BOOL ClearRecvBox();
	BOOL OpenMsgComm(int iCom);
	void CloseMsgComm();
private:
	typedef LONG (CALLBACK* LPFNDLLFUNC1)(LONG);
	typedef LONG (CALLBACK* LPFNDLLFUNC2)(LONG);
	typedef LONG (CALLBACK* LPFNDLLFUNC3)(LONG,CHAR *,CHAR *);
	typedef LONG (CALLBACK* LPFNDLLFUNC4)(LONG,CHAR *,CHAR *);
	typedef LONG (CALLBACK* LPFNDLLFUNC5)(LONG);
	typedef LONG (CALLBACK* LPFNDLLFUNC6)();
	typedef LONG (CALLBACK* LPFNDLLFUNC7)(LONG,LONG);
	typedef LONG (CALLBACK* LPFNDLLFUNC8)(CHAR *,CHAR *,CHAR *);
	HINSTANCE hInsDLL;
	LPFNDLLFUNC1 fnInitModem;     // Function pointer 1
	LPFNDLLFUNC2 fnCloseModem;    // Function pointer 2
	LPFNDLLFUNC3 fnSendMsg;       // Function pointer 3
	LPFNDLLFUNC4 fnReadMsgEx;     // Function pointer 4
	LPFNDLLFUNC4 fnReadMsgExNotDelete;     // Function pointer 4
	LPFNDLLFUNC5 fnGetStatus;     // Function pointer 5
	LPFNDLLFUNC5 fnGetSndCount;
	LPFNDLLFUNC5 fnGetRecCount;
	LPFNDLLFUNC5 fnClrSndBuf;	
	LPFNDLLFUNC5 fnClrRecBuf;	
	LPFNDLLFUNC6 fnDogSerial;
	LPFNDLLFUNC7 fnDeleteSimMsg;
	LPFNDLLFUNC8 fnWapPushCvt;
	//long __stdcall fnDeleteSimMsg(long pno,long nSimIdx)

	LPFNDLLFUNC7 fnSetModemType;
	
	static DWORD WINAPI ReadProc(LPVOID pParam);
	HANDLE m_ReadThread;
	long m_lCom; 
	

	CString m_strLogFilePath;
	void WriteErrLog(CString strErr);
	void WriteLogWithTime(CString strSockLog);			//–¥»Î¥ÌŒÛ»’÷æ
	BOOL AppendWriteFile(CString strText, CString strFileName);
};
