// SLZWindowSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "SLZWindowSetDlg.h"
#include "SLZStaff.h"
#include "WndQueSetDlg.h"
#include "CallSoundSetDlg.h"
#include "CommonStrMethod.h"
#include "ShowMsgSetDlg.h"
#include "CallJttsSetDlg.h"
#include "DataDef.h"

#define SERNUM 0
#define  WNDNUM 1
#define  WNDNAME 2
#define  CALLNAME 3
#define  CALLERNUM 4
#define  EVALNUM 5
#define  WNDSCR 6
#define  COMSCR 7
#define  LOGSTAFF 8
// SLZWindowSetDlg 对话框

IMPLEMENT_DYNAMIC(SLZWindowSetDlg, CDialog)

SLZWindowSetDlg::SLZWindowSetDlg(CWnd* pParent /* =/* = NULL */)
: CDialog(SLZWindowSetDlg::IDD,pParent)
{
	m_staffinfo_path = convert.GetExeFullFilePath();
	m_staffinfo_path += _T("\\StaffBasicInfo\\StaffBasicInfo.dat");
	m_infofile_path=m_WindowFile.GetExeFullFilePath();
	//m_infofile_path+=_T("\\WindowBasicInfo");
	//m_WindowFile.CreateMyDirectory(m_infofile_path);
	m_infofile_path+=_T("\\Window.dat");
	m_maxnumfile_path = m_WindowFile.GetExeFullFilePath();
	m_maxnumfile_path+=_T("\\wndmaxnum.dat");
	m_Queinfo_path = convert.GetExeFullFilePath();
	m_Queinfo_path += _T("\\QueBasicInfo\\QueBasicInfo.dat");
}

SLZWindowSetDlg::~SLZWindowSetDlg()
{
}

void SLZWindowSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_WNDLIST,m_ListCtr_Window);
	DDX_Control(pDX,IDC_COMBO_LOGSTAFF,m_cs_LogStaff);
	DDX_Control(pDX,IDC_STATIC_ABLEQUE,m_cs_AbleQue);
	DDX_Text(pDX,IDC_COMBO_LOGSTAFF,m_LogStaffName);
	DDX_Text(pDX,IDC_EDIT_WNDID,m_cs_WindowId);
	DDX_Text(pDX,IDC_EDIT_WNDNAME,m_cs_WindowName);
	DDX_Text(pDX,IDC_EDIT_WNDCALLNAME,m_cs_WndCallName);
	DDX_Text(pDX,IDC_EDIT_CALLID,m_cs_CallerID);
	DDX_Text(pDX,IDC_EDIT_EVAID,m_cs_EvalId);
	DDX_Text(pDX,IDC_EDIT_WNDSCRID,m_cs_WndScrName);
	DDX_Text(pDX,IDC_EDIT_COMSCRID,m_cs_ComScrName);
	DDX_Text(pDX,IDC_EDIT_EVALTIMEOUT,m_cs_EvalTimeOut);
	DDX_Text(pDX,IDC_EDIT_LEDPHYID,m_cs_LEDPhyId);
	DDX_Text(pDX,IDC_EDIT_LEDIPID,m_cs_LEDIPId);
	DDX_Text(pDX,IDC_EDIT_LEDPIPE,m_cs_LEDPipeId);
}


BEGIN_MESSAGE_MAP(SLZWindowSetDlg, CDialog)
	ON_BN_CLICKED(IDC_ADDWND, &SLZWindowSetDlg::OnBnClickedAddwnd)
	ON_BN_CLICKED(IDC_DELWND, &SLZWindowSetDlg::OnBnClickedDelwnd)
	ON_EN_CHANGE(IDC_EDIT_WNDNAME, &SLZWindowSetDlg::OnEnChangeEditWndname)
	ON_EN_CHANGE(IDC_EDIT_WNDCALLNAME, &SLZWindowSetDlg::OnEnChangeEditWndcallname)
	ON_EN_CHANGE(IDC_EDIT_CALLID, &SLZWindowSetDlg::OnEnChangeEditCallid)
	ON_EN_CHANGE(IDC_EDIT_EVAID, &SLZWindowSetDlg::OnEnChangeEditEvaid)
	ON_EN_CHANGE(IDC_EDIT_WNDSCRID, &SLZWindowSetDlg::OnEnChangeEditWndscrid)
	ON_EN_CHANGE(IDC_EDIT_COMSCRID, &SLZWindowSetDlg::OnEnChangeEditComscrid)
	//ON_BN_CLICKED(IDC_AUTOLOGIN, &SLZWindowSetDlg::OnBnClickedAutologin)
	ON_CBN_SELCHANGE(IDC_COMBO_LOGSTAFF, &SLZWindowSetDlg::OnCbnSelchangeComboLogstaff)
	ON_NOTIFY(NM_CLICK, IDC_WNDLIST, &SLZWindowSetDlg::OnNMClickWndlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_WNDLIST, &SLZWindowSetDlg::OnLvnItemchangedWndlist)
	ON_BN_CLICKED(IDC_BTN_CHOOSEQUE, &SLZWindowSetDlg::OnBnClickedBtnChooseque)
	ON_BN_CLICKED(IDC_BN_WNDOK,&SLZWindowSetDlg::OnBnClickedWndOk)
	ON_BN_CLICKED(IDC_BN_WNDCANCEL,&SLZWindowSetDlg::OnBnClickedWndCancel)
	ON_BN_CLICKED(IDC_BTN_CALLSOUND, &SLZWindowSetDlg::OnBnClickedBtnCallsound)
	ON_BN_CLICKED(IDC_BTN_WAITSOUND, &SLZWindowSetDlg::OnBnClickedBtnWaitsound)
	ON_BN_CLICKED(IDC_BTN_CALLSHOW, &SLZWindowSetDlg::OnBnClickedBtnCallshow)
	ON_BN_CLICKED(IDC_BTN_WAITSHOW, &SLZWindowSetDlg::OnBnClickedBtnWaitshow)
	ON_EN_CHANGE(IDC_EDIT_WNDID, &SLZWindowSetDlg::OnEnChangeEditWndid)
	ON_EN_CHANGE(IDC_EDIT_EVALTIMEOUT, &SLZWindowSetDlg::OnEnChangeEditEvaltimeout)
	ON_EN_CHANGE(IDC_EDIT_LEDPHYID, &SLZWindowSetDlg::OnEnChangeEditLedphyid)
	ON_EN_CHANGE(IDC_EDIT_LEDIPID, &SLZWindowSetDlg::OnEnChangeEditLedipid)
	ON_EN_CHANGE(IDC_EDIT_LEDPIPE, &SLZWindowSetDlg::OnEnChangeEditLedpipe)
	ON_BN_CLICKED(IDC_AUTOLOGIN, &SLZWindowSetDlg::OnBnClickedAutologin)
	ON_BN_CLICKED(IDC_CANCELAUTOLOGIN, &SLZWindowSetDlg::OnBnClickedCancelautologin)
END_MESSAGE_MAP()

extern unsigned int golbal_windowserial_id;
// CSLZWindowSetDlg 消息处理程序

BOOL SLZWindowSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ListCtr_Window.SetBkColor(RGB(255,255,255));
	m_ListCtr_Window.SetRowHeigt(15);
	m_ListCtr_Window.SetFontHW(13, 0);
	m_ListCtr_Window.ModifyStyle(0,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	m_ListCtr_Window.SetExtendedStyle(LVS_EX_GRIDLINES);
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(&rect);
	//rect.right-=2;
	m_ListCtr_Window.InsertColumn(SERNUM,_T("序号"),LVCFMT_CENTER,rect.Width()*6/100,-1);
	m_ListCtr_Window.InsertColumn(WNDNUM,_T("窗口编号"),LVCFMT_CENTER,rect.Width()*9/100,-1);
	m_ListCtr_Window.InsertColumn(WNDNAME,_T("窗口名称"),LVCFMT_CENTER,rect.Width()*10/100,-1);
	m_ListCtr_Window.InsertColumn(CALLNAME,_T("呼叫名称"),LVCFMT_CENTER,rect.Width()*10/100,-1);
	m_ListCtr_Window.InsertColumn(CALLERNUM,_T("呼叫器"),LVCFMT_CENTER,rect.Width()*7/100,-1);
	m_ListCtr_Window.InsertColumn(EVALNUM,_T("评价器"),LVCFMT_CENTER,rect.Width()*7/100,-1);
	m_ListCtr_Window.InsertColumn(WNDSCR,_T("窗口屏"),LVCFMT_CENTER,rect.Width()*7/100,-1);
	m_ListCtr_Window.InsertColumn(COMSCR,_T("综合屏"),LVCFMT_CENTER,rect.Width()*7/100,-1);
	m_ListCtr_Window.InsertColumn(LOGSTAFF,_T("自动登录员工"),LVCFMT_CENTER,rect.Width()*12/100,-1);
	m_cs_LogStaff.AddString(L"无");
	ReadMaxIdFromFile();
	ReadStaffInfoFromFile();
	ReadQueInfoFromFile();
	ReadWindowBasicIntoFile();
	if (m_ListCtr_Window.GetItemCount())
	{
		m_ListCtr_Window.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
	
	/*
	CSetPropertySheet* pSheet =(CSetPropertySheet*) GetParent();
	if(pSheet->m_queDlg.m_hWnd)
	{
	int count = pSheet->m_queDlg.m_list_queinfo.GetCount();
	CString str;
	str.Format(_T("%d"),count);
	AfxMessageBox(str);
	}
	*/
	return TRUE;
}
BOOL SLZWindowSetDlg::ReadStaffInfoFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_staffinfo_path,CFile::modeRead,&e))
	{
		SLZStaff* staffinfo=NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>staffinfo;
				if (staffinfo)
				{
					m_map_staff[m_cs_LogStaff.AddString(staffinfo->GetStaffName())-1]
					= *staffinfo;
					delete staffinfo;
					staffinfo = NULL;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
			return TRUE;
	}
	else return FALSE;
}

int SLZWindowSetDlg::GetNumFromString(CString strNum)
{
	if (strNum==_T(""))
	{
		return 0;
	}
	CString strRealNum;
	int nStartNumIndex = strNum.FindOneOf(_T("0123456789"));
	strRealNum = strNum.Mid(nStartNumIndex);
	char szbuf[32] = {0};
	CCommonConvert::WChar2Char(szbuf, 32, strRealNum.GetBuffer());
	int nMaxID = ::atoi(szbuf);
	return nMaxID;
}

void SLZWindowSetDlg::OnBnClickedAddwnd()
{
	// TODO: 在此添加控件通知处理程序代码
	golbal_windowserial_id++;
	SLZWindow windowinfo;
	int count=m_ListCtr_Window.GetItemCount()+1;
	int iMaxID = GetNumFromString(m_ListCtr_Window.GetItemText(0,2));
	int itemp=0;
	for (int index = 1;index<count-1;index++)
	{
		CString strMaxID  = m_ListCtr_Window.GetItemText(index,2);
		int nStartNumIndex = GetNumFromString(strMaxID);
		if (nStartNumIndex>iMaxID)
		{
			iMaxID = nStartNumIndex;
			itemp = index;
		}
	}
	int windownum = m_List_WindowInfo.GetCount();
	CString WindowSerNum;
	convert.intToCString(count,WindowSerNum);
	CString n_WindowName;
	n_WindowName.Format(_T("%d号窗口"), iMaxID+1);
	/*CString windowid;
	convert.intToCString(golbal_windowserial_id,windowid);*/
	windowinfo.SetWindowId(golbal_windowserial_id);
	windowinfo.SetWindowName(n_WindowName);
	windowinfo.SetWindowCallName(n_WindowName);
	windowinfo.SetCallerId(iMaxID+1);
	windowinfo.SetEvaluatorId(iMaxID+1);
	windowinfo.SetWndScreenId(iMaxID+1);
	windowinfo.SetComScreenId(iMaxID+1);
	windowinfo.SetShowWndId(iMaxID+1);
	CString strNewNum;
	CCommonConvert::intToCString(iMaxID+1,strNewNum);
	m_List_WindowInfo.AddTail(windowinfo);
	m_ListCtr_Window.InsertItem(count-1,WindowSerNum);
	m_ListCtr_Window.SetItemText(count-1,WNDNUM,strNewNum);
	m_ListCtr_Window.SetItemText(count-1,WNDNAME,n_WindowName);
	m_ListCtr_Window.SetItemText(count-1,CALLNAME,n_WindowName);
	m_ListCtr_Window.SetItemText(count-1,CALLERNUM,strNewNum);
	m_ListCtr_Window.SetItemText(count-1,EVALNUM,strNewNum);
	m_ListCtr_Window.SetItemText(count-1,WNDSCR,strNewNum);
	m_ListCtr_Window.SetItemText(count-1,COMSCR,strNewNum);
	//m_ListCtr_Window.SetItemText(count-1,8,_T("否"));
	m_ListCtr_Window.SetItemText(count-1,LOGSTAFF,_T("无"));
	m_ListCtr_Window.SetItemState(count-1, LVIS_SELECTED, LVIS_SELECTED);
	
	//	SetModified();
}


void SLZWindowSetDlg::OnBnClickedDelwnd()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	CString WindowSerNum;
	m_ListCtr_Window.DeleteItem(index);
	int count=m_ListCtr_Window.GetItemCount();
	for(int i=index;i<=count;i++)
	{
		convert.intToCString(i+1,WindowSerNum);
		m_ListCtr_Window.SetItemText(i,SERNUM,WindowSerNum);
	}
	m_List_WindowInfo.RemoveAt(m_List_WindowInfo.FindIndex(index));
	//删到顶端时把焦点设置在最上边
	if(index>0)
	{
		m_ListCtr_Window.SetItemState(index-1,LVIS_SELECTED,LVIS_SELECTED);
	}
	else
	{
		m_ListCtr_Window.SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);
	}
	//	SetModified();
}

void SLZWindowSetDlg::OnEnChangeEditWndid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	int WindowId;
		BOOL btemp=TRUE;
		if (m_cs_WindowId=="")
		{
			m_cs_WindowId = _T("0");
		}
		convert.CStringToint(WindowId,m_cs_WindowId);
		if (btemp)
		{
			windowinfo.SetShowWndId(WindowId);
			m_ListCtr_Window.SetItemText(index,WNDNUM,m_cs_WindowId);
			m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
			m_ListCtr_Window.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
		}
}

void SLZWindowSetDlg::OnEnChangeEditWndname()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	windowinfo.SetWindowName(m_cs_WindowName);
	
	m_ListCtr_Window.SetItemText(index,WNDNAME,m_cs_WindowName);
	m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
	m_ListCtr_Window.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
	//	SetModified();
}


void SLZWindowSetDlg::OnEnChangeEditWndcallname()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	windowinfo.SetWindowCallName(m_cs_WndCallName);
	
	m_ListCtr_Window.SetItemText(index,CALLNAME,m_cs_WndCallName);
	m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
	m_ListCtr_Window.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);

	//	SetModified();
}

void SLZWindowSetDlg::OnEnChangeEditCallid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	int callerid;
	

		BOOL btemp=TRUE;
		if (m_cs_CallerID=="")
		{
			m_cs_CallerID = _T("0");  
		}
		convert.CStringToint(callerid,m_cs_CallerID);
		
		if (btemp)
		{
			m_ListCtr_Window.SetItemText(index,CALLERNUM,m_cs_CallerID);
			windowinfo.SetCallerId(callerid);
			m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
			m_ListCtr_Window.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);

		}
		

	
	//	SetModified();
}

void SLZWindowSetDlg::OnEnChangeEditEvaid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	int evalid;

		if (m_cs_EvalId=="")
		{
			m_cs_EvalId = _T("0");
		}
		BOOL btemp=TRUE;
		convert.CStringToint(evalid,m_cs_EvalId);
		
		if (btemp)
		{
			windowinfo.SetEvaluatorId(evalid);
			m_ListCtr_Window.SetItemText(index,EVALNUM,m_cs_EvalId);
			m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
			m_ListCtr_Window.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
		}
	
	
	//	SetModified();
}

void SLZWindowSetDlg::OnEnChangeEditWndscrid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	int wndscrid;

		if (m_cs_WndScrName=="")
		{
			m_cs_WndScrName = _T("0");
		}
		BOOL btemp = TRUE;
		convert.CStringToint(wndscrid,m_cs_WndScrName);
		
		if (btemp)
		{
		windowinfo.SetWndScreenId(wndscrid);
		
		m_ListCtr_Window.SetItemText(index,WNDSCR,m_cs_WndScrName);
		
		m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
		m_ListCtr_Window.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
		}
	
	
	//	SetModified();
}

void SLZWindowSetDlg::OnEnChangeEditComscrid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	int comscrid;

		if (m_cs_ComScrName=="")
		{
			m_cs_ComScrName = _T("0");
		}
		BOOL btemp = TRUE;
		convert.CStringToint(comscrid,m_cs_ComScrName);
		
		if (btemp)
		{
		windowinfo.SetComScreenId(comscrid);
		m_ListCtr_Window.SetItemText(index,COMSCR,m_cs_ComScrName);
		m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
		m_ListCtr_Window.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
		}

	//	SetModified();
}

//void SLZWindowSetDlg::OnBnClickedAutologin()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData();
//	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
//	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
//	POSITION posFind = m_List_WindowInfo.FindIndex(index);
//	if(posFind==NULL)return;
//	SLZWindow windowinfo=m_List_WindowInfo.GetAt(posFind);
//	if (index<0)
//	{
//		AfxMessageBox(_T("请选择一个窗口"));
//		return;
//	}
//
//	if(m_ListCtr_Window.GetItemText(index,8)==_T("否"))
//	{
//		m_ListCtr_Window.SetItemText(index,8,_T("是"));
//		GetDlgItem(IDC_COMBO_LOGSTAFF)->EnableWindow(TRUE);
//		if(!m_cs_LogStaff.GetCount())
//			ReadStaffInfoFromFile();
//	}
//	else
//	{
//		m_ListCtr_Window.SetItemText(index,8,_T("否"));
//		GetDlgItem(IDC_COMBO_LOGSTAFF)->EnableWindow(FALSE);
//		m_ListCtr_Window.SetItemText(index,9,_T("无"));
//		CString str;
//		windowinfo.SetLoginStaff(str);
//		m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
//		m_cs_LogStaff.ResetContent();
//	}
//	//	SetModified();
//}

void SLZWindowSetDlg::OnCbnSelchangeComboLogstaff()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	SLZStaff staffinfo;
	int CurSel = m_cs_LogStaff.GetCurSel();
	if (CurSel==0)
	{
		windowinfo.SetLoginStaff(L"");
	}
	else
	{
		m_map_staff.Lookup(CurSel-1,staffinfo);
		windowinfo.SetLoginStaff(staffinfo.GetStaffId());
	}
	//windowinfo.SetDefStaffName(m_LogStaffName);
	
	m_ListCtr_Window.SetItemText(index,LOGSTAFF,m_LogStaffName);
	m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
	m_ListCtr_Window.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
	//	SetModified();
}

void SLZWindowSetDlg::OnBnClickedBtnChooseque()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	CWndQueSetDlg WndQueSetDlg(this);
	if(IDOK == WndQueSetDlg.DoModal())
	{
		//		SetModified();
		if (WndQueSetDlg.m_bMakeAll)
		{
			for (int i =0;i<m_List_WindowInfo.GetCount();i++)
			{
				POSITION wpos = m_List_WindowInfo.FindIndex(i);
				if (wpos)
				{
					SLZWindow window = m_List_WindowInfo.GetAt(wpos);
					window.SetArrayQueId(WndQueSetDlg.m_AbleQueId);
					m_List_WindowInfo.GetAt(wpos) = window;
				}
			}
		}
		else
		{
			windowinfo.SetArrayQueId(WndQueSetDlg.m_AbleQueId);
			m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
		}
		//windowinfo.SetAbleQueName(WndQueSetDlg.AbleQueName);
		m_cs_AbleQue.SetWindowText(WndQueSetDlg.m_AbleQueName);
	}
	
}


void SLZWindowSetDlg::OnNMClickWndlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int index=m_ListCtr_Window.GetItemCount();
	SLZStaff staffinfo;
	for (int i=0;i<index;i++)
	{
		if (m_ListCtr_Window.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			SLZWindow window=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(i));
			m_cs_WindowName=window.GetWindowName();
			CString strcallname = window.GetWindowCallName();
			m_cs_WndCallName= strcallname;
			m_cs_LEDIPId=window.GetLEDIPId();

			//CString queid=window.GetStrAbleQueId();
			CStringArray questr;
			window.GetArrayQueId(questr);
			//QueIdStringToArray(queid,questr);
			CString quename=ArrayToQueIdString(questr);
			m_cs_AbleQue.SetWindowText(quename);
			int windowid=window.GetShowWndId();
			int callerid=window.GetCallerId();
			int evaluatorid=window.GetEvaluatorId();
			int wndscrid=window.GetWndScreenId();
			int comscrid=window.GetComScreenId();
			int evaltimeout=window.GetEvaTimeOut();
			int msgshowtime=window.GetMsgShowTime();
			int ledphyid=window.GetLEDPhyId();
			int ledpipeid = window.GetLEDPipeId();
			convert.intToCString(windowid,m_cs_WindowId);
			convert.intToCString(callerid,m_cs_CallerID);
			convert.intToCString(evaluatorid,m_cs_EvalId);
			convert.intToCString(wndscrid,m_cs_WndScrName);
			convert.intToCString(comscrid,m_cs_ComScrName);
			convert.intToCString(evaltimeout,m_cs_EvalTimeOut);
			convert.intToCString(ledphyid,m_cs_LEDPhyId);
			convert.intToCString(ledpipeid,m_cs_LEDPipeId);
			//CButton* pBtn = (CButton*)GetDlgItem(IDC_AUTOLOGIN);
			CString staffId=window.GetLoginStaff();
			if (!staffId.IsEmpty())
			{
				CString staffName=GetStaffNameFromMap(staffId);
				m_cs_LogStaff.SelectString(-1,staffName);
			}
			else 
			{
				/*pBtn->SetCheck(0);*/
				m_cs_LogStaff.SelectString(-1,L"无");
				//m_ListCtr_Window.SetItemText(i,8,_T("否"));
				//GetDlgItem(IDC_COMBO_LOGSTAFF)->EnableWindow(FALSE);
			}
			UpdateData(FALSE);
		}
	}
	*pResult = 0;
}

void SLZWindowSetDlg::OnLvnItemchangedWndlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i<m_ListCtr_Window.GetItemCount();i++)
	{
		if (m_ListCtr_Window.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			SLZWindow window=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(i));
			m_cs_WindowName=window.GetWindowName();
			CString strcallname = window.GetWindowCallName();
			m_cs_WndCallName= strcallname;
			m_cs_LEDIPId=window.GetLEDIPId();

			//m_cs_AbleQue.SetWindowText(window.GetAbleQueName());
			CStringArray questr;
			window.GetArrayQueId(questr);
			//QueIdStringToArray(queid,questr);
			CString quename=ArrayToQueIdString(questr);
			m_cs_AbleQue.SetWindowText(quename);
			int windowid=window.GetShowWndId();
			int callerid=window.GetCallerId();
			int evaluatorid=window.GetEvaluatorId();
			int wndscrid=window.GetWndScreenId();
			int comscrid=window.GetComScreenId();
			int evaltimeout=window.GetEvaTimeOut();
			int msgshowtime=window.GetMsgShowTime();
			int ledphyid=window.GetLEDPhyId();
			int ledpipeid = window.GetLEDPipeId();
			convert.intToCString(windowid,m_cs_WindowId);
			convert.intToCString(callerid,m_cs_CallerID);
			convert.intToCString(evaluatorid,m_cs_EvalId);
			convert.intToCString(wndscrid,m_cs_WndScrName);
			convert.intToCString(comscrid,m_cs_ComScrName);
			convert.intToCString(evaltimeout,m_cs_EvalTimeOut);
			convert.intToCString(ledphyid,m_cs_LEDPhyId);
			convert.intToCString(ledpipeid,m_cs_LEDPipeId);
			//CButton* pBtn = (CButton*)GetDlgItem(IDC_AUTOLOGIN);
			CString staffId=window.GetLoginStaff();
			if (!staffId.IsEmpty())
			{
				/*pBtn->SetCheck(1);*/
				//if(staffName==_T("")) m_cs_LogStaff.SetCurSel(-1);
				CString staffName=GetStaffNameFromMap(staffId);
				m_cs_LogStaff.SelectString(-1,staffName);
				//GetDlgItem(IDC_COMBO_LOGSTAFF)->EnableWindow(TRUE);
				if(!m_cs_LogStaff.GetCount())
					ReadStaffInfoFromFile();
			}
			else 
			{
				/*pBtn->SetCheck(0);*/
				m_cs_LogStaff.SelectString(-1,L"无");
				//m_ListCtr_Window.SetItemText(i,8,_T("否"));
				m_ListCtr_Window.SetItemText(i,LOGSTAFF,_T("无"));
				//GetDlgItem(IDC_COMBO_LOGSTAFF)->EnableWindow(FALSE);
			}

			UpdateData(FALSE);
		}
	}
	*pResult = 0;
}

// BOOL SLZWindowSetDlg::OnApply()
// {
// 	WriteWindowBasicIntoFile();
// 	return CPropertyPage::OnApply();
// }

extern void MyWriteConsole(CString str);

BOOL SLZWindowSetDlg::WriteWindowBasicIntoFile()
{
#ifdef _DEBUG
	m_ComputerTime.SetStartTime(clock());
#endif
	CFile file;
	CFileException e;
	if (file.Open(m_infofile_path,CFile::modeCreate|CFile::modeWrite,&e))
	{
		int count=m_List_WindowInfo.GetCount();
		for (int i=0;i<count;i++)
		{
			CArchive ar(&file,CArchive::store);
			SLZWindow windowinfo;
			windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(i));
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

BOOL SLZWindowSetDlg::ReadWindowBasicIntoFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_infofile_path,CFile::modeRead,&e))
	{
		SLZWindow* windowinfo=NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>windowinfo;
				if (windowinfo)
				{
					m_List_WindowInfo.AddTail(*windowinfo);
					int windowid=windowinfo->GetShowWndId();
					int windowserid = windowinfo->GetWindowId();
					/*m_ReadMaxSerialID=m_ReadMaxSerialID>windowserid?
m_ReadMaxSerialID:windowserid;*/
					int count=m_ListCtr_Window.GetItemCount()+1;
					CString WindowSerNum;
					convert.intToCString(count,WindowSerNum);
					m_ListCtr_Window.InsertItem(count-1,WindowSerNum);
					m_ListCtr_Window.SetItemText(count-1,WNDNAME,windowinfo->GetWindowName());
					m_ListCtr_Window.SetItemText(count-1,CALLNAME,windowinfo->GetWindowCallName());
					//int windowid=windowinfo->GetWindowId();
					int callerid=windowinfo->GetCallerId();
					int evaluatorid=windowinfo->GetEvaluatorId();
					int wndscrid=windowinfo->GetWndScreenId();
					int comscrid=windowinfo->GetComScreenId();
					int evaltimeout=windowinfo->GetEvaTimeOut();
					int msgshowtime=windowinfo->GetMsgShowTime();
					int ledphyid=windowinfo->GetLEDPhyId();
					CString strwindowid;
					CString strcallerid;
					CString	strevalid;
					CString strwndscrname;
					CString strcomscrname;
					convert.intToCString(windowid,strwindowid);
					convert.intToCString(callerid,strcallerid);
					convert.intToCString(evaluatorid,strevalid);
					convert.intToCString(wndscrid,strwndscrname);
					convert.intToCString(comscrid,strcomscrname);
					convert.intToCString(evaltimeout,m_cs_EvalTimeOut);
					convert.intToCString(ledphyid,m_cs_LEDPhyId);
					m_ListCtr_Window.SetItemText(count-1,WNDNUM,strwindowid);
					m_ListCtr_Window.SetItemText(count-1,CALLERNUM,strcallerid);
					m_ListCtr_Window.SetItemText(count-1,EVALNUM,strevalid);
					m_ListCtr_Window.SetItemText(count-1,WNDSCR,strwndscrname);
					m_ListCtr_Window.SetItemText(count-1,COMSCR,strcomscrname);

					if (!windowinfo->GetLoginStaff().IsEmpty())
					{

						//m_ListCtr_Window.SetItemText(count-1,8,_T("是"));
						CString staffname=GetStaffNameFromMap(windowinfo->GetLoginStaff());
						m_ListCtr_Window.SetItemText(count-1,LOGSTAFF,staffname);
					}
					else 
					{
						//m_ListCtr_Window.SetItemText(count-1,8,_T("否"));
						m_ListCtr_Window.SetItemText(count-1,LOGSTAFF,_T("无"));
					}
					
					delete windowinfo;
					windowinfo = NULL;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
			/*golbal_windowserial_id=m_ReadMaxSerialID;*/
	}
	else return FALSE;
	return TRUE;

}

BOOL SLZWindowSetDlg::RefrushWindowInfo()
{
	return ReadWindowBasicIntoFile();
}

void SLZWindowSetDlg::OnBnClickedWndCancel()
{
	return CDialog::OnCancel();
}

void SLZWindowSetDlg::OnBnClickedWndOk()
{
	WriteMaxIdToFile();
	WriteWindowBasicIntoFile();
	return CDialog::OnOK();
}
void SLZWindowSetDlg::OnBnClickedBtnCallsound()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	CString callmsg=windowinfo.GetCallMsg();
	if (theApp.m_logicVariables.IsUseJtts)
	{
		CCallSoundSetDlg CallSoundSetDlg(callmsg,this);
		if(IDOK == CallSoundSetDlg.DoModal())
		{
			if (CallSoundSetDlg.m_bMksndall)
			{
				for (int i =0;i<m_List_WindowInfo.GetCount();i++)
				{
					POSITION wpos = m_List_WindowInfo.FindIndex(i);
					if (wpos)
					{
						SLZWindow window = m_List_WindowInfo.GetAt(wpos);
						window.SetCallMsg(callmsg);
						m_List_WindowInfo.SetAt(wpos,window);
					}
				}
			}
			else
			{
				windowinfo.SetCallMsg(callmsg);
				m_List_WindowInfo.SetAt(m_List_WindowInfo.FindIndex(index),windowinfo);
			}
		}
	}
	else
	{
		CCallJttsSetDlg CallJttsSetDlg(callmsg,this);
		if(IDOK == CallJttsSetDlg.DoModal())
		{
			if (CallJttsSetDlg.m_bMkJttsAll)
			{
				for (int i =0;i<m_List_WindowInfo.GetCount();i++)
				{
					POSITION wpos = m_List_WindowInfo.FindIndex(i);
					if (wpos)
					{
						SLZWindow window = m_List_WindowInfo.GetAt(wpos);
						window.SetCallMsg(callmsg);
						m_List_WindowInfo.SetAt(wpos,window);
					}
				}
			}
			else
			{
				windowinfo.SetCallMsg(callmsg);
				m_List_WindowInfo.SetAt(m_List_WindowInfo.FindIndex(index),windowinfo);
			}
		}	
	}
	
}

void SLZWindowSetDlg::OnBnClickedBtnWaitsound()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	CString waitcallmsg=windowinfo.GetWaitCallMsg();
	if (theApp.m_logicVariables.IsUseJtts)
	{
		CCallSoundSetDlg CallSoundSetDlg(waitcallmsg,this);
		if(IDOK == CallSoundSetDlg.DoModal())
		{
			if (CallSoundSetDlg.m_bMksndall)
			{
				for (int i =0;i<m_List_WindowInfo.GetCount();i++)
				{
					POSITION wpos = m_List_WindowInfo.FindIndex(i);
					if (wpos)
					{
						SLZWindow window = m_List_WindowInfo.GetAt(wpos);
						window.SetWaitCallMsg(waitcallmsg);
						m_List_WindowInfo.SetAt(wpos,window);
					}
				}
			}
			else
			{
				windowinfo.SetWaitCallMsg(waitcallmsg);
				m_List_WindowInfo.SetAt(m_List_WindowInfo.FindIndex(index),windowinfo);
			}
		}
	}
	else
	{
		CCallJttsSetDlg CallJttsSetDlg(waitcallmsg,this);
		if(IDOK == CallJttsSetDlg.DoModal())
		{
			if (CallJttsSetDlg.m_bMkJttsAll)
			{
				for (int i =0;i<m_List_WindowInfo.GetCount();i++)
				{
					POSITION wpos = m_List_WindowInfo.FindIndex(i);
					if (wpos)
					{
						SLZWindow window = m_List_WindowInfo.GetAt(wpos);
						window.SetWaitCallMsg(waitcallmsg);
						m_List_WindowInfo.SetAt(wpos,window);
					}
				}
			}
			else
			{
				windowinfo.SetWaitCallMsg(waitcallmsg);
				m_List_WindowInfo.SetAt(m_List_WindowInfo.FindIndex(index),windowinfo);
			}
		}	
	}
}


void SLZWindowSetDlg::OnBnClickedBtnCallshow()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	pShowInfo showinfo = new ShowInfo();
	showinfo->strShowMsg = windowinfo.GetShowMsg();
	showinfo->iShowTime = windowinfo.GetMsgShowTime();
	showinfo->strAdMsg = windowinfo.GetAdMsg();
	CShowMsgSetDlg ShowMsgSetDlg(showinfo,this);
	if (IDOK==ShowMsgSetDlg.DoModal())
	{
		if (ShowMsgSetDlg.m_bMkShowAll)
		{
			for (int i =0;i<m_List_WindowInfo.GetCount();i++)
			{
				POSITION wpos = m_List_WindowInfo.FindIndex(i);
				if (wpos)
				{
					SLZWindow window = m_List_WindowInfo.GetAt(wpos);
					window.SetShowMsg(ShowMsgSetDlg.m_strConf);
					window.SetAdMsg(ShowMsgSetDlg.m_strAdMsg);
					window.SetMsgShowTime(ShowMsgSetDlg.m_iShowTime);
					m_List_WindowInfo.SetAt(wpos,window);
				}
			}
		}
		else
		{
			windowinfo.SetShowMsg(ShowMsgSetDlg.m_strConf);
			windowinfo.SetAdMsg(ShowMsgSetDlg.m_strAdMsg);
			windowinfo.SetMsgShowTime(ShowMsgSetDlg.m_iShowTime);
			m_List_WindowInfo.SetAt(m_List_WindowInfo.FindIndex(index),windowinfo);
		}
	}
	delete showinfo;
}


void SLZWindowSetDlg::OnBnClickedBtnWaitshow()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	pShowInfo showinfo=new ShowInfo();
	showinfo->strShowMsg = windowinfo.GetWaitShowMsg();
	showinfo->iShowTime = windowinfo.GetMsgShowTime();
	showinfo->strAdMsg = windowinfo.GetAdMsg();
	CShowMsgSetDlg ShowMsgSetDlg(showinfo,this);
	if (IDOK==ShowMsgSetDlg.DoModal())
	{
		if (ShowMsgSetDlg.m_bMkShowAll)
		{
			for (int i =0;i<m_List_WindowInfo.GetCount();i++)
			{
				POSITION wpos = m_List_WindowInfo.FindIndex(i);
				if (wpos)
				{
					SLZWindow window = m_List_WindowInfo.GetAt(wpos);
					window.SetWaitShowMsg(ShowMsgSetDlg.m_strConf);
					window.SetAdMsg(ShowMsgSetDlg.m_strAdMsg);
					window.SetMsgShowTime(ShowMsgSetDlg.m_iShowTime);
					m_List_WindowInfo.SetAt(wpos,window);
				}
			}
		}
		else
		{
			windowinfo.SetWaitShowMsg(ShowMsgSetDlg.m_strConf);
			windowinfo.SetAdMsg(ShowMsgSetDlg.m_strAdMsg);
			windowinfo.SetMsgShowTime(ShowMsgSetDlg.m_iShowTime);
			m_List_WindowInfo.SetAt(m_List_WindowInfo.FindIndex(index),windowinfo);
		}
	}
	delete showinfo;
}



CString SLZWindowSetDlg::GetStaffNameFromMap(CString StaffId)
{
	SLZStaff staffinfo;
	for (int len=0;len<m_map_staff.GetSize();len++)
	{
		m_map_staff.Lookup(len,staffinfo);
		if (staffinfo.GetStaffId()==StaffId)
		{
			return staffinfo.GetStaffName();
			break;
		}
		else continue;
	}
	return _T("");
}

CString SLZWindowSetDlg::GetQueNameFromMap(CString QueId)
{
	CQueueInfo queinfo;
	for (int len=0;len<m_map_UndstQueInfo.GetSize();len++)
	{
		m_map_UndstQueInfo.Lookup(len,queinfo);
		if (queinfo.GetQueID()==QueId)
		{
			return queinfo.GetBussName();
			break;
		}
		else continue;
	}
	return _T("");
}
int SLZWindowSetDlg::QueIdStringToArray(CString strSrc,CArray<CString,CString&>& arrStr)
{
	int nIndex=-1;
	for (;strSrc.GetLength()!=0;)
	{
		CString strTempLeft;
		nIndex=strSrc.Find(_T("|"));
		if (nIndex!=-1)
		{
			strTempLeft=strSrc.Left(nIndex);
			if (!strTempLeft.IsEmpty())
			{
				arrStr.Add(strTempLeft);
			}
			strSrc=strSrc.Mid(nIndex+1);
		}
		else
		{
			arrStr.Add(strSrc);
			break;
		}
	}
	return 0;
}

CString SLZWindowSetDlg::ArrayToQueIdString(CStringArray& arrStr)
{
	CString QueNameString;
	
	int size=arrStr.GetSize();
	for (int i=0;i<size;i++)
	{	
		CString quename;
		quename=GetQueNameFromMap(arrStr[i]);
		QueNameString+=quename+_T(" ");
	}
	return QueNameString;
}

BOOL SLZWindowSetDlg::ReadQueInfoFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_Queinfo_path,CFile::modeRead,&e))
	{
		CQueueInfo* queinfo=NULL;
		int i=0;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>queinfo;
				if (queinfo)
				{
					m_map_UndstQueInfo[i]= *queinfo;
					i++;
				}
				delete queinfo;
				queinfo = NULL;
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();

	}
	else return FALSE;
	return TRUE;
}

void SLZWindowSetDlg::OnEnChangeEditEvaltimeout()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));

		if (m_cs_EvalTimeOut=="")
		{
			m_cs_EvalTimeOut = _T("0");
		}
		int iEvalTimeOut;
		convert.CStringToint(iEvalTimeOut,m_cs_EvalTimeOut);
		windowinfo.SetEvaTimeOut(iEvalTimeOut);
		m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
		m_ListCtr_Window.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
}

void SLZWindowSetDlg::OnEnChangeEditLedphyid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));

		int iledphyid;
		convert.CStringToint(iledphyid,m_cs_LEDPhyId);
		
		windowinfo.SetLEDPhyId(iledphyid);
		m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
		m_ListCtr_Window.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
}

void SLZWindowSetDlg::OnEnChangeEditLedpipe()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	int iLEDPipeid;
	convert.CStringToint(iLEDPipeid,m_cs_LEDPipeId);
	windowinfo.SetLEDPipeId(iLEDPipeid);
		m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
	m_ListCtr_Window.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
}


void SLZWindowSetDlg::OnEnChangeEditLedipid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Window.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Window.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一个窗口"));
		return;
	}
	BOOL btemp= TRUE;
	SLZWindow windowinfo=m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index));
	
	if (btemp)
	{
	windowinfo.SetLEDIPId(m_cs_LEDIPId);
	m_List_WindowInfo.GetAt(m_List_WindowInfo.FindIndex(index))=windowinfo;
	m_ListCtr_Window.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);

	}
}

BOOL SLZWindowSetDlg::WriteMaxIdToFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_maxnumfile_path,CFile::modeCreate|CFile::modeWrite,&e))
	{
		UINT iMaxId = golbal_windowserial_id;
		file.Write(&iMaxId,sizeof(UINT));
		//		file.Flush();
		file.Close();
	}
	else 
	{
		TRACE(_T("File could not be opened %d\n"), e.m_cause);
		return FALSE;
	}
	return TRUE;
}

BOOL SLZWindowSetDlg::ReadMaxIdFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_maxnumfile_path,CFile::modeRead,&e))
	{
		UINT iMaxId;
		file.Read(&iMaxId,sizeof(UINT));
		golbal_windowserial_id = iMaxId;
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void SLZWindowSetDlg::OnBnClickedAutologin()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i<m_List_WindowInfo.GetCount();i++)
	{
		POSITION pos = m_List_WindowInfo.FindIndex(i);
		if (pos!=NULL)
		{
			SLZWindow window = m_List_WindowInfo.GetAt(pos);
			SLZStaff staffinfo;
			if (i<m_map_staff.GetCount())
			{
				if (m_map_staff.Lookup(i,staffinfo))
				{
					window.SetLoginStaff(staffinfo.GetStaffId());
					m_ListCtr_Window.SetItemText(i,LOGSTAFF,staffinfo.GetStaffName());
					m_List_WindowInfo.SetAt(pos,window);
				}	
			}
			else
			{
				window.SetLoginStaff(L"");
				m_List_WindowInfo.SetAt(pos,window);
			}
		}
	}	

	int index=m_ListCtr_Window.GetItemCount()-1;
	m_ListCtr_Window.SetItemState(index, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);   //选中行
	m_ListCtr_Window.SetSelectionMark(index);
}

void SLZWindowSetDlg::OnBnClickedCancelautologin()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i<m_List_WindowInfo.GetCount();i++)
	{
		POSITION pos = m_List_WindowInfo.FindIndex(i);
		if (pos!=NULL)
		{
			SLZWindow window = m_List_WindowInfo.GetAt(pos);
			window.SetLoginStaff(L"");
			m_ListCtr_Window.SetItemText(i,LOGSTAFF,L"无");
			m_List_WindowInfo.SetAt(pos,window);
		}
	}	
	int index=m_ListCtr_Window.GetItemCount()-1;
	m_ListCtr_Window.SetItemState(index, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);   //选中行
	m_ListCtr_Window.SetSelectionMark(index);
}
