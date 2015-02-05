#pragma once
#include "SLZStaff.h"
#include "CommonConvert.h"


class SLZWindow:public CObject
{
public:
	SLZWindow();
	~SLZWindow();
	SLZWindow(const SLZWindow& obj);
	SLZWindow& operator=(const SLZWindow& obj);	

	//获取窗口编号
	UINT GetWindowId() const
	{
		return m_iWindowId;
	}
	//设置窗口编号
	void SetWindowId(UINT iWindowId)
	{
		m_iWindowId = iWindowId;
	}
	//获取显示编号
	UINT GetShowWndId() const
	{
		return m_iShowWndId;
	}
	//设置显示编号
	void SetShowWndId(UINT iShowWndId)
	{
		m_iShowWndId = iShowWndId;
	}
	//获取窗口名称
	CString GetWindowName() const
	{
		return m_strWindowName;
	}
	//设置窗口名称
	void SetWindowName(const CString& strWindowName)
	{
		m_strWindowName = strWindowName;
	}
	//获取窗口名称
	CString GetWindowCallName() const
	{
		return m_strWindowCallName;
	}
	//设置窗口名称
	void SetWindowCallName(const CString& strWindowCallName)
	{
		m_strWindowCallName = strWindowCallName;
	}
	//获取呼叫器地址
	int GetCallerId() const
	{
		return m_iCallerId;
	}
	//设置呼叫器地址
	void SetCallerId(int iCallerId)
	{
		m_iCallerId = iCallerId;
	}
	//获取评价器地址
	int GetEvaluatorId() const
	{
		return m_iEvaluatorId;
	}
	//设置评价器地址
	void SetEvaluatorId(int iEvaluatorId)
	{
		m_iEvaluatorId = iEvaluatorId;
	}
	//获取窗口屏地址
	int GetWndScreenId()const
	{
		return m_iWndScreenId;
	}
	//设置窗口屏地址
	void SetWndScreenId(int iWndScreenId)
	{
		m_iWndScreenId=iWndScreenId;
	}
	//获取综合屏地址
	int GetComScreenId() const
	{
		return m_iComScreenId;	 
	}
	//设置综合屏地址
	void SetComScreenId(int iComScreenId) 
	{
		m_iComScreenId=iComScreenId;
	}

	//获取通屏IP地址
	CString GetLEDIPId()
	{
		return m_strLEDIPId;
	}

	void SetLEDIPId(const CString& LEDIPId)
	{
		m_strLEDIPId=LEDIPId;
	}
	
	//获取通屏物理地址
	int GetLEDPhyId()
	{
		return m_iLEDPhyId;
	}

	void SetLEDPhyId(const int LEDPhyId)
	{
		m_iLEDPhyId=LEDPhyId;
	}

	int GetLEDPipeId()
	{
		return m_iLEDPipeId;
	}

	void SetLEDPipeId(const int LEDPipeId)
	{
		m_iLEDPipeId = LEDPipeId;
	}

	CString GetStbId()const 
	{
		return m_strStbId;
	}
	//机顶盒
	void SetStbId(const CString stbid)
	{
		m_strStbId=stbid;
	}

	////获取登录员工
	//CString GetCurLoginStaff() 
	//{
	//	return m_staffLoginId;
	//}
	////设置登录员工
	//void SetCurLoginStaff(CString& staff)
	//{
	//	m_staffLoginId=staff;
	//}

	//获取预设员工
	CString GetLoginStaff() 
	{
		return m_staffDefaultId;
	}

	//设置预设员工

	void SetLoginStaff(const CString& staff)
	{
		m_staffDefaultId = staff;
	}

	//CString GetCurStaffName()
	//{
	//	return m_StrCurStaffName;
	//}
	//void SetCurStaffName(const CString& CurStaffName )
	//{
	//	m_StrCurStaffName=CurStaffName;
	//}
	//CString GetDefStaffName()
	//{
	//	return m_StrDefStaffName;
	//}
	//void SetDefStaffName(const CString& DefStaffName)
	//{
	//	m_StrDefStaffName=DefStaffName;
	//}
	void GetArrayQueId(CStringArray& arrQueId)
	{
		arrQueId.Copy(m_arrBussId);
	}

	void SetArrayQueId( CStringArray& arrQueId)
	{
		m_arrBussId.Copy(arrQueId);
	}
	int GetArraySize()const
	{
		return m_ArraySize;
	}
	void SetArraySize(const int size)
	{
		m_ArraySize=size;
	}
	//获取呼叫信息
	CString GetCallMsg()
	{
		return m_CalledMsg;
	}
	void SetCallMsg(const CString& CallMsg)
	{
		m_CalledMsg=CallMsg;
	}
	//获取等待信息
	CString GetWaitCallMsg()
	{
		return m_WaitCalledMsg;
	}
	void SetWaitCallMsg(const CString& CallMsg)
	{
		m_WaitCalledMsg=CallMsg;
	}
	//获取显示信息
	CString GetShowMsg()
	{
		return m_ShowMsg;
	}
	void SetShowMsg(const CString& ShowMsg)
	{
		m_ShowMsg=ShowMsg;
	}
	//获取等待显示信息
	CString GetWaitShowMsg()
	{
		return m_WaitShowMsg;
	}
	void SetWaitShowMsg(const CString& ShowMsg)
	{
		m_WaitShowMsg=ShowMsg;
	}
	
	UINT GetMsgShowTime()
	{
		return m_iMsgShowTime;
	}

	void SetMsgShowTime(const UINT& MsgShowTime)
	{
		m_iMsgShowTime=MsgShowTime;
	}

	CString GetAdMsg()
	{
		return m_strAdMsg;
	}

	void SetAdMsg(const CString& AdMsg)
	{
		m_strAdMsg=AdMsg;
	}

	//BOOL GetFlagMustEval()
	//{
	//	return m_FlagMustEval;
	//}
	//void SetFlagMustEval(const BOOL& FlagMustEval)
	//{
	//	m_FlagMustEval=FlagMustEval;
	//}
	UINT GetEvaTimeOut() const
	{
		return m_iEvaTimeOut;
	}
	void SetEvaTimeOut(UINT iEvaTimeOut)
	{
		m_iEvaTimeOut = iEvaTimeOut;
	}
	virtual void Serialize( CArchive& ar );
	DECLARE_SERIAL(SLZWindow)

private:
	UINT m_iWindowId;				//窗口唯一编号
	UINT m_iShowWndId;			//窗口显示编号
	CString m_strWindowName;		//窗口名称
	CString m_strWindowCallName;	//呼叫名称
	int m_iCallerId;				//呼叫器地址
	int m_iEvaluatorId;				//评价器地址
	int m_iWndScreenId;				//窗口屏地址
	int m_iComScreenId;				//综合屏地址
	CString m_strLEDIPId;			//通屏IP地址
	int m_iLEDPhyId;				//通屏物理地址
	int m_iLEDPipeId;				//通屏通道号
	CString m_strStbId;				//机顶盒编号
	//CString m_staffLoginId;			//当前登录员工
	CString m_staffDefaultId;		//自动登录时预设员工
	//CString m_StrCurStaffName;
	//CString m_StrDefStaffName;
	//CString m_strAbleQueId;			//可处理队列
	int m_ArraySize;				//队列数组大小
	CStringArray m_arrBussId;		//可处理业务队列数组
	//CString	m_AbleQueName;			//可处理业务名称
	CString m_CalledMsg;			//呼叫信息
	CString m_WaitCalledMsg;		//等待呼叫信息
	CString m_ShowMsg;				//显示信息
	CString m_WaitShowMsg;			//等待显示信息
	unsigned int m_iMsgShowTime;		//信息显示时间
	CString m_strAdMsg;				//广告信息
	//BOOL m_FlagMustEval;			//是否强制评价
	UINT m_iEvaTimeOut;			//评价超时秒数
};

