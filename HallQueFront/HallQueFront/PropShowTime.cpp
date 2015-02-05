// PropShowTime.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "PropShowTime.h"
#include "PropEditing.h"


// CPropShowTime 对话框

IMPLEMENT_DYNAMIC(CPropShowTime, CPropertyPage)

CPropShowTime::CPropShowTime()
	: CPropertyPage(CPropShowTime::IDD)
	, m_textColor(RGB(0,0,0))
{
	
}

CPropShowTime::~CPropShowTime()
{
//	KillTimer(ShowTime);
}

void CPropShowTime::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_TIMESTYLE, m_com_timeStyle);
	DDX_Control(pDX, IDC_EDIT_EXAMPLE, m_ed_example);
	DDX_Control(pDX, IDC_STA_COLOR, m_sta_color);
}


BEGIN_MESSAGE_MAP(CPropShowTime, CPropertyPage)
	
	ON_BN_CLICKED(IDC_BN_FONT, &CPropShowTime::OnBnClickedBnFont)
	ON_CBN_SELCHANGE(IDC_COM_TIMESTYLE, &CPropShowTime::OnCbnSelchangeComTimestyle)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
//	ON_STN_CLICKED(IDC_STA_COLOR, &CPropShowTime::OnStnClickedStaColor)
ON_STN_CLICKED(IDC_STA_COLOR, &CPropShowTime::OnStnClickedStaColor)
END_MESSAGE_MAP()


void CPropShowTime::OnBnClickedBnFont()
{
	// TODO: 在此添加控件通知处理程序代码
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	CFont* pFont = m_ed_example.GetFont();
	if(pFont != NULL)
		pFont->GetLogFont(&lf);


	CFontDialog fdlg(&lf);
	LOGFONT lplf;
	if(IDOK==fdlg.DoModal())
	{
		fdlg.GetCurrentFont(&lplf);
		m_textFont.DeleteObject();
		m_textFont.CreateFontIndirect(&lplf);
		m_ed_example.SetFont(&m_textFont,TRUE);//改变标题编辑框的字体和颜色
	}
}

BOOL CPropShowTime::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CTime currentTime;
	currentTime = CTime::GetCurrentTime();
	CString time;
	time.Format(_T("%d年%d月%d日 %d时%d分%d秒"),
		currentTime.GetYear(),currentTime.GetMonth(),currentTime.GetDay(),
		currentTime.GetHour(),currentTime.GetMinute(),currentTime.GetSecond());
	m_com_timeStyle.AddString(time);
	time = currentTime.Format(_T("%Y-%m-%d"));
	m_com_timeStyle.AddString(time);
	time = currentTime.Format(_T("%H:%M:%S"));
	m_com_timeStyle.AddString(time);
	time = currentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	m_com_timeStyle.AddString(time);
	//初始化
	InitFont();

	SetTimer(ShowTime,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropShowTime::OnCbnSelchangeComTimestyle()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CPropShowTime::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CTime currentTime;
	currentTime = CTime::GetCurrentTime();
	CString time;

	switch(nIDEvent)
	{
	case ShowTime:
		{
			int index = m_com_timeStyle.GetCurSel();
			if(index!=-1)
			{
				switch(index)
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
			}
			m_ed_example.SetWindowText(time);
		}
		break;
	}
	CPropertyPage::OnTimer(nIDEvent);
}

void CPropShowTime::InitFont()
{
	CPropEditing* pParent = (CPropEditing*)GetParent();
	CHallQueFrontView* pView = pParent->m_pView;
	if(pView->m_pTrackCtrl->m_pRightBnSelect && pView->m_isEdit)
	{
		LOGFONT lfFont = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic
			->GetTextFont();
		m_textFont.DeleteObject();
		m_textFont.CreateFontIndirect(&lfFont);
		m_textColor = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic
			->GetTextColor();
		m_timeFormat = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic
			->GetTimeFormat();
		m_com_timeStyle.SetCurSel(m_timeFormat);
	}
	else
	{
		CClientDC dc(this);
		m_textFont.CreateFont(
			-MulDiv(20, dc.GetDeviceCaps(LOGPIXELSY), 72),                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("宋体")); 
	}

	m_ed_example.SetFont(&m_textFont,TRUE);
	m_sta_color.SetBkColor(m_textColor);
}
HBRUSH CPropShowTime::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if(nCtlColor==CTLCOLOR_EDIT &&
		pWnd->GetDlgCtrlID()==IDC_EDIT_EXAMPLE)//注意此处的（pWnd->），否则没效果
	{
		CFont * pfntOld = pDC->SelectObject(&m_textFont);
		pDC->SetTextColor(m_textColor);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CPropShowTime::OnStnClickedStaColor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg;
	if(IDOK == dlg.DoModal())
	{
		m_textColor = dlg.GetColor();
		m_sta_color.SetBkColor(m_textColor);
	}
}

void CPropShowTime::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropEditing* pParent = (CPropEditing*)GetParent();
	CHallQueFrontView* pView = pParent->m_pView;
	//获取字体
	LOGFONT lfFont;
	m_textFont.GetLogFont(&lfFont);
	int index = m_com_timeStyle.GetCurSel();
	if(index==-1)
	{
		return;
	}
	else
	{
		switch(index)
		{
		case 0:
			m_timeFormat = enumTimeChinese;
			break;
		case 1:
			m_timeFormat = enumTimeYearMonthDay;
			break;
		case 2:
			m_timeFormat = enumTimeHourMinSec;
			break;
		case 3:
			m_timeFormat = enumTimeYMDHMS;
			break;
		}
	}
	CString text;
	m_ed_example.GetWindowText(text);
	CRect rect;
	CSize textSize;
	HDC hdc = ::GetDC(m_ed_example.m_hWnd);
	HFONT hOldFont =(HFONT) ::SelectObject(hdc,m_textFont);
	GetTextExtentPoint32(hdc,text,text.GetLength(),&textSize);
	::SelectObject(hdc,hOldFont);
	::ReleaseDC(m_ed_example.m_hWnd,hdc);
	////////////////////////////////////////
	if(pView->m_pTrackCtrl->m_pRightBnSelect && pView->m_isEdit)
	{
		rect = pView->m_pTrackCtrl->m_pRightBnSelect->m_rect;
		//获取OldRect
		CRect oldRect;
		pView->m_pTrackCtrl->m_pRightBnSelect->GetTrueRect(&oldRect);
		/////////////////////////////////////////
		rect.right = rect.left + textSize.cx;
		rect.bottom = rect.top + textSize.cy;
		pView->m_pTrackCtrl->m_pRightBnSelect->m_rect = rect;
		pView->m_pTrackCtrl->m_pRightBnSelect->MoveWindow(&rect,FALSE);
		//设置字体颜色
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->
			SetTextColor(m_textColor);
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->
			SetFont(lfFont);
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->
			SetTimeFormat(m_timeFormat);
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->
			SetWindowText(text);
		//重绘
		pView->m_pTrackCtrl->m_pRightBnSelect->GetParent()->
			InvalidateRect(&oldRect);
		CRect newRect;
		pView->m_pTrackCtrl->m_pRightBnSelect->GetTrueRect(newRect);
		pView->m_pTrackCtrl->m_pRightBnSelect->GetParent()->
			InvalidateRect(&newRect);
	}
	else
	{
		//创建显示时间
		rect.left=0;rect.right=textSize.cx;
		rect.top=0;rect.bottom=textSize.cy+2;
		
		CControlRect *pStatic = new CControlRect(rect);
		pStatic->Create(enmStatic,text,pView,0,rect,0,0);
		pView->m_pTrackCtrl->Add(pStatic);
		pStatic->m_pTransStatic->SetShowTime(TRUE);
		//设置字体颜色
		pStatic->m_pTransStatic->SetTextColor(m_textColor);
		pStatic->m_pTransStatic->SetFont(lfFont);
		pStatic->m_pTransStatic->SetTimeFormat(m_timeFormat);
		pStatic->m_pTransStatic->SetWindowText(text);
//		pStatic->m_pTransStatic->Invalidate();
	}
	CPropertyPage::OnOK();
}
