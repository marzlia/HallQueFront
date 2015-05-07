#include "StdAfx.h"
#include "SLZWindowQueryView.h"
#include "CommonConvert.h"
#include "DoFile.h"
#include "HallQueFront.h"

extern void MyWriteConsole(CString str);

SLZWindowQueryView::SLZWindowQueryView(void)
{
}

SLZWindowQueryView::~SLZWindowQueryView(void)
{
//	Clear();
}
/*
BOOL SLZWindowQueryView::LoadWindows()
{
	/////
	CDoFile doFile;
	CString exePath = doFile.GetExeFullFilePath();
	exePath+=_T("\\Window.dat");
	CFile file;
	CFileException e;
	if (file.Open(exePath,CFile::modeRead,&e))
	{
		Clear();
		SLZWindow* pWindow = NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength())
		{
			do
			{
				ar>>pWindow;
				if (pWindow)
				{
					int iWindowID = pWindow->GetWindowId();
					if(iWindowID>0)
						m_mapIdWindow[iWindowID] = pWindow;
					int iCallerId = pWindow->GetCallerId();
					if(iCallerId > 0)
					{
						m_mapCallerIdWindow[iCallerId] = pWindow;
					}
					UINT iEvaId = pWindow->GetEvaluatorId();
					if(iEvaId > 0)
					{
						m_mapEvaIdWindow[iEvaId] = pWindow;
					}
					UINT iLedPhyId = pWindow->GetLEDPhyId();
					if (iLedPhyId > 0)
					{
						m_mapLedPhyIdWindow[iLedPhyId] = pWindow;
					}
					UINT iWndScrId = pWindow->GetWndScreenId();
					if (iWndScrId > 0)
					{
						m_mapWndScrIdWindow[iWndScrId] = pWindow;
					}
					UINT iWndComId = pWindow->GetComScreenId();
					if (iWndComId>0)
					{
						m_mapWndComIdWindow[iWndComId] = pWindow;
					}
//					delete pWindow;
				}
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
*/
BOOL SLZWindowQueryView::LoadWindows()
{
	/////
	CDoFile doFile;
	CString exePath = doFile.GetExeFullFilePath();
	exePath+=_T("\\Window.dat");
	CFile file;
	CFileException e;
	if (file.Open(exePath,CFile::modeRead,&e))
	{
		Clear();//清空
		SLZWindow* pWindow = NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength())
		{
			do
			{
				ar>>pWindow;
				if (pWindow)
				{
					int iWindowID = pWindow->GetWindowId();
					if(iWindowID>0)
						m_mapIdWindow[iWindowID] = *pWindow;
					int iCallerId = pWindow->GetCallerId();
					if(iCallerId > 0)
					{
						m_mapCallerIdWindow[iCallerId] = *pWindow;
					}
					UINT iEvaId = pWindow->GetEvaluatorId();
					if(iEvaId > 0)
					{
						m_mapEvaIdWindow[iEvaId] = *pWindow;
					}
// 					UINT iLedPhyId = pWindow->GetLEDPhyId();
// 					if (iLedPhyId > 0)
// 					{
// 						m_mapLedPhyIdWindow[iLedPhyId] = *pWindow;
// 					}
// 					UINT iWndScrId = pWindow->GetWndScreenId();
// 					if (iWndScrId > 0)
// 					{
// 						m_mapWndScrIdWindow[iWndScrId] = *pWindow;
// 					}
// 					UINT iWndComId = pWindow->GetComScreenId();
// 					if (iWndComId>0)
// 					{
// 						m_mapWndComIdWindow[iWndComId] = *pWindow;
// 					}
					delete pWindow;
				}
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

/*
SLZWindow* SLZWindowQueryView::QueryWindowById(UINT iWindowId)
{
	SLZWindow* pWindow = NULL;
	std::map<UINT,SLZWindow*>::const_iterator itera;
	itera = m_mapIdWindow.find(iWindowId);
	if(itera!=m_mapIdWindow.end())
	{
		pWindow = itera->second;
	}
	return pWindow;
	//return m_mapIdWindow[iWindowId];
}
*/
BOOL SLZWindowQueryView::QueryWindowById(UINT iWindowId,SLZWindow& Window)
{
	BOOL flag = FALSE;
	std::map<UINT,SLZWindow>::const_iterator itera;
	itera = m_mapIdWindow.find(iWindowId);
	if(itera!=m_mapIdWindow.end())
	{
		Window = itera->second;
		flag = TRUE;
	}
	return flag;
}
/*
SLZWindow* SLZWindowQueryView::QueryWindowByCallerId(UINT iCallerId)
{
	SLZWindow* pWindow = NULL;
// 	if(!m_mapCallerIdWindow.Lookup(iCallerId, pWindow))
// 	{
// 		pWindow = NULL;
// 	}
	std::map<UINT,SLZWindow*>::const_iterator itera = m_mapCallerIdWindow.find(iCallerId);
	if(itera!=m_mapCallerIdWindow.end())
	{
		pWindow = itera->second;
	}
	return pWindow;
	
	//return m_mapCallerIdWindow[iCallerId];
}
*/

BOOL SLZWindowQueryView::QueryWindowByCallerId(UINT iCallerId,SLZWindow& Window)
{
//	SLZWindow Window;
	// 	if(!m_mapCallerIdWindow.Lookup(iCallerId, pWindow))
	// 	{
	// 		pWindow = NULL;
	// 	}
	BOOL flag = FALSE;
	std::map<UINT,SLZWindow>::const_iterator itera = m_mapCallerIdWindow.find(iCallerId);
	if(itera!=m_mapCallerIdWindow.end())
	{
		Window = itera->second;
		flag = TRUE;
	}
	return flag;
}
/*
SLZWindow* SLZWindowQueryView::QueryWindowByEvaId(UINT iEvaId)
{
	SLZWindow* pWindow = NULL;
// 	if(!m_mapEvaIdWindow.Lookup(iEvaId,pWindow))
// 	{
// 		pWindow = NULL;
// 	}
	std::map<UINT,SLZWindow*>::const_iterator itera = m_mapEvaIdWindow.find(iEvaId);
	if(itera!=m_mapEvaIdWindow.end())
	{
		pWindow = itera->second;
	}
	return pWindow;
}
*/
BOOL SLZWindowQueryView::QueryWindowByEvaId(UINT iEvaId,SLZWindow& Window)
{
	// 	if(!m_mapEvaIdWindow.Lookup(iEvaId,pWindow))
	// 	{
	// 		pWindow = NULL;
	// 	}
	BOOL flag = FALSE;
	std::map<UINT,SLZWindow>::const_iterator itera = m_mapEvaIdWindow.find(iEvaId);
	if(itera!=m_mapEvaIdWindow.end())
	{
		Window = itera->second;
		flag = TRUE;
	}
	return flag;
}
/*
SLZWindow* SLZWindowQueryView::QueryWindowByLedPhyId(UINT iLedPhyId)
{
	SLZWindow* pWindow = NULL;
// 	if (!m_mapLedPhyIdWindow.Lookup(iLedPhyId,pWindow))
// 	{
// 		pWindow = NULL;
// 	}
	std::map<UINT,SLZWindow*>::const_iterator itera = m_mapLedPhyIdWindow.find(iLedPhyId);
	if(itera!=m_mapLedPhyIdWindow.end())
	{
		pWindow = itera->second;
	}
	return pWindow;
}
*/
// BOOL SLZWindowQueryView::QueryWindowByLedPhyId(UINT iLedPhyId,SLZWindow& Window)
// {
// 	//SLZWindow Window;
// 	// 	if (!m_mapLedPhyIdWindow.Lookup(iLedPhyId,pWindow))
// 	// 	{
// 	// 		pWindow = NULL;
// 	// 	}
// 	BOOL flag = FALSE;
// 	std::map<UINT,SLZWindow>::const_iterator itera = m_mapLedPhyIdWindow.find(iLedPhyId);
// 	if(itera!=m_mapLedPhyIdWindow.end())
// 	{
// 		Window = itera->second;
// 		flag = TRUE;
// 	}
// 	return flag;
// }
/*
SLZWindow* SLZWindowQueryView::QueryWindowByWndScrId(UINT iWndScrId)
{
	SLZWindow* pWindow = NULL;
// 	if (!m_mapWndScrIdWindow.Lookup(iWndScrId,pWindow))
// 	{
// 		pWindow = NULL;
// 	}
	std::map<UINT,SLZWindow*>::const_iterator itera = m_mapWndScrIdWindow.find(iWndScrId);
	if(itera!=m_mapWndScrIdWindow.end())
	{
		pWindow = itera->second;
	}
	return pWindow;
}
*/

// BOOL SLZWindowQueryView::QueryWindowByWndScrId(UINT iWndScrId,SLZWindow& Window)
// {
// 	// 	if (!m_mapWndScrIdWindow.Lookup(iWndScrId,pWindow))
// 	// 	{
// 	// 		pWindow = NULL;
// 	// 	}
// 	BOOL flag = FALSE;
// 	std::map<UINT,SLZWindow>::const_iterator itera = m_mapWndScrIdWindow.find(iWndScrId);
// 	if(itera!=m_mapWndScrIdWindow.end())
// 	{
// 		Window = itera->second;
// 		flag = TRUE;
// 	}
// 	return flag;
// }

// BOOL SLZWindowQueryView::QueryWindowByWndComId(UINT iWndComId,SLZWindow& Window)
// {
// 	BOOL flag = FALSE;
// 	std::map<UINT,SLZWindow>::const_iterator itera = m_mapWndComIdWindow.find(iWndComId);
// 	if(itera!=m_mapWndComIdWindow.end())
// 	{
// 		Window = itera->second;
// 		flag = TRUE;
// 	}
// 	return flag;
// }

void SLZWindowQueryView::Clear()
{
//	SLZWindow* pWindow = NULL;
//	std::map<UINT,SLZWindow*>::const_iterator itera = m_mapIdWindow.begin();
//	for(itera;itera!=m_mapIdWindow.end();itera++)
//	{
//		delete itera->second;
//	}
	m_mapIdWindow.clear();
	m_mapCallerIdWindow.clear();
	m_mapEvaIdWindow.clear();
// 	m_mapLedPhyIdWindow.clear();
// 	m_mapWndScrIdWindow.clear();
}
/*
BOOL SLZWindowQueryView::InitLoginMap()
{
	if(m_mapIdWindow.size()==0)return FALSE;
	std::map<UINT,SLZWindow*>::const_iterator itera = m_mapIdWindow.begin();
	SLZWindow* pWindow = NULL;
	for(itera;itera!=m_mapIdWindow.end();itera++)
	{
		//		m_map_que[pWindow->GetWindowId()] = pWindow->GetLoginStaff();
		pWindow = itera->second;
		ASSERT(pWindow);
		CString staffID = pWindow->GetLoginStaff();//默认登录staffID
		std::map<UINT,CString>::const_iterator itera;
		itera = theApp.m_Controller.m_mapLoginList.find(pWindow->GetWindowId());
		if(itera != theApp.m_Controller.m_mapLoginList.end())//找到了.
		{
			CString mapStaffID = itera->second;
			if(!staffID.IsEmpty())//不为空，设置了默认登录
			{
				if(mapStaffID != staffID)
				{
					theApp.m_Controller.m_mapLoginList[pWindow->GetWindowId()] = staffID;
				}
			}
			else//没设置默认登录
			{
//				theApp.m_Controller.m_mapLoginList[pWindow->GetWindowId()] = _T("");
			}
		}
		else//没找到
		{
			if(!staffID.IsEmpty())
				theApp.m_Controller.m_mapLoginList[pWindow->GetWindowId()] = staffID;
		}
	}
	return TRUE;
}
*/
// int SLZWindowQueryView::GetAllThroughID(int throughID[])
// {
// 	int size = 0;
// 	int* tempID = new int[m_mapIdWindow.size()+1];
// 	memset(tempID,0,4*(m_mapIdWindow.size()+1));
// 	std::map<UINT,SLZWindow>::const_iterator itera = m_mapIdWindow.begin();
// 	for(itera;itera!=m_mapIdWindow.end();itera++)
// 	{
// 		SLZWindow window = itera->second;
// 		int phID = window.GetLEDPhyId();
// 		if(phID>0)
// 		{
// 			BOOL flag = FALSE;
// 			for(int i=0;i<(int)m_mapIdWindow.size()+1;i++)
// 			{
// 				if(tempID[i]==phID)
// 				{
// 					flag = TRUE;
// 					break;
// 				}
// 			}
// 			if(!flag)
// 			{
// 				tempID[size] = phID;
// 				size++;
// 			}
// 		}
// 	}
// 	for(int j=0;j<size;j++)
// 	{
// 		throughID[j] = tempID[j];
// 	}
// 	delete [] tempID;
// 	return size;
// }