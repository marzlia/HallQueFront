// ShowPageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "ShowPageDlg.h"
#include "TrackContrl.h"


// CShowPageDlg 对话框
extern UINT g_nPageID;

IMPLEMENT_DYNAMIC(CShowPageDlg, CDialog)

CShowPageDlg::CShowPageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowPageDlg::IDD, pParent)
	, m_bIsHide(FALSE)
{

}

CShowPageDlg::~CShowPageDlg()
{
}

void CShowPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PAGE, m_OwnerListBox);
}


BEGIN_MESSAGE_MAP(CShowPageDlg, CDialog)
	ON_BN_CLICKED(IDC_BN_ADDPAGE, &CShowPageDlg::OnBnClickedBnAddpage)
	ON_BN_CLICKED(IDC_BN_DELPAGE, &CShowPageDlg::OnBnClickedBnDelpage)
	ON_BN_CLICKED(IDC_BN_TIMEHIDE, &CShowPageDlg::OnBnClickedBnTimehide)
	ON_WM_TIMER()
	ON_LBN_SELCHANGE(IDC_LIST_PAGE, &CShowPageDlg::OnLbnSelchangeListPage)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CShowPageDlg 消息处理程序

BOOL CShowPageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	list<CTrackContrl*>::const_iterator itera = theApp.m_pView->m_list_trackCtrl.begin();
	for(itera;itera!=theApp.m_pView->m_list_trackCtrl.end();itera++)
	{
		CTrackContrl* pTrackCtrl = *itera;
		if(pTrackCtrl->GetSerialID()==0)
		{
			int index = m_OwnerListBox.AddListBoxItem(_T("主页面"),theApp.LoadIcon(IDI_ICON_PAGE));
			m_map_index[index] = pTrackCtrl->GetSerialID();//关联
			m_OwnerListBox.SetCurSel(index);
			OnLbnSelchangeListPage();
		}
		else
		{
			CString strItemName;
			strItemName.Format(_T("页面[%d]"),pTrackCtrl->GetSerialID());
			int index = m_OwnerListBox.AddListBoxItem(strItemName,theApp.LoadIcon(IDI_ICON_PAGE));
			m_map_index[index] = pTrackCtrl->GetSerialID();//关联
			pTrackCtrl->SetAllCtrlHide();
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CShowPageDlg::OnBnClickedBnAddpage()
{
	// TODO: 在此添加控件通知处理程序代码
	g_nPageID++;
	CTrackContrl* pTrackCtrl = new CTrackContrl;
	ASSERT(pTrackCtrl!=NULL);
	pTrackCtrl->SetSerialID(g_nPageID);
	pTrackCtrl->Create(theApp.m_pView);
	theApp.m_pView->m_list_trackCtrl.push_back(pTrackCtrl);
	CString strName;
	strName.Format(_T("页面[%d]"),g_nPageID);
	int index = m_OwnerListBox.AddListBoxItem(strName,theApp.LoadIcon(IDI_ICON_PAGE));
	m_map_index[index]=g_nPageID;//关联
}

void CShowPageDlg::OnBnClickedBnDelpage()
{
	// TODO: 在此添加控件通知处理程序代码
	if(IDYES!=theApp.m_pView->MessageBox(_T("删除本页面将删除页面上所有内容,确定删除?"),_T("注意"),MB_YESNO |
		MB_ICONQUESTION))
		return;
	int nIndex = m_OwnerListBox.GetCurSel();
	if(nIndex==CB_ERR)return;
	m_OwnerListBox.DeleteListBoxItem(nIndex);
	map<int,UINT>::const_iterator itera = m_map_index.find(nIndex);
	if(itera!=m_map_index.end())
	{
		list<CTrackContrl*>::const_iterator trakItera = theApp.m_pView->m_list_trackCtrl.begin();
		for(trakItera;trakItera!=theApp.m_pView->m_list_trackCtrl.end();trakItera++)
		{
			CTrackContrl* ptrackCtr = *trakItera;
			if(ptrackCtr->GetSerialID()==itera->second && ptrackCtr->GetSerialID()!=0)
			{
				UINT nDelPageID = ptrackCtr->GetSerialID();
				delete ptrackCtr;
				ptrackCtr = NULL;
				////对应的当前选择的pTrackCtr置空,因为删除时选择的pTrackCtr销毁了
				theApp.m_pView->m_pTrackCtrl=NULL;
				////////////////////////////////////
				theApp.m_pView->m_list_trackCtrl.erase(trakItera);
				/////////////////////////////////////////////////////
				ClearCtrlLinkPageID(nDelPageID);//重置关联这个页面的按钮
				/////////////////////////////////////////////////////
				break;
			}
		}
	}
	m_map_index.clear();
	list<CTrackContrl*>::const_iterator secondItera = theApp.m_pView->m_list_trackCtrl.begin();
	int count = m_OwnerListBox.GetCount();
	for(int i=0;i<count;i++)
	{
		if(secondItera!=theApp.m_pView->m_list_trackCtrl.end())
		{
			CTrackContrl* pSecondTrackCtrl = *secondItera;
			ASSERT(pSecondTrackCtrl!=NULL);
			m_map_index[i] = pSecondTrackCtrl->GetSerialID();
		}
		secondItera++;
	}
	if(count>0&&nIndex>0)m_OwnerListBox.SetCurSel(nIndex-1);
	OnLbnSelchangeListPage();
//	theApp.m_pView->ShowPage(0);
}

void CShowPageDlg::OnBnClickedBnTimehide()
{
	// TODO: 在此添加控件通知处理程序代码
//	SetTimer(TIMEHIDESHOW,1000,NULL);
	if(!m_bIsHide)
	{
		CRect windowRect;
		GetWindowRect(windowRect);
		m_rOldWindowRect = windowRect;//保存原来的rect
		CRect listBoxRect;
		m_OwnerListBox.GetWindowRect(listBoxRect);
		ScreenToClient(listBoxRect);
		windowRect.left-=listBoxRect.right;
		windowRect.right-=listBoxRect.right;
		MoveWindow(windowRect);
		CButton* pButton = (CButton*)GetDlgItem(IDC_BN_TIMEHIDE);
		pButton->SetWindowText(_T(">"));
		m_bIsHide=!m_bIsHide;
	}
	else
	{
		MoveWindow(m_rOldWindowRect);
		CButton* pButton = (CButton*)GetDlgItem(IDC_BN_TIMEHIDE);
		pButton->SetWindowText(_T("<"));
		m_bIsHide=!m_bIsHide;
	}
}

void CShowPageDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnTimer(nIDEvent);
}

void CShowPageDlg::OnLbnSelchangeListPage()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_OwnerListBox.GetCurSel();
	if(index==CB_ERR)return;
	map<int,UINT>::const_iterator itera = m_map_index.find(index);
	if(itera!=m_map_index.end())
	{
		list<CTrackContrl*>::const_iterator trakItera = theApp.m_pView->m_list_trackCtrl.begin();
		for(trakItera;trakItera!=theApp.m_pView->m_list_trackCtrl.end();trakItera++)
		{
			CTrackContrl* ptrackCtr = *trakItera;
			if(ptrackCtr->GetSerialID()==itera->second && ptrackCtr->GetSerialID()==0)
			{
				CButton* pDelButton = (CButton*)GetDlgItem(IDC_BN_DELPAGE);
				pDelButton->EnableWindow(FALSE);
// 				if(theApp.m_pView->m_pTrackCtrl!=NULL)
// 					theApp.m_pView->m_pTrackCtrl->SetAllCtrlHide();//原来的隐藏
// 				theApp.m_pView->m_pTrackCtrl = ptrackCtr;
// 				theApp.m_pView->m_pTrackCtrl->SetAllCtrlShow();
				theApp.m_pView->ShowPage(ptrackCtr->GetSerialID());
//				theApp.m_pView->Invalidate(TRUE);
				break;
			}
			else if(ptrackCtr->GetSerialID()==itera->second && ptrackCtr->GetSerialID()!=0)
			{
				theApp.m_pView->ShowPage(ptrackCtr->GetSerialID());
				CButton* pDelButton = (CButton*)GetDlgItem(IDC_BN_DELPAGE);
				pDelButton->EnableWindow(TRUE);
// 				if(theApp.m_pView->m_pTrackCtrl!=NULL)
// 					theApp.m_pView->m_pTrackCtrl->SetAllCtrlHide();//原来的隐藏
// 				theApp.m_pView->m_pTrackCtrl = ptrackCtr;
// 				theApp.m_pView->m_pTrackCtrl->SetAllCtrlShow();
// 				theApp.m_pView->Invalidate(TRUE);
				break;
			}
			/*
			if(ptrackCtr->GetSerialID()==0)//主页面不能删除
			{
				CButton* pDelButton = (CButton*)GetDlgItem(IDC_BN_DELPAGE);
				pDelButton->EnableWindow(FALSE);
			}
			else
			{
				CButton* pDelButton = (CButton*)GetDlgItem(IDC_BN_DELPAGE);
				pDelButton->EnableWindow(TRUE);
			}
			if(ptrackCtr->GetSerialID()==itera->second)
			{
				theApp.m_pView->m_pTrackCtrl->SetAllCtrlHide();//原来的隐藏
				theApp.m_pView->m_pTrackCtrl = ptrackCtr;
				theApp.m_pView->m_pTrackCtrl->SetAllCtrlShow();
				theApp.m_pView->Invalidate(TRUE);
				break;
			}
			*/
		}
	}
}

void CShowPageDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
//	delete this;
	CDialog::PostNcDestroy();
}

void CShowPageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	map<int,UINT>::const_iterator itera = m_map_index.begin();
	for(itera;itera!=m_map_index.end();itera++)
	{
		if(theApp.m_pView->m_pTrackCtrl->GetSerialID() == itera->second)
		{
			m_OwnerListBox.SetCurSel(itera->first);
			OnLbnSelchangeListPage();
			break;
		}
	}
}

void CShowPageDlg::ClearCtrlLinkPageID(UINT nPageID)
{
	list<CTrackContrl*>::const_iterator itera = theApp.m_pView->m_list_trackCtrl.begin();
	for(itera;itera!=theApp.m_pView->m_list_trackCtrl.end();itera++)
	{
		CTrackContrl* pTrackContrl = *itera;
		pTrackContrl->SetAllCtrlLinkPageIDNULL(nPageID);
	}
}

void CShowPageDlg::SetBnTimeHide(BOOL isHide)
{
	m_bIsHide = isHide;
}