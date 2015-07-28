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

	//��ȡ���ڱ��
	UINT GetWindowId() const
	{
		return m_iWindowId;
	}
	//���ô��ڱ��
	void SetWindowId(UINT iWindowId)
	{
		m_iWindowId = iWindowId;
	}
	//��ȡ��ʾ���
	UINT GetShowWndId() const
	{
		return m_iShowWndId;
	}
	//������ʾ���
	void SetShowWndId(UINT iShowWndId)
	{
		m_iShowWndId = iShowWndId;
	}
	//��ȡ��������
	CString GetWindowName() const
	{
		return m_strWindowName;
	}
	//���ô�������
	void SetWindowName(const CString& strWindowName)
	{
		m_strWindowName = strWindowName;
	}
	//��ȡ��������
	CString GetWindowCallName() const
	{
		return m_strWindowCallName;
	}
	//���ô�������
	void SetWindowCallName(const CString& strWindowCallName)
	{
		m_strWindowCallName = strWindowCallName;
	}
	//��ȡ��������ַ
	int GetCallerId() const
	{
		return m_iCallerId;
	}
	//���ú�������ַ
	void SetCallerId(int iCallerId)
	{
		m_iCallerId = iCallerId;
	}
	//��ȡ��������ַ
	int GetEvaluatorId() const
	{
		return m_iEvaluatorId;
	}
	//������������ַ
	void SetEvaluatorId(int iEvaluatorId)
	{
		m_iEvaluatorId = iEvaluatorId;
	}
// 	��ȡ��������ַ
// 	CStringArray& GetWndScreenId()//const
// 	{
// 		return m_sWndScreenIdArray;
// 	}
// 	//���ô�������ַ
// 	void SetWndScreenId(const CStringArray& sWndScreenIdArray)
// 	{
// 		m_sWndScreenIdArray.Copy(sWndScreenIdArray);
// 	}
// 	//��ȡ�ۺ�����ַ
// 	CStringArray& GetComScreenId() //const
// 	{
// 		return m_sComScreenIdArray;	 
// 	}
// 	//�����ۺ�����ַ
// 	void SetComScreenId(const CStringArray& sComScreenIdArray) 
// 	{
// 		m_sComScreenIdArray.Copy(sComScreenIdArray);
// 	}


	CString GetStbId()const 
	{
		return m_strStbId;
	}
	//������
	void SetStbId(const CString stbid)
	{
		m_strStbId=stbid;
	}

	//��ȡԤ��Ա��
	CString GetLoginStaff() 
	{
		return m_staffDefaultId;
	}

	//����Ԥ��Ա��

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
	//��ȡ������Ϣ
	CString GetCallMsg()
	{
		return m_CalledMsg;
	}
	void SetCallMsg(const CString& CallMsg)
	{
		m_CalledMsg=CallMsg;
	}
	//��ȡ�ȴ���Ϣ
	CString GetWaitCallMsg()
	{
		return m_WaitCalledMsg;
	}
	void SetWaitCallMsg(const CString& CallMsg)
	{
		m_WaitCalledMsg=CallMsg;
	}
	//��ȡ��ʾ��Ϣ
	CString GetShowMsg()
	{
		return m_ShowMsg;
	}
	void SetShowMsg(const CString& ShowMsg)
	{
		m_ShowMsg=ShowMsg;
	}
	//��ȡ�ȴ���ʾ��Ϣ
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
	UINT m_iWindowId;				//����Ψһ���
	UINT m_iShowWndId;			//������ʾ���
	CString m_strWindowName;		//��������
	CString m_strWindowCallName;	//��������
	int m_iCallerId;				//��������ַ
	int m_iEvaluatorId;				//��������ַ
//	CStringArray m_sWndScreenIdArray;	//��������ַ
//	CStringArray m_sComScreenIdArray;	//�ۺ�����ַ

	CString m_strStbId;				//�����б��
	//CString m_staffLoginId;			//��ǰ��¼Ա��
	CString m_staffDefaultId;		//�Զ���¼ʱԤ��Ա��
									//�ɴ������
	int m_ArraySize;				//���������С
	CStringArray m_arrBussId;		//�ɴ���ҵ���������
	
	CString m_CalledMsg;			//������Ϣ
	CString m_WaitCalledMsg;		//�ȴ�������Ϣ
	CString m_ShowMsg;				//��ʾ��Ϣ
	CString m_WaitShowMsg;			//�ȴ���ʾ��Ϣ
	unsigned int m_iMsgShowTime;		//��Ϣ��ʾʱ��
	CString m_strAdMsg;				//�����Ϣ
	
	UINT m_iEvaTimeOut;			//���۳�ʱ����
	BOOL m_bIsUsePower; //�Ƿ������ȼ�
public:
	CArray<CThroughWndScreenInfo> m_throughscreen_array;//������
};

