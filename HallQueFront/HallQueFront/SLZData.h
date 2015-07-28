#pragma once
#include "DataDef.h"
class SLZData : public CObject
{
public:
	SLZData(void);
	~SLZData(void);
 	SLZData(const SLZData& obj);
 	SLZData& operator=(const SLZData& obj);

	//////	���� Get/Set���� //////
	//��ȡҵ����ˮ��
	CString GetSerialId() const
	{
		return m_strSerialId;
	}
	//����ҵ����ˮ��
	void SetSerialId(CString strSerialId)
	{
		m_strSerialId = strSerialId;
	}
	//��ȡ�������.֧�д���
	CString GetOrganId() const
	{
		return m_strOrganId;
	}
	//���û������.֧�д���
	void SetOrganId(const CString& strOrganId)
	{
		m_strOrganId = strOrganId;
	}
	//���û�������
	void SetOrganName(const CString& organName)
	{
		m_strOrganName = organName;
	}
	//��ȡ��������
	CString GetOrganName() const
	{
		return m_strOrganName;
	}
	
	//��ȡҵ������
	CString GetBussName() const
	{
		return m_strBussName;
	}
	//����ҵ������
	void SetBussName(CString strBussName)
	{
		m_strBussName=strBussName;
	}
	//��ȡҵ������
	CString GetBussinessType() const
	{
		return m_strBType;
	}
	//����ҵ������
	void SetBussinessType(CString strBType)
	{
		m_strBType = strBType;
	}
	//��ȡ���б��
	CString GetQueSerialID() const
	{
		return m_queSerialId;
	}
	//���ö��б��
	void SetQueSerialID(CString queSerialID)
	{
		m_queSerialId = queSerialID;
	}
	//��ȡ�ŶӺ���
	CString GetQueueNumber() const
	{
		return m_strQueueNum;
	}
	//�����ŶӺ���
	void SetQueueNumber(CString strQueueNum)
	{
		m_strQueueNum = strQueueNum;
	}
	//��ȡ������
	CardType GetCardType() const
	{
		return m_CardType;
	}
	//���ÿ�����
	void SetCardType(CardType cardType)
	{
		m_CardType = cardType;
	}
	//��ȡ����
	CString GetCardNumber() const
	{
		return m_strCardNum;
	}
	//���ÿ���
	void SetCardNumber(CString strCardNum)
	{
		m_strCardNum = strCardNum;
	}
	//���ÿͻ�����
	void SetCustName(CString CustName)
	{
		m_CustName=CustName;
	}
	//��ȡ�ͻ�����
	CString GetCustName() const
	{
		return m_CustName;
	}
	
	//��ȡ�ͻ��ȼ�
	UINT GetCustomerLevel() const
	{
		return m_iCusLevel;
	}
	//���ÿͻ��ȼ�
	void SetCustomerLevel(UINT iCusLevel)
	{
		m_iCusLevel = iCusLevel;
	}
	//��ȡȡ��ʱ��
	CTime GetTakingNumTime() const
	{
		return m_timeTakingNum;
	}
	//����ȡ��ʱ��
	void SetTakingNumTime(const CTime& time)
	{
		m_timeTakingNum = time;
	}
	//��ȡԱ�����
	CString GetStaffId() const
	{
		return m_strStaffId;
	}
	//����Ա�����
	void SetStaffId(const CString& strStaffId)
	{
		m_strStaffId = strStaffId;
	}
	//��ȡ����ID
	UINT GetWindowId() const
	{
		return m_iWindowId;
	}
	//���ô���ID
	void SetWindowId(UINT iWindowId)
	{
		m_iWindowId = iWindowId;
	}
	//��ȡ���ڱ��
	UINT GetWindowShowId()const
	{
		return m_iWindowShowId;
	}
	//���ô��ڱ��
	void SetWindowShowId(UINT iWindowShowId)
	{
		m_iWindowShowId = iWindowShowId;
	}
	//��ȡ����ʱ��
	CTime GetCallTime() const
	{
		return m_timeCall;
	}
	//���ú���ʱ��
	void SetCallTime(const CTime& time)
	{
		m_timeCall = time;
	}
	//��ȡ���ʱ��
	CTime GetFinishTime() const
	{
		return m_timeFinish;
	}
	//�������ʱ��
	void SetFinishTime(const CTime& time)
	{
		m_timeFinish = time;
	}
	//��ȡ���۽��
	EvaLevel GetEvaluateLevel() const
	{
		return m_EvaLevel;
	}
	//�������۽��
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
	////���ÿ�������
	void SetIsOpenEva(const BOOL isOpen){m_bIsOpenEva = isOpen;}
	BOOL GetIsOpenEva()const{return m_bIsOpenEva;}
	////�����������
	void SetIsFinshEva(const BOOL isFinsh){m_bIsFinshEva = isFinsh;}
	BOOL GetIsFinshEva()const {return m_bIsFinshEva;}
	////��ȡ��ǰ���ݶ�Ӧ�Ķ��е�����
// 	void SetCurrWaitNum(int nWaitNum){m_nCurrWaitNum = nWaitNum;}
// 	int GetCurrWaitNum(void)const{return m_nCurrWaitNum;}

	BOOL GetIsLocalData() const;

	virtual void Serialize( CArchive& ar );//֧�����л�
	DECLARE_SERIAL(SLZData)
private:
	CString m_strSerialId;	//ҵ����ˮ��
	CString m_strOrganId;	//�������
	CString m_strOrganName;//��������
	CString m_strBType;		//ҵ������
	CString m_strBussName;	//ҵ������
	CString m_strQueueNum;	//�ŶӺ���
	CardType m_CardType;	//������
	CString m_strCardNum;	//����
	CString m_CustName;		//�ͻ�����
	//CString m_CustGender;	//�ͻ��Ա�
	UINT m_iCusLevel;		//�ͻ��ȼ�
	CTime m_timeTakingNum;	//ȡ��ʱ��
	UINT m_iWindowId;		//����ID
	UINT m_iWindowShowId;//���ڱ��
	CString m_strStaffId;	//Ա�����
	CTime m_timeCall;		//����ʱ��
	CTime m_timeFinish;		//���ʱ��
	EvaLevel m_EvaLevel;	//���۽��
	BOOL m_bIsOpenEva; //������Ϣ�Ƿ���������
	BOOL m_bIsFinshEva;//������Ϣ�Ƿ����������
	CString m_queSerialId;//���б�ţ���������ȡ��
	UINT m_iQueNum;    //INT���ŶӺ�
	CString m_strPhoneNum;   //Ҫ���Ͷ��ŵĵ绰��
	CString m_strMsg; //���Ͷ��ŵ�����
private:
	int GetSrand(const int &max);
	CString GetOnlyId();//ȡ��ΨһID
};
