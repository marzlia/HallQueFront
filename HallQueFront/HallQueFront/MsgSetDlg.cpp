// MsgSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MsgSetDlg.h"
#include "CommonStrMethod.h"

// CMsgSetDlg 对话框

IMPLEMENT_DYNAMIC(CMsgSetDlg, CDialog)

CMsgSetDlg::CMsgSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgSetDlg::IDD, pParent)
{
	m_strMsgPath = CommonStrMethod::GetModuleDir()+L"ShortMsg\\";
	CommonStrMethod::CreatePath(m_strMsgPath);
	m_strMsgPath +=L"ShortMsg.dat";
}

CMsgSetDlg::~CMsgSetDlg()
{
}

void CMsgSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_MSG,m_list_Msg);
	DDX_Control(pDX,IDC_EDIT_MSG,m_edit_Msg);
}


BEGIN_MESSAGE_MAP(CMsgSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADDMSG, &CMsgSetDlg::OnBnClickedButtonAddmsg)
	ON_BN_CLICKED(IDC_BUTTON_DELMSG, &CMsgSetDlg::OnBnClickedButtonDelmsg)
	ON_LBN_SELCHANGE(IDC_LIST_MSG, &CMsgSetDlg::OnLbnSelchangeListMsg)
	ON_EN_CHANGE(IDC_EDIT_MSG, &CMsgSetDlg::OnEnChangeEditMsg)
	ON_BN_CLICKED(IDOK, &CMsgSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMsgSetDlg 消息处理程序

BOOL CMsgSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ReadMsgFromFile();
	CFont font;
	font.CreatePointFont(110,L"微软雅黑");
	m_edit_Msg.SetFont(&font);
	return TRUE;
}

void CMsgSetDlg::OnBnClickedButtonAddmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	m_list_Msg.AddString(L"提示语");
	m_list_Msg.SetCurSel(m_list_Msg.GetCount()-1);
	OnLbnSelchangeListMsg();
}

void CMsgSetDlg::OnBnClickedButtonDelmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelect = m_list_Msg.GetCurSel();
	m_list_Msg.DeleteString(nSelect);
	if(m_list_Msg.GetCount()>0)
	{
		m_list_Msg.SetCurSel(m_list_Msg.GetCount()-1);
		OnLbnSelchangeListMsg();
	}
	else
	{
		m_edit_Msg.SetWindowText(L"");
		UpdateData(FALSE);
	}
}

void CMsgSetDlg::OnLbnSelchangeListMsg()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_list_Msg.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	CString strMsg;
	m_list_Msg.GetText(index,strMsg);
	m_edit_Msg.SetWindowText(strMsg);
	m_list_Msg.SetCurSel(index);
}

void CMsgSetDlg::OnEnChangeEditMsg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	int index = m_list_Msg.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	CString strMsg;
	m_edit_Msg.GetWindowText(strMsg);
	m_list_Msg.DeleteString(index);
	m_list_Msg.InsertString(index,strMsg);
	m_list_Msg.SetCurSel(index);
}

void CMsgSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_arr_Msg.RemoveAll();
	int nlength = m_list_Msg.GetCount();
	for (int i = 0;i<nlength;i++)
	{
		CString strMsg;
		m_list_Msg.GetText(i,strMsg);
		if (strMsg.GetLength()<=70)
		{		
			m_arr_Msg.Add(strMsg);
		}
		else
		{
			AfxMessageBox(L"提示信息过长，请控制在70个字符以内");
			return;
		}
	}
	WriteMsgIntoFile();
	OnOK();
}

BOOL CMsgSetDlg::WriteMsgIntoFile()
{
	CFile file;
	CFileException e;
	if(file.Open(m_strMsgPath,CFile::modeCreate|CFile::modeWrite))
	{
		CArchive ar(&file,CArchive::store);
		int ilength = m_arr_Msg.GetCount();
		for (int i=0;i<ilength;i++)
		{
			ar<<m_arr_Msg.GetAt(i);
		}
		ar.Close();
		file.Close();
		return TRUE;
	}
	else
	{
		TRACE(_T("File could not be opened %d\n"), e.m_cause);
		return FALSE;
	}
}

BOOL CMsgSetDlg::ReadMsgFromFile()
{
	m_arr_Msg.RemoveAll();
	CFile file;
	CFileException e;
	if (file.Open(m_strMsgPath,CFile::modeRead,&e))
	{
		if (file.GetLength())
		{
			CArchive ar(&file,CArchive::load);
			CString strMsg=NULL;
			do 
			{
				ar>>strMsg;
				m_list_Msg.AddString(strMsg);
				strMsg=_T("");
			} while (!ar.IsBufferEmpty());
			ar.Close();
		}
		file.Close();
		return TRUE;
	}
	else return FALSE;
}