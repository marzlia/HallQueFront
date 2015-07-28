// WndQueSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "WndQueSetDlg.h"

#include "QueSetDlg.h"
#include "SLZWindowSetDlg.h"

// CWndQueSetDlg 对话框

IMPLEMENT_DYNAMIC(CWndQueSetDlg, CDialog)

CWndQueSetDlg::CWndQueSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWndQueSetDlg::IDD, pParent)
	,m_bIsUsePower(FALSE)
{
	m_Queinfo_path = convert.GetExeFullFilePath();
	m_Queinfo_path += _T("\\QueBasicInfo\\QueBasicInfo.dat");
	m_bMakeAll = FALSE;
}

CWndQueSetDlg::~CWndQueSetDlg()
{
}

void CWndQueSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_UNDSTQUE,m_list_undstque);
	DDX_Control(pDX,IDC_LIST_ABLEQUE,m_list_ableque);
	DDX_Control(pDX,IDC_CHECK_MKALL,m_check_makeall);
	DDX_Control(pDX, IDC_CHECK_USEPOWER, m_check_usepower);
}


BEGIN_MESSAGE_MAP(CWndQueSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADDQUE, &CWndQueSetDlg::OnBnClickedBtnAddque)
	ON_BN_CLICKED(IDC_BTN_DELQUE, &CWndQueSetDlg::OnBnClickedBtnDelque)
	ON_BN_CLICKED(IDOK, &CWndQueSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_UP, &CWndQueSetDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, &CWndQueSetDlg::OnBnClickedBtnDown)
	ON_BN_CLICKED(IDC_CHECK_MKALL, &CWndQueSetDlg::OnBnClickedCheckMkall)
END_MESSAGE_MAP()


// CWndQueSetDlg 消息处理程序

BOOL CWndQueSetDlg::ReadQueInfoFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_Queinfo_path,CFile::modeRead,&e))
	{
		CQueueInfo* queinfo=NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>queinfo;
				if (queinfo)
				{
					m_list_undstque.AddString(queinfo->GetBussName());
					m_list_UndstQueInfo.AddTail(*queinfo);
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();

	}
	else return FALSE;
	return TRUE;
}

BOOL CWndQueSetDlg::ReadWndQueInfo()
{
	SLZWindowSetDlg *pWndSet = (SLZWindowSetDlg*)this->GetParent();
	POSITION pos=pWndSet->m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=pWndSet->m_ListCtr_Window.GetNextSelectedItem(pos);
	SLZWindow windowinfo=pWndSet->m_List_WindowInfo.GetAt(pWndSet->m_List_WindowInfo.FindIndex(index));
	if(windowinfo.GetIsUsePower())
	{
		m_check_usepower.SetCheck(BST_CHECKED);
	}
	else
	{
		m_check_usepower.SetCheck(BST_UNCHECKED);
	}
	CStringArray queidarray;
	windowinfo.GetArrayQueId(queidarray);
	CArray<int,int> aryListBoxSel;
	int count =0;
	for (int i = 0;i<queidarray.GetCount();i++)
	{
		for (int j = m_list_UndstQueInfo.GetCount()-1;j>=0;j--)
		{
			CQueueInfo queinfo;
			queinfo = m_list_UndstQueInfo.GetAt(m_list_UndstQueInfo.FindIndex(j));   //从不能处理队列中取出
			if (queidarray[i]==queinfo.GetQueID())
			{
				m_list_ableque.AddString(queinfo.GetBussName());		//添加到可处理队列ListBox中
				m_list_AbleQueInfo.AddTail(queinfo);
				count++;
				//aryListBoxSel.SetSize(count);
				aryListBoxSel.Add(j);
				//m_list_undstque.DeleteString(j);
				break;
			}
		}
	}
	ArrayOrder(aryListBoxSel);
	for (int k = aryListBoxSel.GetSize()-1;k>=0;k--)			//从后向前删除不能处理队列中ClistBox
	{
		m_list_undstque.DeleteString(aryListBoxSel[k]);			
		m_list_UndstQueInfo.RemoveAt(m_list_UndstQueInfo.FindIndex(aryListBoxSel[k]));
	}
	return TRUE;
}

void CWndQueSetDlg::ArrayOrder(CArray<int,int>& iArray)
{
	int i,j,temp;
	for (i=0;i<iArray.GetSize()-1;i++)
	{
		for (j=0;j<iArray.GetSize()-1-i;j++)
		{
			if (iArray[j]>iArray[j+1])
			{
				temp = iArray[j];
				iArray[j] = iArray[j+1];
				iArray[j+1] = temp;
			}
		}
	}
}

BOOL CWndQueSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list_UndstQueInfo.RemoveAll();
	m_list_AbleQueInfo.RemoveAll();
	ReadQueInfoFromFile();
	ReadWndQueInfo();
	if (!m_bMakeAll)
	{
		m_check_makeall.SetCheck(BST_UNCHECKED);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CWndQueSetDlg::OnBnClickedBtnAddque()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_list_undstque.GetCurSel();
	if (index==CB_ERR)
	{
		return;
	}
	CString strSelect;
	m_list_undstque.GetText(index,strSelect);
	m_list_undstque.DeleteString(index);
	POSITION pos = m_list_UndstQueInfo.FindIndex(index);
	if (pos!=NULL)
	{
		CQueueInfo queinfo=m_list_UndstQueInfo.GetAt(pos);
		m_list_AbleQueInfo.AddTail(queinfo);
		m_list_ableque.AddString(queinfo.GetBussName());
		m_list_UndstQueInfo.RemoveAt(pos);

	}

	/*CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(count);
	m_list_undstque.GetSelItems(count, aryListBoxSel.GetData()); 
	for (int index=0;index<count;index++)
	{
		CString item;
		m_list_undstque.GetText(aryListBoxSel[index],item);
		m_list_ableque.AddString(item);
		
	}
	for (int index=count-1;index>=0;index--)
	{
		m_list_undstque.DeleteString(aryListBoxSel[index]);
	}*/
}

void CWndQueSetDlg::OnBnClickedBtnDelque()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_list_ableque.GetCurSel();
	if (index==CB_ERR)
	{
		return;
	}
	CString strSelect;
	m_list_ableque.GetText(index,strSelect);
	m_list_undstque.AddString(strSelect);
	m_list_ableque.DeleteString(index);
	POSITION pos = m_list_AbleQueInfo.FindIndex(index);
	if (pos!=NULL)
	{
		m_list_UndstQueInfo.AddTail(m_list_AbleQueInfo.GetAt(pos));
		m_list_AbleQueInfo.RemoveAt(pos);

	}
	/*CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(count);
	m_list_ableque.GetSelItems(count, aryListBoxSel.GetData()); 
	for (int index=0;index<count;index++)
	{
		CString item;
		m_list_ableque.GetText(aryListBoxSel[index],item);
		m_list_undstque.AddString(item);

	}
	for (int index=count-1;index>=0;index--)
	{
		m_list_ableque.DeleteString(aryListBoxSel[index]);
	}*/
}

void CWndQueSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int count=m_list_AbleQueInfo.GetCount();
	for (int i=0;i<count;i++)
	{
		POSITION pos = m_list_AbleQueInfo.FindIndex(i);
		if (pos!=NULL)
		{
			CQueueInfo MapQueInfo;
			CString ListQueName;
			MapQueInfo = m_list_AbleQueInfo.GetAt(pos);
			m_list_ableque.GetText(i,ListQueName);
			m_AbleQueId.Add(MapQueInfo.GetQueID());
			m_AbleQueName=m_AbleQueName+_T("  ")+ListQueName;
		}

	}

	if(m_check_usepower.GetCheck() == BST_CHECKED)
	{
		m_bIsUsePower = TRUE;
	}
	else
	{
		m_bIsUsePower = FALSE;
	}
	OnOK();
}

void CWndQueSetDlg::OnBnClickedBtnUp()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem = m_list_ableque.GetCurSel();
	if(nItem <= 0) return;
	CString strText;
	m_list_ableque.GetText(nItem, strText);
	m_list_ableque.DeleteString(nItem);
	m_list_ableque.InsertString(nItem-1, strText);
	m_list_ableque.SetCurSel(nItem-1);

	POSITION pos = m_list_AbleQueInfo.FindIndex(nItem);
	if (pos!=NULL)
	{
		CQueueInfo queinfo = m_list_AbleQueInfo.GetAt(pos);
		m_list_AbleQueInfo.RemoveAt(pos);
		pos = m_list_AbleQueInfo.FindIndex(nItem-1);
		if (pos!=NULL)
		{
			m_list_AbleQueInfo.InsertBefore(pos,queinfo);
		}

	}
}

void CWndQueSetDlg::OnBnClickedBtnDown()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem = m_list_ableque.GetCurSel();
	if(nItem==CB_ERR)return;
	if(nItem >= m_list_ableque.GetCount()-1 || nItem < 0) return;
	CString strText;
	m_list_ableque.GetText(nItem, strText);
	m_list_ableque.DeleteString(nItem);
	m_list_ableque.InsertString(++nItem, strText);
	m_list_ableque.SetCurSel(nItem);

	POSITION pos = m_list_AbleQueInfo.FindIndex(nItem-1);
	if (pos!=NULL)
	{
		CQueueInfo queinfo = m_list_AbleQueInfo.GetAt(pos);
		m_list_AbleQueInfo.RemoveAt(pos);
		POSITION pos = m_list_AbleQueInfo.FindIndex(nItem-1);
		m_list_AbleQueInfo.InsertAfter(pos,queinfo);
	}
}

void CWndQueSetDlg::OnBnClickedCheckMkall()
{
	// TODO: 在此添加控件通知处理程序代码
	if(BST_CHECKED == m_check_makeall.GetCheck())
	{
		m_bMakeAll = TRUE;
	}
	else
	{
		m_bMakeAll = FALSE;
	}
}
