#pragma once
#include "DataDef.h"

class SLZStaff : public CObject
{
public:
	SLZStaff(void);
	~SLZStaff(void);
	SLZStaff(const SLZStaff& staff);
	SLZStaff& operator=(const SLZStaff& staff);

	DECLARE_SERIAL(SLZStaff)

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
	//获取员工姓名
	CString GetStaffName() const
	{
		return m_strStaffName;
	}
	//设置员工姓名
	void SetStaffName(const CString& strStaffName)
	{
		m_strStaffName = strStaffName;
	}
	//获取员工性别
	GENDER GetStaffGender() const
	{
		return m_iGender;
	}
	//设置员工性别
	void SetStaffGender(GENDER gender)
	{
		m_iGender = gender;
	}
	//获取员工照片
	CString GetStaffPhoto() const
	{
		return m_strPhotoPath;
	}
	//设置员工照片
	void SetStaffPhoto(CString strPhotoPath)
	{
		m_strPhotoPath = strPhotoPath;
	}
	////获取呼叫器地址
	//int GetCallerId() const
	//{
	//	return m_iCallerId;
	//}
	////设置呼叫器地址
	//void SetCallerId(int iCallerId)
	//{
	//	m_iCallerId = iCallerId;
	//}
	////获取评价器地址
	//int GetEvaluatorId() const
	//{
	//	return m_iEvaluatorId;
	//}
	////设置评价器地址
	//void SetEvaluatorId(int iEvaluatorId)
	//{
	//	m_iEvaluatorId = iEvaluatorId;
	//}
	virtual void Serialize( CArchive& ar );

private:
	CString m_strStaffId;	//员工编号
	CString m_strStaffName;	//员工姓名
	GENDER m_iGender;		//性别
	CString m_strPhotoPath;	//照片路径


	//int m_iBirthYear;		//出生年
	//int m_iBirthMon;		//出生月
	//int m_iBirthDay;		//出生日
	////int m_iAge;				//年龄
	//int m_iEntryYear;		//入职年
	//int m_iEntryMon;		//入职月
	//int m_iEntryDay;		//入职日
	////int m_iSeniority;		//工龄
};

//IMPLEMENT_SERIAL(SLZStaff,CObject,1)

