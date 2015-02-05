#pragma once
#include "SLZWindow.h"

class SLZWindowManager
{
public:
	SLZWindowManager(void);
	~SLZWindowManager(void);

	//将保存在文件中的员工信息导入内存
	BOOL LoadWindows();
	//将内存中的员工信息写入文件
	BOOL SaveWindows();
	//
	int AddNew(SLZWindow& window)
	{
		m_arrWindow.Add(window);
		//m_iIndex = m_arrWindow.GetCount() - 1;
	}
	//
	const CArray<SLZWindow>& GetWindowArray()
	{
		return m_arrWindow;
	}
	////
	//SLZWindow GetWindow()
	//{
	//	return m_arrWindow[m_iIndex];
	//}

	//
	UINT GetId() const
	{
		m_arrWindow[m_iIndex].GetWindowId();
	}
	//
	CString GetName() const
	{
		m_arrWindow[m_iIndex].GetWindowName();
	}
	//
	BOOL EditId(UINT iId)
	{
		for(int i = 0; i < m_arrWindow.GetCount(); i++)
		{
			if(iId == m_arrWindow[i].GetWindowId())
			{
				return FALSE;
			}
		}
		m_arrWindow[m_iIndex].SetWindowId(iId);
		return TRUE;
	}
	BOOL EditCallerId(int iId)
	{
		for(int i = 0; i < m_arrWindow.GetCount(); i++)
		{
			if(iId == m_arrWindow[i].GetCallerId())
			{
				return FALSE;
			}
		}
		m_arrWindow[m_iIndex].SetCallerId(iId);
		return TRUE;
	}

	BOOL EditName(CString strName)
	{
		m_arrWindow[m_iIndex].SetWindowName(strName);
		return TRUE;
	}

	BOOL EditCallName(CString strCallName)
	{
		m_arrWindow[m_iIndex].SetWindowCallName(strCallName);
		return TRUE;
	}

	BOOL EditEvalId(int iEvalId)
	{
		m_arrWindow[m_iIndex].SetEvaluatorId(iEvalId);
		return TRUE;
	}

	BOOL EditWndScrId(int iWndScrId)
	{
		m_arrWindow[m_iIndex].SetWndScreenId(iWndScrId);
		return TRUE;
	}

	BOOL EditComScrId(int iComScrId)
	{
		m_arrWindow[m_iIndex].SetComScreenId(iComScrId);
		return TRUE;
	}

	BOOL EditLEDIPId(int iLEDIPId)
	{
		
		return TRUE;
	}
	void MoveFirst()
	{
		m_iIndex = 0;
	}
	void MoveNext()
	{
		if(m_iIndex < m_arrWindow.GetCount() -1)
		{
			m_iIndex++;
		}
	}
	void MoveLast()
	{
		m_iIndex = m_arrWindow.GetCount() -1;
		if(m_iIndex < 0)
		{
			m_iIndex = 0;
		}
	}
	void MovePreview()
	{
		if(m_iIndex > 0)
		{
			m_iIndex--;
		}
	}
	void MoveTo(int iTo)
	{
		if(iTo >= 0 || iTo <= m_arrWindow.GetCount()-1)
		{
			m_iIndex = iTo;
		}
	}
	//
	BOOL MoveWindowTo(int iTo)
	{
		if(iTo < 0 || iTo > m_arrWindow.GetCount() -1)
		{
			return FALSE;
		}
		SLZWindow Window = m_arrWindow[m_iIndex];
		m_arrWindow.RemoveAt(m_iIndex);
		m_arrWindow.InsertAt(iTo, Window);

		return TRUE;
	}

private:
	CArray<SLZWindow> m_arrWindow;
	int m_iIndex;
};

