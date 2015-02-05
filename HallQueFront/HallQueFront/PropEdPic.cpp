// PropEdPic.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "PropEdPic.h"
#include "PropEditing.h"

#include "CommonConvert.h"

// CPropEdPic 对话框
#ifdef _DEBUG
extern void MyWriteConsole(CString str);
#endif

IMPLEMENT_DYNAMIC(CPropEdPic, CPropertyPage)

CPropEdPic::CPropEdPic()
	: CPropertyPage(CPropEdPic::IDD)
	, m_picType(_T("files(*.jpg,*.jpeg,*.bmp,*png,*gif)|*.jpg;*.jpeg;*.bmp;*png;*gif||"))
{
//	m_picType=_T("files(*.jpg,*.jpeg,*.bmp,*png,*gif)|*.jpg;*.jpeg;*.bmp;*png;*gif||");
}

CPropEdPic::~CPropEdPic()
{
}

void CPropEdPic::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_ALPHA, m_com_alpha);
	DDX_Control(pDX, IDC_ED_PATH, m_ed_path);
	DDX_Control(pDX, IDC_ED_HEIGHT, m_ed_picheight);
	DDX_Control(pDX, IDC_ED_WIDTH, m_ed_picwidth);
	//	DDX_Control(pDX, IDC_STA_PIC, m_sta_pic);
	DDX_Control(pDX, IDC_STA_PIC, m_sta_pic);
}


BEGIN_MESSAGE_MAP(CPropEdPic, CPropertyPage)
	ON_BN_CLICKED(IDC_BN_CHOOSEPIC, &CPropEdPic::OnBnClickedBnChoosepic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPropEdPic 消息处理程序

void CPropEdPic::OnBnClickedBnChoosepic()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog picdlg(TRUE, 0, 0, 4|2, m_picType);
	if(IDOK==picdlg.DoModal())
	{
		CString path=picdlg.GetPathName();
		m_ed_path.SetWindowText(path);
		LoadPicImage(path);
		Invalidate(TRUE);
	}
}

BOOL CPropEdPic::LoadPicImage(const CString& path)
{
	if(!path.IsEmpty())
	{
		if(!m_picImage.IsNull())
			m_picImage.Destroy();
		HRESULT hResult=m_picImage.Load(path);
		if(FAILED(hResult))
		{
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CPropEdPic::OnInitDialog()
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

	InitStaticPicInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropEdPic::InitStaticPicInfo()
{
	CPropEditing* pParent = (CPropEditing*)GetParent();
	CHallQueFrontView* pView = pParent->m_pView;
	if(pView->m_pTrackCtrl->m_pRightBnSelect && pView->m_isEdit)
	{
		//获取高、宽
		CRect rect;
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->GetClientRect(&rect);
		CString c_width,c_height,c_nAlpha;
		c_width.Format(_T("%d"),rect.Width());
		c_height.Format(_T("%d"),rect.Height());
		m_ed_picwidth.SetWindowText(c_width);
		m_ed_picheight.SetWindowText(c_height);

		m_ed_picwidth.EnableWindow(FALSE);
		m_ed_picheight.EnableWindow(FALSE);
		/////////////////////////////////////////
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

		CString lastPath = pView->m_pTrackCtrl->m_pRightBnSelect->
			m_pTransStatic->GetLoadImagePath();
		m_ed_path.SetWindowText(lastPath);
		LoadPicImage(lastPath);
	}
	else
	{
		m_ed_picwidth.SetWindowText(_T("200"));
		m_ed_picheight.SetWindowText(_T("200"));
		int count = m_com_alpha.GetCount();
		if(count >0)
			m_com_alpha.SetCurSel(count-1);
	}
}
void CPropEdPic::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropEditing* pParent = (CPropEditing*)GetParent();
	CHallQueFrontView* pView = pParent->m_pView;
	if(pView->m_pTrackCtrl->m_pRightBnSelect && pView->m_isEdit)
	{
		CString path;
		m_ed_path.GetWindowText(path);	
		CString lastPath = pView->m_pTrackCtrl->m_pRightBnSelect->
			m_pTransStatic->GetLoadImagePath();
		if(path != lastPath)
		{
			pView->m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->SetLoadImagePath(path);
			pView->m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->LoadPathImage();
		}
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
		pView->m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->Invalidate(TRUE);
	}
	else
	{
		CRect rect;
		int width = 0;
		int height = 0;
		CString c_width,c_heigh;
		CCommonConvert convert;
		m_ed_picheight.GetWindowText(c_heigh);
		m_ed_picwidth.GetWindowText(c_width);
		convert.CStringToint(width,c_width);
		convert.CStringToint(height,c_heigh);
		rect.left = 0; rect.right = width;
		rect.top = 0;  rect.bottom = height;

		CControlRect *pPic = new CControlRect(rect);
		CString path; 
		m_ed_path.GetWindowText(path);
		pPic->Create(enmStatic,L"",pView,SS_CENTERIMAGE,rect,0,0,path);
		pView->m_pTrackCtrl->Add(pPic);

		int index = m_com_alpha.GetCurSel();
		if(index != CB_ERR)
		{
			CString c_Alpha;
			m_com_alpha.GetLBText(index,c_Alpha);
			CCommonConvert convert;
			int nAlpha = 0;
			convert.CStringToint(nAlpha,c_Alpha);

			UINT nValue = abs(255*nAlpha/100);
			pPic->m_pTransStatic->SetAlpha(nValue);
		}
	}
	CPropertyPage::OnOK();
}

int CPropEdPic::FindMinDvalue(int num[],int per,int size)
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

void CPropEdPic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPropertyPage::OnPaint()
}

BOOL CPropEdPic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	CPropertyPage::OnEraseBkgnd(pDC);
	if(!m_picImage.IsNull())
	{
		CRect rect;
		m_sta_pic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.top += 7;
		SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);//避免颜色失真
		m_picImage.StretchBlt(pDC->GetSafeHdc(),rect.left,rect.top,rect.Width(),
			rect.Height(),SRCCOPY);
	}
	return TRUE;
}


