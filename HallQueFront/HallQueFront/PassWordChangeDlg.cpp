// PassWordChangeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "PassWordChangeDlg.h"
#include "PassWordDlg.h"

// CPassWordChangeDlg 对话框

IMPLEMENT_DYNAMIC(CPassWordChangeDlg, CDialog)

CPassWordChangeDlg::CPassWordChangeDlg(CWnd* pParent/* =NULL*/)
	: CDialog(CPassWordChangeDlg::IDD, pParent)
{
	m_infofile_path = m_PswdFile.GetExeFullFilePath();
	m_infofile_path += _T("\\PassWord.dat");
	m_iFocus = 0;
}

CPassWordChangeDlg::~CPassWordChangeDlg()
{
}

void CPassWordChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_OLDPSWD,m_strOldPassWord);
	DDX_Text(pDX,IDC_EDIT_NEWPSWD,m_strNewPassWord);
	DDX_Control(pDX,IDC_STATIC_KEY2,m_KeyPic);
	DDX_Control(pDX,IDC_EDIT_OLDPSWD,m_ed_OldPassWord);
	DDX_Control(pDX,IDC_EDIT_NEWPSWD,m_ed_NewPassWord);
}


BEGIN_MESSAGE_MAP(CPassWordChangeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPassWordChangeDlg::OnBnClickedOk)
	ON_WM_PAINT()

	ON_EN_SETFOCUS(IDC_EDIT_OLDPSWD, &CPassWordChangeDlg::OnEnSetfocusEditOldpswd)
	ON_EN_SETFOCUS(IDC_EDIT_NEWPSWD, &CPassWordChangeDlg::OnEnSetfocusEditNewpswd)
	ON_BN_CLICKED(IDC_BTN1, &CPassWordChangeDlg::OnBnClickedBtn1)
	ON_BN_CLICKED(IDC_BTN2, &CPassWordChangeDlg::OnBnClickedBtn2)
	ON_BN_CLICKED(IDC_BTN3, &CPassWordChangeDlg::OnBnClickedBtn3)
	ON_BN_CLICKED(IDC_BTN4, &CPassWordChangeDlg::OnBnClickedBtn4)
	ON_BN_CLICKED(IDC_BTN5, &CPassWordChangeDlg::OnBnClickedBtn5)
	ON_BN_CLICKED(IDC_BTN6, &CPassWordChangeDlg::OnBnClickedBtn6)
	ON_BN_CLICKED(IDC_BTN7, &CPassWordChangeDlg::OnBnClickedBtn7)
	ON_BN_CLICKED(IDC_BTN8, &CPassWordChangeDlg::OnBnClickedBtn8)
	ON_BN_CLICKED(IDC_BTN9, &CPassWordChangeDlg::OnBnClickedBtn9)
	ON_BN_CLICKED(IDC_BTN0, &CPassWordChangeDlg::OnBnClickedBtn0)
	ON_BN_CLICKED(IDC_BTNBK, &CPassWordChangeDlg::OnBnClickedBtnbk)
END_MESSAGE_MAP()


// CPassWordChangeDlg 消息处理程序
void CPassWordChangeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CPassWordDlg *pMPD = (CPassWordDlg*)this->GetParent();
	if (m_strOldPassWord==pMPD->GetPassWord())
	{
		if (m_strNewPassWord.IsEmpty())
		{
			AfxMessageBox(_T("新密码不能为空。"));
		}
		else
		{
			pMPD->SetPassWord(m_strNewPassWord);
			WritePassWordIntoFile();
			OnOK();
		}
	} 
	else
	{
		AfxMessageBox(_T("旧密码输入错误！"));
	}

}

BOOL CPassWordChangeDlg::WritePassWordIntoFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_infofile_path,CFile::modeCreate|CFile::modeWrite,&e))
	{
		WCHAR* pWBuf = new WCHAR[sizeof(WCHAR)*m_strNewPassWord.GetLength()+1];
		pWBuf = m_strNewPassWord.GetBuffer(0);

		file.Write(pWBuf,sizeof(WCHAR)*m_strNewPassWord.GetLength()+1);
		m_strNewPassWord.ReleaseBuffer(0);
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

BOOL CPassWordChangeDlg::ShowPhoto(CStatic& picPhoto)
{
	CDC* pDC = picPhoto.GetWindowDC();
	CRect rect;
	picPhoto.GetClientRect(&rect);
	CImage img;
	CBitmap bHead;
	BOOL flag = bHead.LoadBitmap(IDB_BITMAP_KEY);
	if(flag)
	{
		img.Attach((HBITMAP)bHead.m_hObject,CImage::DIBOR_DEFAULT);
		pDC->SetStretchBltMode(STRETCH_HALFTONE);
		img.Draw(pDC->m_hDC, 0, 0, rect.Width()-1, rect.Height()-1, 0, 0, img.GetWidth(), img.GetHeight());
	}
	return TRUE;                                  
}
void CPassWordChangeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	ShowPhoto(m_KeyPic);
	// 不为绘图消息调用 CDialog::OnPaint()
}



void CPassWordChangeDlg::OnEnSetfocusEditOldpswd()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iFocus = 1;
}

void CPassWordChangeDlg::OnEnSetfocusEditNewpswd()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iFocus = 2;
}

void CPassWordChangeDlg::OnBnClickedBtn1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iFocus==1)
	{
		m_ed_OldPassWord.SetFocus();
		keybd_event(VK_NUMPAD1,MapVirtualKey(VK_NUMPAD1,0),0,0);  
		keybd_event(VK_NUMPAD1,MapVirtualKey(VK_NUMPAD1,0),KEYEVENTF_KEYUP,0);
	}
	if (m_iFocus==2)
	{
		m_ed_NewPassWord.SetFocus();
		keybd_event(VK_NUMPAD1,MapVirtualKey(VK_NUMPAD1,0),0,0);  
		keybd_event(VK_NUMPAD1,MapVirtualKey(VK_NUMPAD1,0),KEYEVENTF_KEYUP,0);
	}
}

void CPassWordChangeDlg::OnBnClickedBtn2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iFocus==1)
	{
		m_ed_OldPassWord.SetFocus();
		keybd_event(VK_NUMPAD2,MapVirtualKey(VK_NUMPAD2,0),0,0);  
		keybd_event(VK_NUMPAD2,MapVirtualKey(VK_NUMPAD2,0),KEYEVENTF_KEYUP,0);
	}
	if (m_iFocus==2)
	{
		m_ed_NewPassWord.SetFocus();
		keybd_event(VK_NUMPAD2,MapVirtualKey(VK_NUMPAD2,0),0,0);  
		keybd_event(VK_NUMPAD2,MapVirtualKey(VK_NUMPAD2,0),KEYEVENTF_KEYUP,0);
	}
}

void CPassWordChangeDlg::OnBnClickedBtn3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iFocus==1)
	{
		m_ed_OldPassWord.SetFocus();
		keybd_event(VK_NUMPAD3,MapVirtualKey(VK_NUMPAD3,0),0,0);  
		keybd_event(VK_NUMPAD3,MapVirtualKey(VK_NUMPAD3,0),KEYEVENTF_KEYUP,0);
	}
	if (m_iFocus==2)
	{
		m_ed_NewPassWord.SetFocus();
		keybd_event(VK_NUMPAD3,MapVirtualKey(VK_NUMPAD3,0),0,0);  
		keybd_event(VK_NUMPAD3,MapVirtualKey(VK_NUMPAD3,0),KEYEVENTF_KEYUP,0);
	}
}

void CPassWordChangeDlg::OnBnClickedBtn4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iFocus==1)
	{
		m_ed_OldPassWord.SetFocus();
		keybd_event(VK_NUMPAD4,MapVirtualKey(VK_NUMPAD4,0),0,0);  
		keybd_event(VK_NUMPAD4,MapVirtualKey(VK_NUMPAD4,0),KEYEVENTF_KEYUP,0);
	}
	if (m_iFocus==2)
	{
		m_ed_NewPassWord.SetFocus();
		keybd_event(VK_NUMPAD4,MapVirtualKey(VK_NUMPAD4,0),0,0);  
		keybd_event(VK_NUMPAD4,MapVirtualKey(VK_NUMPAD4,0),KEYEVENTF_KEYUP,0);
	}
}

void CPassWordChangeDlg::OnBnClickedBtn5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iFocus==1)
	{
		m_ed_OldPassWord.SetFocus();
		keybd_event(VK_NUMPAD5,MapVirtualKey(VK_NUMPAD5,0),0,0);  
		keybd_event(VK_NUMPAD5,MapVirtualKey(VK_NUMPAD5,0),KEYEVENTF_KEYUP,0);
	}
	if (m_iFocus==2)
	{
		m_ed_NewPassWord.SetFocus();
		keybd_event(VK_NUMPAD5,MapVirtualKey(VK_NUMPAD5,0),0,0);  
		keybd_event(VK_NUMPAD5,MapVirtualKey(VK_NUMPAD5,0),KEYEVENTF_KEYUP,0);
	}
}

void CPassWordChangeDlg::OnBnClickedBtn6()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iFocus==1)
	{
		m_ed_OldPassWord.SetFocus();
		keybd_event(VK_NUMPAD6,MapVirtualKey(VK_NUMPAD6,0),0,0);  
		keybd_event(VK_NUMPAD6,MapVirtualKey(VK_NUMPAD6,0),KEYEVENTF_KEYUP,0);
	}
	if (m_iFocus==2)
	{
		m_ed_NewPassWord.SetFocus();
		keybd_event(VK_NUMPAD6,MapVirtualKey(VK_NUMPAD6,0),0,0);  
		keybd_event(VK_NUMPAD6,MapVirtualKey(VK_NUMPAD6,0),KEYEVENTF_KEYUP,0);
	}
}

void CPassWordChangeDlg::OnBnClickedBtn7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iFocus==1)
	{
		m_ed_OldPassWord.SetFocus();
		keybd_event(VK_NUMPAD7,MapVirtualKey(VK_NUMPAD7,0),0,0);  
		keybd_event(VK_NUMPAD7,MapVirtualKey(VK_NUMPAD7,0),KEYEVENTF_KEYUP,0);
	}
	if (m_iFocus==2)
	{
		m_ed_NewPassWord.SetFocus();
		keybd_event(VK_NUMPAD7,MapVirtualKey(VK_NUMPAD7,0),0,0);  
		keybd_event(VK_NUMPAD7,MapVirtualKey(VK_NUMPAD7,0),KEYEVENTF_KEYUP,0);
	}
}

void CPassWordChangeDlg::OnBnClickedBtn8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iFocus==1)
	{
		m_ed_OldPassWord.SetFocus();
		keybd_event(VK_NUMPAD8,MapVirtualKey(VK_NUMPAD8,0),0,0);  
		keybd_event(VK_NUMPAD8,MapVirtualKey(VK_NUMPAD8,0),KEYEVENTF_KEYUP,0);
	}
	if (m_iFocus==2)
	{
		m_ed_NewPassWord.SetFocus();
		keybd_event(VK_NUMPAD8,MapVirtualKey(VK_NUMPAD8,0),0,0);  
		keybd_event(VK_NUMPAD8,MapVirtualKey(VK_NUMPAD8,0),KEYEVENTF_KEYUP,0);
	}
}

void CPassWordChangeDlg::OnBnClickedBtn9()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iFocus==1)
	{
		m_ed_OldPassWord.SetFocus();
		keybd_event(VK_NUMPAD9,MapVirtualKey(VK_NUMPAD9,0),0,0);  
		keybd_event(VK_NUMPAD9,MapVirtualKey(VK_NUMPAD2,0),KEYEVENTF_KEYUP,0);
	}
	if (m_iFocus==2)
	{
		m_ed_NewPassWord.SetFocus();
		keybd_event(VK_NUMPAD9,MapVirtualKey(VK_NUMPAD9,0),0,0);  
		keybd_event(VK_NUMPAD9,MapVirtualKey(VK_NUMPAD9,0),KEYEVENTF_KEYUP,0);
	}
}

void CPassWordChangeDlg::OnBnClickedBtn0()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iFocus==1)
	{
		m_ed_OldPassWord.SetFocus();
		keybd_event(VK_NUMPAD0,MapVirtualKey(VK_NUMPAD0,0),0,0);  
		keybd_event(VK_NUMPAD0,MapVirtualKey(VK_NUMPAD0,0),KEYEVENTF_KEYUP,0);
	}
	if (m_iFocus==2)
	{
		m_ed_NewPassWord.SetFocus();
		keybd_event(VK_NUMPAD0,MapVirtualKey(VK_NUMPAD0,0),0,0);  
		keybd_event(VK_NUMPAD0,MapVirtualKey(VK_NUMPAD0,0),KEYEVENTF_KEYUP,0);
	}
}

void CPassWordChangeDlg::OnBnClickedBtnbk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iFocus==1)
	{
		m_ed_OldPassWord.SetFocus();
		keybd_event(VK_BACK,MapVirtualKey(VK_BACK,0),0,0);  
		keybd_event(VK_BACK,MapVirtualKey(VK_BACK,0),KEYEVENTF_KEYUP,0);
	}
	if (m_iFocus==2)
	{
		m_ed_NewPassWord.SetFocus();
		keybd_event(VK_BACK,MapVirtualKey(VK_BACK,0),0,0);  
		keybd_event(VK_BACK,MapVirtualKey(VK_BACK,0),KEYEVENTF_KEYUP,0);
	}
}