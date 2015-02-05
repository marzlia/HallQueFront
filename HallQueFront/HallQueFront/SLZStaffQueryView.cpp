#include "StdAfx.h"
#include "SLZStaffQueryView.h"
#include "DoFile.h"

SLZStaffQueryView::SLZStaffQueryView(void)
{
}

SLZStaffQueryView::~SLZStaffQueryView(void)
{
	Clear();
}

BOOL SLZStaffQueryView::LoadStaffs()
{
	CDoFile doFile;
	CString exePath = doFile.GetExeFullFilePath();
	exePath+=_T("\\StaffBasicInfo\\StaffBasicInfo.dat");
	CFile file;
	CFileException e;
	if (file.Open(exePath,CFile::modeRead,&e))
	{
		SLZStaff* pStaff=NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength())
		{
			do
			{
				ar>>pStaff;
				if (pStaff)
					m_mapIdStaff[pStaff->GetStaffId()] = pStaff;
			}while(!ar.IsBufferEmpty());
	
		}
		ar.Close();
		file.Close();
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

SLZStaff* SLZStaffQueryView::QueryStaffById(CString strStaffId)
{
	std::map<CString,SLZStaff*>::const_iterator itera = m_mapIdStaff.find(strStaffId);
	if(itera!=m_mapIdStaff.end())
	{
		return itera->second;
	}
	return NULL;
}
void SLZStaffQueryView::Clear()
{
	/*
	POSITION pos = m_mapIdStaff.GetStartPosition();
	CString strKey;
	SLZStaff* pStaff = NULL;
	for(; pos; m_mapIdStaff.GetNextAssoc(pos, strKey, pStaff))
	{
		if(pStaff)
		{
			delete pStaff;
			pStaff = NULL;
		}
	}
	*/
	std::map<CString,SLZStaff*>::const_iterator itera = m_mapIdStaff.begin();
	for(itera;itera!=m_mapIdStaff.end();itera++)
	{
		delete itera->second;
	}
	m_mapIdStaff.clear();
}

