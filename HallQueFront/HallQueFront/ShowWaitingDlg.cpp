// ShowWaitingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "ShowWaitingDlg.h"


// CShowWaitingDlg 对话框

IMPLEMENT_DYNAMIC(CShowWaitingDlg, CDialog)

CShowWaitingDlg::CShowWaitingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowWaitingDlg::IDD, pParent)
{

}

CShowWaitingDlg::~CShowWaitingDlg()
{
}

void CShowWaitingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STA_SHOWWAITPIC, m_sta_showWaitPic);
}


BEGIN_MESSAGE_MAP(CShowWaitingDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CShowWaitingDlg 消息处理程序

BOOL CShowWaitingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_picImage.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_LOGO));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CShowWaitingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rect;
	m_sta_showWaitPic.GetWindowRect(rect);
	ScreenToClient(rect);
	SetStretchBltMode(dc, COLORONCOLOR);//避免颜色失真
	if(!m_picImage.IsNull())
		m_picImage.StretchBlt(dc,rect.left,rect.top,rect.Width(),rect.Height(),SRCCOPY);
}
