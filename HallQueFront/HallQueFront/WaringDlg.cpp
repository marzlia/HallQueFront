// WaringDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "WaringDlg.h"


// CWaringDlg 对话框

IMPLEMENT_DYNAMIC(CWaringDlg, CDialog)

CWaringDlg::CWaringDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaringDlg::IDD, pParent)
{

}

CWaringDlg::~CWaringDlg()
{
}

void CWaringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STA_SHOWPIC, m_sta_showPic);
}


BEGIN_MESSAGE_MAP(CWaringDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CWaringDlg 消息处理程序

void CWaringDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	if(theApp.m_pView->m_showVaria.bShowNoPage)
	{
		if(!m_picImage.IsNull())
			m_picImage.Destroy();
		m_picImage.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_NOPAGE));
	}
	else if(theApp.m_pView->m_showVaria.bShowWait)
	{
		if(!m_picImage.IsNull())
			m_picImage.Destroy();
		m_picImage.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_WAIT));
	}
	else if (theApp.m_pView->m_showVaria.bSendMsg)
	{
		if(!m_picImage.IsNull())
			m_picImage.Destroy();
		m_picImage.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_SENDMSG));
	}
	CRect rect;
	m_sta_showPic.GetWindowRect(rect);
	ScreenToClient(rect);
	SetStretchBltMode(dc, COLORONCOLOR);//避免颜色失真
	if(!m_picImage.IsNull())
		m_picImage.StretchBlt(dc,rect.left,rect.top,rect.Width(),rect.Height(),SRCCOPY);
}

BOOL CWaringDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_picImage.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_WAIT));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CWaringDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialog::PostNcDestroy();
}
