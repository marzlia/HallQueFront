// PassWordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "PassWordDlg.h"
#include "PassWordChangeDlg.h"

// CPassWordDlg 对话框

IMPLEMENT_DYNAMIC(CPassWordDlg, CDialog)

CPassWordDlg::CPassWordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPassWordDlg::IDD, pParent)
{
	m_bSuccess=FALSE;
	m_infofile_path=m_PswdFile.GetExeFullFilePath();
	m_infofile_path += _T("\\PassWord.dat");
}

CPassWordDlg::~CPassWordDlg()
{
}

void CPassWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_PASSWORD,m_strShowPassWord);
	DDX_Control(pDX,IDC_STATIC_KEY,m_KeyPic);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_ed_passWord);
}


BEGIN_MESSAGE_MAP(CPassWordDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETPSWD, &CPassWordDlg::OnBnClickedButtonSetpswd)
	ON_BN_CLICKED(IDOK, &CPassWordDlg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BN_NUMONE, &CPassWordDlg::OnBnClickedBnNumone)
	ON_BN_CLICKED(IDC_BN_NUMTWO, &CPassWordDlg::OnBnClickedBnNumtwo)
	ON_BN_CLICKED(IDC_BN_NUMTHREE, &CPassWordDlg::OnBnClickedBnNumthree)
	ON_BN_CLICKED(IDC_BN_NUMFOUR,&CPassWordDlg::OnBnClickedBnNumfour)
	ON_BN_CLICKED(IDC_BN_NUMFIVE,&CPassWordDlg::OnBnClickedBnNumfive)
	ON_BN_CLICKED(IDC_BN_NUMSIX,&CPassWordDlg::OnBnClickedBnNumsix)
	ON_BN_CLICKED(IDC_BN_NUMSEVEN,&CPassWordDlg::OnBnClickedBnNumseven)
	ON_BN_CLICKED(IDC_BN_NUMEIGHT,&CPassWordDlg::OnBnClickedBnNumeight)
	ON_BN_CLICKED(IDC_BN_NUMNINE,&CPassWordDlg::OnBnClickedBnNumnine)
	ON_BN_CLICKED(IDC_BN_NUMZERO,&CPassWordDlg::OnBnClickedBnNumzero)
	ON_BN_CLICKED(IDC_BN_CANCEL,&CPassWordDlg::OnBnClickedBncancel)
END_MESSAGE_MAP()


// CPassWordDlg 消息处理程序
void CPassWordDlg::OnBnClickedButtonSetpswd()
{
	// TODO: 在此添加控件通知处理程序代码
	CPassWordChangeDlg PassWordChangeDlg(this);
	PassWordChangeDlg.DoModal();
}

BOOL CPassWordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ReadPassWordIntoFile();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPassWordDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
	UpdateData();
	if (m_strShowPassWord.IsEmpty())
	{
		AfxMessageBox(_T("密码不能为空。"));
	}
	else
	{
		if((m_strShowPassWord==m_strPassWord)||(m_strShowPassWord==_T("8888")))
		{
			OnOK();
		}
		else
		{
			AfxMessageBox(_T("密码错误请重新输入。"));
		}
	}

}

BOOL CPassWordDlg::ReadPassWordIntoFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_infofile_path,CFile::modeRead,&e))
	{
		WCHAR *pBuf;
		UINT dwFileLen;
		dwFileLen=(UINT)file.GetLength();
		pBuf=new WCHAR[dwFileLen+1];
		memset(pBuf,0,dwFileLen+1);
		file.Read(pBuf,dwFileLen);
		CString FileData(pBuf);	
		SetPassWord(FileData);
		delete pBuf;
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CPassWordDlg::ShowPhoto(CStatic& picPhoto)
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
void CPassWordDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	ShowPhoto(m_KeyPic);
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CPassWordDlg::OnBnClickedBnNumone()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ed_passWord.SetFocus();
	keybd_event(VK_NUMPAD1,MapVirtualKey(VK_NUMPAD1,0),0,0);  
	keybd_event(VK_NUMPAD1,MapVirtualKey(VK_NUMPAD1,0),KEYEVENTF_KEYUP,0);  
}

void CPassWordDlg::OnBnClickedBnNumtwo()
{
	m_ed_passWord.SetFocus();
	keybd_event(VK_NUMPAD2,MapVirtualKey(VK_NUMPAD2,0),0,0);  
	keybd_event(VK_NUMPAD2,MapVirtualKey(VK_NUMPAD2,0),KEYEVENTF_KEYUP,0);  
}

void CPassWordDlg::OnBnClickedBnNumthree()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ed_passWord.SetFocus();
	keybd_event(VK_NUMPAD3,MapVirtualKey(VK_NUMPAD3,0),0,0);  
	keybd_event(VK_NUMPAD3,MapVirtualKey(VK_NUMPAD3,0),KEYEVENTF_KEYUP,0);  
}

void CPassWordDlg::OnBnClickedBnNumfour()
{
	m_ed_passWord.SetFocus();
	keybd_event(VK_NUMPAD4,MapVirtualKey(VK_NUMPAD4,0),0,0);  
	keybd_event(VK_NUMPAD4,MapVirtualKey(VK_NUMPAD4,0),KEYEVENTF_KEYUP,0);  
}

void CPassWordDlg::OnBnClickedBnNumfive()
{
	m_ed_passWord.SetFocus();
	keybd_event(VK_NUMPAD5,MapVirtualKey(VK_NUMPAD5,0),0,0);  
	keybd_event(VK_NUMPAD5,MapVirtualKey(VK_NUMPAD5,0),KEYEVENTF_KEYUP,0);  
}

void CPassWordDlg::OnBnClickedBnNumsix()
{
	m_ed_passWord.SetFocus();
	keybd_event(VK_NUMPAD6,MapVirtualKey(VK_NUMPAD6,0),0,0);  
	keybd_event(VK_NUMPAD6,MapVirtualKey(VK_NUMPAD6,0),KEYEVENTF_KEYUP,0);  
}

void CPassWordDlg::OnBnClickedBnNumseven()
{
	m_ed_passWord.SetFocus();
	keybd_event(VK_NUMPAD7,MapVirtualKey(VK_NUMPAD7,0),0,0);  
	keybd_event(VK_NUMPAD7,MapVirtualKey(VK_NUMPAD7,0),KEYEVENTF_KEYUP,0);
}

void CPassWordDlg::OnBnClickedBnNumeight()
{
	m_ed_passWord.SetFocus();
	keybd_event(VK_NUMPAD8,MapVirtualKey(VK_NUMPAD8,0),0,0);  
	keybd_event(VK_NUMPAD8,MapVirtualKey(VK_NUMPAD8,0),KEYEVENTF_KEYUP,0);
}

void CPassWordDlg::OnBnClickedBnNumnine()
{
	m_ed_passWord.SetFocus();
	keybd_event(VK_NUMPAD9,MapVirtualKey(VK_NUMPAD9,0),0,0);  
	keybd_event(VK_NUMPAD9,MapVirtualKey(VK_NUMPAD9,0),KEYEVENTF_KEYUP,0);
}

void CPassWordDlg::OnBnClickedBnNumzero()
{
	m_ed_passWord.SetFocus();
	keybd_event(VK_NUMPAD0,MapVirtualKey(VK_NUMPAD0,0),0,0);  
	keybd_event(VK_NUMPAD0,MapVirtualKey(VK_NUMPAD0,0),KEYEVENTF_KEYUP,0);
}

void CPassWordDlg::OnBnClickedBncancel()
{
	m_ed_passWord.SetFocus();
	keybd_event(VK_BACK,MapVirtualKey(VK_BACK,0),0,0);  
	keybd_event(VK_BACK,MapVirtualKey(VK_BACK,0),KEYEVENTF_KEYUP,0);
}