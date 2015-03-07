// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "TrackDef.h"
#include "MainFrm.h"
#include "HallQueFrontDoc.h"//注意包含view时要先包含doc
#include "HallQueFrontView.h"
#include "CommonStrMethod.h"

#define  AUTOSHUTDOWN 9673

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(IDI_ICON_LEFT,&CMainFrame::OnIconLeft)
	ON_COMMAND(IDI_ICON_MIDDLE,&CMainFrame::OnIconMiddle)
	ON_COMMAND(IDI_ICON_RIGHT,&CMainFrame::OnIconRight)
	ON_COMMAND(IDI_ICON_WIDTH,&CMainFrame::OnIconWidth)
	ON_COMMAND(IDI_ICON_HEIGHT,&CMainFrame::OnIconHeight)
	ON_COMMAND(IDI_ICON_SAMESIZE,&CMainFrame::OnIconSameSize)
	ON_COMMAND(IDI_ICON_TOP,&CMainFrame::OnIconTop)
	ON_COMMAND(IDI_ICON_BOTTOM,&CMainFrame::OnIconBottom)
	ON_COMMAND(IDI_ICON_HORI,&CMainFrame::OnIconHor)
	ON_COMMAND(IDI_ICON_VERI,&CMainFrame::OnIconVer)
	
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_MESSAGE(WM_MY_TRAYICON, &CMainFrame::OnTrayIcon)//界面隐藏
	ON_COMMAND(ID_TIP_SHOW, &CMainFrame::OnTipShow)
	ON_COMMAND(ID_TIP_EXIT, &CMainFrame::OnTipExit)
//	ON_COMMAND(ID_HIDEMAIN, &CMainFrame::OnHidemain)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame() : 
	m_bFullScreen(FALSE)
{
	// TODO: 在此添加成员初始化代码
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	///工具栏创建
	
	m_imageList.Create(16,16,ILC_COLOR32|ILC_MASK,0,0);
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_LEFT));//0
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_MIDDLE));//1
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_RIGHT));//2
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_TOP));//6
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_BOTTOM));//7
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_WIDTH));//3
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_HEIGHT));//4
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SAMESIZE));//5
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_HORI));//8
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_VERI));//9

	UINT btnIDs[12]; 
	btnIDs[0]=IDI_ICON_LEFT;
	btnIDs[1]=IDI_ICON_MIDDLE;
	btnIDs[2]=IDI_ICON_RIGHT;
	btnIDs[3]=IDI_ICON_TOP;
	btnIDs[4]=IDI_ICON_BOTTOM;
	btnIDs[5]=ID_SEPARATOR;
	btnIDs[6]=IDI_ICON_WIDTH;
	btnIDs[7]=IDI_ICON_HEIGHT;
	btnIDs[8]=IDI_ICON_SAMESIZE;
	btnIDs[9]=ID_SEPARATOR;
	btnIDs[10]=IDI_ICON_HORI;
	btnIDs[11]=IDI_ICON_VERI;

	m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_NOALIGN
		| CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBar.SetButtons(btnIDs,12);
	m_wndToolBar.SetButtonText(0,toolLeftText);
	m_wndToolBar.SetButtonText(1,toolMiddleText);
	m_wndToolBar.SetButtonText(2,toolRightText);
	m_wndToolBar.SetButtonText(3,toolTopText);
	m_wndToolBar.SetButtonText(4,toolBottomText);
	m_wndToolBar.SetButtonText(6,toolWidthText);
	m_wndToolBar.SetButtonText(7,toolHeightText);
	m_wndToolBar.SetButtonText(8,toolSameSizeText);
	m_wndToolBar.SetButtonText(10,toolHorText);
	m_wndToolBar.SetButtonText(11,toolVerText);
	
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	/*
	TBMETRICS tbmer;
	tbmer.cbSize=sizeof(TBMETRICS);
	tbmer.dwMask=TBMF_BUTTONSPACING;
	tbmer.cxButtonSpacing=5;//设置间隔
	m_wndToolBar.GetToolBarCtrl().SetMetrics(&tbmer);
	*/
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imageList);
	CRect temp;
	m_wndToolBar.GetItemRect(0,&temp);
	m_wndToolBar.SetSizes(CSize(temp.Width(),temp.Height()),CSize(16,16));   
	//Make the toolbar dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndToolBar);

	////////////////////////////去掉默认菜单栏
	SetMenu(NULL);
	DestroyMenu(m_hMenuDefault);
	///////////////////////////
	LONG style=::GetWindowLong(this->m_hWnd,GWL_STYLE);//取消标题栏
	style &= ~WS_CAPTION;
	SetWindowLong(m_hWnd,GWL_STYLE,style);

	
	
//	GetWindowPlacement(&m_oldWindowPlaceMent);
	

//	FullScreen();
	///////////////////////////////////////
	theApp.ReadLogicVariablesFromFile();

	SetTimer(AUTOSHUTDOWN,1000,NULL);
	///////////////////////
	DeleteLogFile();
	//////////////////////
	AddTrayIcon();
	///////////////
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style &= ~WS_TILED;
	cs.style &= ~WS_SIZEBOX;
	cs.style &= ~WS_BORDER;
	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::FullScreen()
{
	if(m_bFullScreen)
	{
		return;
	}
 	CRect windowRect;
 	GetWindowRect(&windowRect);
 	m_windowRect=windowRect;
 	CRect ClientRect;
 	//参见相关文档关于RepositionBars的用法
 	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery,&ClientRect);
 	ClientToScreen(ClientRect);
	//获取屏幕尺寸
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);

	m_FullScreenRect.left = windowRect.left - ClientRect.left;
	m_FullScreenRect.top = windowRect.top - ClientRect.top;
	m_FullScreenRect.right = nFullWidth;//windowRect.right - ClientRect.right + nFullWidth;
	m_FullScreenRect.bottom = nFullHeight;//windowRect.bottom - ClientRect.bottom + nFullHeight;
	m_bFullScreen = TRUE;
/*
	WINDOWPLACEMENT wndpl;
	wndpl.length=sizeof(WINDOWPLACEMENT);
	wndpl.flags=0;
	wndpl.showCmd=SW_SHOWNORMAL;
	wndpl.rcNormalPosition=m_FullScreenRect;
	//设置窗口的状态及最大、小的位置
	SetWindowPlacement(&wndpl);
*/	
	MoveWindow(&m_FullScreenRect);
	///隐藏工具烂
//	ShowControlBar(&m_wndToolBar,FALSE,FALSE);
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if(m_bFullScreen)
	{
		lpMMI->ptMaxSize.x=m_FullScreenRect.Width();
		lpMMI->ptMaxSize.y=m_FullScreenRect.Height();
		lpMMI->ptMaxPosition.x=m_FullScreenRect.Width();
		lpMMI->ptMaxPosition.y=m_FullScreenRect.Height();
		// 最大的Track尺寸也要改变
		lpMMI->ptMaxTrackSize.x=m_FullScreenRect.Width();
		lpMMI->ptMaxTrackSize.y=m_FullScreenRect.Height();
	}
	
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::EndFullScreen()
{
	if(m_bFullScreen)
	{
		  //退出全屏显示, 恢复原窗口显示	
		MoveWindow(&m_windowRect);
		m_bFullScreen = FALSE;
//		ShowWindow(SW_SHOW);
		ShowWindow(SW_MAXIMIZE);
		ShowWindow(SW_SHOW);
		//显示工具栏
//		ShowControlBar(&m_wndToolBar,TRUE,FALSE);
		UpdateWindow();
	}
}

void CMainFrame::OnIconLeft()
{
	CHallQueFrontView* pView = (CHallQueFrontView*)GetActiveView();
	pView->m_pTrackCtrl->AlignLeftCtr();
}

void CMainFrame::OnIconMiddle()
{
	CHallQueFrontView* pView = (CHallQueFrontView*)GetActiveView();
	pView->m_pTrackCtrl->DoMiddleCtr();
}

void CMainFrame::OnIconRight()
{
	CHallQueFrontView* pView = (CHallQueFrontView*)GetActiveView();
	pView->m_pTrackCtrl->AlignRightCtr();
}

void CMainFrame::OnIconWidth()
{
	CHallQueFrontView* pView = (CHallQueFrontView*)GetActiveView();
	pView->m_pTrackCtrl->SameWidthCtr();
}

void CMainFrame::OnIconHeight()
{
	CHallQueFrontView* pView = (CHallQueFrontView*)GetActiveView();
	pView->m_pTrackCtrl->SameHeightCtr();
}

void CMainFrame::OnIconSameSize()
{
	CHallQueFrontView* pView = (CHallQueFrontView*)GetActiveView();
	pView->m_pTrackCtrl->SameSizeCtr();
}

void CMainFrame::OnIconTop()
{
	CHallQueFrontView* pView = (CHallQueFrontView*)GetActiveView();
	pView->m_pTrackCtrl->AlignTopCtr();
}

void CMainFrame::OnIconBottom()
{
	CHallQueFrontView* pView = (CHallQueFrontView*)GetActiveView();
	pView->m_pTrackCtrl->AlignBottomCtr();
}

void CMainFrame::OnIconHor()
{
	CHallQueFrontView* pView = (CHallQueFrontView*)GetActiveView();
	pView->m_pTrackCtrl->DivideHorCtr();
}

void CMainFrame::OnIconVer()
{
	CHallQueFrontView* pView = (CHallQueFrontView*)GetActiveView();
	pView->m_pTrackCtrl->DivideVerCtr();
}
void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// TODO: 在此添加专用代码和/或调用基类
//	SetWindowText(_T("智能排队软件v1.1"));
	CFrameWnd::OnUpdateFrameTitle(bAddToTitle);
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	// TODO: 在此处添加消息处理程序代码
	if(!m_bFullScreen)
		ShowWindow(SW_MAXIMIZE);
	FullScreen();

	//移动工具栏到中间
	CRect ToolBarRect;
	m_wndToolBar.GetWindowRect(&ToolBarRect);
	m_wndToolBarRect = ToolBarRect;
// 	CRect windowRect;
// 	GetWindowRect(&windowRect);

	CRect destRect;
	destRect.left = m_windowRect.Width()/2 - ToolBarRect.Width()/2;
	destRect.right = m_windowRect.Width()/2 + ToolBarRect.Width()/2;


	destRect.top = ToolBarRect.top;//ToolBarRect.top;
	destRect.bottom = ToolBarRect.bottom;//ToolBarRect.bottom;
	m_wndToolBar.MoveWindow(destRect);
	///////////////////////////////////////
	theApp.m_pView->GetBackDC();
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case AUTOSHUTDOWN:
		CTime currTime = CTime::GetCurrentTime();
		if(theApp.m_logicVariables.IsAutoCloseComputer)
		{
			CString strCurrtime;
			strCurrtime.Format(_T("%d:%d:%d"),currTime.GetHour(),currTime.GetMinute(),
				currTime.GetSecond());
			CString strCloseComputerTime;
			strCloseComputerTime.Format(_T("%d:%d:%d"),theApp.m_logicVariables.CloseComputerTime.GetHour(),
				theApp.m_logicVariables.CloseComputerTime.GetMinute(),
				theApp.m_logicVariables.CloseComputerTime.GetSecond());
			if(strCurrtime==strCloseComputerTime)
			{
				ShutDown();
			}
		}
		break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}

BOOL CMainFrame::ShutDown()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		return FALSE;
	LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);

	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;

	if(!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMainFrame::DeleteLogFile()
{
	CDoFile doFile;
	CString path = doFile.GetExeFullFilePath();
	path+=_T("\\log\\");
	return doFile.MyDeleteDirectory(path);
}


void CMainFrame::AddTrayIcon()
{
	CString strTip = _T("智能排队");
	TaskBarAddIcon(this->m_hWnd, IDR_MAINFRAME, m_hIcon, (LPCWSTR)strTip.GetBuffer(0));
	strTip.ReleaseBuffer();
}

BOOL CMainFrame::TaskBarAddIcon(HWND hwnd, UINT uID, HICON hIcon, LPCWSTR lpszTip)
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

BOOL CMainFrame::TaskBarDeleteIcon(HWND hwnd, UINT uID)
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = uID;
	return Shell_NotifyIcon(NIM_DELETE, &nid);
}

void CMainFrame::RemoveTrayIcon()
{
	TaskBarDeleteIcon(this->m_hWnd, IDR_MAINFRAME);
}

LRESULT CMainFrame::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME)
	{
		return 1;
	}
	switch(lParam)
	{
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		{
			ShowWindow(SW_SHOW);
			UpdateWindow();
		}
		break;
	case WM_RBUTTONUP:
		{
			SetForegroundWindow();//点击界面其他位置右下角弹出菜单消失
			CMenu menu;
			menu.LoadMenu(IDR_TIPMENU);
			CMenu   *pContextMenu=menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单 
			CPoint point;//定义一个用于确定光标位置的位置  
			GetCursorPos(&point);//获取当前光标的位置，以便使得菜单可以跟随光标  
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,AfxGetMainWnd()); //在指定位置显示弹出菜单
		}
		break;
	}
	return 0;
}

void CMainFrame::OnTipShow()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOW);
	UpdateWindow();
}

void CMainFrame::OnTipExit()
{
	// TODO: 在此添加命令处理程序代码
	if(IDOK==MessageBox(_T("确定退出吗?"),_T("警告"),MB_OKCANCEL | MB_ICONINFORMATION))
	{
		RemoveTrayIcon();
		DestroyWindow();
	}
}

