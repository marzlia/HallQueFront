#pragma once
#include "DataDef.h"
class SLZData : public CObject
{
public:
	SLZData(void);
	~SLZData(void);
 	SLZData(const SLZData& obj);
 	SLZData& operator=(const SLZData& obj);

	//////	定义 Get/Set方法 //////
	//获取业务流水号
	CString GetSerialId() const
	{
		return m_strSerialId;
	}
	//设置业务流水号
	void SetSerialId(CString strSerialId)
	{
		m_strSerialId = strSerialId;
	}
	//获取机构编号.支行代码
	CString GetOrganId() const
	{
		return m_strOrganId;
	}
	//设置机构编号.支行代码
	void SetOrganId(const CString& strOrganId)
	{
		m_strOrganId = strOrganId;
	}
	//设置机构名称
	void SetOrganName(const CString& organName)
	{
		m_strOrganName = organName;
	}
	//获取机构名称
	CString GetOrganName() const
	{
		return m_strOrganName;
	}
	
	//获取业务名称
	CString GetBussName() const
	{
		return m_strBussName;
	}
	//设置业务名称
	void SetBussName(CString strBussName)
	{
		m_strBussName=strBussName;
	}
	//获取业务类型
	CString GetBussinessType() const
	{
		return m_strBType;
	}
	//设置业务类型
	void SetBussinessType(CString strBType)
	{
		m_strBType = strBType;
	}
	//获取队列编号
	CString GetQueSerialID() const
	{
		return m_queSerialId;
	}
	//设置队列编号
	void SetQueSerialID(CString queSerialID)
	{
		m_queSerialId = queSerialID;
	}
	//获取排队号码
	CString GetQueueNumber() const
	{
		return m_strQueueNum;
	}
	//设置排队号码
	void SetQueueNumber(CString strQueueNum)
	{
		m_strQueueNum = strQueueNum;
	}
	//获取卡类型
	CardType GetCardType() const
	{
		return m_CardType;
	}
	//设置卡类型
	void SetCardType(CardType cardType)
	{
		m_CardType = cardType;
	}
	//获取卡号
	CString GetCardNumber() const
	{
		return m_strCardNum;
	}
	//设置卡号
	void SetCardNumber(CString strCardNum)
	{
		m_strCardNum = strCardNum;
	}
	//设置客户姓名
	void SetCustName(CString CustName)
	{
		m_CustName=CustName;
	}
	//获取客户姓名
	CString GetCustName() const
	{
		return m_CustName;
	}
	
	//获取客户等级
	UINT GetCustomerLevel() const
	{
		return m_iCusLevel;
	}
	//设置客户等级
	void SetCustomerLevel(UINT iCusLevel)
	{
		m_iCusLevel = iCusLevel;
	}
	//获取取号时间
	CTime GetTakingNumTime() const
	{
		return m_timeTakingNum;
	}
	//设置取号时间
	void SetTakingNumTime(const CTime& time)
	{
		m_timeTakingNum = time;
	}
	//获取员工编号
	CString GetStaffId() const
	{
		return m_strStaffId;
	}
	//设置员工编号
	void SetStaffId(const CString& strStaffId)
	{
		m_strStaffId = strStaffId;
	}
	//获取窗口ID
	UINT GetWindowId() const
	{
		return m_iWindowId;
	}
	//设置窗口ID
	void SetWindowId(UINT iWindowId)
	{
		m_iWindowId = iWindowId;
	}
	//获取窗口编号
	UINT GetWindowShowId()const
	{
		return m_iWindowShowId;
	}
	//设置窗口编号
	void SetWindowShowId(UINT iWindowShowId)
	{
		m_iWindowShowId = iWindowShowId;
	}
	//获取呼叫时间
	CTime GetCallTime() const
	{
		return m_timeCall;
	}
	//设置呼叫时间
	void SetCallTime(const CTime& time)
	{
		m_timeCall = time;
	}
	//获取完成时间
	CTime GetFinishTime() const
	{
		return m_timeFinish;
	}
	//设置完成时间
	void SetFinishTime(const CTime& time)
	{
		m_timeFinish = time;
	}
	//获取评价结果
	EvaLevel GetEvaluateLevel() const
	{
		return m_EvaLevel;
	}
	//设置评价结果
	void SetEvaluateLevel(EvaLevel evaLevel)
	{
		m_EvaLevel = evaLevel;
	}

	UINT GetIntQueNum() const
	{
		return m_iQueNum;
	}

	void SetIntQueNum(UINT iQuenum)
	{
		m_iQueNum = iQuenum;
	}

	CString GetPhoneNum() const
	{
		return m_strPhoneNum;
	}
	
	void SetPhoneNum(CString strPhoneNum)
	{
		m_strPhoneNum = strPhoneNum;
	}

	CString GetSendMsg() const
	{
		return m_strMsg;
	}

	void SetSendMsg(CString strMsg)
	{
		m_strMsg = strMsg;
	}
	////设置开启评价
	void SetIsOpenEva(const BOOL isOpen){m_bIsOpenEva = isOpen;}
	BOOL GetIsOpenEva(){return m_bIsOpenEva;}
	////设置完成评价
	void SetIsFinshEva(const BOOL isFinsh){m_bIsFinshEva = isFinsh;}
	BOOL GetIsFinshEva(){return m_bIsFinshEva;}
	
	
	/////设置呼叫窗口可处理的所有队列的排队人数
	void SetWndLefNum(int leftNum){m_leftNum = leftNum;}
	int GetWndLefNum()const {return m_leftNum;}
	////////福州返回信息
	void SetFuZhouCustLev(CustLev custLev){m_custLev = custLev;}
	CustLev GetFuZhouCustLev()const {return m_custLev;}

	virtual void Serialize( CArchive& ar );//支持序列化
	DECLARE_SERIAL(SLZData)
private:
	CString m_strSerialId;	//业务流水号
	CString m_strOrganId;	//机构编号
	CString m_strOrganName;//机构名称
	CString m_strBType;		//业务类型
	CString m_strBussName;	//业务名称
	CString m_strQueueNum;	//排队号码
	CardType m_CardType;	//卡类型
	CString m_strCardNum;	//卡号
	CString m_CustName;		//客户姓名
	//CString m_CustGender;	//客户性别
	UINT m_iCusLevel;		//客户等级
	CTime m_timeTakingNum;	//取号时间
	UINT m_iWindowId;		//窗口ID
	UINT m_iWindowShowId;//窗口编号
	CString m_strStaffId;	//员工编号
	CTime m_timeCall;		//呼叫时间
	CTime m_timeFinish;		//完成时间
	EvaLevel m_EvaLevel;	//评价结果
	BOOL m_bIsOpenEva; //单条信息是否开启了评价
	BOOL m_bIsFinshEva;//单条信息是否完成了评价
	CString m_queSerialId;//队列编号，用于与窗口信息管理系统对接
	UINT m_iQueNum;    //INT型排队号
	CString m_strPhoneNum;   //要发送短信的电话号
	CString m_strMsg; //发送短信的内容
private:
	int GetSrand(const int &max);
	CString GetOnlyId();//取号唯一ID

	int m_leftNum;//当呼叫此条信息后，设置呼叫窗口可处理的所有队列的排队人数
	CustLev m_custLev;//福州返回的客户等级信息
};
