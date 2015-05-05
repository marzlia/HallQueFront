// WndContentDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WndContentDlg.h"


// CWndContentDlg 对话框

IMPLEMENT_DYNAMIC(CWndContentDlg, CDialog)

CWndContentDlg::CWndContentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWndContentDlg::IDD, pParent)
{

}

CWndContentDlg::~CWndContentDlg()
{
}

void CWndContentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWndContentDlg, CDialog)
END_MESSAGE_MAP()


// CWndContentDlg 消息处理程序
