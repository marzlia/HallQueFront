// HallQueFrontView.cpp : CHallQueFrontView 类的实现
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "DoComInOut.h"
#include "HallQueFrontDoc.h"
#include "HallQueFrontView.h"
#include "MainFrm.h"
#include "PropEditing.h"
#include "CommonConvert.h"
#include "PrintSetDlg.h"
#include "SLZController.h"
#include "QueSetDlg.h"
#include "StaffSet.h"
#include "PropConnectInfo.h"
#include "SLZWindowSetDlg.h"
#include "CardConfigQue.h"
#include "PassWordDlg.h"
#include "ShowWaitingDlg.h"
#include "SLZCWndScreen.h"
#include "CommonStrMethod.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////保存背景内存DC
CDC*         g_pBackDC = new CDC;
HBITMAP  g_hBackMemBitmap = NULL;
HBITMAP   g_hOldBackMemBitmap;
UINT g_nPageID;//界面ID
///////////////////////////////////
extern void MyWriteConsole(CString str);
// CHallQueFrontView

IMPLEMENT_DYNCREATE(CHallQueFrontView, CView)

BEGIN_MESSAGE_MAP(CHallQueFrontView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_RBUTTONDOWN()
	
	ON_COMMAND(ID_QUITMANAGE, &CHallQueFrontView::OnQuitmanage)
	ON_COMMAND(ID_MANAGE, &CHallQueFrontView::OnManage)
	ON_COMMAND(ID_ADDBUTTON, &CHallQueFrontView::OnAddbutton)
	ON_COMMAND(ID_ADDTEXT, &CHallQueFrontView::OnAddtext)
	ON_COMMAND(ID_ADDPIC, &CHallQueFrontView::OnAddpic)
	ON_COMMAND(ID_QUITHALLQUE, &CHallQueFrontView::OnQuithallque)
	ON_COMMAND(ID_QUITSYS, &CHallQueFrontView::OnQuitsys)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_ERASEBACKPIC, &CHallQueFrontView::OnErasebackpic)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDITCTR, &CHallQueFrontView::OnEditctr)
	ON_COMMAND(ID_LINKCTR, &CHallQueFrontView::OnLinkctr)
	ON_COMMAND(ID_DELETECTR, &CHallQueFrontView::OnDeletectr)
//	ON_COMMAND(ID_SYSSETING, &CHallQueFrontView::OnSysseting)
	ON_COMMAND(ID_SHOW_TIME, &CHallQueFrontView::OnShowTime)
	ON_COMMAND(ID_SHOW_QUENUM, &CHallQueFrontView::OnShowQuenum)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND_RANGE(10001,10001+BUTTONNUM,&CHallQueFrontView::OnButtonClick)
	ON_COMMAND(ID_QUESET, &CHallQueFrontView::OnQueset)
	ON_COMMAND(ID_STAFFSET, &CHallQueFrontView::OnStaffset)
	ON_COMMAND(ID_WINDOWSET, &CHallQueFrontView::OnWindowset)
	ON_COMMAND(ID_PRINTSET, &CHallQueFrontView::OnPrintset)
	ON_COMMAND(ID_CARDSET, &CHallQueFrontView::OnCardset)
	ON_COMMAND(ID_COMPREHSET, &CHallQueFrontView::OnComprehset)
	ON_COMMAND(ID_STBSET, &CHallQueFrontView::OnStbset)
	ON_MESSAGE(WM_SHOWMSG,&CHallQueFrontView::OnMyShowMessage)
	ON_MESSAGE(WM_SHOWPAGE,&CHallQueFrontView::OnMyShowPage)
	ON_COMMAND(ID_VIEWMINSIZE, &CHallQueFrontView::OnViewminsize)
	ON_COMMAND(ID_LEADINWAV, &CHallQueFrontView::OnLeadinwav)
	ON_COMMAND(ID_HIDEMAIN, &CHallQueFrontView::OnHidemain)
END_MESSAGE_MAP()

// CHallQueFrontView 构造/析构

CHallQueFrontView::CHallQueFrontView() : 
m_workTimeOut(_T("停止取票"))
, m_pWaringDlg(NULL)
, m_pShowPageDlg(NULL)
, m_pTrackCtrl(NULL)
{
	// TODO: 在此处添加构造代码
	m_isManage=FALSE;
	m_picType=_T("files(*.jpg,*.jpeg,*.bmp,*png,*gif)|*.jpg;*.jpeg;*.bmp;*png;*gif||");
	m_strPicFilter = _T("语音文件(*.wav)|*.wav||");
	m_isEdit = FALSE;
	m_isAddPic = FALSE;
	m_isAddText = FALSE;
	m_isAddButton = FALSE;
	m_isShowTime = FALSE;
	m_isShowQueNum = FALSE;
//	m_showVaria.bShowNoPage = TRUE;
	memset(&m_showVaria,0,sizeof(m_showVaria));
	CDoFile doFile;
	m_strPageFilePath = doFile.GetExeFullFilePath();
	m_strPageFilePath+=_T("\\page.dat");
}

CHallQueFrontView::~CHallQueFrontView()
{
	ClearPage();
	if(g_pBackDC)
	{
		DeleteObject(g_pBackDC->GetSafeHdc());
		delete g_pBackDC;
		g_pBackDC = NULL;
	}
	theApp.m_pView = NULL;
	if(m_pWaringDlg)
	{
		m_pWaringDlg->DestroyWindow();
		delete m_pWaringDlg;
		m_pWaringDlg = NULL;
	}
	if(m_pShowPageDlg)
	{
		m_pShowPageDlg->DestroyWindow();
		delete m_pShowPageDlg;
		m_pShowPageDlg = NULL;
	}
}

BOOL CHallQueFrontView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	return CView::PreCreateWindow(cs);
}

// CHallQueFrontView 绘制

void CHallQueFrontView::OnDraw(CDC* pDC)
{
	/////调正显示等待，缺纸窗口位置
 	CRect rect;
 	GetClientRect(rect);
	if(m_pWaringDlg && m_pWaringDlg->m_hWnd)
 		m_pWaringDlg->MoveWindow(rect.left+rect.Width()/2-m_rWaringRect.Width()/2,rect.top+rect.Height()/2-m_rWaringRect.Height()/2,m_rWaringRect.Width(),m_rWaringRect.Height(),FALSE);
	MyDraw(pDC);
	////////////////////////////////
	CHallQueFrontDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此处为本机数据添加绘制代码
}


// CHallQueFrontView 打印

BOOL CHallQueFrontView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHallQueFrontView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CHallQueFrontView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CHallQueFrontView 诊断

#ifdef _DEBUG
void CHallQueFrontView::AssertValid() const
{
	CView::AssertValid();
}

void CHallQueFrontView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHallQueFrontDoc* CHallQueFrontView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHallQueFrontDoc)));
	return (CHallQueFrontDoc*)m_pDocument;
}
#endif //_DEBUG


// CHallQueFrontView 消息处理程序

void CHallQueFrontView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt;
	GetCursorPos(&pt);
	int result = m_pTrackCtrl->Track( point,nFlags);
	if(result == TRACK_CTR_RBUTTON)
	{
		CMenu menu;
		menu.LoadMenu(IDR_MENU_EDITDTAILS);
		CMenu *pContentMenu=menu.GetSubMenu(0);
//		if(m_pTrackCtrl->m_pRightBnSelect->GetWindowType() != enmButton)
//		{
			pContentMenu->EnableMenuItem(ID_LINKCTR,MF_GRAYED);
//		}
		pContentMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this); //在指定位置显示弹出菜单
		pContentMenu->DestroyMenu();
	}
	else
	{
		if(m_isManage)
		{
			CMenu menu;
			menu.LoadMenu(IDR_MENU_MANAGE);
			CMenu *pContentMenu=menu.GetSubMenu(0);
			pContentMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this); //在指定位置显示弹出菜单
			pContentMenu->DestroyMenu();
		}
		else
		{
			CMenu menu;
			menu.LoadMenu(IDR_MENU_NOMANAGE);
			CMenu *pContentMenu=menu.GetSubMenu(0);
			pContentMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this); //在指定位置显示弹出菜单
			pContentMenu->DestroyMenu();
		}
	}
	CView::OnRButtonDown(nFlags, point);
}


void CHallQueFrontView::OnQuitmanage()
{
	// TODO: 在此添加命令处理程序代码
	if(!m_isManage)
	{
		return;
	}
	m_isManage=FALSE;
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRect rect;
	pFrame->m_wndToolBar.GetWindowRect(&rect);
	list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
	for(itera;itera!=m_list_trackCtrl.end();itera++)
	{
		CTrackContrl* pTrackContrl = *itera;
		ASSERT(pTrackContrl!=NULL);
		pTrackContrl->SetAllCtrContralEnable();
		if(pTrackContrl->GetSerialID()!=m_pTrackCtrl->GetSerialID())
		{
			pTrackContrl->SetAllCtrlHide();
		}
		pTrackContrl->AlterAllCtrTop(rect.Height());
	}
	
	pFrame->FullScreen();
	//////////////////保存背景DC
	GetBackDC();
	//////////////////////
	//////////////////////
	m_pShowPageDlg->ShowWindow(SW_HIDE);
	/////////////////////
	WritePageToFile();//写界面ID到文件，背景信息到文件
	WriteCtrlDataToList();//先把控件写到链表里面
	////////////////////
	//写文件
	WriteCtrInfoToFile();//写控件信息
	//写背景信息
//	WriteBackPicPathToFile();
}

void CHallQueFrontView::OnManage()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(m_isManage)
	{
		return;
	}
	CPassWordDlg PassWordDlg(this);
	if(PassWordDlg.DoModal()==IDOK)
	{
		CRect rect;
		pFrame->m_wndToolBar.GetWindowRect(&rect);
		m_isManage=TRUE;
		list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
		for(itera;itera!=m_list_trackCtrl.end();itera++)
		{
			CTrackContrl* pTrackCtrl = *itera;
			ASSERT(pTrackCtrl!=NULL);
			pTrackCtrl->AlterAllCtrTop(-rect.Height());
			pTrackCtrl->SetAllCtrContralDisable();
		}
		//退出全屏
		pFrame->EndFullScreen();
		//////////////////保存背景DC
		GetBackDC();
		//////////////////////重画
		for(int i=0;i<m_pTrackCtrl->m_selTrackerAll.GetSize();i++)
		{
			m_pTrackCtrl->m_selTrackerAll[i]->Invalidate();
		}
		/////////////////////
		CRect clientRect;
		GetClientRect(clientRect);
		CRect newRect;
		newRect.left=clientRect.left;newRect.right=m_rShowPageRect.right;
		newRect.top=m_rShowPageRect.top+clientRect.Height()/2-m_rShowPageRect.Height()/2;
		newRect.bottom = newRect.top+m_rShowPageRect.Height();
		m_pShowPageDlg->MoveWindow(newRect,FALSE);
		m_pShowPageDlg->ShowWindow(SW_SHOWNORMAL);
		CButton* pButton = (CButton*)m_pShowPageDlg->GetDlgItem(IDC_BN_TIMEHIDE);
		if(pButton)
			pButton->SetWindowText(_T("<"));
	}
//	Invalidate();
}

void CHallQueFrontView::OnAddbutton()
{
	// TODO: 在此添加命令处理程序代码
	m_isEdit = FALSE;
	m_isAddButton = TRUE;
	m_isAddPic = FALSE;
	m_isAddText =FALSE;
	m_isShowTime = FALSE;
	m_isShowQueNum = FALSE;
	CPropEditing prop(_T("编辑"),this);
	prop.DoModal();
}

void CHallQueFrontView::OnAddtext()
{
	// TODO: 在此添加命令处理程序代码
	m_isEdit = FALSE;
	m_isAddButton = FALSE;
	m_isAddPic = FALSE;
	m_isAddText = TRUE;
	m_isShowTime = FALSE;
	m_isShowQueNum = FALSE;
	CPropEditing prop(_T("编辑"),this);
	prop.DoModal();
}

void CHallQueFrontView::OnAddpic()
{
	// TODO: 在此添加命令处理程序代码
	m_isEdit = FALSE;
	m_isAddButton = FALSE;
	m_isAddPic = TRUE;
	m_isAddText = FALSE;
	m_isShowTime = FALSE;
	m_isShowQueNum = FALSE;
	CPropEditing prop(_T("编辑"),this);
	prop.DoModal();
}

void CHallQueFrontView::OnQuithallque()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(theApp.GetMainWnd()->m_hWnd,WM_CLOSE,0,0);
//	theApp.m_Controller.WriteInlineDataToFile();//把上次排队数据写入文件
}

void CHallQueFrontView::OnQuitsys()
{
	// TODO: 在此添加命令处理程序代码
	CPassWordDlg PassWordDlg(this);
	if(PassWordDlg.DoModal()==IDOK)
	{
		::PostMessage(theApp.GetMainWnd()->m_hWnd,WM_CLOSE,0,0);
//		theApp.m_Controller.WriteInlineDataToFile();//把上i次没处理完的数据写入文件
	}
}

void CHallQueFrontView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pTrackCtrl->Track( point,nFlags,true );
	CView::OnLButtonDown(nFlags, point);
}

int CHallQueFrontView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码
	////为theApp.m_pView赋值
	theApp.m_pView = this;
	/////////////////////////////
	////////////////////////////初始化主界面
	m_pTrackCtrl = new CTrackContrl;
	m_pTrackCtrl->SetSerialID(0);
	m_pTrackCtrl->Create(this);
	m_list_trackCtrl.push_back(m_pTrackCtrl);
	////////////////////////////
//	ReadBackPicPathFromFile();
	/////为全局DC创建内存DC
	g_pBackDC->CreateCompatibleDC(NULL);
	///////////////////////////////////////////
	////初始化界面信息,从文件读出界面信息
	ReadPageFromFile();
	if(!MyLoadBackImage(m_pTrackCtrl->GetBackPath()))
	{
		CDoFile doFile;
		CString path = doFile.GetExeFullFilePath();
		path += _T("\\背景\\初始背景.jpg");
		MyLoadBackImage(path);
	}
	ReadCtrInfoFromFile();
	
	return 0;
}

BOOL CHallQueFrontView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( m_pTrackCtrl->SetCursor( nHitTest, message ) )
		return TRUE;
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CHallQueFrontView::OnErasebackpic()
{
	// TODO: 在此添加命令处理程序代码
	
	CFileDialog picdlg(TRUE, 0, 0, 4|2, m_picType);
	if(IDOK==picdlg.DoModal())
	{
		CString backPath = picdlg.GetPathName();
		m_pTrackCtrl->SetBackPath(backPath);//先设置背景图片路径
		MyLoadBackImage(m_pTrackCtrl->GetSerialID());//然后加载图片
		//////////////////保存背景DC
		GetBackDC();//保存全局DC
		//////////////////////
		Invalidate(TRUE);
	}
}

BOOL CHallQueFrontView::MyLoadBackImage(CString path)
{
	if(path.IsEmpty())
	{
		if(!m_backImage.IsNull())
			m_backImage.Destroy();
		return FALSE;
	}
	else
	{
		if(!m_backImage.IsNull())
			m_backImage.Destroy();
		HRESULT hResult = m_backImage.Load(path);
		if(FAILED(hResult))
		{
			return FALSE;
		}
		return TRUE;
	}
}

BOOL CHallQueFrontView::MyLoadBackImage(UINT nPageID)
{
	list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
	for(itera;itera!=m_list_trackCtrl.end();itera++)
	{
		CTrackContrl* pTrackContrl = *itera;
		ASSERT(pTrackContrl!=NULL);
		if(nPageID == pTrackContrl->GetSerialID())
		{
			if(!MyLoadBackImage(pTrackContrl->GetBackPath()))
			{
				CDoFile doFile;
				CString path = doFile.GetExeFullFilePath();
				path += _T("\\背景\\初始背景.jpg");
				return MyLoadBackImage(path);
			}
		}
	}
	return FALSE;
}

BOOL CHallQueFrontView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	CView::OnEraseBkgnd(pDC);
	return TRUE;
}

void CHallQueFrontView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	switch(nChar)
	{
	case VK_ESCAPE:
		OnQuitmanage();
		break;
	case VK_F1:
		OnManage();
		break;
	case VK_UP:
		m_pTrackCtrl->KeyUpMove();
		break;
	case VK_DOWN:
		m_pTrackCtrl->KeyDownMove();
		break;
	case VK_LEFT:
		m_pTrackCtrl->KeyLeftMove();
		break;
	case VK_RIGHT:
		m_pTrackCtrl->KeyRightMove();
		break;
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CHallQueFrontView::MyDraw(CDC* pDC)
{
	CRect destRect;
	GetClientRect(&destRect);
	if(m_backImage.IsNull())
	{
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->FillRect(&destRect, pDC->GetCurrentBrush());
		m_pTrackCtrl->Draw(pDC);
		return;	
	}
	//双缓冲绘制创建兼容DC
	HDC hTempDC = CreateCompatibleDC(pDC->GetSafeHdc());
	HBITMAP hTempBitmap = CreateCompatibleBitmap(pDC->GetSafeHdc(),destRect.Width(),destRect.Height());
	HBITMAP hOldTempBitmap = (HBITMAP)SelectObject(hTempDC,hTempBitmap);
	SetBkMode(hTempDC,TRANSPARENT);

	SetStretchBltMode(hTempDC, COLORONCOLOR);//避免颜色失真
	m_backImage.StretchBlt(hTempDC,0,0,destRect.Width(),destRect.Height(),SRCCOPY);
	CDC* pTempDC = CDC::FromHandle(hTempDC);
	m_pTrackCtrl->Draw(pTempDC);
	BitBlt(pDC->GetSafeHdc(),0,0,destRect.Width(),destRect.Height(),hTempDC,0,0,SRCCOPY);
	SelectObject(hTempDC,hOldTempBitmap);
	DeleteObject(hTempBitmap);
	DeleteDC(hTempDC);
	///////////////////////////
}

void CHallQueFrontView::GetBackDC()
{
	int nWidth = m_clientRect.Width();
	int nHeight = m_clientRect.Height();
	if(g_pBackDC)
	{
		HDC hDC = ::GetDC(this->m_hWnd);
		if(g_hBackMemBitmap == NULL)
		{
			g_hBackMemBitmap = CreateCompatibleBitmap(hDC,
				m_clientRect.Width(),m_clientRect.Height());
			g_hOldBackMemBitmap = (HBITMAP)g_pBackDC->
				SelectObject(g_hBackMemBitmap);
		}
		else
		{
			////释放
			SelectObject(g_pBackDC->GetSafeHdc(),g_hOldBackMemBitmap);
			DeleteObject(g_hBackMemBitmap);
			g_hBackMemBitmap = NULL;
			///创建
			g_hBackMemBitmap = CreateCompatibleBitmap(hDC,
				m_clientRect.Width(),m_clientRect.Height());
			g_hOldBackMemBitmap = (HBITMAP)g_pBackDC->
				SelectObject(g_hBackMemBitmap);
		}
		//避免颜色失真
		if(!m_backImage.IsNull())
		{
			g_pBackDC->SetStretchBltMode(COLORONCOLOR);
			m_backImage.StretchBlt(g_pBackDC->GetSafeHdc(),
				0,0,nWidth,nHeight,SRCCOPY);
		}
		else
		{
			FillRect(g_pBackDC->GetSafeHdc(),&m_clientRect,WHITE_BRUSH);
		}
	}
}

void CHallQueFrontView::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0,WS_CLIPCHILDREN | WS_CLIPSIBLINGS );
	CView::PreSubclassWindow();
}

void CHallQueFrontView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case FORSHOWTIME:
		{
			int count = m_pTrackCtrl->m_selTrackerAll.GetCount();
			CString time;
			CTime currentTime;
			currentTime = CTime::GetCurrentTime();
			for(int i=0;i<count;i++)
			{
				if(enmStatic==m_pTrackCtrl->m_selTrackerAll[i]->GetWindowType())
				{
					if(m_pTrackCtrl->m_selTrackerAll[i]->m_pTransStatic
						->GetIsShowTime())
					{
						switch(m_pTrackCtrl->m_selTrackerAll[i]->m_pTransStatic
							->GetTimeFormat())
						{
						case enumTimeChinese:
							time.Format(_T("%d年%d月%d日 %d时%d分%d秒"),
								currentTime.GetYear(),currentTime.GetMonth(),currentTime.GetDay(),
								currentTime.GetHour(),currentTime.GetMinute(),currentTime.GetSecond());
							break;
						case enumTimeYearMonthDay:
							time = currentTime.Format(_T("%Y-%m-%d"));
							break;
						case enumTimeHourMinSec:
							time = currentTime.Format(_T("%H:%M:%S"));
							break;
						case enumTimeYMDHMS:
							time = currentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
							break;
						}
						m_pTrackCtrl->m_selTrackerAll[i]->m_pTransStatic->
							SetWindowText(time);
						m_pTrackCtrl->m_selTrackerAll[i]->m_pTransStatic->
							Invalidate(TRUE);
					}
				}
			}
		}
		break;
	case WORKTIME:
		JudgeButtonWorkOut();//判断队列工作时间
		theApp.m_Controller.ClearSystemData();//判断清空系统排队数据时间
		break;
	case DOINITThROUGHSCREEN:
		{
			SLZCWndScreen* pWndScreen = SLZCWndScreen::GetInstance();
//			pWndScreen->DoThroughInitMsg();
			KillTimer(DOINITThROUGHSCREEN);
		}
		break;
	}
	CView::OnTimer(nIDEvent);
}

void CHallQueFrontView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnMouseMove(nFlags, point);
}

void CHallQueFrontView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	m_IsDown = FALSE;
	CView::OnLButtonUp(nFlags, point);
}

void CHallQueFrontView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_clientRect.left=0;
	m_clientRect.top=0;
	m_clientRect.right=cx;
	m_clientRect.bottom=cy;
}

void CHallQueFrontView::OnEditctr()
{
	// TODO: 在此添加命令处理程序代码
	m_isEdit = TRUE;
	m_isAddPic = FALSE;
	m_isAddButton = FALSE;
	m_isAddText = FALSE;
	m_isShowTime = FALSE;
	m_isShowQueNum = FALSE;
	CPropEditing prop(_T("编辑"),this);
	prop.DoModal();
}

void CHallQueFrontView::OnLinkctr()
{
	// TODO: 在此添加命令处理程序代码
}

void CHallQueFrontView::OnDeletectr()
{
	// TODO: 在此添加命令处理程序代码
	m_pTrackCtrl->DeleteRightBnSelectCtr();
}

BOOL CHallQueFrontView::WriteCtrInfoToFile()
{
	CCommonConvert convert;
	CString exepath = convert.GetExeFullFilePath();
	exepath += _T("\\wndctrinfo.dat");
	CFile file;
	CFileException e;
	WINDOWCTRINFO wndctrInfo;
	if(file.Open(exepath,CFile::modeWrite | CFile::modeCreate,&e))
	{
		POSITION pos=m_list_allCtrInfo.GetHeadPosition();
		while(pos)
		{
			wndctrInfo=m_list_allCtrInfo.GetNext(pos);
			file.Write(&wndctrInfo,sizeof(WINDOWCTRINFO));
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CHallQueFrontView::ReadCtrInfoFromFile()
{
	CCommonConvert convert;
	CString exepath = convert.GetExeFullFilePath();
	exepath += _T("\\wndctrinfo.dat");
	CFile file;
	if(file.Open(exepath,CFile::modeRead))
	{
		ULONGLONG count=file.GetLength()/sizeof(WINDOWCTRINFO);
		for(ULONGLONG i=0;i<count;i++)
		{
			WINDOWCTRINFO windowctrinfo;
			if(file.Read(&windowctrinfo,sizeof(WINDOWCTRINFO))>0)
			{
				m_list_allCtrInfo.AddTail(windowctrinfo);
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void CHallQueFrontView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	//////////////////////创建控件
	CreateAllCtr();
	/////////////////////
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
	CShowWaitingDlg *pShowWaitDlg = new CShowWaitingDlg();
	pShowWaitDlg->Create(IDD_DIALOG_WAITING,this);
	CRect showWaitRect;
	pShowWaitDlg->GetClientRect(showWaitRect);
	CRect newRect;
	newRect.left = nFullWidth/2-showWaitRect.Width()/2;
	newRect.top = nFullHeight/2-showWaitRect.Height()/2;
	newRect.right = newRect.left+showWaitRect.Width();
	newRect.bottom = newRect.top+showWaitRect.Height();
	pShowWaitDlg->MoveWindow(newRect,FALSE);
	/////////////////////
	pShowWaitDlg->ShowWindow(SW_SHOWNORMAL);
	pShowWaitDlg->UpdateWindow();
	SetCursor(LoadCursor(NULL,IDC_WAIT));
	////////////////启动数据处理
	theApp.m_Controller.Start();
	//等待DoComInout线程1秒,等待通屏读取通道的信息
	CDoComInOut* pDoComInout = CDoComInOut::GetInstance();
	WaitForSingleObject(pDoComInout->m_hReadCallerThread,2000);
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	pShowWaitDlg->DestroyWindow();delete pShowWaitDlg;pShowWaitDlg = NULL;
	/////////////////////////
	//////////////////保存背景DC
//	GetBackDC();
	SetTimer(FORSHOWTIME,1000,NULL);//显示界面时间
	SetTimer(WORKTIME,1000,NULL);//检测工作时间
	SetTimer(DOINITThROUGHSCREEN,5*1000,NULL);//

	//////////////////////启动警告窗口
	m_pWaringDlg = new CWaringDlg();
	m_pWaringDlg->Create(IDD_DIALOG_WARING,this);
	m_pWaringDlg->ShowWindow(SW_HIDE);
	m_pWaringDlg->GetClientRect(m_rWaringRect);
	/////////////////////showpagedlg窗口
	m_pShowPageDlg = new CShowPageDlg();
	m_pShowPageDlg->Create(IDD_DIALOG_PAGE,this);
	m_pShowPageDlg->ShowWindow(SW_HIDE);
	m_pShowPageDlg->GetClientRect(m_rShowPageRect);
	///////////////////////////////////////////////////////////////////
	theApp.m_Controller.InitShowInlineQueNum();//显示界面排队人数
}

void CHallQueFrontView::CreateAllCtr()
{
	WINDOWCTRINFO winfo;
	POSITION pos = m_list_allCtrInfo.GetHeadPosition();
	while(pos)
	{
		winfo = m_list_allCtrInfo.GetNext(pos);
		switch(winfo.windowType)
		{
		case enmButton:
			{
				CRect rect;
				rect.left=winfo.rectLeft;rect.right=winfo.rectRight;
				rect.top=winfo.rectTop;rect.bottom=winfo.rectBottom;
				CControlRect *pButton = new CControlRect(rect);			
				pButton->Create(enmButton,winfo.ctrTextContent,this,0,&rect,0,0,m_pTrackCtrl->GetBackPath());
				pButton->SetPageID(winfo.nPageID);//设置属于那个界面ID
				//设置颜色
				pButton->m_pTransButton->SetTextColor(winfo.textColor);
				pButton->m_pTransButton->SetBkColor(winfo.bkColor);
				pButton->m_pTransButton->SetFont(winfo.lfFont);
				pButton->m_pTransButton->SetButtonType(winfo.ButtonType);
				pButton->m_pTransButton->SetAlpha(winfo.nAlpha);
				pButton->m_pTransButton->SetBkPic(winfo.bkPicPath);
				CString address(winfo.address);//设置链接队列
				pButton->m_pTransButton->AttachToQueue(address);
				pButton->m_pTransButton->AttachToPage(winfo.nLinkPageID);//设置链接界面
				list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
				for(itera;itera!=m_list_trackCtrl.end();itera++)
				{
					CTrackContrl* pTrackContrl = *itera;
					ASSERT(pTrackContrl!=NULL);
					if(pTrackContrl->GetSerialID()==pButton->GetPageID())
					{
						pTrackContrl->Add(pButton);
					}
				}
				break;
			}
		case enmStatic:
			{
				CRect rect;
				rect.left=winfo.rectLeft;rect.right=winfo.rectRight;
				rect.top=winfo.rectTop;rect.bottom=winfo.rectBottom;
				if(winfo.IsForImage)//静态框for图片
				{
					CControlRect *pPic = new CControlRect(rect);
					pPic->Create(enmStatic,L"",this,SS_CENTERIMAGE,rect,0,0,
						winfo.staticPicPath);
					//设置透明度
					pPic->m_pTransStatic->SetAlpha(winfo.nAlpha);
					pPic->SetPageID(winfo.nPageID);
					list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
					for(itera;itera!=m_list_trackCtrl.end();itera++)
					{
						CTrackContrl* pTrackContrl = *itera;
						ASSERT(pTrackContrl!=NULL);
						if(pTrackContrl->GetSerialID()==pPic->GetPageID())
						{
							pTrackContrl->Add(pPic);
						}
					}
				}
				else if(winfo.IsShowTime && !winfo.IsForImage)
				{
					CControlRect* pShowTime = new CControlRect(rect);
					pShowTime->Create(enmStatic,winfo.ctrTextContent,this,0,rect,0,0);
					pShowTime->m_pTransStatic->SetShowTime(TRUE);
					pShowTime->m_pTransStatic->SetTimeFormat(winfo.timeFormat);
					pShowTime->m_pTransStatic->SetFont(winfo.lfFont);
					pShowTime->m_pTransStatic->SetTextColor(winfo.textColor);
					pShowTime->SetPageID(winfo.nPageID);
					list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
					for(itera;itera!=m_list_trackCtrl.end();itera++)
					{
						CTrackContrl* pTrackContrl = *itera;
						ASSERT(pTrackContrl!=NULL);
						if(pTrackContrl->GetSerialID()==pShowTime->GetPageID())
						{
							pTrackContrl->Add(pShowTime);
						}
					}
				}
				else if(winfo.IsShowQueNum && !winfo.IsForImage)
				{
					CControlRect* pShowQueNum = new CControlRect(rect);
					pShowQueNum->Create(enmStatic,winfo.forShowQueNumHead,this,0,rect,0,0);
					pShowQueNum->m_pTransStatic->SetShowQueNum(TRUE);
					pShowQueNum->m_pTransStatic->SetFont(winfo.lfFont);
					pShowQueNum->m_pTransStatic->SetTextColor(winfo.textColor);
					pShowQueNum->m_pTransStatic->SetForShowQueNumHead(winfo.forShowQueNumHead);
					pShowQueNum->m_pTransStatic->SetLinkAddress(winfo.address);
					pShowQueNum->SetPageID(winfo.nPageID);
					list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
					for(itera;itera!=m_list_trackCtrl.end();itera++)
					{
						CTrackContrl* pTrackContrl = *itera;
						ASSERT(pTrackContrl!=NULL);
						if(pTrackContrl->GetSerialID()==pShowQueNum->GetPageID())
						{
							pTrackContrl->Add(pShowQueNum);
						}
					}	
				}
				else//静态框for文本
				{
					CControlRect *pStatic = new CControlRect(rect);
					pStatic->Create(enmStatic,winfo.ctrTextContent,this,0,rect,0,0);
					//设置字体颜色和背景颜色
					pStatic->m_pTransStatic->SetTextColor(winfo.textColor);
					pStatic->m_pTransStatic->SetBkColor(winfo.bkColor);
					pStatic->m_pTransStatic->SetFont(winfo.lfFont);
					pStatic->m_pTransStatic->SetWindowText(winfo.ctrTextContent);
					//设置对齐方式
					pStatic->m_pTransStatic->SetStaticTextAlign(winfo.textAlign);
					//设置透明度
					pStatic->m_pTransStatic->SetAlpha(winfo.nAlpha);
					pStatic->SetPageID(winfo.nPageID);

					list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
					for(itera;itera!=m_list_trackCtrl.end();itera++)
					{
						CTrackContrl* pTrackContrl = *itera;
						ASSERT(pTrackContrl!=NULL);
						if(pTrackContrl->GetSerialID()==pStatic->GetPageID())
						{
							pTrackContrl->Add(pStatic);
						}
					}	
				}	
			}
			break;
		}
	}
	//使所有控件处于可用状态
	list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
	for(itera;itera!=m_list_trackCtrl.end();itera++)
	{
		CTrackContrl* pTrackContrl = *itera;
		ASSERT(pTrackContrl!=NULL);
		pTrackContrl->SetAllCtrContralEnable();
	}
}
/*
void CHallQueFrontView::OnSysseting()
{
	// TODO: 在此添加命令处理程序代码
//	CSetPropertySheet propertysheet(_T("系统设置"),this);
//	propertysheet.DoModal();
}
*/
void CHallQueFrontView::OnShowTime()
{
	// TODO: 在此添加命令处理程序代码
	m_isEdit = FALSE;
	m_isAddButton = FALSE;
	m_isAddPic = FALSE;
	m_isAddText = FALSE;
	m_isShowTime = TRUE;
	m_isShowQueNum = FALSE;
	CPropEditing prop(_T("编辑"),this);
	prop.DoModal();
}

void CHallQueFrontView::OnShowQuenum()
{
	// TODO: 在此添加命令处理程序代码
	m_isEdit = FALSE;
	m_isAddButton = FALSE;
	m_isAddPic = FALSE;
	m_isAddText = FALSE;
	m_isShowTime = FALSE;
	m_isShowQueNum = TRUE;
	CPropEditing prop(_T("编辑"),this);
	prop.DoModal();
}

void CHallQueFrontView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt;
	GetCursorPos(&pt);
	
	int result = m_pTrackCtrl->Track( point,nFlags);
	if(result == TRACK_CTR_MIDORHANDLE)
	{
		OnEditctr();
	}
	else
	{
		int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
		int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
		CRect inRect;
		inRect.left = nFullWidth - 300;
		inRect.top = nFullHeight - 200;
		inRect.right = nFullWidth;
		inRect.bottom = nFullHeight;
		if(inRect.PtInRect(pt))
		{
			if(!m_isManage)
			{
				CMenu menu;
				menu.LoadMenu(IDR_MENU_NOMANAGE);
				CMenu *pContentMenu=menu.GetSubMenu(0);
				pContentMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this); //在指定位置显示弹出菜单
				pContentMenu->DestroyMenu();
			}
			else
			{
				CMenu menu;
				menu.LoadMenu(IDR_MENU_MANAGE);
				CMenu *pContentMenu=menu.GetSubMenu(0);
				pContentMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this); //在指定位置显示弹出菜单
				pContentMenu->DestroyMenu();
			}
		}
	}
	CView::OnLButtonDblClk(nFlags, point);
}

void CHallQueFrontView::OnButtonClick(UINT uID)
{
	int count = m_pTrackCtrl->m_selTrackerAll.GetCount();
	for(int i=0;i<count;i++)
	{
		if(m_pTrackCtrl->m_selTrackerAll[i]->GetWindowType() == enmButton
			&& m_pTrackCtrl->m_selTrackerAll[i]->GetCtrWindowID() == uID)
		{
			//获取队列
			CString strAddress = m_pTrackCtrl->m_selTrackerAll[i]->
				m_pTransButton->GetAttchAddress();
			if(!strAddress.IsEmpty())
			{
				//判断是否超出工作时间
				if(!theApp.m_Controller.JudgeWorkTimeOut(strAddress))
				{
					m_Mlock.Lock();
					m_list_address.AddTail(strAddress);
					m_Mlock.Unlock();
				}
			}
			else//链接界面
			{
				int nPageID = m_pTrackCtrl->m_selTrackerAll[i]->
					m_pTransButton->GetAttachPageID();
				ShowPage(nPageID);
			}
			break;
		}
	}
/*	
	SLZCWndScreen* pWnd = SLZCWndScreen::GetInstance();
	CString str=_T("#0#1号通道");
 	pWnd->AddThroughScreenMsg(str,1,1);
	
// 	str=_T("#2#张文博#1#你妈叫你#0#回家吃饭");
// 	pWnd->AddThroughScreenMsg(str,1,1);
// 	str=_T("#1#送咬住#2#呼叫#0#张三");
// 	pWnd->AddThroughScreenMsg(str,1,1);
 	str=_T("#1#2号通道");
 	pWnd->AddThroughScreenMsg(str,1,2);
	
	str=_T("#2#3号通道");
	pWnd->AddThroughScreenMsg(str,1,3);
	str=_T("#0#4号通道");
	pWnd->AddThroughScreenMsg(str,1,4);
	str=_T("#1#5号通道");
	pWnd->AddThroughScreenMsg(str,1,5);
	str=_T("#2#6号通道");
	pWnd->AddThroughScreenMsg(str,1,6);
	str=_T("#0#7号通道");
// 	pWnd->AddThroughScreenMsg(str,1,7);
// 	str=_T("#1#8号通道");
// 	pWnd->AddThroughScreenMsg(str,1,8);
//	str=_T("什么的发生的发生的123243");
//	pWnd->AddThroughScreenMsg(str,1,1);
*/	
}

BOOL CHallQueFrontView::HasData()
{
	return !m_list_address.IsEmpty();
}

CString CHallQueFrontView::GetData()
{
	m_Mlock.Lock();
	CString address;
	if(HasData())
	{
		address = m_list_address.GetHead();
		m_list_address.RemoveHead();
	}
	m_Mlock.Unlock();
	return address;
}


void CHallQueFrontView::ShowWaitNum(const CString& queID,const int waitNum )
{
	int count = m_pTrackCtrl->m_selTrackerAll.GetCount();
	for(int i=0;i<count;i++)
	{
		if(m_pTrackCtrl->m_selTrackerAll[i]->GetWindowType() == enmStatic)
		{
			if(m_pTrackCtrl->m_selTrackerAll[i]->m_pTransStatic->GetIsShowQueNum())
			{
				if(m_pTrackCtrl->m_selTrackerAll[i]->m_pTransStatic->GetLinkAddress() 
					== queID)
				{
					CString head = m_pTrackCtrl->m_selTrackerAll[i]->m_pTransStatic->
						GetForShowQueNumHead();
					head.AppendFormat(_T("%d"),waitNum);
					m_pTrackCtrl->m_selTrackerAll[i]->m_pTransStatic->SetWindowText(head);
					/*
					//获取字的尺寸
					CSize textSize;
					HDC hdc = ::GetDC(m_pTrackCtrl->m_selTrackerAll[i]->m_pTransStatic->m_hWnd);
					LOGFONT lf = m_pTrackCtrl->m_selTrackerAll[i]->m_pTransStatic->GetTextFont();
					CFont font;
					font.CreateFontIndirect(&lf);
					HFONT hOldFont = (HFONT) SelectObject(hdc,font);
					GetTextExtentPoint32(hdc,head,head.GetLength(),&textSize);
					SelectObject(hdc,hOldFont);
					::ReleaseDC(m_pTrackCtrl->m_selTrackerAll[i]->m_pTransStatic->m_hWnd,hdc);
					//////////////////////////////////
					//重新设置大小
					CRect oldRect;
					m_pTrackCtrl->m_selTrackerAll[i]->GetTrueRect(oldRect);
					CRect newRect;
					newRect.left = oldRect.left;newRect.right = oldRect.left + textSize.cx;
					newRect.top = oldRect.top;newRect.bottom = oldRect.top + textSize.cy;
					m_pTrackCtrl->m_selTrackerAll[i]->SetRect(newRect);
					*/
					m_pTrackCtrl->m_selTrackerAll[i]->m_pTransStatic->Invalidate(TRUE);
					break;
				}
			}
		}
	}
}
void CHallQueFrontView::OnQueset()
{
	// TODO: 在此添加命令处理程序代码
	CQueSetDlg queSetDlg(this);
	if(IDOK == queSetDlg.DoModal())
	{
		theApp.m_Controller.ReFlushQueInfoTable();//刷新队列信息
	}
}

void CHallQueFrontView::OnStaffset()
{
	// TODO: 在此添加命令处理程序代码
	CStaffSetDlg staffDlg(this);
	if(IDOK == staffDlg.DoModal())
	{
		theApp.m_Controller.ReFlushStaffTable();//刷新员工
	}
}

void CHallQueFrontView::OnWindowset()
{
	// TODO: 在此添加命令处理程序代码
	SLZWindowSetDlg wndDlg(this);
	if(IDOK == wndDlg.DoModal())
	{
		theApp.m_Controller.ReFlushWindowTable();
	}
}

void CHallQueFrontView::OnPrintset()
{
	// TODO: 在此添加命令处理程序代码
	CPrintSetDlg printDlg(this);
	if(IDOK == printDlg.DoModal())
	{
		theApp.m_Controller.ReFlushPrintInfoTable();
	}
}

void CHallQueFrontView::OnCardset()
{
	// TODO: 在此添加命令处理程序代码
	CCardConfigQue cardConfigDlg(this);
	if(IDOK == cardConfigDlg.DoModal())
	{
		theApp.m_Controller.ReFlushSwingCardTable();
	}
}

void CHallQueFrontView::OnComprehset()
{
	// TODO: 在此添加命令处理程序代码
	CPropConnectInfo connectDlg(this);
	if(IDOK == connectDlg.DoModal())
	{
		theApp.m_Controller.ReFlushSysLogicVarlibles();
		theApp.m_Controller.m_pPlaySound->GetReplayTimes();
		theApp.m_Controller.m_pPlaySound->SetSoundSpeed(theApp.m_logicVariables.playSpeed);
	}
}
/*
判断按钮对应的队列是否超时
*/
void CHallQueFrontView::JudgeButtonWorkOut()
{
	int count  = m_pTrackCtrl->m_selTrackerAll.GetCount();
	for(int i=0;i<count;i++)
	{
		if(enmButton == m_pTrackCtrl->m_selTrackerAll[i]->GetWindowType())
		{
			//队列ID
			CString queID = m_pTrackCtrl->m_selTrackerAll[i]->m_pTransButton->GetAttchAddress();
			CString buttonText;
			m_pTrackCtrl->m_selTrackerAll[i]->m_pTransButton->GetWindowText(buttonText);
			if(theApp.m_Controller.JudgeWorkTimeOut(queID))//不在时间范围内
			{
				if(buttonText != m_workTimeOut)
				{
					m_pTrackCtrl->m_selTrackerAll[i]->m_pTransButton->SetWindowText(m_workTimeOut);
				}
			}
			else//在时间范围内
			{
				if(buttonText == m_workTimeOut)
				{
					CString queName = theApp.m_Controller.GetQueNameFromID(queID);
					m_pTrackCtrl->m_selTrackerAll[i]->m_pTransButton->SetWindowText(queName);
				}
			}
		}
	}
}
void CHallQueFrontView::OnStbset()
{
	// TODO: 在此添加命令处理程序代码
}

LRESULT CHallQueFrontView::OnMyShowMessage(WPARAM wParam, LPARAM lParam)
{
	ShowVariables* pShowVaria = (ShowVariables*)wParam;
	m_showVaria = *pShowVaria;
	m_pWaringDlg->ShowWindow(SW_SHOWNORMAL);
	m_pWaringDlg->UpdateWindow();
	Sleep(800);
	m_pWaringDlg->ShowWindow(SW_HIDE);
	m_pWaringDlg->UpdateWindow();
	if(pShowVaria)
	{
		delete pShowVaria;
		pShowVaria = NULL;
	}
	return 0;
}
void CHallQueFrontView::OnViewminsize()
{
	// TODO: 在此添加命令处理程序代码
	theApp.GetMainWnd()->ShowWindow(SW_SHOWMINIMIZED);
}

void CHallQueFrontView::WriteCtrlDataToList()
{
	m_list_allCtrInfo.RemoveAll();
	list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
	for(itera;itera!=m_list_trackCtrl.end();itera++)
	{
		CTrackContrl* pTrackContrl = *itera;
		ASSERT(pTrackContrl!=NULL);
		int count = pTrackContrl->m_selTrackerAll.GetCount();
		for(int i=0;i<count;i++)
		{
			WINDOWCTRINFO windowCtrInfo;
//			memset(&windowCtrInfo,0,sizeof(windowCtrInfo));
			windowCtrInfo.nPageID = pTrackContrl->GetSerialID();//界面ID赋值
			windowCtrInfo.windowType = pTrackContrl->m_selTrackerAll[i]->GetWindowType();
			if(windowCtrInfo.windowType == enmStatic)
			{
				if(pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->IsForImage())
				{
					windowCtrInfo.IsForImage = TRUE;
				}
				if(pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->GetIsShowTime())
				{
					windowCtrInfo.IsShowTime = TRUE;
					windowCtrInfo.timeFormat = pTrackContrl->m_selTrackerAll[i]->
						m_pTransStatic->GetTimeFormat();
				}
				if(pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->GetIsShowQueNum())
				{
					windowCtrInfo.IsShowQueNum = TRUE;
					CString head = pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->
						GetForShowQueNumHead();
					wcsncpy_s(windowCtrInfo.forShowQueNumHead, addNum, head,
						head.GetLength());
					CString address = pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->
						GetLinkAddress();
					wcsncpy_s(windowCtrInfo.address, addNum, address,
						address.GetLength());
				}
				windowCtrInfo.bkColor = pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->
					GetBkColor();
				windowCtrInfo.textAlign = pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->
					GetStaticTextAlign();
				windowCtrInfo.textColor = pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->
					GetTextColor();
				windowCtrInfo.nAlpha = pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->
					GetAlpha();
				windowCtrInfo.lfFont = pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->
					GetTextFont();
				CString strContent;
				pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->GetWindowText(strContent);
				_tcsncpy_s(windowCtrInfo.ctrTextContent, textNum, 
					strContent,strContent.GetLength());

				CString picPath = pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->GetLoadImagePath();
				wcsncpy_s(windowCtrInfo.staticPicPath, MAX_PATH, picPath,picPath.GetLength());

				CRect rect;
				//pTrackContrl->m_selTrackerAll[i]->m_pTransStatic->GetWindowRect(&rect);
				rect = pTrackContrl->m_selTrackerAll[i]->m_rect;
				windowCtrInfo.rectLeft = rect.left;
				windowCtrInfo.rectRight = rect.right;
				windowCtrInfo.rectTop = rect.top;
				windowCtrInfo.rectBottom = rect.bottom;
			}
			else if(windowCtrInfo.windowType == enmButton)
			{
				windowCtrInfo.bkColor = pTrackContrl->m_selTrackerAll[i]->m_pTransButton->
					GetBkColor();
				windowCtrInfo.textColor =  pTrackContrl->m_selTrackerAll[i]->m_pTransButton->
					GetTextColor();
				CString address =  pTrackContrl->m_selTrackerAll[i]->m_pTransButton->
					GetAttchAddress();
				_tcsncpy_s(windowCtrInfo.address, addNum, address,address.GetLength());
				windowCtrInfo.nLinkPageID = pTrackContrl->m_selTrackerAll[i]->m_pTransButton->
					GetAttachPageID();
				windowCtrInfo.nAlpha = pTrackContrl->m_selTrackerAll[i]->m_pTransButton->
					GetAlpha();
				windowCtrInfo.ButtonType = pTrackContrl->m_selTrackerAll[i]->m_pTransButton->
					GetButtonType();
				windowCtrInfo.lfFont = pTrackContrl->m_selTrackerAll[i]->m_pTransButton->
					GetFont();
				CString bkpic = pTrackContrl->m_selTrackerAll[i]->m_pTransButton->GetBkPic();
				_tcsncpy_s(windowCtrInfo.bkPicPath,MAX_PATH,bkpic,bkpic.GetLength());
				CString content;
				pTrackContrl->m_selTrackerAll[i]->m_pTransButton->GetWindowText(content);
				_tcsncpy_s(windowCtrInfo.ctrTextContent,textNum,content,content.GetLength());
				CRect rect;
				rect = pTrackContrl->m_selTrackerAll[i]->m_rect;
				windowCtrInfo.rectLeft = rect.left;
				windowCtrInfo.rectRight = rect.right;
				windowCtrInfo.rectTop = rect.top;
				windowCtrInfo.rectBottom = rect.bottom;
			}
			m_list_allCtrInfo.AddTail(windowCtrInfo);
		}
	}
}

BOOL CHallQueFrontView::WritePageToFile()
{
	CFile file;
	CFileException e;
	if(file.Open(m_strPageFilePath,CFile::modeWrite | CFile::modeCreate,&e))
	{
		TrackInfo trackInfo;
		memset(&trackInfo,0,sizeof(trackInfo));
		list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
		for(itera;itera!=m_list_trackCtrl.end();itera++)
		{
			CTrackContrl* pTrackContrl = *itera;
			ASSERT(pTrackContrl!=NULL);
			trackInfo.nPageID = pTrackContrl->GetSerialID();
			CString path = pTrackContrl->GetBackPath();
			wcsncpy_s(trackInfo.strBackPath,MAX_PATH,path.GetBuffer(0),path.GetLength());
			file.Write(&trackInfo,sizeof(trackInfo));
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}

BOOL CHallQueFrontView::ReadPageFromFile()
{
	CFile file;
	CFileException e;
	if(file.Open(m_strPageFilePath,CFile::modeRead,&e))
	{
		ULONGLONG count=file.GetLength()/sizeof(TrackInfo);
		for(int i=0;i<count;i++)
		{
			TrackInfo trackInfo;
			memset(&trackInfo,0,sizeof(trackInfo));
			file.Read(&trackInfo,sizeof(trackInfo));
			CString path(trackInfo.strBackPath);
			if(trackInfo.nPageID>0)//子界面
			{
				CTrackContrl* pTrackContrl = new CTrackContrl;
				pTrackContrl->Create(this);
				pTrackContrl->SetSerialID(trackInfo.nPageID);
				pTrackContrl->SetBackPath(path);
				g_nPageID = g_nPageID > trackInfo.nPageID ? g_nPageID : trackInfo.nPageID;
				m_list_trackCtrl.push_back(pTrackContrl);
			}
			else if(trackInfo.nPageID==0 && !path.IsEmpty())
			{
				list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
				for(itera;itera!=m_list_trackCtrl.end();itera++)
				{
					CTrackContrl* pTrackContrl = *itera;
					ASSERT(pTrackContrl!=NULL);
					if(pTrackContrl->GetSerialID()==0)//主界面
					{
						pTrackContrl->SetBackPath(path);
						break;
					}
				}
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}

void CHallQueFrontView::ClearPage()
{
	list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
	for(itera;itera!=m_list_trackCtrl.end();itera++)
	{
		CTrackContrl* pTrackContrl = *itera;
		ASSERT(pTrackContrl!=NULL);
		delete pTrackContrl;
		pTrackContrl = NULL;
	}	
	m_list_trackCtrl.clear();
}

void CHallQueFrontView::ShowPage(int nPageID)
{
	if(nPageID!=-1)
	{
		list<CTrackContrl*>::const_iterator itera = m_list_trackCtrl.begin();
		for(itera;itera!=m_list_trackCtrl.end();itera++)
		{
			CTrackContrl* pTrackContrl = *itera;
			if(nPageID == pTrackContrl->GetSerialID())//找到界面
			{
				if(m_pTrackCtrl!=NULL)
					m_pTrackCtrl->SetAllCtrlHide();//隐藏原来界面
				m_pTrackCtrl = pTrackContrl;//新界面赋值
				if(!MyLoadBackImage(pTrackContrl->GetBackPath()))
				{
					CDoFile doFile;
					CString path = doFile.GetExeFullFilePath();
					path += _T("\\背景\\初始背景.jpg");
					MyLoadBackImage(path);
				}
				GetBackDC();//重新加载背景DC
				///////////////////////////////////////////////////////////////////
				theApp.m_Controller.InitShowInlineQueNum();//显示界面排队人数
				pTrackContrl->SetAllCtrlShow();//显示
				Invalidate();
				break;
			}
		}
//		theApp.m_Controller.InitShowInlineQueNum();//显示界面排队人数
	}
}

LRESULT CHallQueFrontView::OnMyShowPage(WPARAM wParam,LPARAM lParam)
{
	UINT nPageID = (UINT)wParam;
	ShowPage(nPageID);
	return 0;
}
void CHallQueFrontView::OnLeadinwav()
{
	// TODO: 在此添加命令处理程序代码
	CStringArray strWavArray;
	CString strWavLibPath = CommonStrMethod::GetModuleDir() + _T("wavLib\\");
	CFileDialog filedlg(TRUE,0,0,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		| OFN_ALLOWMULTISELECT,m_strPicFilter);
	if(IDOK == filedlg.DoModal())
	{
		POSITION pos;
		pos = filedlg.GetStartPosition();
		while (pos)
		{
			strWavArray.Add(filedlg.GetNextPathName(pos));
		}
		BOOL bResult = TRUE;
		for (int i=0;i<strWavArray.GetCount();i++)
		{
			CString strName = CommonStrMethod::GetNameFromAbsName(strWavArray[i]);
			bResult = ::CopyFile(strWavArray[i],strWavLibPath+strName,TRUE);
			if (!bResult)
			{
				AfxMessageBox(strName+L"已存在，请重命名。");
				break;
			}
		}
		if (bResult)
		{
			AfxMessageBox(L"语音文件导入完毕！");
		}
	}
}

void CHallQueFrontView::OnHidemain()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ShowWindow(SW_HIDE);
	pFrame->UpdateWindow();
	pFrame->SetFullScreen(FALSE);
}
