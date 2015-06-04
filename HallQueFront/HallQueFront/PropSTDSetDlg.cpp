// PropSTDSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PropSTDSetDlg.h"
#include "StbContent.h"
#include "DoFile.h"
#include "CommonConvert.h"


// CPropSTDSetDlg 对话框

IMPLEMENT_DYNAMIC(CPropSTDSetDlg, CDialog)

CPropSTDSetDlg::CPropSTDSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPropSTDSetDlg::IDD, pParent)
	, m_cs_stdnum(_T(""))
	, m_cs_stdname(_T(""))
	, m_cs_stdtitle(_T(""))
	, m_cs_stbnotice(_T(""))
	, m_cs_titlepic(_T(""))
	, m_cs_linenum(_T(""))
{
	CDoFile doFile;
	m_strExePath = doFile.GetExeFullFilePath();

	m_strExePath += _T("\\StbBasicInfo");
	doFile.CreateMyDirectory(m_strExePath);

	m_strExePath += _T("\\StbInfo.dat");
}

CPropSTDSetDlg::~CPropSTDSetDlg()
{
	ClearStbInfo();
}

void CPropSTDSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STD, m_listctr_std);
	// 	DDX_Control(pDX, IDC_EDIT_STBNUM, m_ed_stbnum);
	// 	DDX_Control(pDX, IDC_EDIT_STBNAME, m_ed_stbname);
	// 	DDX_Control(pDX, IDC_EDIT_STBTITLE, m_ed_stbtitle);
	// 	DDX_Control(pDX, IDC_EDIT_STBNOTICE, m_ed_stbnotice);
	DDX_Control(pDX, IDC_EDIT_TITLEPIC, m_ed_titlepic);
	DDX_Text(pDX, IDC_EDIT_STBNUM, m_cs_stdnum);
	DDX_Text(pDX, IDC_EDIT_STBNAME, m_cs_stdname);
	DDX_Text(pDX, IDC_EDIT_STBTITLE, m_cs_stdtitle);
	DDX_Text(pDX, IDC_EDIT_STBNOTICE, m_cs_stbnotice);
	DDX_Text(pDX, IDC_EDIT_TITLEPIC, m_cs_titlepic);
	DDX_Text(pDX, IDC_EDIT_LINENUM, m_cs_linenum);
}


BEGIN_MESSAGE_MAP(CPropSTDSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BN_CHOOSEPIC, &CPropSTDSetDlg::OnBnClickedBnChoosepic)
	ON_BN_CLICKED(IDC_BN_ADDSTD, &CPropSTDSetDlg::OnBnClickedBnAddstd)
	ON_BN_CLICKED(IDC_BN_DELSTD, &CPropSTDSetDlg::OnBnClickedBnDelstd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STD, &CPropSTDSetDlg::OnLvnItemchangedListStd)
	ON_EN_CHANGE(IDC_EDIT_STBNUM, &CPropSTDSetDlg::OnEnChangeEditStbnum)
	ON_EN_CHANGE(IDC_EDIT_STBNAME, &CPropSTDSetDlg::OnEnChangeEditStbname)
	ON_EN_CHANGE(IDC_EDIT_STBTITLE, &CPropSTDSetDlg::OnEnChangeEditStbtitle)
	ON_EN_CHANGE(IDC_EDIT_STBNOTICE, &CPropSTDSetDlg::OnEnChangeEditStbnotice)
	ON_BN_CLICKED(IDOK, &CPropSTDSetDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_LINENUM, &CPropSTDSetDlg::OnEnChangeEditLinenum)
END_MESSAGE_MAP()


// CPropSTDSetDlg 消息处理程序

void CPropSTDSetDlg::OnBnClickedBnChoosepic()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog picdlg(TRUE, 0, 0, 4|2, _T("files(*.jpg)|*.jpg;||"));
	if(IDOK == picdlg.DoModal())
	{
		CStbContent* pStb = GetSelectStb();
		if(pStb)
		{
			m_cs_titlepic = picdlg.GetPathName();
			pStb->SetStbTitlePicPath(m_cs_titlepic);
			m_listctr_std.SetItemText(m_nCurrentItem,5,m_cs_titlepic);
			UpdateData(FALSE);
		}
	}
}

BOOL CPropSTDSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listctr_std.SetBkColor(RGB(255,255,255));
	m_listctr_std.SetRowHeigt(15);
	m_listctr_std.SetFontHW(13, 0);
	m_listctr_std.ModifyStyle(0,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	m_listctr_std.SetExtendedStyle(LVS_EX_GRIDLINES);


	CRect rect;
	m_listctr_std.GetClientRect(rect);
	//rect.right-=2;
	int nCol = 0;
	m_listctr_std.InsertColumn(nCol++,_T("编号"),LVCFMT_CENTER,rect.Width()*7/100,-1);
	m_listctr_std.InsertColumn(nCol++,_T("名称"),LVCFMT_CENTER,rect.Width()*13/100,-1);
	m_listctr_std.InsertColumn(nCol++,_T("行数"),LVCFMT_CENTER,rect.Width()*7/100,-1);
	m_listctr_std.InsertColumn(nCol++,_T("标题"),LVCFMT_CENTER,rect.Width()*20/100,-1);
	m_listctr_std.InsertColumn(nCol++,_T("通知"),LVCFMT_CENTER,rect.Width()*28/100,-1);
	m_listctr_std.InsertColumn(nCol++,_T("标题图片"),LVCFMT_CENTER,rect.Width()*25/100,-1);

	m_ed_titlepic.SetReadOnly();

	ReadStbInofFormFile();
	ShowListCtrStbInfo();
	
	int count = m_listctr_std.GetItemCount();
	m_listctr_std.SetItemState(count-1,LVIS_SELECTED, LVIS_SELECTED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPropSTDSetDlg::ShowListCtrStbInfo()
{
	list<CStbContent*>::const_iterator itera = m_list_stdContent.begin();
	CStbContent* pStb = NULL;
	int nItem = 0;
	CString strLineNum;
	for(itera;itera!=m_list_stdContent.end();++itera)
	{
		pStb = *itera;
		int nCol = 1;
		m_listctr_std.InsertItem(nItem,pStb->GetStbNum());
		m_listctr_std.SetItemText(nItem,nCol++,pStb->GetStbName());
		strLineNum.Format(_T("%d"),pStb->GetStbLineNum());
		m_listctr_std.SetItemText(nItem,nCol++,strLineNum);
		m_listctr_std.SetItemText(nItem,nCol++,pStb->GetStbTitle());
		m_listctr_std.SetItemText(nItem,nCol++,pStb->GetStbNotice());
		m_listctr_std.SetItemText(nItem,nCol++,pStb->GetStbTitlePicPath());
		nItem++;
	}
}

void CPropSTDSetDlg::OnBnClickedBnAddstd()
{
	// TODO: 在此添加控件通知处理程序代码
	int count = m_list_stdContent.size();
	CStbContent* pStb = new CStbContent;
	CString strName,strNum;
	strNum.Format(_T("%d"),count+1);
	strName.Format(_T("%d号机顶盒"),count+1);
	pStb->SetStbName(strName);
	pStb->SetStbNum(strNum);
	
	m_list_stdContent.push_back(pStb);
	int nHor = 1;
	CString strLineNum;
	strLineNum.Format(_T("%d"),pStb->GetStbLineNum());
	m_listctr_std.InsertItem(count,strNum);
	m_listctr_std.SetItemText(count,nHor++,strName);
	m_listctr_std.SetItemText(count,nHor++,strLineNum);

	m_listctr_std.SetItemState(count, LVIS_SELECTED, LVIS_SELECTED);
}

void CPropSTDSetDlg::OnBnClickedBnDelstd()
{
	// TODO: 在此添加控件通知处理程序代码
	int count = m_listctr_std.GetItemCount();
	if(m_nCurrentItem < 0 || m_nCurrentItem >= count)
		return;

	m_listctr_std.DeleteItem(m_nCurrentItem);

	int i=0;
	list<CStbContent*>::const_iterator itera = m_list_stdContent.begin();
	for(itera;itera != m_list_stdContent.end();++itera)
	{
		if(i == m_nCurrentItem)
		{
			CStbContent* pStb = *itera;
			delete pStb;
			m_list_stdContent.erase(itera);
			break;
		}
		i++;
	}
	
	count = m_listctr_std.GetItemCount();
	m_listctr_std.SetItemState(count - 1, LVIS_SELECTED, LVIS_SELECTED);
}

void CPropSTDSetDlg::OnLvnItemchangedListStd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_nCurrentItem = pNMLV->iItem;
	ShowEdSTBInfo();
	*pResult = 0;
}

void CPropSTDSetDlg::ShowEdSTBInfo()
{
	int count = m_list_stdContent.size();
	if(m_nCurrentItem < 0 || m_nCurrentItem >= count)
		return;
	
	int i = 0;
	CString strLineNum;
	list<CStbContent*>::const_iterator itera = m_list_stdContent.begin();
	for(itera; itera != m_list_stdContent.end();++itera)
	{
		if(i == m_nCurrentItem)
		{
			CStbContent* pStb = *itera;
			m_cs_stdnum = pStb->GetStbNum();
			m_cs_stdname = pStb->GetStbName();
			m_cs_stdtitle = pStb->GetStbTitle();
			m_cs_stbnotice = pStb->GetStbNotice();
			m_cs_titlepic = pStb->GetStbTitlePicPath();
			strLineNum.Format(_T("%d"),pStb->GetStbLineNum());
			m_cs_linenum = strLineNum;
			UpdateData(FALSE);
			return;
		}
		i++;
	}
}

void CPropSTDSetDlg::ClearStbInfo()
{
	list<CStbContent*>::const_iterator itera = m_list_stdContent.begin();
	CStbContent* pStb = NULL;
	for(itera;itera != m_list_stdContent.end();++itera)
	{
		pStb = *itera;
		delete pStb;
		pStb = NULL;
	}
	m_list_stdContent.clear();
}

CStbContent* CPropSTDSetDlg::GetSelectStb()
{
	int i = 0;
	list<CStbContent*>::const_iterator itera = m_list_stdContent.begin();
	for(itera; itera != m_list_stdContent.end();++itera)
	{
		if(i == m_nCurrentItem)
		{
			CStbContent* pStb = *itera;
			return pStb;
		}
		i++;
	}
	return NULL;
}
void CPropSTDSetDlg::OnEnChangeEditStbnum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	CStbContent* pStb = GetSelectStb();
	if(pStb)
	{
		pStb->SetStbNum(m_cs_stdnum);
		m_listctr_std.SetItemText(m_nCurrentItem,0,m_cs_stdnum);
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CPropSTDSetDlg::OnEnChangeEditStbname()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	CStbContent* pStb = GetSelectStb();
	if(pStb)
	{
		pStb->SetStbName(m_cs_stdname);
		m_listctr_std.SetItemText(m_nCurrentItem,1,m_cs_stdname);
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CPropSTDSetDlg::OnEnChangeEditStbtitle()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	CStbContent* pStb = GetSelectStb();
	if(pStb)
	{
		pStb->SetStbTitle(m_cs_stdtitle);
		m_listctr_std.SetItemText(m_nCurrentItem,3,m_cs_stdtitle);
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CPropSTDSetDlg::OnEnChangeEditStbnotice()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	CStbContent* pStb = GetSelectStb();
	if(pStb)
	{
		pStb->SetStbNotice(m_cs_stbnotice);
		m_listctr_std.SetItemText(m_nCurrentItem,4,m_cs_stbnotice);
	}
	// TODO:  在此添加控件通知处理程序代码
}

BOOL CPropSTDSetDlg::ReadStbInofFormFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_strExePath,CFile::modeRead,&e))
	{
		CStbContent* pStb=NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>pStb;
				if (pStb)
				{
					m_list_stdContent.push_back(pStb);
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
			return TRUE;
	}
	return FALSE;
}

BOOL CPropSTDSetDlg::WriteStbInfoToFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_strExePath,CFile::modeCreate|CFile::modeWrite,&e))
	{
		CStbContent* pStb = NULL;
		list<CStbContent*>::const_iterator itera = m_list_stdContent.begin();
		for(itera;itera!=m_list_stdContent.end();++itera)
		{
			CArchive ar(&file,CArchive::store);
			pStb = *itera;
			ar<<pStb;
			ar.Close();
		}
		file.Close();
		return TRUE;
	}
	return FALSE;
}
void CPropSTDSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	WriteStbInfoToFile();
	OnOK();
}

void CPropSTDSetDlg::OnEnChangeEditLinenum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	CStbContent* pStb = GetSelectStb();
	if(pStb)
	{
		int nLineNum = -1;
		CCommonConvert::CStringToint(nLineNum,m_cs_linenum);
		pStb->SetStbLineNum(nLineNum);
		m_listctr_std.SetItemText(m_nCurrentItem,2,m_cs_linenum);
	}
	// TODO:  在此添加控件通知处理程序代码
}
