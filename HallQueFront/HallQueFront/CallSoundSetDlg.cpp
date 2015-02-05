// CallSoundSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "CallSoundSetDlg.h"
#include "CommonStrMethod.h"

// CCallSoundSetDlg 对话框

IMPLEMENT_DYNAMIC(CCallSoundSetDlg, CDialog)

CCallSoundSetDlg::CCallSoundSetDlg(CString& strConf,CWnd* pParent /*=NULL*/)
	: CDialog(CCallSoundSetDlg::IDD, pParent)
	, m_StrConf(strConf)
{
	m_bMksndall = FALSE;
}

CCallSoundSetDlg::~CCallSoundSetDlg()
{
}

void CCallSoundSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_TEXT_ADD,m_editTextAdd);
	DDX_Control(pDX,IDC_LIST_SOUNDMSG,m_lbList);
	DDX_Control(pDX,IDC_CHECK_MKSDALL,m_check_mksndall);
	DDX_Control(pDX,IDC_RADIO_COMCHN,m_Sel_CommonChinese);
	DDX_Control(pDX,IDC_RADIO_ENGLISH,m_Sel_English);
}


BEGIN_MESSAGE_MAP(CCallSoundSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADDTEXT, &CCallSoundSetDlg::OnBnClickedBtnAddtext)
	ON_LBN_SELCHANGE(IDC_LIST_SOUNDMSG, &CCallSoundSetDlg::OnLbnSelchangeListSoundmsg)
	ON_BN_CLICKED(IDC_BTN_CHANGEMSG, &CCallSoundSetDlg::OnBnClickedBtnChangemsg)
	ON_BN_CLICKED(IDC_BTN_DELETEMSG, &CCallSoundSetDlg::OnBnClickedBtnDeletemsg)
	ON_BN_CLICKED(IDC_BTN_QUENUM, &CCallSoundSetDlg::OnBnClickedBtnQuenum)
	ON_BN_CLICKED(IDC_BTN_STAFFNAME, &CCallSoundSetDlg::OnBnClickedBtnStaffname)
	ON_BN_CLICKED(IDC_BTN_WNDNAME, &CCallSoundSetDlg::OnBnClickedBtnWndname)
	ON_BN_CLICKED(IDOK, &CCallSoundSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_UP, &CCallSoundSetDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, &CCallSoundSetDlg::OnBnClickedBtnDown)
	ON_BN_CLICKED(IDC_CHECK_MKSDALL, &CCallSoundSetDlg::OnBnClickedCheckMksdall)
	ON_BN_CLICKED(IDC_BTN_BUSNAME, &CCallSoundSetDlg::OnBnClickedBtnBusname)
END_MESSAGE_MAP()

BOOL CCallSoundSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//CString strSound;
	//m_pRset->GetFieldValue(_T("sound_wait"), strSound);
	m_check_mksndall.SetCheck(BST_UNCHECKED);
	m_Sel_CommonChinese.SetCheck(BST_CHECKED);
	CStringArray arrStr;
	CommonStrMethod::StrSplit(m_StrConf, arrStr,_T("#"));
	for(int i = 1; i < arrStr.GetCount(); i++)
	{
		m_lbList.AddString(arrStr[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CCallSoundSetDlg 消息处理程序

int CCallSoundSetDlg::SplitStringToArray(CString strSrc, CArray<CString>& arrStr)
{
	//CString strTemp(strSrc);
	int nIndex = -1;
	//for(; !strSrc.IsEmpty();)
	for(; strSrc.GetLength() != 0;)
	{
		CString strTempLeft, strTempMid;
		nIndex = strSrc.Find(_T('['));
		if(nIndex != -1)
		{
			strTempLeft = strSrc.Left(nIndex);
			if(!strTempLeft.IsEmpty())
			{
				arrStr.Add(strTempLeft);
			}
			strSrc = strSrc.Mid(nIndex);
			nIndex = strSrc.Find(_T(']'));
			if(nIndex != -1)
			{
				strTempMid = strSrc.Left(nIndex+1);
				arrStr.Add(strTempMid);
				strSrc = strSrc.Mid(nIndex+1);
			}
		}
		else
		{
			arrStr.Add(strSrc);
			break;
		}
	}
	return 0;
}

void CCallSoundSetDlg::OnBnClickedBtnAddtext()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strText;
	m_editTextAdd.GetWindowText(strText);
	if (!strText.IsEmpty())
	{
		int nItem = m_lbList.AddString(strText);
		m_lbList.SetCurSel(nItem);
	}
	else AfxMessageBox(_T("请输入要添加的信息。"));
}
	

void CCallSoundSetDlg::OnLbnSelchangeListSoundmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_lbList.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	CString strlistbox;
	m_lbList.GetText(index,strlistbox);
	m_editTextAdd.SetWindowText(strlistbox);
}

void CCallSoundSetDlg::OnBnClickedBtnChangemsg()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_lbList.GetCurSel();
	if (index==CB_ERR)
	{
		MessageBox(_T("请选择要更改项"),MB_OK,MB_ICONINFORMATION);
		return;
	}
	CString strlistbox;
	m_editTextAdd.GetWindowText(strlistbox);
	m_lbList.DeleteString(index);
	m_lbList.InsertString(index,strlistbox);
	m_lbList.SetCurSel(index);
}

void CCallSoundSetDlg::OnBnClickedBtnDeletemsg()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_lbList.GetCurSel();
	int count=m_lbList.GetCount();
	if(count<=0)
	{
		return;
	}
	if (index==CB_ERR)
	{
		MessageBox(_T("请选择要删除项"),MB_OK,MB_ICONINFORMATION);
		return;
	}
	m_lbList.DeleteString(index);
	m_lbList.SetCurSel(index);
	if(m_lbList.GetCount()>0)
	{
		m_lbList.SetCurSel(m_lbList.GetCount()-1);
		OnLbnSelchangeListSoundmsg();
	}
	else m_editTextAdd.SetWindowText(_T(""));
}

void CCallSoundSetDlg::OnBnClickedBtnQuenum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Sel_CommonChinese.GetCheck()==BST_CHECKED)
	{
		int nItem = m_lbList.AddString(STRING_QUEUEID_CALL);
		m_lbList.SetCurSel(nItem);
	}
	else if (m_Sel_English.GetCheck()==BST_CHECKED)
	{
		int nItem = m_lbList.AddString(_T("[排队号码(英)]"));
		m_lbList.SetCurSel(nItem);
	}
}

void CCallSoundSetDlg::OnBnClickedBtnStaffname()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Sel_CommonChinese.GetCheck()==BST_CHECKED)
	{
		int nItem = m_lbList.AddString(STRING_CUSTNAME_CALL);
		m_lbList.SetCurSel(nItem);
	}
	else
	{
		AfxMessageBox(_T("客户姓名只支持普通话。"));
	}
}

void CCallSoundSetDlg::OnBnClickedBtnWndname()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Sel_CommonChinese.GetCheck()==BST_CHECKED)
	{
		int nItem = m_lbList.AddString(_T("[呼叫名称]"));
		m_lbList.SetCurSel(nItem);
	}
	else if (m_Sel_English.GetCheck()==BST_CHECKED)
	{
		int nItem = m_lbList.AddString(_T("[呼叫名称(英)]"));
		m_lbList.SetCurSel(nItem);
	}
}

void CCallSoundSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strAll;
	for(int i = 0; i < m_lbList.GetCount(); i++)
	{
		CString strLine;
		m_lbList.GetText(i, strLine);
		strAll += _T("#")+strLine;
	}
	m_StrConf = strAll;

	OnOK();
}

void CCallSoundSetDlg::OnBnClickedBtnUp()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem = m_lbList.GetCurSel();
	if(nItem <= 0) return;
	CString strText;
	m_lbList.GetText(nItem, strText);
	m_lbList.DeleteString(nItem);
	m_lbList.InsertString(--nItem, strText);
	m_lbList.SetCurSel(nItem);
}

void CCallSoundSetDlg::OnBnClickedBtnDown()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem = m_lbList.GetCurSel();
	if(nItem >= m_lbList.GetCount()-1 || nItem < 0) return;
	CString strText;
	m_lbList.GetText(nItem, strText);
	m_lbList.DeleteString(nItem);
	m_lbList.InsertString(++nItem, strText);
	m_lbList.SetCurSel(nItem);
}


void CCallSoundSetDlg::OnBnClickedCheckMksdall()
{
	// TODO: 在此添加控件通知处理程序代码
	if(BST_CHECKED == m_check_mksndall.GetCheck())
	{
		m_bMksndall = TRUE;
	}
	else
	{
		m_bMksndall = FALSE;
	}
}

void CCallSoundSetDlg::OnBnClickedBtnBusname()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Sel_CommonChinese.GetCheck()==BST_CHECKED)
	{
		int nItem = m_lbList.AddString(_T("[业务名称]"));
		m_lbList.SetCurSel(nItem);
	}
	else 
	{
		AfxMessageBox(_T("业务名称只支持汉语"));
	}
}
