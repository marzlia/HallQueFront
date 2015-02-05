// PropEdText.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "PropEdText.h"
#include "PropEditing.h"
#include "HallQueFrontView.h"
#include "CommonConvert.h"


// CPropEdText 对话框

IMPLEMENT_DYNAMIC(CPropEdText, CPropertyPage)

CPropEdText::CPropEdText()
	: CPropertyPage(CPropEdText::IDD)
{
}

CPropEdText::~CPropEdText()
{
}

void CPropEdText::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STA_BKCOLOR, m_sta_bkcolor);
	DDX_Control(pDX, IDC_STA_TEXTCOLOR, m_sta_textcolor);
	DDX_Control(pDX, IDC_ED_CONTENT, m_ed_content);
	DDX_Control(pDX, IDC_ED_TEXTWIDTH, m_ed_textwidth);
	DDX_Control(pDX, IDC_EDIT_TEXTHEIGHT, m_ed_textheight);
	DDX_Control(pDX, IDC_COM_ALPHA, m_com_alpha);
}


BEGIN_MESSAGE_MAP(CPropEdText, CPropertyPage)
	ON_STN_CLICKED(IDC_STA_TEXTCOLOR, &CPropEdText::OnStnClickedStaTextcolor)
	ON_STN_CLICKED(IDC_STA_BKCOLOR, &CPropEdText::OnStnClickedStaBkcolor)
	ON_BN_CLICKED(IDC_BN_TEXTFONT, &CPropEdText::OnBnClickedBnTextfont)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RA_TEXTLEFT, &CPropEdText::OnBnClickedRaTextleft)
	ON_BN_CLICKED(IDC_RA_TEXTCENTER, &CPropEdText::OnBnClickedRaTextcenter)
	ON_BN_CLICKED(IDC_RA_TEXTRIGHT, &CPropEdText::OnBnClickedRaTextright)
END_MESSAGE_MAP()


// CPropEdText 消息处理程序

void CPropEdText::OnStnClickedStaTextcolor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog colorDlg;
	if(IDOK == colorDlg.DoModal())
	{
		m_textColor = colorDlg.GetColor();
		m_sta_textcolor.SetBkColor(m_textColor);
		Invalidate(TRUE);
	}
}

void CPropEdText::OnStnClickedStaBkcolor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog colorDlg;
	if(IDOK == colorDlg.DoModal())
	{
		m_bkColor = colorDlg.GetColor();
		m_sta_bkcolor.SetBkColor(m_bkColor);
		Invalidate(TRUE);
	}
}

BOOL CPropEdText::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_com_alpha.AddString(_T("0"));
	m_com_alpha.AddString(_T("10"));
	m_com_alpha.AddString(_T("20"));
	m_com_alpha.AddString(_T("30"));
	m_com_alpha.AddString(_T("40"));
	m_com_alpha.AddString(_T("50"));
	m_com_alpha.AddString(_T("60"));
	m_com_alpha.AddString(_T("70"));
	m_com_alpha.AddString(_T("80"));
	m_com_alpha.AddString(_T("90"));
	m_com_alpha.AddString(_T("100"));

	InitStaticTextInfo();
	InitStaticTextFont();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropEdText::InitStaticTextInfo()
{
	CPropEditing* pParent = (CPropEditing*)GetParent();
	CHallQueFrontView* pView = pParent->m_pView;
	if(pView->m_pTrackCtrl->m_pRightBnSelect && pView->m_isEdit)////编辑状态下获取文本的信息
	{
		//获取高、宽
		CRect rect;
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->GetClientRect(&rect);
		CString c_width,c_height,c_nAlpha;
		c_width.Format(_T("%d"),rect.Width());
		c_height.Format(_T("%d"),rect.Height());
		m_ed_textwidth.SetWindowText(c_width);
		m_ed_textheight.SetWindowText(c_height);

		m_ed_textwidth.EnableWindow(FALSE);
		m_ed_textheight.EnableWindow(FALSE);
		////////////////////////////////////////////获取透明度
		int nAlpha = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->GetAlpha();
		UINT per = abs((nAlpha*100)/255);
		int count = m_com_alpha.GetCount();
		CCommonConvert convert;
		//动态数组
		int *num=new int[count];
		for(int i=0;i<count;i++)
		{
			CString content;
			m_com_alpha.GetLBText(i,content);
			convert.CStringToint(num[i],content);
		}

		int value = FindMinDvalue(num,per,count);
		delete [] num;
		m_com_alpha.SetCurSel(value);
		////////////////////////////////////////////
		StaticTextAlign textAlign = pView->m_pTrackCtrl->m_pRightBnSelect->
			m_pTransStatic->GetStaticTextAlign();
		switch(textAlign)
		{
		case enumLeft:
			((CButton*)GetDlgItem(IDC_RA_TEXTLEFT))->SetCheck(BST_CHECKED);
			m_ed_content.ModifyStyle(ES_RIGHT | ES_CENTER,ES_LEFT);
			break;
		case enumCenter:
			((CButton*)GetDlgItem(IDC_RA_TEXTCENTER))->SetCheck(BST_CHECKED);
			m_ed_content.ModifyStyle(ES_RIGHT | ES_LEFT,ES_CENTER);
			break;
		case enumRight:
			((CButton*)GetDlgItem(IDC_RA_TEXTRIGHT))->SetCheck(BST_CHECKED);
			m_ed_content.ModifyStyle(ES_LEFT | ES_CENTER,ES_RIGHT);
			break;
		}
	}
	else
	{
		m_ed_textwidth.SetWindowText(_T("200"));
		m_ed_textheight.SetWindowText(_T("200"));
		int count = m_com_alpha.GetCount();
		if(count >0)
			m_com_alpha.SetCurSel(0);
		((CButton*)GetDlgItem(IDC_RA_TEXTLEFT))->SetCheck(BST_CHECKED);

		m_bkColor = RGB(255,255,255);
		m_textColor = RGB(0,0,0);
	}
}

void CPropEdText::OnBnClickedBnTextfont()
{
	// TODO: 在此添加控件通知处理程序代码
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	CFont* pFont = m_ed_content.GetFont();
	if(pFont != NULL)
		pFont->GetLogFont(&lf);


	CFontDialog fdlg(&lf);
//	fdlg.m_cf.rgbColors = m_textColor;//设置CFontDialog默认颜色
	LOGFONT lplf;
	if(IDOK==fdlg.DoModal())
	{
//		m_textColor = fdlg.GetColor();
		fdlg.GetCurrentFont(&lplf);

		m_textFont.DeleteObject();
		m_textFont.CreateFontIndirect(&lplf);
		m_ed_content.SetFont(&m_textFont,TRUE);//改变内容编辑框的字体和颜色
	}
}

void CPropEdText::InitStaticTextFont()
{
	CPropEditing* pParent = (CPropEditing*)GetParent();
	CHallQueFrontView* pView = pParent->m_pView;
	if(pView->m_pTrackCtrl->m_pRightBnSelect && pView->m_isEdit)////编辑状态下获取文本的字体
	{
		LOGFONT lfFont = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->GetTextFont();

		m_textFont.DeleteObject();
		m_textFont.CreateFontIndirect(&lfFont);

		m_textColor = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->GetTextColor();
		m_bkColor = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->GetBkColor();
		CString title;
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->GetWindowText(title);
		m_ed_content.SetWindowText(title);
	}
	else///////////////////新建状态下设置默认字体
	{
		CClientDC dc(this);
		m_textFont.CreateFont(
			-MulDiv(15, dc.GetDeviceCaps(LOGPIXELSY), 72),                        // nHeight
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
	m_sta_bkcolor.SetBkColor(m_bkColor);//设置背景颜色
	m_sta_textcolor.SetBkColor(m_textColor);//设置文字颜色
}

int CPropEdText::FindMinDvalue(int num[],int per,int size)
{
	int *Dvaule=new int[size];

	for(int i=0;i<size;i++)
	{
		Dvaule[i] = abs(per - num[i]);
	}

	int min=Dvaule[0];
	for(int i=0;i<size;i++)
	{
		min = min<Dvaule[i] ? min : Dvaule[i];
	}

	int loop=0;
	for(int i=0;i<size;i++)
	{
		if(min == Dvaule[i])
		{
			loop = i;
			break;
		}
	}
	delete [] Dvaule;
	return loop;
}
HBRUSH CPropEdText::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if(nCtlColor==CTLCOLOR_EDIT &&
		pWnd->GetDlgCtrlID()==IDC_ED_CONTENT)//注意此处的（pWnd->），否则没效果
	{
		CFont * pfntOld = pDC->SelectObject(&m_textFont);
		pDC->SetBkColor(m_bkColor);
		pDC->SetTextColor(m_textColor);
	}
	return hbr;
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
}

void CPropEdText::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropEditing* pParent = (CPropEditing*)GetParent();
	CHallQueFrontView* pView = pParent->m_pView;
	//获取字体
	LOGFONT lfFont;
	m_textFont.GetLogFont(&lfFont);
	
	CString content;
	m_ed_content.GetWindowText(content);
	//////////////////编辑状态下设置文本
	if(pView->m_pTrackCtrl->m_pRightBnSelect && pView->m_isEdit)
	{
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->SetBkColor(m_bkColor);
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->SetTextColor(m_textColor);

		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->SetFont(lfFont);

		
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->SetWindowText(content);

		int index = m_com_alpha.GetCurSel();
		if(index != CB_ERR)
		{
			CString c_Alpha;
			m_com_alpha.GetLBText(index,c_Alpha);
			CCommonConvert convert;
			int nAlpha = 0;
			convert.CStringToint(nAlpha,c_Alpha);

			UINT nValue = abs(255*nAlpha/100);
			pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->SetAlpha(nValue);
		}

		if(((CButton*)GetDlgItem(IDC_RA_TEXTLEFT))->GetCheck())
		{
			pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->SetStaticTextAlign(enumLeft);
		}
		else if(((CButton*)GetDlgItem(IDC_RA_TEXTCENTER))->GetCheck())
		{	
			pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->SetStaticTextAlign(enumCenter);
		}
		else if(((CButton*)GetDlgItem(IDC_RA_TEXTRIGHT))->GetCheck())
		{
			pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->SetStaticTextAlign(enumRight);
		}
//		pView->InvalidateRect(pView->m_pTrackCtrl->m_pRightBnSelect->m_rect);
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->Invalidate();
	}
	else/////新建状态下设置文本
	{
		//获取按钮的高宽
		CRect rect;
		int width = 0;
		int height = 0;
		CString c_width,c_heigh;
		CCommonConvert convert;
		m_ed_textheight.GetWindowText(c_heigh);
		m_ed_textwidth.GetWindowText(c_width);
		convert.CStringToint(width,c_width);
		convert.CStringToint(height,c_heigh);
		rect.left = 0; rect.right = width;
		rect.top = 0;  rect.bottom = height;
		//创建文本
		CControlRect *pStatic = new CControlRect(rect);
		pStatic->Create(enmStatic,content,pView,0,rect,0,0);
		pView->m_pTrackCtrl->Add(pStatic);
		//设置字体颜色和背景颜色
		pStatic->m_pTransStatic->SetTextColor(m_textColor);
		pStatic->m_pTransStatic->SetBkColor(m_bkColor);
		pStatic->m_pTransStatic->SetWindowText(content);

		if(((CButton*)GetDlgItem(IDC_RA_TEXTLEFT))->GetCheck())
		{
			pStatic->m_pTransStatic->SetStaticTextAlign(enumLeft);
		}
		else if(((CButton*)GetDlgItem(IDC_RA_TEXTCENTER))->GetCheck())
		{	
			pStatic->m_pTransStatic->SetStaticTextAlign(enumCenter);
		}
		else if(((CButton*)GetDlgItem(IDC_RA_TEXTRIGHT))->GetCheck())
		{
			pStatic->m_pTransStatic->SetStaticTextAlign(enumRight);
		}

		//设置文本字体
		pStatic->m_pTransStatic->SetFont(lfFont);
		//设置文本透明度
		int index = m_com_alpha.GetCurSel();
		if(index != CB_ERR)
		{
			CString c_Alpha;
			m_com_alpha.GetLBText(index,c_Alpha);
			int nAlpha = 0;
			CCommonConvert::CStringToint(nAlpha,c_Alpha);
			UINT nValue = abs(255*nAlpha/100);
			pStatic->m_pTransStatic->SetAlpha(nValue);
		}
	}
	CPropertyPage::OnOK();
}

void CPropEdText::OnBnClickedRaTextleft()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ed_content.ModifyStyle(ES_RIGHT | ES_CENTER,ES_LEFT);
	m_ed_content.Invalidate();
}

void CPropEdText::OnBnClickedRaTextcenter()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ed_content.ModifyStyle(ES_RIGHT | ES_LEFT,ES_CENTER);
	m_ed_content.Invalidate();
}

void CPropEdText::OnBnClickedRaTextright()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ed_content.ModifyStyle(ES_LEFT | ES_CENTER,ES_RIGHT);
	m_ed_content.Invalidate();
}
