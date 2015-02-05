#pragma once

class CCardLevel : public CObject
{
public:
	CCardLevel(void);
	~CCardLevel(void);
	//拷贝和赋值
	CCardLevel(const CCardLevel& obj);
	CCardLevel& operator=(const CCardLevel& obj);
	///////////卡等级
	void SetCardLevel(const int level){m_cardLevel = level;}
	int GetCardLevel(){return m_cardLevel;}
	//////////卡等级名称
	void SetCardLevelName(const CString& levName){m_cardLevName = levName;}
	CString GetCardLevelName(){return m_cardLevName;}
	//////////该卡等级对应的排队队列
	void SetCardLevAttchToQue(const CString& queID){m_levAttchToQueID = queID;}
	CString GetCardLevAttchToQue(){return m_levAttchToQueID;}
	/////////卡等级唯一ID，序号
//	void SetCardLevSerialID(const UINT serialID){m_cardLevSerialID = serialID;}
	UINT GetCardLecSerialID(){return m_cardLevSerialID;}
	////////////////////////////
	virtual void Serialize( CArchive& ar );//支持序列化
	DECLARE_SERIAL(CCardLevel)
	//////////////////////////
	void SetCardLevAttchToPageID(int nPageID){m_nLevAttchToPageID = nPageID;}
	int GetCardLevAttchToPageID(){return m_nLevAttchToPageID;}
private:
	int m_cardLevel;//卡等级如：金卡,白金卡等待
	CString m_cardLevName;//卡等级名称
	CString m_levAttchToQueID;//卡等级关联的ID
	UINT m_cardLevSerialID;//卡等级唯一ID，序号
	int m_nLevAttchToPageID;//卡等级关联界面ID
};
