
#include "stdafx.h"
#include "PrintSetDlg.h"
#include "HallQueFront.h"
#include "PropPrintSet.h"
#include "DoFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern void MyWriteConsole(CString str);
IMPLEMENT_DYNAMIC(CPrintSetDlg, CDialog)

CPrintSetDlg::CPrintSetDlg(CWnd* pParent /*=NULL*/): CDialog(CPrintSetDlg::IDD,pParent)
,m_bIsAddPicture(FALSE),m_bIsAddText(FALSE),m_bIsForEdit(FALSE),
m_bIsShowTime(FALSE),m_bIsForPrintObject(FALSE)
{
// 	m_bIsAddPicture = FALSE;
// 	m_bIsAddText = FALSE;
// 	m_bIsForEdit = FALSE;
// 	m_bIsShowTime = FALSE;
// 	m_bIsForPrintObject = FALSE;
	CDoFile dofile;
	m_printInfoPath = dofile.GetExeFullFilePath();
	m_printInfoPath += _T("\\PrintInfo");
	dofile.CreateMyDirectory(m_printInfoPath);
}

CPrintSetDlg::~CPrintSetDlg(void)
{
}

void CPrintSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PRINT_RECT, m_sta_printRect);
}

BEGIN_MESSAGE_MAP(CPrintSetDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD_TEXT, &CPrintSetDlg::OnBnClickedAddText)
	ON_BN_CLICKED(IDC_ADD_PIC, &CPrintSetDlg::OnBnClickedAddPic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_PRINT_EDIT, &CPrintSetDlg::OnPrintEdit)
	ON_COMMAND(ID_PRINT_DELETE, &CPrintSetDlg::OnPrintDelete)
	ON_BN_CLICKED(IDC_CUR_TIME, &CPrintSetDlg::OnBnClickedCurTime)
	ON_BN_CLICKED(IDC_NUM, &CPrintSetDlg::OnBnClickedNum)
	ON_BN_CLICKED(IDC_WAIT_NUM, &CPrintSetDlg::OnBnClickedWaitNum)
	ON_BN_CLICKED(IDC_CLI_NAME, &CPrintSetDlg::OnBnClickedCliName)
	
	ON_BN_CLICKED(IDC_QUEUE, &CPrintSetDlg::OnBnClickedQueue)
	ON_BN_CLICKED(IDC_BN_CLLEV, &CPrintSetDlg::OnBnClickedBnCllev)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BN_OK, &CPrintSetDlg::OnBnClickedBnOk)
	ON_BN_CLICKED(IDC_BN_CANCEL, &CPrintSetDlg::OnBnClickedBnCancel)
END_MESSAGE_MAP()

void CPrintSetDlg::OnBnClickedAddText()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = FALSE;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
	//	SetModified();
	}
}

void CPrintSetDlg::OnBnClickedAddPic()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = TRUE;
	m_bIsAddText = FALSE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = FALSE;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
//		SetModified();
	}
}

BOOL CPrintSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	
	m_trackContrl.Create(this);
	m_trackContrl.SetForAdjustMaxRect(TRUE);
	m_trackContrl.SetMaxRect(GetShowRect());

	ReadPrintInfoFromFile();
	CreateAllPrintGDI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPrintSetDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPropertyPage::OnPaint()
}

BOOL CPrintSetDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnEraseBkgnd(pDC);
	m_trackContrl.Draw(pDC);
	return TRUE;
}

CRect CPrintSetDlg::GetShowRect()
{
	m_sta_printRect.GetWindowRect(m_printRect);
	m_printRect.left += 2;
	m_printRect.top += 8;
	m_printRect.right -= 2;
	m_printRect.bottom -= 2;
	ScreenToClient(m_printRect);
	return m_printRect;
}
void CPrintSetDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int result = m_trackContrl.Track( point,nFlags);
//	if(result == TRACK_CTR_MIDORHANDLE)
//	{
//		SetModified();
//	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CPrintSetDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int result = m_trackContrl.Track( point,nFlags);
	if(result == TRACK_CTR_MIDORHANDLE)
	{
		OnPrintEdit();
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CPrintSetDlg::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialog::PreSubclassWindow();
}

BOOL CPrintSetDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( m_trackContrl.SetCursor( nHitTest, message ) )
		return TRUE;
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CPrintSetDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt;
	GetCursorPos(&pt);
	int result = m_trackContrl.Track( point,nFlags);
	if(result == TRACK_CTR_RBUTTON)
	{
		CMenu menu;
		menu.LoadMenu(IDR_MENU_PRINT);
		CMenu *pContentMenu=menu.GetSubMenu(0);
		pContentMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this); //在指定位置显示弹出菜单
		pContentMenu->DestroyMenu();
	}
	CDialog::OnRButtonDown(nFlags, point);
}

void CPrintSetDlg::OnPrintEdit()
{
	// TODO: 在此添加命令处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = FALSE;
	m_bIsForEdit = TRUE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = FALSE;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
	//	SetModified();
	}
}

void CPrintSetDlg::OnPrintDelete()
{
	// TODO: 在此添加命令处理程序代码
	m_trackContrl.DeleteRightBnSelectCtr();
// 	{
// 		SetModified(TRUE);
// 	}
}

void CPrintSetDlg::OnBnClickedCurTime()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = FALSE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = TRUE;
	m_bIsForPrintObject = FALSE;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
//		SetModified();
	}
}
//排队号码
void CPrintSetDlg::OnBnClickedNum()
{
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintClientNum;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
//		SetModified();
	}
	// TODO: 在此添加控件通知处理程序代码
}
//等候人数
void CPrintSetDlg::OnBnClickedWaitNum()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintWaitNum;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
//		SetModified();
	}
}
//客户姓名
void CPrintSetDlg::OnBnClickedCliName()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintClientName;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
//		SetModified();
	}
}
//所在队列
void CPrintSetDlg::OnBnClickedQueue()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintType;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
//		SetModified();
	}
}
//客户星级
void CPrintSetDlg::OnBnClickedBnCllev()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintClientLevel;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
//		SetModified();
	}
}

BOOL CPrintSetDlg::WritePrintInfoToFile()
{
	m_list_printInfo.RemoveAll();
	int count = m_trackContrl.m_selTrackerAll.GetCount();
	for(int i=0;i<count;i++)
	{
		WINDOWCTRINFO windowCtrInfo;
//		memset(&windowCtrInfo,0,sizeof(windowCtrInfo));
		windowCtrInfo.windowType = m_trackContrl.m_selTrackerAll[i]->GetWindowType();
		if(windowCtrInfo.windowType == enmStatic)
		{
			if(m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->IsForImage())
			{
				windowCtrInfo.IsForImage = TRUE;
			}
			if(m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->GetIsShowTime())
			{
				windowCtrInfo.IsShowTime = TRUE;
				windowCtrInfo.timeFormat = m_trackContrl.m_selTrackerAll[i]->
					m_pTransStatic->GetTimeFormat();
			}
			if(m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->GetStaForPrintChangeObject())
			{
				windowCtrInfo.IsForPrintChangeObjec = TRUE;
				windowCtrInfo.printObject = m_trackContrl.m_selTrackerAll[i]->
					m_pTransStatic->GetStaPrintObject();
			}
			if(m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->GetStaticForPrint())
			{
				windowCtrInfo.IsForPrint = TRUE;
			}
			windowCtrInfo.bkColor = m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->
				GetBkColor();
			windowCtrInfo.textAlign = m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->
				GetStaticTextAlign();
			windowCtrInfo.textColor = m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->
				GetTextColor();
			windowCtrInfo.nAlpha = m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->
				GetAlpha();
			windowCtrInfo.lfFont = m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->
				GetTextFont();
			CString strContent;
			m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->GetWindowText(strContent);
			_tcsncpy_s(windowCtrInfo.ctrTextContent, textNum, 
				strContent,strContent.GetLength());

			CString picPath = m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->GetLoadImagePath();
			wcsncpy_s(windowCtrInfo.staticPicPath, MAX_PATH, picPath,picPath.GetLength());

			CRect rect;
			//m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->GetWindowRect(&rect);
			rect = m_trackContrl.m_selTrackerAll[i]->m_rect;
			windowCtrInfo.rectLeft = rect.left;
			windowCtrInfo.rectRight = rect.right;
			windowCtrInfo.rectTop = rect.top;
			windowCtrInfo.rectBottom = rect.bottom;
		}
		m_list_printInfo.AddTail(windowCtrInfo);
	}

	CFile file;
	CFileException e;
	WINDOWCTRINFO wndctrInfo;
	if(file.Open(m_printInfoPath,CFile::modeWrite | CFile::modeCreate,&e))
	{
		POSITION pos=m_list_printInfo.GetHeadPosition();
		while(pos)
		{
			wndctrInfo=m_list_printInfo.GetNext(pos);
			file.Write(&wndctrInfo,sizeof(WINDOWCTRINFO));
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CPrintSetDlg::ReadPrintInfoFromFile()
{
	m_list_printInfo.RemoveAll();
	m_printInfoPath += _T("\\printinfo.dat");
	CFile file;
	if(file.Open(m_printInfoPath,CFile::modeRead))
	{
		ULONGLONG count=file.GetLength()/sizeof(WINDOWCTRINFO);
		for(ULONGLONG i=0;i<count;i++)
		{
			WINDOWCTRINFO windowctrinfo;
			if(file.Read(&windowctrinfo,sizeof(WINDOWCTRINFO))>0)
			{
				m_list_printInfo.AddTail(windowctrinfo);
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void CPrintSetDlg::CreateAllPrintGDI()
{
	WINDOWCTRINFO winfo;
	POSITION pos = m_list_printInfo.GetHeadPosition();
	while(pos)
	{
		winfo = m_list_printInfo.GetNext(pos);
		switch(winfo.windowType)
		{
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
					pPic->m_pTransStatic->SetStaticForPrint(TRUE);
					pPic->m_pTransStatic->SetAlpha(winfo.nAlpha);
					m_trackContrl.Add(pPic);
				}
				else if(winfo.IsShowTime && !winfo.IsForImage)//时间
				{
					CControlRect* pShowTime = new CControlRect(rect);
					pShowTime->Create(enmStatic,winfo.ctrTextContent,this,0,rect,0,0);
					pShowTime->m_pTransStatic->SetShowTime(TRUE);
					pShowTime->m_pTransStatic->SetStaticForPrint(TRUE);
					pShowTime->m_pTransStatic->SetTimeFormat(winfo.timeFormat);
					pShowTime->m_pTransStatic->SetFont(winfo.lfFont);
					pShowTime->m_pTransStatic->SetTextColor(winfo.textColor);
					pShowTime->m_pTransStatic->SetWindowText(winfo.ctrTextContent);
					m_trackContrl.Add(pShowTime);
				}
				else if(winfo.IsForPrintChangeObjec && !winfo.IsForImage)//用于打印的变量
				{
					CControlRect* pPrintObject = new CControlRect(rect);
					pPrintObject->Create(enmStatic,winfo.ctrTextContent,this,0,rect,0,0);
					pPrintObject->m_pTransStatic->SetStaticForPrint(TRUE);
					pPrintObject->m_pTransStatic->SetStaForPrintChangeObject(TRUE);
					pPrintObject->m_pTransStatic->SetStaForPrintObject(winfo.printObject);
					pPrintObject->m_pTransStatic->SetFont(winfo.lfFont);
					pPrintObject->m_pTransStatic->SetTextColor(winfo.textColor);
					pPrintObject->m_pTransStatic->SetWindowText(winfo.ctrTextContent);
					m_trackContrl.Add(pPrintObject);
				}
				else if(winfo.IsShowQueNum && !winfo.IsForImage)//显示等候人数
				{
					continue;
				}
				else//文本
				{
					CControlRect *pStatic = new CControlRect(rect);
					pStatic->Create(enmStatic,winfo.ctrTextContent,this,0,rect,0,0);
					pStatic->m_pTransStatic->SetStaticForPrint(TRUE);
					//设置字体颜色和背景颜色
					pStatic->m_pTransStatic->SetTextColor(winfo.textColor);
					pStatic->m_pTransStatic->SetBkColor(winfo.bkColor);
					pStatic->m_pTransStatic->SetFont(winfo.lfFont);
					pStatic->m_pTransStatic->SetWindowText(winfo.ctrTextContent);
					//设置对齐方式
					pStatic->m_pTransStatic->SetStaticTextAlign(winfo.textAlign);
					//设置透明度
					pStatic->m_pTransStatic->SetAlpha(winfo.nAlpha);
					m_trackContrl.Add(pStatic);
				}
			}
			break;
		}
	}
}

BOOL CPrintSetDlg::WritePrintRectInfoToFile()
{
	POINT pt[2]={0};
	CRect rect = GetShowRect();
	pt[0].x = rect.left;	pt[0].y = rect.top;
	pt[1].x = rect.right; pt[1].y = rect.bottom;
	CDoFile dofile;
	m_printInfoPath = dofile.GetExeFullFilePath();
	m_printInfoPath += _T("\\PrintInfo\\PrintRect.dat");
	CFile file;
	CFileException e;
	if(file.Open(m_printInfoPath,CFile::modeWrite | CFile::modeCreate,&e))
	{
		for(int i=0;i<2;i++)
		{
			file.Write(&pt[i],sizeof(POINT));
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void CPrintSetDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CPrintSetDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_UP:
			m_trackContrl.KeyUpMove();
			break;
		case VK_DOWN:
			m_trackContrl.KeyDownMove();
			break;
		case VK_LEFT:
			m_trackContrl.KeyLeftMove();
			break;
		case VK_RIGHT:
			m_trackContrl.KeyRightMove();
			break;
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

/*
BOOL CPrintSetDlg::OnApply()
{
// TODO: 在此添加专用代码和/或调用基类
WritePrintInfoToFile();
WritePrintRectInfoToFile();
return CPropertyPage::OnApply();
}
*/
void CPrintSetDlg::OnBnClickedBnOk()
{
	// TODO: 在此添加控件通知处理程序代码
	WritePrintInfoToFile();
	WritePrintRectInfoToFile();
	return CDialog::OnOK();
}

void CPrintSetDlg::OnBnClickedBnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	return CDialog::OnCancel();
}
