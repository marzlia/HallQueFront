// PropEdButton.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "PropEdButton.h"
#include "HallQueFrontDoc.h"
#include "HallQueFrontView.h"
#include "PropEditing.h"
#include "ControlRect.h"
// CPropEdButton 对话框

IMPLEMENT_DYNAMIC(CPropEdButton, CPropertyPage)

CPropEdButton::CPropEdButton()
	: CPropertyPage(CPropEdButton::IDD)
{
	m_infofile_path = m_convert.GetExeFullFilePath();
	m_infofile_path += _T("\\QueBasicInfo\\QueBasicInfo.dat");
}

CPropEdButton::~CPropEdButton()
{
}

void CPropEdButton::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TRANS, m_com_trans);
	DDX_Control(pDX, IDC_STA_COLOR, m_sta_color);
	DDX_Control(pDX, IDC_ED_BNWIDTH, m_ed_width);
	DDX_Control(pDX, IDC_ED_BNHEIGHT, m_ed_height);
	//	DDX_Control(pDX, IDC_ED_TITLE, m_ed_title);
	DDX_Control(pDX, IDC_EDIT_TITLE, m_ed_title);
	DDX_Control(pDX, IDC_COM_QUE, m_com_queInfo);
	DDX_Control(pDX,IDC_STA_BTNBK,m_sta_bkpic);
}


BEGIN_MESSAGE_MAP(CPropEdButton, CPropertyPage)
	ON_STN_CLICKED(IDC_STA_COLOR, &CPropEdButton::OnStnClickedStaColor)
	ON_BN_CLICKED(IDC_BN_TITLEFONT, &CPropEdButton::OnBnClickedBnTitlefont)
	ON_WM_CTLCOLOR()
//	ON_CBN_SELCHANGE(IDC_COMBO2, &CPropEdButton::OnCbnSelchangeCombo2)
ON_BN_CLICKED(IDC_BTN_SETBK, &CPropEdButton::OnBnClickedBtnSetbk)
ON_BN_CLICKED(IDC_BTN_CLRBK, &CPropEdButton::OnBnClickedBtnClrbk)
ON_WM_PAINT()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPropEdButton 消息处理程序
void CPropEdButton::OnStnClickedStaColor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg;
	if(IDOK == dlg.DoModal())
	{
		m_bkColor = dlg.GetColor();
		m_sta_color.SetBkColor(m_bkColor);
	}
}

void CPropEdButton::OnBnClickedBnTitlefont()
{
	// TODO: 在此添加控件通知处理程序代码
	
	////使弹出的CFontDialog基于按钮的字体和颜色
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	CFont* pFont = m_ed_title.GetFont();
	if(pFont != NULL)
		pFont->GetLogFont(&lf);

	
	CFontDialog fdlg(&lf);
	fdlg.m_cf.rgbColors = m_textColor;//设置CFontDialog默认颜色
	LOGFONT lplf;
	if(IDOK==fdlg.DoModal())
	{
		m_textColor = fdlg.GetColor();
		fdlg.GetCurrentFont(&lplf);
		
		m_hTextFont.DeleteObject();
		m_hTextFont.CreateFontIndirect(&lplf);
		m_ed_title.SetFont(&m_hTextFont,TRUE);//改变标题编辑框的字体和颜色
	}
}

BOOL CPropEdButton::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_com_queInfo.AddString(_T("短信提示"));
	m_com_trans.AddString(_T("0"));
	m_com_trans.AddString(_T("10"));
	m_com_trans.AddString(_T("20"));
	m_com_trans.AddString(_T("30"));
	m_com_trans.AddString(_T("40"));
	m_com_trans.AddString(_T("50"));
	m_com_trans.AddString(_T("60"));
	m_com_trans.AddString(_T("70"));
	m_com_trans.AddString(_T("80"));
	m_com_trans.AddString(_T("90"));
	m_com_trans.AddString(_T("100"));
	ReadQueInfoFromFile();
	list<CTrackContrl*>::const_iterator itera = theApp.m_pView->m_list_trackCtrl.begin();
	for(itera;itera!=theApp.m_pView->m_list_trackCtrl.end();itera++)
	{
		CTrackContrl* pTrackContrl = *itera;
		CString pageName;
		if(pTrackContrl->GetSerialID()==0)
			pageName=_T("主页面");
		else
			pageName.Format(_T("页面[%d]"),pTrackContrl->GetSerialID());
		m_map_pageID[m_com_queInfo.AddString(pageName)] = pTrackContrl->GetSerialID();
	}
	InitButtonInfo();
	InitFont();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CPropEdButton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if(nCtlColor==CTLCOLOR_EDIT &&
		pWnd->GetDlgCtrlID()==IDC_EDIT_TITLE)//注意此处的（pWnd->），否则没效果
	{
		CFont * pfntOld = pDC->SelectObject(&m_hTextFont);
		pDC->SetTextColor(m_textColor);
	}
	return hbr;
}


void CPropEdButton::InitFont()
{
	CPropEditing* pParent = (CPropEditing*)GetParent();
	CHallQueFrontView* pView = pParent->m_pView;
	if(pView->m_pTrackCtrl->m_pRightBnSelect && pView->m_isEdit)////编辑状态下获取按钮的字体
	{
		LOGFONT lfFont = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->GetFont();
		
		m_hTextFont.DeleteObject();
		m_hTextFont.CreateFontIndirect(&lfFont);

		m_textColor = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->GetTextColor();
		m_bkColor = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->GetBkColor();
		CString title;
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->GetWindowText(title);
		m_ed_title.SetWindowText(title);
	}
	else///////////////////新建状态下设置默认字体
	{
		CClientDC dc(this);
		m_hTextFont.CreateFont(
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
	m_ed_title.SetFont(&m_hTextFont,TRUE);//设置字体
	m_sta_color.SetBkColor(m_bkColor);//设置背景颜色
}

void CPropEdButton::InitButtonInfo()
{
	CPropEditing* pParent = (CPropEditing*)GetParent();
	CHallQueFrontView* pView = pParent->m_pView;
	if(pView->m_pTrackCtrl->m_pRightBnSelect && pView->m_isEdit)////编辑状态下获取按钮的信息
	{
		CRect rect;
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->GetClientRect(&rect);
		CString c_width,c_height,c_nAlpha;
		c_width.Format(_T("%d"),rect.Width());
		c_height.Format(_T("%d"),rect.Height());
		m_ed_width.SetWindowText(c_width);
		m_ed_height.SetWindowText(c_height);
		
		m_ed_width.EnableWindow(FALSE);
		m_ed_height.EnableWindow(FALSE);
		/////////////////////////////////////
		int nAlpha = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->GetAlpha();
		UINT per = abs((nAlpha*100)/255);
		int count = m_com_trans.GetCount();
		CCommonConvert convert;
		//动态数组
		int *num=new int[count];
		for(int i=0;i<count;i++)
		{
			CString content;
			m_com_trans.GetLBText(i,content);
			convert.CStringToint(num[i],content);
		}
		
		int value = FindMinDvalue(num,per,count);
		delete [] num;
		m_com_trans.SetCurSel(value);
		//////////////////////////////////////
		ButtonTypeEnum type = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->GetButtonType();
		switch(type)
		{
		case enumRectButton:
			((CButton*)GetDlgItem(IDC_RAD_RECT))->SetCheck(BST_CHECKED);
			break;
		case enumRoundRectButton:
			((CButton*)GetDlgItem(IDC_RAD_ROUNDRECT))->SetCheck(BST_CHECKED);
			break;
		case enumEllipseButton:
			((CButton*)GetDlgItem(IDC_RAD_ELLIPSE))->SetCheck(BST_CHECKED);
			break;
		}
		/////////选择链接队列
		CString address = pView->m_pTrackCtrl->m_pRightBnSelect->
			m_pTransButton->GetAttchAddress();
		if (address == L"短信提示")
		{
			m_com_queInfo.SelectString(0,L"短信提示");
		}
		else
		{
			for(int i=0;i<m_com_queInfo.GetCount();i++)
			{
				CQueueInfo queInfo;
				if(m_map_que.Lookup(i,queInfo))
				{
					if(queInfo.GetQueID() == address)
					{
						m_com_queInfo.SetCurSel(i);
						break;
					}
				}
			}
		}

		//////////选择链接的界面
		int nPageID = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton
			->GetAttachPageID();
		map<int,UINT>::const_iterator itera = m_map_pageID.begin();
		for(itera;itera!=m_map_pageID.end();itera++)
		{
			if(nPageID == itera->second)
			{
				m_com_queInfo.SetCurSel(itera->first);
				break;
			}
		}
		/////////////////////////////////
		m_BkPicPath = pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->GetBkPic();
		LoadPicImage(m_BkPicPath);
		Invalidate(TRUE);
	}
	else////新建状态下设置默认值
	{
		//设置默认值
		m_ed_width.SetWindowText(_T("200"));
		m_ed_height.SetWindowText(_T("50"));
		int count = m_com_trans.GetCount();
		m_com_trans.SetCurSel(count-1);
		((CButton*)GetDlgItem(IDC_RAD_RECT))->SetCheck(BST_CHECKED);

		m_bkColor = RGB(95,158,160);
		m_textColor = RGB(0,0,0);
	}
}

int CPropEdButton::FindMinDvalue(int num[],int per,int size)
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

BOOL CPropEdButton::OnApply()
{
	// TODO: 在此添加专用代码和/或调用基类
	return CPropertyPage::OnApply();
}

void CPropEdButton::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropEditing* pParent = (CPropEditing*)GetParent();
	CHallQueFrontView* pView = pParent->m_pView;
	//获取字体
	LOGFONT lfFont;
	m_hTextFont.GetLogFont(&lfFont);
	//
	CString title;
	m_ed_title.GetWindowText(title);
	/////////////////////////////////////////
	int queIndex = m_com_queInfo.GetCurSel();
	/////////////////////////////////////////
	//////////////////编辑状态下设置按钮
	if(pView->m_pTrackCtrl->m_pRightBnSelect && pView->m_isEdit)
	{
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->SetBkColor(m_bkColor);
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->SetTextColor(m_textColor);
		
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->SetFont(lfFont);
		
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->SetBkPic(m_BkPicPath);
		if(queIndex != CB_ERR)
		{
			if (queIndex==0)
			{
				CString strMsg = L"短信提示";
				pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->
					AttachToQueue(strMsg);
				pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->
					AttachToPage(-1);
			}
			////链接队列
			CQueueInfo queInfo;
			if(m_map_que.Lookup(queIndex,queInfo))
			{
				CString address = queInfo.GetQueID();
				pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->
					AttachToQueue(address);
				pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->
					AttachToPage(-1);
			}
			//链接界面
			map<int,UINT>::const_iterator itera = m_map_pageID.find(queIndex);
			if(itera!=m_map_pageID.end())
			{
				pView->m_pTrackCtrl->m_pRightBnSelect->
					m_pTransButton->AttachToPage(itera->second);
				CString str;
				pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->
					AttachToQueue(str);
			}
		}
		
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->SetWindowText(title);
		
		int index = m_com_trans.GetCurSel();
		if(index != CB_ERR)
		{
			CString c_Alpha;
			m_com_trans.GetLBText(index,c_Alpha);
			CCommonConvert convert;
			int nAlpha = 0;
			convert.CStringToint(nAlpha,c_Alpha);
			
			UINT nValue = abs(255*nAlpha/100);
			pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->SetAlpha(nValue);
		}

		if(((CButton*)GetDlgItem(IDC_RAD_RECT))->GetCheck())
		{
			pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->SetButtonType(enumRectButton);
		}
		else if(((CButton*)GetDlgItem(IDC_RAD_ROUNDRECT))->GetCheck())
		{	
			pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->SetButtonType(enumRoundRectButton);
		}
		else if(((CButton*)GetDlgItem(IDC_RAD_ELLIPSE))->GetCheck())
		{
			pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->SetButtonType(enumEllipseButton);
		}
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransButton->Invalidate(TRUE);//重绘
	}
	else/////新建状态下设置按钮
	{
		//获取按钮的高宽
		CRect rect;
		int width = 0;
		int height = 0;
		CString c_width,c_heigh;
		CCommonConvert convert;
		m_ed_height.GetWindowText(c_heigh);
		m_ed_width.GetWindowText(c_width);
		convert.CStringToint(width,c_width);
		convert.CStringToint(height,c_heigh);
		rect.left = 0; rect.right = width;
		rect.top = 0;  rect.bottom = height;
		//创建按钮
		CControlRect *pButton = new CControlRect(rect);
		pButton->Create(enmButton,title,pView,0,rect,0,0);
		pView->m_pTrackCtrl->Add(pButton);
		//设置字体颜色和背景颜色
		pButton->m_pTransButton->SetTextColor(m_textColor);
		pButton->m_pTransButton->SetBkColor(m_bkColor);
		pButton->m_pTransButton->SetBkPic(m_BkPicPath);
		//设置按钮形状
		if(((CButton*)GetDlgItem(IDC_RAD_RECT))->GetCheck())
		{
			pButton->m_pTransButton->SetButtonType(enumRectButton);
		}
		else if(((CButton*)GetDlgItem(IDC_RAD_ROUNDRECT))->GetCheck())
		{	
			pButton->m_pTransButton->SetButtonType(enumRoundRectButton);
		}
		else if(((CButton*)GetDlgItem(IDC_RAD_ELLIPSE))->GetCheck())
		{
			pButton->m_pTransButton->SetButtonType(enumEllipseButton);
		}
		//设置按钮字体
		pButton->m_pTransButton->SetFont(lfFont);
		//设置按钮透明度
		int index = m_com_trans.GetCurSel();
		if(index != CB_ERR)
		{
			CString c_Alpha;
			m_com_trans.GetLBText(index,c_Alpha);
			CCommonConvert convert;
			int nAlpha = 0;
			convert.CStringToint(nAlpha,c_Alpha);

			UINT nValue = abs(255*nAlpha/100);
			pButton->m_pTransButton->SetAlpha(nValue);
		}
		//设置链接队列
		if(queIndex != CB_ERR)
		{
			if (queIndex==0)
			{
			   CString strMsg = L"短信提示";
				pButton->m_pTransButton->AttachToQueue(strMsg);
				pButton->m_pTransButton->AttachToPage(-1);
					
			}
			////链接队列
			CQueueInfo queInfo;
			if(m_map_que.Lookup(queIndex,queInfo))
			{
				CString address = queInfo.GetQueID();
				pButton->m_pTransButton->AttachToQueue(address);
				pButton->m_pTransButton->AttachToPage(-1);
			}
			//链接界面
			map<int,UINT>::const_iterator itera = m_map_pageID.find(queIndex);
			if(itera!=m_map_pageID.end())
			{
				pButton->m_pTransButton->AttachToPage(itera->second);
				CString str;
				pButton->m_pTransButton->AttachToQueue(str);
			}
		}
	}
	CPropertyPage::OnOK();
}

void CPropEdButton::ReadQueInfoFromFile()
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

BOOL CPropEdButton::LoadPicImage(CString PicPath)
{

	if (PicPath.IsEmpty())
	{
		/*pDC->SelectStockObject(LTGRAY_BRUSH);
		pDC->FillRect(	
			&rect, pDC->GetCurrentBrush());*/
		return FALSE;
	}
	else
	{
		if(!m_img.IsNull())
			m_img.Destroy();
		HRESULT hResult = m_img.Load(m_BkPicPath);
		if(FAILED(hResult))
		{
			//MessageBox(_T("图片路径错误，导入失败"));
			return FALSE;
		}
		
	}

	return TRUE;
}

void CPropEdButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	//LoadPicImage(m_BkPicPath,(CWnd*)&m_sta_bkpic);
	// 不为绘图消息调用 CPropertyPage::OnPaint()
}
void CPropEdButton::OnBnClickedBtnSetbk()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog picdlg(TRUE, 0, 0, 4|2, _T("files(*.jpg,*.jpeg,*.bmp,*png,*gif)|*.jpg;*.jpeg;*.bmp;*png;*gif||"));
	if(IDOK==picdlg.DoModal())
	{
		m_BkPicPath=picdlg.GetPathName();
		LoadPicImage(m_BkPicPath);
		Invalidate(TRUE);
	}
}

void CPropEdButton::OnBnClickedBtnClrbk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_BkPicPath = _T("");
	if(!m_img.IsNull())
		m_img.Destroy();
	UpdateData(FALSE);
}

BOOL CPropEdButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BOOL flag = CPropertyPage::OnEraseBkgnd(pDC);
	DrawImage((CWnd*)&m_sta_bkpic);
	return flag;
}

BOOL CPropEdButton::DrawImage(CWnd* pWnd)
{
	if (!m_img.IsNull())
	{
		CStatic* picPhoto = (CStatic*)pWnd;
		CDC* pDC = picPhoto->GetWindowDC();
		CRect rect;
		picPhoto->GetWindowRect(&rect);
		pDC->SetStretchBltMode(STRETCH_HALFTONE);
		m_img.Draw(pDC->m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0, m_img.GetWidth(), m_img.GetHeight());
		ReleaseDC(pDC);
		return TRUE;
	}
	else
	{
		CStatic* picPhoto = (CStatic*)(CWnd*)pWnd;
		CDC* pDC = picPhoto->GetWindowDC();
		CRect rect;
		picPhoto->GetClientRect(&rect);
		pDC->SelectStockObject(LTGRAY_BRUSH);
		pDC->FillRect(	
			&rect, pDC->GetCurrentBrush());
		return TRUE;
	}
	return TRUE;
}

