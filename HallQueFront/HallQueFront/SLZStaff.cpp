#include "stdafx.h"
#include "HallQueFront.h"
#include "SLZStaff.h"


IMPLEMENT_SERIAL(SLZStaff,CObject,1)

SLZStaff::SLZStaff(void)
{
	m_iGender=MYGENDER_NONE;
}

SLZStaff::~SLZStaff(void)
{
	
}

SLZStaff::SLZStaff(const SLZStaff& staff)
{
	m_strStaffId = staff.m_strStaffId;
	m_strStaffName = staff.m_strStaffName;
	m_iGender = staff.m_iGender;
	m_strPhotoPath = staff.m_strPhotoPath;
}

SLZStaff& SLZStaff::operator=(const SLZStaff& staff)
{
	m_strStaffId = staff.m_strStaffId;
	m_strStaffName = staff.m_strStaffName;
	m_iGender = staff.m_iGender;
	m_strPhotoPath = staff.m_strPhotoPath;

	return *this;
}

void SLZStaff::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar << m_strStaffId << m_strStaffName << m_iGender << m_strPhotoPath;
	}
	else
	{
		ar >> m_strStaffId >> m_strStaffName >> m_iGender >> m_strPhotoPath;
	}
}
