// HallQueFrontServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFrontServer.h"
#include "HallQueFrontServerDlg.h"
#include "ConnectToMySql.h"
#include "../HallQueFront/CommonConvert.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CHallQueFrontServerDlg 对话框




CHallQueFrontServerDlg::CHallQueFrontServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHallQueFrontServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAINFRAME);
}

void CHallQueFrontServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_SERVERIP, m_ed_serverIP);
	DDX_Control(pDX, IDC_ED_USERNAME, m_ed_userName);
	DDX_Control(pDX, IDC_ED_PASS, m_ed_userPass);
	DDX_Control(pDX, IDC_ED_SERVERPORT, m_ed_serverPort);
	DDX_Control(pDX, IDC_ED_DATABASEPORT, m_dataBasePort);
}

BEGIN_MESSAGE_MAP(CHallQueFrontServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CHallQueFrontServerDlg::OnBnClickedOk)
	ON_MESSAGE(WM_MY_TRAYICON, OnTrayIcon)
	ON_COMMAND(ID_MENU_SHOW, &CHallQueFrontServerDlg::OnMenuShow)
	ON_COMMAND(ID_MENU_QUIT, &CHallQueFrontServerDlg::OnMenuQuit)
	ON_WM_CLOSE()
	ON_WM_QUERYOPEN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CHallQueFrontServerDlg 消息处理程序

BOOL CHallQueFrontServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
// 	CConnectToMySql mySql;
// 	int i_port = 0;
// 	CCommonConvert::CStringToint(i_port,m_dbaConfig.GetDBPort());
// 	mySql.ConnectToDB(m_dbaConfig.GetServerIP(),
// 		i_port,m_dbaConfig.GetServerAcount(),
// 		m_dbaConfig.GetServerPassword());
	//
	m_selectServer.InitServer();
	/////////////////////////////////
	m_ed_serverIP.SetWindowText(m_dbaConfig.GetServerIP());
	m_ed_serverPort.SetWindowText(m_dbaConfig.GetServerPort());
	m_ed_userName.SetWindowText(m_dbaConfig.GetServerAcount());
	m_ed_userPass.SetWindowText(m_dbaConfig.GetServerPassword());
	m_dataBasePort.SetWindowText(m_dbaConfig.GetDBPort());
	//////
	AddTrayIcon();
	/////

	SetTimer(11,10,NULL);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHallQueFrontServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
// 		if(nID == SC_MINIMIZE)
// 		{
// 			ShowWindow(SW_HIDE);
// 			UpdateWindow();
// 		}
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHallQueFrontServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHallQueFrontServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHallQueFrontServerDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString userName,userPass,userIP,userPort,DBPort;
	m_ed_userName.GetWindowText(userName);
	m_dbaConfig.SetServerAcount(userName);
	m_ed_serverIP.GetWindowText(userIP);
	m_dbaConfig.SetServerIP(userIP);
	m_ed_serverPort.GetWindowText(userPort);
	m_dbaConfig.SetServerPort(userPort);
	m_ed_userPass.GetWindowText(userPass);
	m_dbaConfig.SetServerPassword(userPass);
	m_dataBasePort.GetWindowText(DBPort);
	m_dbaConfig.SetDBPort(DBPort);
	m_dbaConfig.Save();
	////////////////////////////链接数据库
	/*
	CADODatabase dataBase;
	m_strConn.Format(_T("Driver={SQL Server};Server=%s;Database=InfoMan;Uid=%s;Pwd=%s;"),
		m_dbaConfig.GetServerIP(),
		m_dbaConfig.GetServerAcount(), m_dbaConfig.GetServerPassword());
	dataBase.SetConnectionString(m_strConn);
	try
	{
	if(dataBase.Open())
	{
		MessageBox(_T("连接成功"),_T("注意"),MB_OK|MB_ICONINFORMATION);
		dataBase.Close();
	}
	else
	{
		MessageBox(_T("连接数据库失败"),_T("注意"),MB_OK|MB_ICONINFORMATION);
	}
	}
	catch(...)
	{
		MessageBox(_T("连接数据库失败"),_T("注意"),MB_OK|MB_ICONINFORMATION);
		return;
	}
	*/
	CConnectToMySql mySql;
	int i_port = 0;
	CCommonConvert::CStringToint(i_port,DBPort);
	if(mySql.ConnectToDB(userIP,i_port,userName,userPass))
	{
		MessageBox(_T("数据库链接成功"),_T("注意"),MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(_T("数据库链接失败"),_T("注意"),MB_OK|MB_ICONINFORMATION);
	}
}


void CHallQueFrontServerDlg::AddTrayIcon(void)
{
	wchar_t tip[128] = {0};
	wsprintf(tip, _T("智能排队系统服务端"));
	//wchar_t tip[] = _T("排队系统硬件驱动程式\0");
	//CHAR tip[] = "排队对接\0";
	TaskBarAddIcon(this->m_hWnd, IDI_MAINFRAME, m_hIcon, (LPCWSTR)tip);
}

BOOL CHallQueFrontServerDlg::TaskBarAddIcon(HWND hwnd, UINT uID, HICON hIcon, LPCWSTR lpszTip)
{
	BOOL res;
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = uID;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
	nid.uCallbackMessage = WM_MY_TRAYICON;
	nid.hIcon = hIcon;
	if (lpszTip)
	{
		//lstrcpyn(nid.szTip, lpszTip, sizeof(nid.szTip));
		wcscpy_s(nid.szTip, sizeof(nid.szTip), lpszTip);
	}
	else
	{
		nid.szTip[0] = '\0';
	}
	res = Shell_NotifyIcon(NIM_ADD, &nid);

	if (hIcon)
	{
		DestroyIcon(hIcon);
	}
	return res;
}

BOOL CHallQueFrontServerDlg::TaskBarDeleteIcon(HWND hwnd, UINT uID)
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = uID;
	return Shell_NotifyIcon(NIM_DELETE, &nid);
}

void CHallQueFrontServerDlg::RemoveTrayIcon()
{
	TaskBarDeleteIcon(this->m_hWnd, IDI_MAINFRAME);
}

LRESULT CHallQueFrontServerDlg::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDI_MAINFRAME)
	{
		return 1;
	}
	switch(lParam)
	{
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		{
			this->ShowWindow(SW_SHOW);
			this->ShowWindow(SW_NORMAL);
		}
		break;
	case WM_RBUTTONUP:
		{
			SetForegroundWindow();//点击界面其他位置右下角弹出菜单消失
			CMenu menu;
			menu.LoadMenu(IDR_MYMENU);
			CMenu   *pContextMenu=menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单 
			CPoint point;//定义一个用于确定光标位置的位置  
			GetCursorPos(&point);//获取当前光标的位置，以便使得菜单可以跟随光标  
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,AfxGetMainWnd()); //在指定位置显示弹出菜单
		}
		break;
	}
	return 0;
}

void CHallQueFrontServerDlg::OnMenuShow()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_NORMAL);
}

void CHallQueFrontServerDlg::OnMenuQuit()
{
	// TODO: 在此添加命令处理程序代码
	if(IDOK==MessageBox(_T("确定退出吗?"),_T("警告"),MB_OKCANCEL | MB_ICONINFORMATION))
	{
		RemoveTrayIcon();
		DestroyWindow();
	}
}

void CHallQueFrontServerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);
//	CDialog::OnClose();
}

BOOL CHallQueFrontServerDlg::OnQueryOpen()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return CDialog::OnQueryOpen();
}

LRESULT CHallQueFrontServerDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	LONG lRes = CDialog::DefWindowProc(message, wParam, lParam);
	switch(message)
	{
	case WM_SYSCOMMAND:
		if(wParam == SC_MINIMIZE)
		{
			ShowWindow(SW_HIDE);
		}
		break;
	}
	return lRes;
}

void CHallQueFrontServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 11:
		ShowWindow(SW_HIDE);
		UpdateWindow();
		KillTimer(11);
		break;
	}
	CDialog::OnTimer(nIDEvent);
}