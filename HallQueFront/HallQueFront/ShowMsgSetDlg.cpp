// ShowMsgSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "ShowMsgSetDlg.h"
#include "ListBoxPlus.h"
#include "CommonStrMethod.h"
// CShowMsgSetDlg 对话框

IMPLEMENT_DYNAMIC(CShowMsgSetDlg, CDialog)

CShowMsgSetDlg::CShowMsgSetDlg(pShowInfo showinfo,CWnd* pParent /*=NULL*/)
	: CDialog(CShowMsgSetDlg::IDD, pParent)
	, m_strConf(showinfo->strShowMsg)
	,m_strAdMsg(showinfo->strAdMsg)
	,m_iShowTime(showinfo->iShowTime)
{
	m_bMkShowAll = FALSE;
}

CShowMsgSetDlg::~CShowMsgSetDlg()
{
}

void CShowMsgSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_TEXT_ADD,m_editTextAdd);
	DDX_Control(pDX,IDC_LIST_SHOWMSG,m_mylbList);
	DDX_Control(pDX,IDC_COMBO_SHOWCOLOR,m_combColor);
	DDX_Control(pDX,IDC_CHECK_MKSHALL,m_check_mkshowall);
	DDX_Control(pDX,IDC_RAD_SHOWCHN,m_Sel_Chinese);
	DDX_Control(pDX,IDC_RAD_SHOWENG,m_Sel_English);
	DDX_Text(pDX,IDC_EDIT_SHOWTIME,m_iShowTime);
	DDX_Text(pDX,IDC_EDIT_ADMSG,m_ed_strAdmsg);
}

BEGIN_MESSAGE_MAP(CShowMsgSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADDTEXT, &CShowMsgSetDlg::OnBnClickedBtnAddtext)
	ON_LBN_SELCHANGE(IDC_LIST_SHOWMSG, &CShowMsgSetDlg::OnLbnSelchangeListShowmsg)
	ON_BN_CLICKED(IDC_BTN_CHANGEMSG, &CShowMsgSetDlg::OnBnClickedBtnChangemsg)
	ON_BN_CLICKED(IDC_BTN_DELETEMSG, &CShowMsgSetDlg::OnBnClickedBtnDeletemsg)
	ON_BN_CLICKED(IDC_BTN_QUENUM, &CShowMsgSetDlg::OnBnClickedBtnQuenum)
	ON_BN_CLICKED(IDC_BTN_STAFFNAME, &CShowMsgSetDlg::OnBnClickedBtnStaffname)
	ON_BN_CLICKED(IDC_BTN_WNDNAME, &CShowMsgSetDlg::OnBnClickedBtnWndname)
	ON_CBN_SELCHANGE(IDC_COMBO_SHOWCOLOR, &CShowMsgSetDlg::OnCbnSelchangeComboShowcolor)
	ON_BN_CLICKED(IDOK, &CShowMsgSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_MOVEUP, &CShowMsgSetDlg::OnBnClickedBtnMoveup)
	ON_BN_CLICKED(IDC_BTN_MOVEDOWN, &CShowMsgSetDlg::OnBnClickedBtnMovedown)
	//ON_LBN_DBLCLK(IDC_LIST_SHOWMSG, &CShowMsgSetDlg::OnLbnDblclkListShowmsg)
	ON_BN_CLICKED(IDC_CHECK_MKSHALL, &CShowMsgSetDlg::OnBnClickedCheckMkshall)
	ON_BN_CLICKED(IDC_BTN_BUSNAME, &CShowMsgSetDlg::OnBnClickedBtnBusname)
END_MESSAGE_MAP()

int  CShowMsgSetDlg::SplitColorStringToArray(CString strSrc, CArray<CString>& arrStr)
{
	int nIndex = -1;
	for(; !strSrc.IsEmpty();)
	{
		CString strTempLeft, strTempMid;
		nIndex = strSrc.Find(_T('#'));
		if(nIndex != -1)
		{
			strTempLeft = strSrc.Left(nIndex);
			if(!strTempLeft.IsEmpty())
			{
				arrStr.Add(strTempLeft);
				strSrc = strSrc.Mid(nIndex);
				continue;
			}
			strTempMid = strSrc.Left(nIndex+1);
			strSrc = strSrc.Mid(nIndex+1);
			nIndex = strSrc.Find(_T('#'));
			if(nIndex != -1)
			{
				strTempMid += strSrc.Left(nIndex+1);
				arrStr.Add(strTempMid);
				strSrc = strSrc.Mid(nIndex+1);
			}
		}
		else
		{
			//arrStr.Add(_T("#0#"));
			arrStr.Add(strSrc);
			break;
		}
	}
	return 0;
}

BOOL CShowMsgSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_check_mkshowall.SetCheck(BST_UNCHECKED);
	m_Sel_Chinese.SetCheck(BST_CHECKED);
	int nItem = m_combColor.AddString(_T("红"));
	m_combColor.SetItemData(nItem, COLORREF_RED);
	nItem = m_combColor.AddString(_T("绿"));
	m_combColor.SetItemData(nItem, COLORREF_GREEN);
	nItem = m_combColor.AddString(_T("黄"));
	m_combColor.SetItemData(nItem, COLORREF_YELLOW);
	m_ed_strShowTime = CommonStrMethod::Int2Str(m_iShowTime);
	m_ed_strAdmsg = m_strAdMsg;
	UpdateData(FALSE);
	CArray<CString> arr;
	SplitColorStringToArray(m_strConf, arr);

// 	/*m_mylbList.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL |  
// 		LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS, NULL, this, IDC_LIST_SHOWMSG);  
//	ListView_SetExtendedListViewStyle(m_mylbList.m_hWnd, WS_CHILD | WS_VISIBLE | WS_BORDER 
//		| WS_HSCROLL | WS_VSCROLL |  LBS_OWNERDRAWFIXED | LBS_HASSTRINGS);
	
	ModifyStyle(0,LBS_HASSTRINGS);

	CMap<CString, LPCWSTR, int, int> mapColorCode_ColorKey;
	mapColorCode_ColorKey[COLORCODE_RED] = COLORREF_RED;
	mapColorCode_ColorKey[COLORCODE_GREEN] = COLORREF_GREEN;
	mapColorCode_ColorKey[COLORCODE_YELLOW] = COLORREF_YELLOW;
	for(int i = 0; i < arr.GetCount();)
	{
		CString strColor = arr[i++];
		int nColorKey = mapColorCode_ColorKey[strColor];
		if( nColorKey != COLORREF_RED
			&& nColorKey != COLORREF_GREEN
			&& nColorKey != COLORREF_YELLOW)
		{
			nColorKey = COLORREF_RED;
		}
		if(i < arr.GetCount())
		{
			m_mylbList.AddStringMy(arr[i++], nColorKey);
		}
	}
	m_mylbList.SetCurSel(m_mylbList.GetCount()-1);
	//OnLbnSelchangeListShowmsg();
	return TRUE;
}
// CShowMsgSetDlg 消息处理程序


void CShowMsgSetDlg::OnBnClickedBtnAddtext()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strText;
	m_editTextAdd.GetWindowText(strText);
	if (!strText.IsEmpty())
	{
		m_mylbList.AddStringMy(strText,COLORREF_RED);
	}
	else AfxMessageBox(_T("请输入要添加的信息。"));
}

void CShowMsgSetDlg::OnLbnSelchangeListShowmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_mylbList.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	CString strlistbox;
	m_mylbList.GetTextMy(index,strlistbox);
	m_editTextAdd.SetWindowText(strlistbox);
	COLORREF iColor;
	if(!m_mylbList.GetColor(m_mylbList.GetCurSel(), iColor)) return;
	int nItem = -1;
	for(int i = 0; i < m_combColor.GetCount(); i++)
	{
		if(m_combColor.GetItemData(i) == iColor)
		{
			nItem = i;
			break;
		}
	}
	m_combColor.SetCurSel(nItem);
}


void CShowMsgSetDlg::OnBnClickedBtnChangemsg()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_mylbList.GetCurSel();
	if (index==CB_ERR)
	{
		MessageBox(_T("请选择要更改项"),MB_OK,MB_ICONINFORMATION);
		return;
	}
	CString strlistbox;
	m_editTextAdd.GetWindowText(strlistbox);
	m_mylbList.DeleteString(index);
	m_mylbList.InsertStringMy(index,strlistbox,COLORREF_RED);
	m_mylbList.SetCurSel(index);
}


void CShowMsgSetDlg::OnBnClickedBtnDeletemsg()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_mylbList.GetCurSel();
	int count=m_mylbList.GetCount();
	if(count<=0)
	{
		return;
	}
	if (index==CB_ERR)
	{
		MessageBox(_T("请选择要删除项"),MB_OK,MB_ICONINFORMATION);
		return;
	}
	m_mylbList.DeleteString(index);
	m_mylbList.SetCurSel(index);
	if(m_mylbList.GetCount()>0)
	{
		m_mylbList.SetCurSel(m_mylbList.GetCount()-1);
		OnLbnSelchangeListShowmsg();
	}
	else m_editTextAdd.SetWindowText(_T(""));
}

void CShowMsgSetDlg::OnBnClickedBtnQuenum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Sel_Chinese.GetCheck()==BST_CHECKED)
	{
		m_mylbList.AddStringMy(STRING_QUEUEID_CALL,COLORREF_RED);
	}
	else if (m_Sel_English.GetCheck()==BST_CHECKED)
	{
		m_mylbList.AddStringMy(_T("[排队号码(英)]"),COLORREF_RED);
	}
	
}

void CShowMsgSetDlg::OnBnClickedBtnStaffname()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Sel_Chinese.GetCheck()==BST_CHECKED)
	{
		m_mylbList.AddStringMy(STRING_CUSTNAME_CALL,COLORREF_RED);
	}
	else
	{
		AfxMessageBox(_T("客户姓名只支持汉语"));
	}
	
}


void CShowMsgSetDlg::OnBnClickedBtnWndname()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Sel_Chinese.GetCheck()==BST_CHECKED)
	{
		m_mylbList.AddStringMy(STRING_WINDOWNAME,COLORREF_RED);
	}
	else if (m_Sel_English.GetCheck()==BST_CHECKED)
	{
		m_mylbList.AddStringMy(_T("[窗口名称(英)]"),COLORREF_RED);
	}
	
}


void CShowMsgSetDlg::OnCbnSelchangeComboShowcolor()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_mylbList.GetCurSel();
	if (index==CB_ERR)
	{
		MessageBox(_T("请选择要设置项"),MB_OK,MB_ICONINFORMATION);
		return;
	}
	m_mylbList.SetColor(m_mylbList.GetCurSel(), m_combColor.GetItemData(m_combColor.GetCurSel()));
}

void CShowMsgSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMap<int, int, CString, LPCWSTR> mapColorKey_ColorCode;
	mapColorKey_ColorCode[COLORREF_RED] = COLORCODE_RED;
	mapColorKey_ColorCode[COLORREF_GREEN] = COLORCODE_GREEN;
	mapColorKey_ColorCode[COLORREF_YELLOW] = COLORCODE_YELLOW;

	CString strDisplayCall;
	for(int i = 0; i < m_mylbList.GetCount(); i++)
	{
		CString strText;
		COLORREF iColor;
		m_mylbList.GetTextAndColor(i, strText, iColor);
		strDisplayCall += mapColorKey_ColorCode[iColor];
		strDisplayCall += strText;
	}
	UpdateData();
	m_strConf = strDisplayCall;
	m_strAdMsg = m_ed_strAdmsg;
	m_iShowTime = CommonStrMethod::Str2Int(m_ed_strShowTime);
	OnOK();
}

void CShowMsgSetDlg::OnBnClickedBtnMoveup()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem = m_mylbList.GetCurSel();
	if(nItem <= 0) return;
	CString strText;
	COLORREF iColor;
	m_mylbList.GetTextAndColor(nItem, strText, iColor);
	m_mylbList.DeleteString(nItem);
	m_mylbList.InsertStringMy(--nItem, strText, iColor);
}

void CShowMsgSetDlg::OnBnClickedBtnMovedown()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem = m_mylbList.GetCurSel();
	if(nItem >= m_mylbList.GetCount()-1 || nItem < 0) return;
	CString strText;
	COLORREF iColor;
	m_mylbList.GetTextAndColor(nItem, strText, iColor);
	m_mylbList.DeleteString(nItem);
	m_mylbList.InsertStringMy(++nItem, strText, iColor);
}

void CShowMsgSetDlg::OnLbnDblclkListShowmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_mylbList.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	CString strlistbox;
	m_mylbList.GetText(index,strlistbox);
	m_editTextAdd.SetWindowText(strlistbox);
	COLORREF iColor;
	if(!m_mylbList.GetColor(m_mylbList.GetCurSel(), iColor)) return;
	int nItem = -1;
	for(int i = 0; i < m_combColor.GetCount(); i++)
	{
		if(m_combColor.GetItemData(i) == iColor)
		{
			nItem = i;
			break;
		}
	}
	m_combColor.SetCurSel(nItem);
}

void CShowMsgSetDlg::OnBnClickedCheckMkshall()
{
	// TODO: 在此添加控件通知处理程序代码
	if(BST_CHECKED == m_check_mkshowall.GetCheck())
	{
		m_bMkShowAll = TRUE;
	}
	else
	{
		m_bMkShowAll = FALSE;
	}
}

void CShowMsgSetDlg::OnBnClickedBtnBusname()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Sel_Chinese.GetCheck()==BST_CHECKED)
	{
		m_mylbList.AddStringMy(_T("[业务名称]"),COLORREF_RED);
	}
	else
	{
		AfxMessageBox(_T("业务名称只支持汉语"));
	}
}
