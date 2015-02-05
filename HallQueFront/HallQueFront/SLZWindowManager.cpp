#include "StdAfx.h"
#include "SLZWindowManager.h"

SLZWindowManager::SLZWindowManager(void)
: m_iIndex(0)
{
}

SLZWindowManager::~SLZWindowManager(void)
{
}

BOOL SLZWindowManager::LoadWindows()
{
	CFile file;
	if(!file.Open(_T("Window.dat"), CFile::modeRead))
	{
		return FALSE;
	}
	CArchive ar(&file, CArchive::load);
	int iCount;
	ar >> iCount;
	if(iCount < 1)
	{
		return FALSE;
	}
	SLZWindow* pWindow = NULL;
	for(int i = 0; i < iCount; i++)
	{
		ar >> pWindow;
		m_arrWindow.Add(*pWindow);
	}

	return TRUE;
}

BOOL SLZWindowManager::SaveWindows()
{
	CFile file;
	if(!file.Open(_T("Window.dat"), CFile::modeWrite|CFile::modeCreate))
	{
		return FALSE;
	}
	CArchive ar(&file, CArchive::store);
	ar << m_arrWindow.GetCount();
	for(int i = 0; i < m_arrWindow.GetCount(); i++)
	{
		ar << &m_arrWindow[i];
	}
	return TRUE;
}

