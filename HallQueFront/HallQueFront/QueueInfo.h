#pragma once

class CQueueInfo :public CObject
{
public:
	CQueueInfo(void);
	~CQueueInfo(void);
	CQueueInfo(const CQueueInfo& obj); //��������
	CQueueInfo& operator=(CQueueInfo& obj); //����=�����

	CString GetBussName()const {return m_queName;} //��ȡҵ������
	void SetBussName(const CString& quename){m_queName=quename;} //����ҵ������

	CString GetQueCallName()const {return m_queCallName;} //��ú�������
	void SetQueCallName(const CString& callName){m_queCallName=callName;} //���ú�������

	CString GetFrontID()const {return m_queFrontID;} //���ǰ׺
	void SetFrontID(const CString& frontID){m_queFrontID=frontID;} //����ǰ׺

	unsigned int GetQueNumStart()const {return m_queNumStart;} //��ÿ�ʼ����
	void SetQueNumStart(const unsigned int numStart){m_queNumStart=numStart;} //���ÿ�ʼ����

	unsigned int GetQueNumEnd()const {return m_queNumEnd;} //��ý�������
	void SetQueNumEnd(const unsigned int numEnd){m_queNumEnd=numEnd;} //���ý�������

	unsigned int GetAmLimitCustomer()const {return m_AmLimitCustomer;} //������������
	void SetAmLimitCustomer(const unsigned int amLimitCustomer){m_AmLimitCustomer=amLimitCustomer;}

	unsigned int GetPmLimitCustomer()const {return m_PmLimitCustomer;} //������������
	void SetPmLimitCustomer(const unsigned int pmLimitCustomer){m_PmLimitCustomer=pmLimitCustomer;}

	unsigned int GetDayLimitCustomer()const {return m_DayLimitCustomer;} //����ȫ������
	void SetDayLimitCustomer(const unsigned int dayLimitCustomer){m_DayLimitCustomer=dayLimitCustomer;}

	CTime GetQueWorkTimeStart()const 
	{
		return m_tQueWorkStart;
	}

	void SetQueWorkTimeStart(COleDateTime workstart)
	{
		
		////////////////////////////////ת��
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
	
	//unsigned int GetstbId()const {return m_stbId;} //������
	//void SetStbId(const unsigned int stbid){m_stbId=stbid;}
	
	void SetQueID(const CString serialid){m_queserial_id=serialid;}
	CString GetQueID()const {return m_queserial_id;}//����IDֻ����ȡ�ӿ�
	void SetQueManNum(const CString& queManNum){m_queManNum = queManNum;}
	CString GetQueManNum()const {return m_queManNum;}//��ȡ���б��

//	void SetCurrWaitNum(UINT nNum){m_currWaitNum = nNum;}//�ö��е��Ŷ���������Ҫ��������ȡ��
//	UINT GetCurrWaitNum(){return m_currWaitNum;}

	virtual void Serialize( CArchive& ar );//֧�����л�
	DECLARE_SERIAL(CQueueInfo)
private:
	CString m_queName; //ҵ������
	CString m_queCallName; //��������
	//CString m_queID; //����ID,���޸�
	CString m_queFrontID; //����IDǰ׺ example A001,AΪǰ׺
	unsigned int m_queNumStart; //���п�ʼ����.�磺��1000�ſ�ʼ�Ŷ�
	unsigned int m_queNumEnd; //���н�������.example:1000--20000
	unsigned int m_AmLimitCustomer; //����������������
	unsigned int m_PmLimitCustomer; //����������������
	unsigned int m_DayLimitCustomer; //�������������=����+����
	CTime m_tQueWorkStart;	//��ʼ����ʱ��
	CTime m_tQueWorkEnd;		//��ֹ����ʱ��
	unsigned int m_queNumber; //ʵ�ʵ��Ŷ�����
	//unsigned int m_stbId; //������ID
	CString m_queserial_id; //����Ψһ���
	CString m_queManNum;//���б�ţ�ע�����ڶԽӴ�����Ϣ����ϵͳ
//	UINT m_currWaitNum;//��ǰ�����Ŷ�����
};

