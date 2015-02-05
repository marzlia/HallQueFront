#include "StdAfx.h"
#include "CascadeMethod.h"
#include "HallQueFront.h"

CCascadeMethod::CCascadeMethod(void)
{
	CString fullPath = convert.GetExeFullFilePath();
	m_QueInfoPath = fullPath + _T("\\QueBasicInfo\\QueBasicInfo.dat");
	m_StaffInfoPath = fullPath + _T("\\StaffBasicInfo\\StaffBasicInfo.dat");
	m_WindowInfoPath = fullPath + _T("\\Window.dat");
	m_SwingInfoPath = fullPath + _T("\\CardConfigInfo\\CardConfigInfo.dat");
	m_connect_path = fullPath + _T("\\CardConfigInfo\\CardConnectInfo.dat");
	m_cardLev_path = fullPath + _T("\\CardConfigInfo\\CardLevInfo.dat");
}

CCascadeMethod::~CCascadeMethod(void)
{
}

BOOL CCascadeMethod::ReadQueInfoFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_QueInfoPath,CFile::modeRead,&e))
	{
		m_map_QueInfo.RemoveAll();
		CQueueInfo* queinfo=NULL;
		int i=0;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>queinfo;
				if (queinfo)
				{
					m_map_QueInfo[i]= *queinfo;
					delete queinfo;
					queinfo = NULL;
					i++;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();

	}
	else return FALSE;
	return TRUE;
}

BOOL CCascadeMethod::ReadStaffInfoFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_StaffInfoPath,CFile::modeRead,&e))
	{
		m_map_StaffInfo.RemoveAll();
		SLZStaff* Staffinfo=NULL;
		int i=0;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>Staffinfo;
				if (Staffinfo)
				{
					m_map_StaffInfo[i]= *Staffinfo;
					delete Staffinfo;
					Staffinfo = NULL;
					i++;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();

	}
	else return FALSE;
	return TRUE;
}

BOOL CCascadeMethod::ReadWindowInfoFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_WindowInfoPath,CFile::modeRead,&e))
	{
		m_map_WindowInfo.RemoveAll(); 
		SLZWindow* Windowinfo=NULL;
		int i=0;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>Windowinfo;
				if (Windowinfo)
				{
					m_map_WindowInfo[i]= *Windowinfo;
					delete Windowinfo;
					Windowinfo = NULL;
					i++;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
	}
	else return FALSE;
	return TRUE;
}

extern void MyWriteConsole(CString str);

BOOL CCascadeMethod::WriteQueInfoToFile()
{
#ifdef _DEBUG
	m_ComputerTime.SetStartTime(clock());
#endif
	CFile file;
	CFileException e;
	if (file.Open(m_QueInfoPath,CFile::modeCreate|CFile::modeWrite,&e))
	{
		int count=m_map_QueInfo.GetCount();
		for (int i=0;i<count;i++)
		{
			CArchive ar(&file,CArchive::store);
			CQueueInfo queinfo;
			m_map_QueInfo.Lookup(i,queinfo);
			ar<<&queinfo;
			ar.Close();
		}
		file.Close();

// #ifdef _DEBUG
// 		m_ComputerTime.SetFinshTime(clock());
// 		CString MyClock;
// 		MyClock.Format(_T("%f"),m_ComputerTime.GetDuration());
// 		MyWriteConsole(_T("CArchive:")+MyClock);
// #endif
		return TRUE;
	}
	else
	{
		TRACE(_T("File could not be opened %d\n"), e.m_cause);
		return FALSE;
	}
}

BOOL CCascadeMethod::WriteStaffInfoToFile()
{
#ifdef _DEBUG
	m_ComputerTime.SetStartTime(clock());
#endif
	CFile file;
	CFileException e;
	if (file.Open(m_StaffInfoPath,CFile::modeCreate|CFile::modeWrite,&e))
	{
		int count=m_map_StaffInfo.GetCount();
		for (int i=0;i<count;i++)
		{
			CArchive ar(&file,CArchive::store);
			SLZStaff staffinfo;
			m_map_StaffInfo.Lookup(i,staffinfo);
			ar<<&staffinfo;
			ar.Close();
		}
		file.Close();

// #ifdef _DEBUG
// 		m_ComputerTime.SetFinshTime(clock());
// 		CString MyClock;
// 		MyClock.Format(_T("%f"),m_ComputerTime.GetDuration());
// 		MyWriteConsole(_T("CArchive:")+MyClock);
// #endif
		return TRUE;
	}
	else
	{
		TRACE(_T("File could not be opened %d\n"), e.m_cause);
		return FALSE;
	}
}

BOOL CCascadeMethod::WriteWindowInfoToFile()
{
#ifdef _DEBUG
	m_ComputerTime.SetStartTime(clock());
#endif
	CFile file;
	CFileException e;
	if (file.Open(m_WindowInfoPath,CFile::modeCreate|CFile::modeWrite,&e))
	{
		int count=m_map_WindowInfo.GetCount();
		for (int i=0;i<count;i++)
		{
			CArchive ar(&file,CArchive::store);
			SLZWindow windowinfo;
			m_map_WindowInfo.Lookup(i,windowinfo);
			ar<<&windowinfo;
			ar.Close();
		}
		file.Close();

// #ifdef _DEBUG
// 		m_ComputerTime.SetFinshTime(clock());
// 		CString MyClock;
// 		MyClock.Format(_T("%f"),m_ComputerTime.GetDuration());
// 		MyWriteConsole(_T("CArchive:")+MyClock);
// #endif
		return TRUE;
	}
	else
	{
		TRACE(_T("File could not be opened %d\n"), e.m_cause);
		return FALSE;
	}
}

BOOL CCascadeMethod::ReadConfigCardInfoFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_SwingInfoPath,CFile::modeRead,&e))
	{
		m_map_SwingInfo.RemoveAll(); 
		CSwingCard* Swinginfo=NULL;
		int i=0;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>Swinginfo;
				if (Swinginfo)
				{
					m_map_SwingInfo[i]= *Swinginfo;
					delete Swinginfo;
					Swinginfo = NULL;
					i++;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
	}
	else return FALSE;
	return TRUE;
}

BOOL CCascadeMethod::WriteConfigCardInfoToFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_SwingInfoPath,CFile::modeCreate|CFile::modeWrite,&e))
	{
		int count=m_map_SwingInfo.GetCount();
		for (int i=0;i<count;i++)
		{
			CArchive ar(&file,CArchive::store);
			CSwingCard swinginfo;
			m_map_SwingInfo.Lookup(i,swinginfo);
			ar<<&swinginfo;
			ar.Close();
		}
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCascadeMethod::ReadConnectInfoFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_connect_path,CFile::modeRead,&e))
	{
		file.Read(&m_cardConnectInfo,sizeof(CARDCONNECTINFO));
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CCascadeMethod::WriteConnectInfoToFile()
{
	
	CFile file;
	CFileException e;
	if (file.Open(m_connect_path,CFile::modeCreate|CFile::modeWrite,&e))
	{
		file.Write(&m_cardConnectInfo,sizeof(CARDCONNECTINFO));
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CCascadeMethod::ReadCardLevelFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_cardLev_path,CFile::modeRead,&e))
	{
		m_map_CardLevelInfo.RemoveAll(); 
		CCardLevel* CardLevelinfo=NULL;
		int i=0;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>CardLevelinfo;
				if (CardLevelinfo)
				{
					m_map_CardLevelInfo[i]= *CardLevelinfo;
					delete CardLevelinfo;
					CardLevelinfo = NULL;
					i++;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
	}
	else return FALSE;
	return TRUE;
}

BOOL CCascadeMethod::WriteCardLevelToFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_cardLev_path,CFile::modeCreate|CFile::modeWrite,&e))
	{
		int count=m_map_CardLevelInfo.GetCount();
		for (int i=0;i<count;i++)
		{
			CArchive ar(&file,CArchive::store);
			CCardLevel CardLevelinfo;
			m_map_CardLevelInfo.Lookup(i,CardLevelinfo);
			ar<<&CardLevelinfo;
			ar.Close();
		}
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CCascadeMethod::DeleteQueInfo(CString strQueId)
{
	if(ReadWindowInfoFromFile())
	{
		for (int index = 0;index < m_map_WindowInfo.GetCount();index++)
		{
			SLZWindow windowinfo;
			CStringArray queidarray;
			m_map_WindowInfo.Lookup(index,windowinfo);
			windowinfo.GetArrayQueId(queidarray);
			for (int j = 0;j<queidarray.GetCount();j++)
			{
				if (strQueId==queidarray[j])
				{
					queidarray.RemoveAt(j,1);
					break;
				}
			}
			windowinfo.SetArrayQueId(queidarray);
			m_map_WindowInfo.SetAt(index,windowinfo);
		}
		WriteWindowInfoToFile();
	}
	if (ReadConfigCardInfoFromFile())
	{
		for (int index = 0;index < m_map_SwingInfo.GetCount();index++)
		{
			CSwingCard swinginfo;
			m_map_SwingInfo.Lookup(index,swinginfo);
			
			if (strQueId==swinginfo.GetAttchQueID())
			{
				swinginfo.SetAttchQueID(_T(""));
			}
			m_map_SwingInfo.SetAt(index,swinginfo);
		}
		WriteConfigCardInfoToFile();
	}
	if (ReadConnectInfoFromFile())
	{
		if (strQueId==m_cardConnectInfo.RegAttchQueID)
		{
			memset(m_cardConnectInfo.RegAttchQueID,0,sizeof(m_cardConnectInfo.RegAttchQueID));
		}
		WriteConnectInfoToFile();
	}
	if (ReadCardLevelFromFile())
	{
		for (int index = 0;index < m_map_CardLevelInfo.GetCount();index++)
		{
			CCardLevel CardLevelinfo;
			m_map_CardLevelInfo.Lookup(index,CardLevelinfo);

			if (strQueId==CardLevelinfo.GetCardLevAttchToQue())
			{
				CardLevelinfo.SetCardLevAttchToQue(_T(""));
			}
			m_map_CardLevelInfo.SetAt(index,CardLevelinfo);
		}
		WriteCardLevelToFile();
	}
}

void CCascadeMethod::DeleteStaffInfo(CString strStaffId)
{
	if (ReadWindowInfoFromFile())
	{
		for (int index = 0;index < m_map_WindowInfo.GetCount();index++)
		{
			SLZWindow windowinfo;
			m_map_WindowInfo.Lookup(index,windowinfo);
			if (strStaffId==windowinfo.GetLoginStaff())
			{
				windowinfo.SetLoginStaff(_T(""));
			}
			m_map_WindowInfo.SetAt(index,windowinfo);
		}
		WriteWindowInfoToFile();
	}
}