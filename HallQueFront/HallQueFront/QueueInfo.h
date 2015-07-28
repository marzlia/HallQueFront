#pragma once

class CQueueInfo :public CObject
{
public:
	CQueueInfo(void);
	~CQueueInfo(void);
	CQueueInfo(const CQueueInfo& obj); //拷贝构造
	CQueueInfo& operator=(CQueueInfo& obj); //重载=运算符

	CString GetBussName()const {return m_queName;} //获取业务名称
	void SetBussName(const CString& quename){m_queName=quename;} //设置业务名称

	CString GetQueCallName()const {return m_queCallName;} //获得呼叫名称
	void SetQueCallName(const CString& callName){m_queCallName=callName;} //设置呼叫名称

	CString GetFrontID()const {return m_queFrontID;} //获得前缀
	void SetFrontID(const CString& frontID){m_queFrontID=frontID;} //设置前缀

	unsigned int GetQueNumStart()const {return m_queNumStart;} //获得开始号码
	void SetQueNumStart(const unsigned int numStart){m_queNumStart=numStart;} //设置开始号码

	unsigned int GetQueNumEnd()const {return m_queNumEnd;} //获得结束号码
	void SetQueNumEnd(const unsigned int numEnd){m_queNumEnd=numEnd;} //设置结束号码

	unsigned int GetAmLimitCustomer()const {return m_AmLimitCustomer;} //限制上午人数
	void SetAmLimitCustomer(const unsigned int amLimitCustomer){m_AmLimitCustomer=amLimitCustomer;}

	unsigned int GetPmLimitCustomer()const {return m_PmLimitCustomer;} //限制下午人数
	void SetPmLimitCustomer(const unsigned int pmLimitCustomer){m_PmLimitCustomer=pmLimitCustomer;}

	unsigned int GetDayLimitCustomer()const {return m_DayLimitCustomer;} //限制全天人数
	void SetDayLimitCustomer(const unsigned int dayLimitCustomer){m_DayLimitCustomer=dayLimitCustomer;}

	CTime GetQueWorkTimeStart()const 
	{
		return m_tQueWorkStart;
	}

	void SetQueWorkTimeStart(COleDateTime workstart)
	{
		
		////////////////////////////////转换
		CTime timestart = CTime::GetCurrentTime();
		workstart.SetDateTime(timestart.GetYear(),timestart.GetMonth(),timestart.GetDay(),
			workstart.GetHour(),workstart.GetMinute(),workstart.GetSecond());
		//////////////////////////////
		SYSTEMTIME sysstart;
		workstart.GetAsSystemTime(sysstart);
		CTime tstart(sysstart);
		m_tQueWorkStart = tstart;
	}

	CTime GetQueWorkTimeEnd()const 
	{
		return m_tQueWorkEnd;
	}

	void SetQueWorkTimeEnd(COleDateTime workend)
	{
		CTime timestart = CTime::GetCurrentTime();
		workend.SetDateTime(timestart.GetYear(),timestart.GetMonth(),timestart.GetDay(),
			workend.GetHour(),workend.GetMinute(),workend.GetSecond());
		//////////////////////////////
		SYSTEMTIME sysstart;
		workend.GetAsSystemTime(sysstart);
		CTime tend(sysstart);
		m_tQueWorkEnd = tend;
	}
	unsigned int GetQuenumber()const {return m_queNumber;} 
	void SetQuenumber(const unsigned int queNum){m_queNumber=queNum;}
	
	//unsigned int GetstbId()const {return m_stbId;} //机顶盒
	//void SetStbId(const unsigned int stbid){m_stbId=stbid;}
	
	void SetQueID(const CString serialid){m_queserial_id=serialid;}
	CString GetQueID()const {return m_queserial_id;}//自增ID只留获取接口
	void SetQueManNum(const CString& queManNum){m_queManNum = queManNum;}
	CString GetQueManNum()const {return m_queManNum;}//获取队列编号

//	void SetCurrWaitNum(UINT nNum){m_currWaitNum = nNum;}//该队列的排队人数，主要用于联机取号
//	UINT GetCurrWaitNum(){return m_currWaitNum;}

	virtual void Serialize( CArchive& ar );//支持序列化
	DECLARE_SERIAL(CQueueInfo)
private:
	CString m_queName; //业务名称
	CString m_queCallName; //呼叫名称
	//CString m_queID; //队列ID,可修改
	CString m_queFrontID; //队列ID前缀 example A001,A为前缀
	unsigned int m_queNumStart; //队列开始号码.如：从1000号开始排队
	unsigned int m_queNumEnd; //队列结束号码.example:1000--20000
	unsigned int m_AmLimitCustomer; //队列上午限制人数
	unsigned int m_PmLimitCustomer; //队列下午限制人数
	unsigned int m_DayLimitCustomer; //整天的限制人数=上午+下午
	CTime m_tQueWorkStart;	//开始工作时间
	CTime m_tQueWorkEnd;		//截止工作时间
	unsigned int m_queNumber; //实际的排队人数
	//unsigned int m_stbId; //机顶盒ID
	CString m_queserial_id; //队列唯一编号
	CString m_queManNum;//队列编号，注意用于对接窗口信息管理系统
//	UINT m_currWaitNum;//当前队列排队人数
};

