// PropShowQueNum.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "PropShowQueNum.h"
#include "PropEditing.h"


// CPropShowQueNum 对话框

IMPLEMENT_DYNAMIC(CPropShowQueNum, CPropertyPage)

CPropShowQueNum::CPropShowQueNum()
	: CPropertyPage(CPropShowQueNum::IDD)
	,m_textColor(RGB(0,0,0))
{
	m_infofile_path = m_convert.GetExeFullFilePath();
	m_infofile_path += _T("\\QueBasicInfo\\QueBasicInfo.dat");
}

CPropShowQueNum::~CPropShowQueNum()
{
}

void CPropShowQueNum::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STA_COLOR, m_sta_textColor);
	DDX_Control(pDX, IDC_ED_CONTENT, m_ed_content);
	DDX_Control(pDX, IDC_COM_QUE, m_com_queInfo);
}


BEGIN_MESSAGE_MAP(CPropShowQueNum, CPropertyPage)
//	ON_CBN_SELCHANGE(IDC_COMBO1, &CPropShowQueNum::OnCbnSelchangeCombo1)
ON_STN_CLICKED(IDC_STA_COLOR, &CPropShowQueNum::OnStnClickedStaColor)
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_BN_FONT, &CPropShowQueNum::OnBnClickedBnFont)
END_MESSAGE_MAP()


// CPropShowQueNum 消息处理程序

void CPropShowQueNum::OnStnClickedStaColor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg;
	if(IDOK == dlg.DoModal())
	{
		m_textColor = dlg.GetColor();
		m_sta_textColor.SetBkColor(m_textColor);
		Invalidate();
	}
}

BOOL CPropShowQueNum::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ReadQueInfoFromFile();
	InitInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropShowQueNum::InitInfo()
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
		CString head=pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic
			->GetForShowQueNumHead();
		m_ed_content.SetWindowText(head);
		///attach队列
		CString address = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic
			->GetLinkAddress();
		for(int i=0;i<m_com_queInfo.GetCount();i++)
		{
			if(address == m_map_que[i].GetQueID())
			{
				m_com_queInfo.SetCurSel(i);
				break;
			}
		}
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
	m_ed_content.SetFont(&m_textFont,TRUE);//设置字体
	m_sta_textColor.SetBkColor(m_textColor);//设置背景颜色
}
void CPropShowQueNum::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropEditing* pParent = (CPropEditing*)GetParent();
	CHallQueFrontView* pView = pParent->m_pView;
	//获取字体
	LOGFONT lfFont;
	m_textFont.GetLogFont(&lfFont);
	CString Content;
	m_ed_content.GetWindowText(Content);
	//////////////////////////////
	CRect rect;
	CSize textSize;
	HDC hdc = ::GetDC(m_ed_content.m_hWnd);
	HFONT hOldFont =(HFONT) ::SelectObject(hdc,m_textFont);
	GetTextExtentPoint32(hdc,Content,Content.GetLength()+3,&textSize);
	::SelectObject(hdc,hOldFont);
	::ReleaseDC(m_ed_content.m_hWnd,hdc);
	//////////////////////////////////
	////////////////////////////////
	int index = m_com_queInfo.GetCurSel();
	/////////////////////////////////
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
			SetWindowText(Content);
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->
			SetForShowQueNumHead(Content);
		//设置链接队列
		if(index != CB_ERR)
		{
			//链接队列的ID
			CString address = m_map_que[index].GetQueID();
			pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->
				SetLinkAddress(address);
		}
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
		rect.left=0;rect.right=textSize.cx;
		rect.top=0;rect.bottom=textSize.cy;
		//创建
		CControlRect *pStatic = new CControlRect(rect);
		pStatic->Create(enmStatic,Content,pView,0,rect,0,0);
		pView->m_pTrackCtrl->Add(pStatic);
		pStatic->m_pTransStatic->SetShowQueNum(TRUE);
		//设置字体颜色
		pStatic->m_pTransStatic->SetTextColor(m_textColor);
		pStatic->m_pTransStatic->SetFont(lfFont);
		pStatic->m_pTransStatic->SetForShowQueNumHead(Content);

		if(index != CB_ERR)
		{
			//链接队列的ID
			CString address = m_map_que[index].GetQueID();
			pStatic->m_pTransStatic->SetLinkAddress(address);
		}
	}
	CPropertyPage::OnOK();
}

HBRUSH CPropShowQueNum::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if(nCtlColor==CTLCOLOR_EDIT &&
		pWnd->GetDlgCtrlID()==IDC_ED_CONTENT)//注意此处的（pWnd->），否则没效果
	{
		CFont * pfntOld = pDC->SelectObject(&m_textFont);
		pDC->SetTextColor(m_textColor);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CPropShowQueNum::OnBnClickedBnFont()
{
	// TODO: 在此添加控件通知处理程序代码
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	CFont* pFont = m_ed_content.GetFont();
	if(pFont != NULL)
		pFont->GetLogFont(&lf);


	CFontDialog fdlg(&lf);
	LOGFONT lplf;
	if(IDOK==fdlg.DoModal())
	{
		fdlg.GetCurrentFont(&lplf);
		m_textFont.DeleteObject();
		m_textFont.CreateFontIndirect(&lplf);
		m_ed_content.SetFont(&m_textFont,TRUE);//改变标题编辑框的字体和颜色
	}
}

void CPropShowQueNum::ReadQueInfoFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_infofile_path,CFile::modeRead,&e))
	{
		CQueueInfo* queinfo=NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>queinfo;
				if (queinfo)
				{
					m_map_que[m_com_queInfo.AddString(queinfo->GetBussName())]
					= *queinfo;
					delete queinfo;
					queinfo = NULL;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
	}
}
