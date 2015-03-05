// ShortMsgSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ShortMsgSetDlg.h"
#include "CommonStrMethod.h"

// CShortMsgSetDlg 对话框

IMPLEMENT_DYNAMIC(CShortMsgSetDlg, CDialog)

CShortMsgSetDlg::CShortMsgSetDlg(CWnd* pParent /*=NULL*/,CInlineQueData& rInlineQueData)
	: CDialog(CShortMsgSetDlg::IDD, pParent)
	,m_rInlineQueData(rInlineQueData)
{
	m_strMsgPath = CommonStrMethod::GetModuleDir() + L"ShortMsg\\";
	m_strMsgPath += L"ShortMsg.dat";
}

CShortMsgSetDlg::~CShortMsgSetDlg()
{
}

void CShortMsgSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_STATIC_CELLPHONE,m_sta_CellPhone);
	DDX_Control(pDX,IDC_STATIC_QUENUM,m_sta_QueNum);
	DDX_Control(pDX,IDC_STATIC_WAITNUM,m_sta_WaitNum);
//	DDX_Control(pDX,IDC_STATIC_MSG,m_sta_Msg);
	DDX_Control(pDX,IDC_EDIT_CELLPHONE,m_edit_CellPhone);
	DDX_Control(pDX,IDC_EDIT_QUENUM,m_edit_QueNum);
	DDX_Control(pDX,IDC_EDIT_WAITNUM,m_edit_WaitNum);
	DDX_Control(pDX,IDC_COMBO_MSG,m_com_Msg);
}


BEGIN_MESSAGE_MAP(CShortMsgSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CShortMsgSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CShortMsgSetDlg::OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_EDIT_CELLPHONE, &CShortMsgSetDlg::OnEnSetfocusEditCellphone)
	ON_EN_CHANGE(IDC_EDIT_CELLPHONE, &CShortMsgSetDlg::OnEnChangeEditCellphone)
	ON_EN_SETFOCUS(IDC_EDIT_QUENUM, &CShortMsgSetDlg::OnEnSetfocusEditQuenum)
	ON_EN_UPDATE(IDC_EDIT_QUENUM, &CShortMsgSetDlg::OnEnUpdateEditQuenum)
	ON_EN_SETFOCUS(IDC_EDIT_WAITNUM, &CShortMsgSetDlg::OnEnSetfocusEditWaitnum)
	ON_EN_UPDATE(IDC_EDIT_WAITNUM, &CShortMsgSetDlg::OnEnUpdateEditWaitnum)
END_MESSAGE_MAP()


// CShortMsgSetDlg 消息处理程序

BOOL CShortMsgSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ReadMsgInfoFromFile();
	m_sta_CellPhone.SetWindowText(L"");
//	m_sta_Msg.SetWindowText(L"");
	m_sta_QueNum.SetWindowText(L"");
	m_sta_WaitNum.SetWindowText(L"");
// 	CFont font;
// 	font.CreatePointFont(100,L"黑体");
// 	m_sta_CellPhone.SetFont(&font,TRUE);
// 	m_sta_Msg.SetFont(&font,TRUE);
// 	m_sta_QueNum.SetFont(&font,TRUE);
// 	m_sta_WaitNum.SetFont(&font,TRUE);
	return TRUE;
}
void CShortMsgSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pWnd = FindWindow(NULL,L"屏幕键盘");
	if(pWnd)
	{
		::SendMessage(pWnd->m_hWnd,WM_DESTROY,0,0);
	}
	CString strWaitNum;
	CString strMsg;
	m_edit_CellPhone.GetWindowText(m_strPhoneNum);
	m_edit_QueNum.GetWindowText(m_strQueNum);
	m_edit_WaitNum.GetWindowText(strWaitNum);
	m_com_Msg.GetWindowText(m_strMsg);
	int ncount = m_strMsg.GetLength();
	if (m_strPhoneNum.IsEmpty()||m_strQueNum.IsEmpty()||strWaitNum.IsEmpty()||m_strMsg.IsEmpty())
	{
		return;
	}
	else
	{
		int icount;
		CCommonConvert::CStringToint(icount,strWaitNum);
		if (icount>0)
		{
			if (GetSendQueNum(m_strQueNum,icount))
			{
				if(m_rInlineQueData.AlterData(m_strQueNum,m_strPhoneNum,m_strMsg))
				{
					OnOK();
				}
				m_sta_WaitNum.SetWindowText(L"等待人数填写有误！");
			}
			else
			{
				m_sta_WaitNum.SetWindowText(L"等待人数填写有误！");
			}
		}
		else
		{
			m_sta_WaitNum.SetWindowText(L"等待人数填写有误！");
		}
	}
}

BOOL CShortMsgSetDlg::GetSendQueNum(CString& strQueNum,int iCount)
{
	int iSendQueNum = GetNumFromString(strQueNum)-iCount-1;
	if (iSendQueNum<=0)
	{
		return FALSE;
	}
	CString strFront = strQueNum.Left(1);
	strQueNum.Format(L"%03d",iSendQueNum);
	strQueNum = strFront+strQueNum;
	return TRUE;
}

int CShortMsgSetDlg::GetNumFromString(CString strNum)
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

void CShortMsgSetDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pWnd = FindWindow(NULL,L"屏幕键盘");
	if(pWnd)
	{
		::SendMessage(pWnd->m_hWnd,WM_DESTROY,0,0);
	}
	OnCancel();
}

void CShortMsgSetDlg::OnEnSetfocusEditCellphone()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strCellPhone;
	m_edit_CellPhone.GetWindowText(strCellPhone);
	if (strCellPhone.IsEmpty())
	{
		m_sta_CellPhone.SetWindowText(L"请填写接收短信的电话号码");
	}

}

void CShortMsgSetDlg::OnEnChangeEditCellphone()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strCellPhone;
	m_edit_CellPhone.GetWindowText(strCellPhone);
	if (strCellPhone.IsEmpty())
	{
		m_sta_CellPhone.SetWindowText(L"请填写接收短信的电话号码");
	}
	else
	{
		m_sta_CellPhone.SetWindowText(L"");
	}
}

void CShortMsgSetDlg::OnEnSetfocusEditQuenum()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strQuenum;
	m_edit_QueNum.GetWindowText(strQuenum);
	if (strQuenum.IsEmpty())
	{
		m_sta_QueNum.SetWindowText(L"请填写您的取票号码");
	}
}

void CShortMsgSetDlg::OnEnUpdateEditQuenum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数，将 EM_SETEVENTMASK 消息发送到控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strQuenum;
	m_edit_QueNum.GetWindowText(strQuenum);
	if (strQuenum.IsEmpty())
	{
		m_sta_QueNum.SetWindowText(L"请填写您的取票号码");
	}
	else
	{
		m_sta_QueNum.SetWindowText(L"");
	}
}



void CShortMsgSetDlg::OnEnSetfocusEditWaitnum()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWaitNum;
	m_edit_WaitNum.GetWindowText(strWaitNum);
	if (strWaitNum.IsEmpty())
	{
		m_sta_WaitNum.SetWindowText(L"请填写您想接收短信时排队的人数。");
	}
}


void CShortMsgSetDlg::OnEnUpdateEditWaitnum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数，将 EM_SETEVENTMASK 消息发送到控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strWaitNum;
	m_edit_WaitNum.GetWindowText(strWaitNum);
	if (strWaitNum.IsEmpty())
	{
		m_sta_WaitNum.SetWindowText(L"请填写您想接收短信时排队的人数。");
	}
	else
	{
		m_sta_WaitNum.SetWindowText(L"");
	}
}

BOOL CShortMsgSetDlg::ReadMsgInfoFromFile()
{
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
				m_com_Msg.AddString(strMsg);
				strMsg=_T("");
			} while (!ar.IsBufferEmpty());
			ar.Close();
		}
		file.Close();
		return TRUE;
	}
	else return FALSE;
}
