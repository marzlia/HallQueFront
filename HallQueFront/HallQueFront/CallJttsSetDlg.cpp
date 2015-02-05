// CallJttsSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "CallJttsSetDlg.h"
#include "CommonStrMethod.h"

// CCallJttsSetDlg 对话框

IMPLEMENT_DYNAMIC(CCallJttsSetDlg, CDialog)

CCallJttsSetDlg::CCallJttsSetDlg(CString& strConf,CWnd* pParent /*=NULL*/)
	: CDialog(CCallJttsSetDlg::IDD, pParent)
	, m_StrConf(strConf)
{
	m_bMkJttsAll = FALSE;
}

CCallJttsSetDlg::~CCallJttsSetDlg()
{
}

void CCallJttsSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_SOUND,m_lbList);
	DDX_Control(pDX,IDC_LIST_LIB,m_lbWavLib);
	DDX_Control(pDX,IDC_CHECK_MKJTTSALL,m_check_mkjttsall);
	DDX_Control(pDX,IDC_RADIO_COMCHN,m_Sel_CommonChinese);
	DDX_Control(pDX,IDC_RADIO_ENGLISH,m_Sel_English);
}


BEGIN_MESSAGE_MAP(CCallJttsSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_QUENUM, &CCallJttsSetDlg::OnBnClickedButtonQuenum)
	ON_BN_CLICKED(IDC_BUTTON_WNDNAME, &CCallJttsSetDlg::OnBnClickedButtonWndname)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCallJttsSetDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MVUP, &CCallJttsSetDlg::OnBnClickedButtonMvup)
	ON_BN_CLICKED(IDC_BUTTON_MVDOWN, &CCallJttsSetDlg::OnBnClickedButtonMvdown)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CCallJttsSetDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDOK, &CCallJttsSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_MKJTTSALL, &CCallJttsSetDlg::OnBnClickedCheckMkjttsall)
	ON_BN_CLICKED(IDC_BUTTON_BUSNAME, &CCallJttsSetDlg::OnBnClickedButtonBusname)
END_MESSAGE_MAP()

BOOL CCallJttsSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//CString strSound;
	//m_pRset->GetFieldValue(_T("sound_wait"), strSound);
	m_check_mkjttsall.SetCheck(BST_UNCHECKED);
	m_Sel_CommonChinese.SetCheck(BST_CHECKED);
	CStringArray arrStr;
	CommonStrMethod::StrSplit(m_StrConf, arrStr,_T("#"));
	for(int i = 1; i < arrStr.GetCount(); i++)
	{
		m_lbList.AddString(arrStr[i]);
	}

	CFileFind finder;
	BOOL bFound = finder.FindFile(CommonStrMethod::GetModuleDir() + _T("wavLib\\*.wav"));
	while(bFound)
	{
		bFound = finder.FindNextFile();
		m_lbWavLib.AddString(finder.GetFileTitle());
	}
	finder.Close();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CCallJttsSetDlg 消息处理程序

void CCallJttsSetDlg::OnBnClickedButtonQuenum()
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



void CCallJttsSetDlg::OnBnClickedButtonWndname()
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


void CCallJttsSetDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_lbWavLib.GetCurSel();
	if (index==CB_ERR)
	{
		return;
	}
	CString strSelect;
	m_lbWavLib.GetText(index,strSelect);
	int nItem = m_lbList.AddString(strSelect);
	m_lbList.SetCurSel(nItem);
}


void CCallJttsSetDlg::OnBnClickedButtonMvup()
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


void CCallJttsSetDlg::OnBnClickedButtonMvdown()
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


void CCallJttsSetDlg::OnBnClickedButtonDel()
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
	}
}

void CCallJttsSetDlg::OnBnClickedOk()
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

void CCallJttsSetDlg::OnBnClickedCheckMkjttsall()
{
	// TODO: 在此添加控件通知处理程序代码
	if(BST_CHECKED == m_check_mkjttsall.GetCheck())
	{
		m_bMkJttsAll = TRUE;
	}
	else
	{
		m_bMkJttsAll = FALSE;
	}
}

void CCallJttsSetDlg::OnBnClickedButtonBusname()
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
