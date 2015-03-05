#pragma once

class CJudgeShortMsg : public CObject
{
public:
	CJudgeShortMsg(void);
	~CJudgeShortMsg(void);
	//拷贝和赋值
	CJudgeShortMsg(const CJudgeShortMsg& obj);
	CJudgeShortMsg& operator=(const CJudgeShortMsg& obj);

	DECLARE_SERIAL(CJudgeShortMsg)
	UINT GetJudgeShortMsgSerialID(){return m_shortMsgSerialID;}
	void SetPhoneNum(CString strPhoneNum){m_strPhoneNum = strPhoneNum;}
	CString GetPhoneNum(){return m_strPhoneNum;}
	void SetShortMsg(CString strShortMsg){m_strShortMsg = strShortMsg;}
	CString GetShortMsg(){return m_strShortMsg;}
	virtual void Serialize( CArchive& ar );//支持序列化
private:
	UINT m_shortMsgSerialID;//唯一序号
	CString m_strPhoneNum;//电话号码
	CString m_strShortMsg;//短信类容
};
