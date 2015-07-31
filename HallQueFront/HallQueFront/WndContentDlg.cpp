// WndContentDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WndContentDlg.h"
#include "SLZWindowSetDlg.h"
#include "CommonConvert.h"


// CWndContentDlg �Ի���

IMPLEMENT_DYNAMIC(CWndContentDlg, CDialog)

CWndContentDlg::CWndContentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWndContentDlg::IDD, pParent)
	, m_cs_wndscrid(_T(""))
	, m_cs_phyid(_T(""))
	, m_cs_comid(_T(""))
	, m_cs_pipeid(_T(""))
	, m_cs_throughip(_T(""))
	, m_nCurrentSelectItem(-1)
{

}

CWndContentDlg::~CWndContentDlg()
{
}

void CWndContentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WNDCONTENT, m_listctr_wndscreen);
	DDX_Text(pDX, IDC_EDIT_WNDSCRID, m_cs_wndscrid);
	DDX_Text(pDX, IDC_EDIT_LEDPHYID, m_cs_phyid);
	DDX_Text(pDX, IDC_EDIT_COMSCRID, m_cs_comid);
	DDX_Text(pDX, IDC_EDIT_LEDPIPE, m_cs_pipeid);
	DDX_Text(pDX, IDC_EDIT_LEDIPID, m_cs_throughip);
}


BEGIN_MESSAGE_MAP(CWndContentDlg, CDialog)
	ON_BN_CLICKED(IDC_BN_ADDWND, &CWndContentDlg::OnBnClickedBnAddwnd)
	ON_BN_CLICKED(IDC_BN_DELWND, &CWndContentDlg::OnBnClickedBnDelwnd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WNDCONTENT, &CWndContentDlg::OnLvnItemchangedListWndcontent)
	ON_EN_CHANGE(IDC_EDIT_WNDSCRID, &CWndContentDlg::OnEnChangeEditWndscrid)
	ON_EN_CHANGE(IDC_EDIT_COMSCRID, &CWndContentDlg::OnEnChangeEditComscrid)
	ON_EN_CHANGE(IDC_EDIT_LEDPHYID, &CWndContentDlg::OnEnChangeEditLedphyid)
	ON_EN_CHANGE(IDC_EDIT_LEDIPID, &CWndContentDlg::OnEnChangeEditLedipid)
	ON_EN_CHANGE(IDC_EDIT_LEDPIPE, &CWndContentDlg::OnEnChangeEditLedpipe)
END_MESSAGE_MAP()


// CWndContentDlg ��Ϣ�������

void CWndContentDlg::OnBnClickedBnAddwnd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int count = m_slzCurSelectWindow.m_throughscreen_array.GetCount();
	SLZWindowSetDlg* pParent = (SLZWindowSetDlg*)this->GetParent();
	int nMaxID = pParent->GetAllMaxWndScreenID();
	CThroughWndScreenInfo WndScreenInfo;
	
	WndScreenInfo.SetWndScreenId(nMaxID+1);
	WndScreenInfo.SetComScreenId(nMaxID+1);
	m_slzCurSelectWindow.m_throughscreen_array.Add(WndScreenInfo);
	
	CString strSerialId,strScreenId,strPhyId,strPipeId,strComId;
	strSerialId.Format(_T("%d"),count+1);
	strScreenId.Format(_T("%d"),WndScreenInfo.GetWndScreenId());
	strPhyId.Format(_T("%d"),WndScreenInfo.GetPhyId());
	strPipeId.Format(_T("%d"),WndScreenInfo.GetPipeId());
	strComId.Format(_T("%d"),WndScreenInfo.GetComScreenId());
	
	int nHor = 1;
	m_listctr_wndscreen.InsertItem(count,strSerialId);
 	m_listctr_wndscreen.SetItemText(count,nHor++,strScreenId);
 	m_listctr_wndscreen.SetItemText(count,nHor++,strComId);
 	m_listctr_wndscreen.SetItemText(count,nHor++,WndScreenInfo.GetLocalIp());
 	m_listctr_wndscreen.SetItemText(count,nHor++,strPhyId);
 	m_listctr_wndscreen.SetItemText(count,nHor++,strPipeId);

	m_listctr_wndscreen.SetItemState(count, LVIS_SELECTED, LVIS_SELECTED);

	ModifyListWndInfo(m_slzCurSelectWindow);
}

void CWndContentDlg::OnBnClickedBnDelwnd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int count = m_slzCurSelectWindow.m_throughscreen_array.GetCount();
	if(m_nCurrentSelectItem < 0 || m_nCurrentSelectItem >= count)
		return;
	m_slzCurSelectWindow.m_throughscreen_array.RemoveAt(m_nCurrentSelectItem);
	ModifyListWndInfo(m_slzCurSelectWindow);

	m_listctr_wndscreen.DeleteItem(m_nCurrentSelectItem);
	count = m_listctr_wndscreen.GetItemCount();
	m_listctr_wndscreen.SetItemState(count - 1, LVIS_SELECTED, LVIS_SELECTED);
}



BOOL CWndContentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
		



	GetCurrentSelectWindow();


	m_listctr_wndscreen.SetBkColor(RGB(255,255,255));
	m_listctr_wndscreen.SetRowHeigt(15);
	m_listctr_wndscreen.SetFontHW(13, 0);
	m_listctr_wndscreen.ModifyStyle(0,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	m_listctr_wndscreen.SetExtendedStyle(LVS_EX_GRIDLINES);


	CRect rect;
	m_listctr_wndscreen.GetClientRect(rect);
	//rect.right-=2;
	int nCol = 0;
	m_listctr_wndscreen.InsertColumn(nCol++,_T("���"),LVCFMT_CENTER,rect.Width()*10/100,-1);
	m_listctr_wndscreen.InsertColumn(nCol++,_T("������"),LVCFMT_CENTER,rect.Width()*15/100,-1);
	m_listctr_wndscreen.InsertColumn(nCol++,_T("�ۺ���"),LVCFMT_CENTER,rect.Width()*15/100,-1);
	m_listctr_wndscreen.InsertColumn(nCol++,_T("ͨ��IP"),LVCFMT_CENTER,rect.Width()*30/100,-1);
	m_listctr_wndscreen.InsertColumn(nCol++,_T("ͨ�������ַ"),LVCFMT_CENTER,rect.Width()*15/100,-1);
	m_listctr_wndscreen.InsertColumn(nCol++,_T("ͨ��ͨ��"),LVCFMT_CENTER,rect.Width()*15/100,-1);
	
	ShowContent();

	int count = m_listctr_wndscreen.GetItemCount();
	m_listctr_wndscreen.SetItemState(count - 1, LVIS_SELECTED, LVIS_SELECTED);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CWndContentDlg::GetCurrentSelectWindow()
{
	SLZWindowSetDlg* pParent = (SLZWindowSetDlg*)GetParent();
	if(pParent)
	{
		POSITION pos = pParent->m_List_WindowInfo.FindIndex(pParent->m_nCurSelectItem);
		if(pos)
		{
			m_slzCurSelectWindow = pParent->m_List_WindowInfo.GetAt(pos);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CWndContentDlg::ModifyListWndInfo(SLZWindow& slzWindow)
{
	SLZWindowSetDlg* pParent = (SLZWindowSetDlg*)GetParent();
	if(pParent)
	{
		POSITION pos = pParent->m_List_WindowInfo.FindIndex(pParent->m_nCurSelectItem);
		if(pos)
		{
			pParent->m_List_WindowInfo.SetAt(pos,slzWindow);
			return TRUE;
		}
	}
	return FALSE;
}

void CWndContentDlg::ShowContent()
{
	int count = m_slzCurSelectWindow.m_throughscreen_array.GetCount();
	CThroughWndScreenInfo WndScreenInfo;
	
	CString strSerialId,strScreenId,strPhyId,strPipeId,strComId;
	for(int i=0;i<count;i++)
	{
		int nHor = 1;
		WndScreenInfo = m_slzCurSelectWindow.m_throughscreen_array.GetAt(i);
		strSerialId.Format(_T("%d"),i+1);
		strScreenId.Format(_T("%d"),WndScreenInfo.GetWndScreenId());
		strPhyId.Format(_T("%d"),WndScreenInfo.GetPhyId());
		strPipeId.Format(_T("%d"),WndScreenInfo.GetPipeId());
		strComId.Format(_T("%d"),WndScreenInfo.GetComScreenId());
		m_listctr_wndscreen.InsertItem(i,strSerialId);
		m_listctr_wndscreen.SetItemText(i,nHor++,strScreenId);
		m_listctr_wndscreen.SetItemText(i,nHor++,strComId);
		m_listctr_wndscreen.SetItemText(i,nHor++,WndScreenInfo.GetLocalIp());
		m_listctr_wndscreen.SetItemText(i,nHor++,strPhyId);
		m_listctr_wndscreen.SetItemText(i,nHor++,strPipeId);
	}
}
void CWndContentDlg::OnLvnItemchangedListWndcontent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nCurrentSelectItem = pNMLV->iItem;

	ShowEditBoxInfo();
	*pResult = 0;
}


void CWndContentDlg::ShowEditBoxInfo()
{
	int count = m_slzCurSelectWindow.m_throughscreen_array.GetCount();
	if(m_nCurrentSelectItem < 0 || m_nCurrentSelectItem >= count)
		return;


	CThroughWndScreenInfo WndInfo = m_slzCurSelectWindow.m_throughscreen_array.GetAt(m_nCurrentSelectItem);

	CString strScreenId,strComId,strPhyId,strPipeId;
	strScreenId.Format(_T("%d"),WndInfo.GetWndScreenId());
	strComId.Format(_T("%d"),WndInfo.GetComScreenId());
	strPhyId.Format(_T("%d"),WndInfo.GetPhyId());
	strPipeId.Format(_T("%d"),WndInfo.GetPipeId());

	m_cs_wndscrid = strScreenId;
	m_cs_comid = strComId;
	m_cs_phyid = strPhyId;
	m_cs_pipeid = strPipeId;
	m_cs_throughip = WndInfo.GetLocalIp();

	UpdateData(FALSE);
}
void CWndContentDlg::OnEnChangeEditWndscrid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData(TRUE);
	int count = m_slzCurSelectWindow.m_throughscreen_array.GetCount();
	if(m_nCurrentSelectItem < 0 || m_nCurrentSelectItem >= count)
		return;

	CThroughWndScreenInfo WndInfo = m_slzCurSelectWindow.m_throughscreen_array.GetAt(m_nCurrentSelectItem);

	int nScreenId = 0;
	CCommonConvert::CStringToint(nScreenId,m_cs_wndscrid);


	m_listctr_wndscreen.SetItemText(m_nCurrentSelectItem,1,m_cs_wndscrid);

	WndInfo.SetWndScreenId(nScreenId);

	m_slzCurSelectWindow.m_throughscreen_array.SetAt(m_nCurrentSelectItem,WndInfo);

	ModifyListWndInfo(m_slzCurSelectWindow);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CWndContentDlg::OnEnChangeEditComscrid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData(TRUE);
	int count = m_slzCurSelectWindow.m_throughscreen_array.GetCount();
	if(m_nCurrentSelectItem < 0 || m_nCurrentSelectItem >= count)
		return;

	CThroughWndScreenInfo WndInfo = m_slzCurSelectWindow.m_throughscreen_array.GetAt(m_nCurrentSelectItem);

	int nComScrId = 0;
	CCommonConvert::CStringToint(nComScrId,m_cs_comid);

	m_listctr_wndscreen.SetItemText(m_nCurrentSelectItem,2,m_cs_comid);
	WndInfo.SetComScreenId(nComScrId);

	m_slzCurSelectWindow.m_throughscreen_array.SetAt(m_nCurrentSelectItem,WndInfo);

	ModifyListWndInfo(m_slzCurSelectWindow);

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CWndContentDlg::OnEnChangeEditLedphyid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	

	UpdateData(TRUE);
	int count = m_slzCurSelectWindow.m_throughscreen_array.GetCount();
	if(m_nCurrentSelectItem < 0 || m_nCurrentSelectItem >= count)
		return;

	CThroughWndScreenInfo WndInfo = m_slzCurSelectWindow.m_throughscreen_array.GetAt(m_nCurrentSelectItem);

	int nPhyId = 0;
	CCommonConvert::CStringToint(nPhyId,m_cs_phyid);

	m_listctr_wndscreen.SetItemText(m_nCurrentSelectItem,4,m_cs_phyid);
	WndInfo.SetPhyId(nPhyId);

	m_slzCurSelectWindow.m_throughscreen_array.SetAt(m_nCurrentSelectItem,WndInfo);

	ModifyListWndInfo(m_slzCurSelectWindow);

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CWndContentDlg::OnEnChangeEditLedipid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData(TRUE);
	int count = m_slzCurSelectWindow.m_throughscreen_array.GetCount();
	if(m_nCurrentSelectItem < 0 || m_nCurrentSelectItem >= count)
		return;

	CThroughWndScreenInfo WndInfo = m_slzCurSelectWindow.m_throughscreen_array.GetAt(m_nCurrentSelectItem);


	m_listctr_wndscreen.SetItemText(m_nCurrentSelectItem,3,m_cs_throughip);
	WndInfo.SetLocalIP(m_cs_throughip);

	m_slzCurSelectWindow.m_throughscreen_array.SetAt(m_nCurrentSelectItem,WndInfo);

	ModifyListWndInfo(m_slzCurSelectWindow);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CWndContentDlg::OnEnChangeEditLedpipe()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData(TRUE);
	int count = m_slzCurSelectWindow.m_throughscreen_array.GetCount();
	if(m_nCurrentSelectItem < 0 || m_nCurrentSelectItem >= count)
		return;

	CThroughWndScreenInfo WndInfo = m_slzCurSelectWindow.m_throughscreen_array.GetAt(m_nCurrentSelectItem);

	int nPipeId = 0;
	CCommonConvert::CStringToint(nPipeId,m_cs_pipeid);

	m_listctr_wndscreen.SetItemText(m_nCurrentSelectItem,5,m_cs_pipeid);
	WndInfo.SetPipeId(nPipeId);

	m_slzCurSelectWindow.m_throughscreen_array.SetAt(m_nCurrentSelectItem,WndInfo);

	ModifyListWndInfo(m_slzCurSelectWindow);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
