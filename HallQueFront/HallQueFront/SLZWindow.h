#pragma once
#include "SLZStaff.h"
#include "ThroughWndScreenInfo.h"

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
// 	获取窗口屏地址
// 	CStringArray& GetWndScreenId()//const
// 	{
// 		return m_sWndScreenIdArray;
// 	}
// 	//设置窗口屏地址
// 	void SetWndScreenId(const CStringArray& sWndScreenIdArray)
// 	{
// 		m_sWndScreenIdArray.Copy(sWndScreenIdArray);
// 	}
// 	//获取综合屏地址
// 	CStringArray& GetComScreenId() //const
// 	{
// 		return m_sComScreenIdArray;	 
// 	}
// 	//设置综合屏地址
// 	void SetComScreenId(const CStringArray& sComScreenIdArray) 
// 	{
// 		m_sComScreenIdArray.Copy(sComScreenIdArray);
// 	}


	CString GetStbId()const 
	{
		return m_strStbId;
	}
	//机顶盒
	void SetStbId(const CString stbid)
	{
		m_strStbId=stbid;
	}

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

	
	UINT GetEvaTimeOut() const
	{
		return m_iEvaTimeOut;
	}
	void SetEvaTimeOut(UINT iEvaTimeOut)
	{
		m_iEvaTimeOut = iEvaTimeOut;
	}

	BOOL GetIsUsePower()
	{
		return m_bIsUsePower;
	}

	void SetIsUsePower(BOOL bUse)
	{
		m_bIsUsePower = bUse;
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
//	CStringArray m_sWndScreenIdArray;	//窗口屏地址
//	CStringArray m_sComScreenIdArray;	//综合屏地址

	CString m_strStbId;				//机顶盒编号
	//CString m_staffLoginId;			//当前登录员工
	CString m_staffDefaultId;		//自动登录时预设员工
									//可处理队列
	int m_ArraySize;				//队列数组大小
	CStringArray m_arrBussId;		//可处理业务队列数组
	
	CString m_CalledMsg;			//呼叫信息
	CString m_WaitCalledMsg;		//等待呼叫信息
	CString m_ShowMsg;				//显示信息
	CString m_WaitShowMsg;			//等待显示信息
	unsigned int m_iMsgShowTime;		//信息显示时间
	CString m_strAdMsg;				//广告信息
	
	UINT m_iEvaTimeOut;			//评价超时秒数
	BOOL m_bIsUsePower; //是否有优先级
public:
	CArray<CThroughWndScreenInfo> m_throughscreen_array;//屏数组
};

