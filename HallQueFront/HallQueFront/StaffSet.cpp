// StaffSet.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "StaffSet.h"
#include "CascadeMethod.h"

//文件过滤
 
// CStaffSet 对话框


IMPLEMENT_DYNAMIC(CStaffSetDlg, CDialog)

CStaffSetDlg::CStaffSetDlg(CWnd* pParent /* = NULL */)
	: CDialog(CStaffSetDlg::IDD,pParent)
	,m_cs_StaffName(_T(""))
	,m_cs_StaffId(_T(""))
	,m_ReadMaxStaffSerNum(0)
{
	m_infofile_path=m_StaffFile.GetExeFullFilePath();
	m_infofile_path+=_T("\\StaffBasicInfo");
	m_StaffFile.CreateMyDirectory(m_infofile_path);
	m_infofile_path+=_T("\\StaffBasicInfo.dat");
}

CStaffSetDlg::~CStaffSetDlg()
{
}

void CStaffSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_STAFFNAME,m_ListCtr_Staff);
	DDX_Text(pDX,IDC_ED_STAFFNAME,m_cs_StaffName);
	DDX_Text(pDX,IDC_ED_STAFFID,m_cs_StaffId);
	DDX_Text(pDX,IDC_COM_STAFFGENDER,m_strGender);
	DDX_Control(pDX,IDC_COM_STAFFGENDER,m_cs_StaffGender);
	DDX_Control(pDX,IDC_STATIC_PHOTO,m_picPhoto);
}


BEGIN_MESSAGE_MAP(CStaffSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BN_ADDSTAFF, &CStaffSetDlg::OnBnClickedBnAddstaff)
	
	ON_BN_CLICKED(IDC_BN_DELSTAFF, &CStaffSetDlg::OnBnClickedBnDelstaff)
	ON_CBN_SELCHANGE(IDC_COM_STAFFGENDER, &CStaffSetDlg::OnCbnSelchangeComStaffgender)
	ON_EN_CHANGE(IDC_ED_STAFFNAME, &CStaffSetDlg::OnEnChangeEdStaffname)
	ON_EN_CHANGE(IDC_ED_STAFFID, &CStaffSetDlg::OnEnChangeEdStaffid)
	ON_BN_CLICKED(IDC_BN_SELPHOTO, &CStaffSetDlg::OnBnClickedBnSetphoto)
	ON_NOTIFY(NM_CLICK, IDC_LIST_STAFFNAME, &CStaffSetDlg::OnNMClickListStaffname)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STAFFNAME, &CStaffSetDlg::OnLvnItemchangedListStaffname)

	ON_BN_CLICKED(IDC_BN_OK, &CStaffSetDlg::OnBnClickedBnOk)
	ON_BN_CLICKED(IDC_BN_CANCEL, &CStaffSetDlg::OnBnClickedBnCancel)
	ON_WM_PAINT()
END_MESSAGE_MAP()




BOOL CStaffSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_ListCtr_Staff.SetBkColor(RGB(255,255,255));

	m_ListCtr_Staff.SetRowHeigt(15);
	m_ListCtr_Staff.SetFontHW(13, 0);
	m_cs_StaffGender.AddString(_T("男"));
	m_cs_StaffGender.AddString(_T("女"));
	
	//过滤文件类型
	m_strPicFilter = _T("Image files(*.jpg,*.jpeg,*.bmp)|*.jpg;*.jpeg;*.bmp||");
	
	//设置listctrl格式
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(&rect);
	rect.right-=2;
	m_ListCtr_Staff.InsertColumn(0,_T("序号"),LVCFMT_CENTER,rect.Width()*6/100,-1);
	m_ListCtr_Staff.InsertColumn(1,_T("姓名"),LVCFMT_CENTER,rect.Width()*9/100,-1);
	m_ListCtr_Staff.InsertColumn(2,_T("工号"),LVCFMT_CENTER,rect.Width()*13/100,-1);
	m_ListCtr_Staff.InsertColumn(3,_T("性别"),LVCFMT_CENTER,rect.Width()*6/100,-1);
	m_ListCtr_Staff.InsertColumn(4,_T("照片"),LVCFMT_CENTER,rect.Width()*8/100,-1);
	m_ListCtr_Staff.ModifyStyle(0,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	m_ListCtr_Staff.SetExtendedStyle(LVS_EX_GRIDLINES);//添加网格风格
	ReadStaffBasicIntoFile();
	if (m_ListCtr_Staff.GetItemCount())
	{	
		m_ListCtr_Staff.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
	return TRUE;
}
// CStaffSet 消息处理程序

int CStaffSetDlg::GetNumFromString(CString strNum)
{
	if (strNum==_T(""))
	{
		return 0;
	}
	CString strRealNum;
	int nStartNumIndex = strNum.FindOneOf(_T("0123456789"));
	strRealNum = strNum.Mid(nStartNumIndex);
	char szbuf[32] = {0};
	CCommonConvert::WChar2Char(szbuf, 32, strRealNum.GetBuffer());
	int nMaxID = ::atoi(szbuf);
	return nMaxID;
}

//添加员工
void CStaffSetDlg::OnBnClickedBnAddstaff()
{
	// TODO: 在此添加控件通知处理程序代码
	SLZStaff staffinfo;
	int count=m_ListCtr_Staff.GetItemCount();
	int iMaxID = GetNumFromString(m_ListCtr_Staff.GetItemText(0,2));
	int itemp=0;
	for (int index = 1;index<count;index++)
	{
		CString strMaxID  = m_ListCtr_Staff.GetItemText(index,2);
		int nStartNumIndex = GetNumFromString(strMaxID);
		if (nStartNumIndex>iMaxID)
		{
			iMaxID = nStartNumIndex;
			itemp = index;
		}
	}
	CString StaffSerNum;
	strMaxDocID = m_ListCtr_Staff.GetItemText(itemp,2);
	int nStartNumIndex = strMaxDocID.FindOneOf(_T("0123456789"));
	CString strPrefix = strMaxDocID.Left(nStartNumIndex);
	strMaxDocID = strMaxDocID.Mid(nStartNumIndex);
	if(strMaxDocID.IsEmpty())
	{
		strMaxDocID.Format(_T("%04d"), iMaxID);
	}

	CString strFormat;
	CString strNewID;
	strFormat.Format(_T("%%s%%0%dd"), strMaxDocID.GetLength());
	strNewID.Format(strFormat, strPrefix, iMaxID+1);
	CString n_StaffName;
	n_StaffName.Format(_T("员工%d"), iMaxID + 1);
	convert.intToCString(count+1,StaffSerNum);
	m_ListCtr_Staff.InsertItem(count,StaffSerNum);
	m_ListCtr_Staff.SetItemText(count,1,n_StaffName);
	m_ListCtr_Staff.SetItemText(count,2,strNewID);
	m_ListCtr_Staff.SetItemText(count,4,_T("未配置"));
	staffinfo.SetStaffName(n_StaffName);
	staffinfo.SetStaffId(strNewID);
	m_List_StaffInfo.AddTail(staffinfo);
	m_ListCtr_Staff.SetItemState(count, LVIS_SELECTED, LVIS_SELECTED);//让最后一项被选中
//	SetModified();
}

//删除员工
void CStaffSetDlg::OnBnClickedBnDelstaff()
{
	// TODO: 在此添加控件通知处理程序代码
	
	POSITION pos=m_ListCtr_Staff.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Staff.GetNextSelectedItem(pos);
	
	CString StaffSerNum;
	if (index<0)
	{
		AfxMessageBox(_T("请选择一名员工"));
		return;
	}
	m_ListCtr_Staff.DeleteItem(index);
	m_DeleteStaffIdArr.Add(m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(index)).GetStaffId());
	int count=m_ListCtr_Staff.GetItemCount();
	for(int i=index;i<=count;i++)
	{
		convert.intToCString(i+1,StaffSerNum);
		m_ListCtr_Staff.SetItemText(i,0,StaffSerNum);
	}
	/*SLZStaff info = m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(index));
	CString staffid = info.GetStaffId();
	if (staffid==strMaxDocID)
	{
		char szbuf[32] = {0};
		CCommonConvert::WChar2Char(szbuf, 32, staffid.GetBuffer());
		int nMaxID = ::atoi(szbuf);
		CString strFormat;
		strFormat.Format(_T("%%s%%0%dd"), staffid.GetLength());
		CString strPrefix;
		strMaxDocID.Format(strFormat,strPrefix,nMaxID-1);
	}*/
	m_List_StaffInfo.RemoveAt(m_List_StaffInfo.FindIndex(index));
	//删到顶端时把焦点设置在最上边
	if(index>0)
	{
		m_ListCtr_Staff.SetItemState(index-1,LVIS_SELECTED,LVIS_SELECTED);
	}
	else
	{
		m_ListCtr_Staff.SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);
	}
//	SetModified();
}

//选择性别
void CStaffSetDlg::OnCbnSelchangeComStaffgender()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	
	POSITION pos=m_ListCtr_Staff.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Staff.GetNextSelectedItem(pos);
//	int comboxnum;
	if (index<0)
	{
		AfxMessageBox(_T("请选择一名员工"));
		return;
	}
	SLZStaff staffinfo=m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(index));
	staffinfo.SetStaffGender(m_cs_StaffGender.GetCurSel()+1);
	
	m_ListCtr_Staff.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
	m_ListCtr_Staff.SetItemText(index,3,m_strGender);
	m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(index))=staffinfo;
//	SetModified();
}



void CStaffSetDlg::OnEnChangeEdStaffname()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Staff.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Staff.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一名员工"));
		return;
	}
	SLZStaff staffinfo=m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(index));
	staffinfo.SetStaffName(m_cs_StaffName);
	m_ListCtr_Staff.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
	m_ListCtr_Staff.SetItemText(index,1,m_cs_StaffName);
	m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(index))=staffinfo;
//	SetModified();
}

void CStaffSetDlg::OnEnChangeEdStaffid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	POSITION pos=m_ListCtr_Staff.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Staff.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一名员工"));
		return;
	}
	BOOL btemp=TRUE;
	SLZStaff staffinfo=m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(index));
	for (int i=0;i<m_List_StaffInfo.GetCount();i++)
	{
		if (m_cs_StaffId==m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(i)).GetStaffId()&&m_cs_StaffId!=staffinfo.GetStaffId())
		{
			AfxMessageBox(_T("该员工ID号已经被创建，请重新输入。"));
			btemp=FALSE;
			UpdateData(FALSE);
			break;
		}
	}
	if(btemp)
	{
		staffinfo.SetStaffId(m_cs_StaffId);
		m_ListCtr_Staff.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
		m_ListCtr_Staff.SetItemText(index,2,m_cs_StaffId);
		m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(index))=staffinfo;
	}

//	SetModified();
}


void CStaffSetDlg::ShowPhoto(const CString& strPhotoPath)
{
	ShowPhoto(strPhotoPath,(CWnd*)&m_picPhoto);
}

BOOL CStaffSetDlg::ShowPhoto(const CString& strPhotoPath, CWnd* pWnd)
{
	
	POSITION pos=m_ListCtr_Staff.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Staff.GetNextSelectedItem(pos);
	SLZStaff staffinfo=m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(index));
	CStatic* picPhoto = (CStatic*)pWnd;
	CDC* pDC = picPhoto->GetWindowDC();
	CRect rect;
	picPhoto->GetClientRect(&rect);
	CImage img;
	if(strPhotoPath.IsEmpty())
	{
		pDC->SelectStockObject(GRAY_BRUSH);
		pDC->FillRect(	
			&rect, pDC->GetCurrentBrush());
		UpdateData(FALSE);
		return FALSE;
	}
	HRESULT hResult = img.Load(strPhotoPath);
	if(FAILED(hResult))
	{
		MessageBox(_T("图片路径错误，导入失败"));
		staffinfo.SetStaffPhoto(_T(""));
		m_ListCtr_Staff.SetItemText(index,4,_T("未配置"));
		m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(index))=staffinfo;
		return FALSE;
	}
	pDC->SetStretchBltMode(STRETCH_HALFTONE);
	img.Draw(pDC->m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0, img.GetWidth(), img.GetHeight());
	//把图片填充在控件中
	return TRUE;
}

void CStaffSetDlg::OnBnClickedBnSetphoto()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos=m_ListCtr_Staff.GetFirstSelectedItemPosition();
	int index=m_ListCtr_Staff.GetNextSelectedItem(pos);
	if (index<0)
	{
		AfxMessageBox(_T("请选择一名员工"));
		return;
	}
	CString m_strPhotoPath;
	CFileDialog fdlg(TRUE, 0, 0, 4|2, m_strPicFilter);
	if(IDOK == fdlg.DoModal())
	{
		m_strPhotoPath = fdlg.GetPathName();
	}
	SLZStaff staffinfo=m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(index));
	staffinfo.SetStaffPhoto(m_strPhotoPath);
	m_ListCtr_Staff.SetItemState(index,LVIS_FOCUSED,LVIS_FOCUSED);
	ShowPhoto(m_strPhotoPath);

	if(staffinfo.GetStaffPhoto()!="") m_ListCtr_Staff.SetItemText(index,4,_T("已配置"));
	else  m_ListCtr_Staff.SetItemText(index,4,_T("未配置"));
	m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(index))=staffinfo;
//	SetModified();
	
}


void CStaffSetDlg::OnNMClickListStaffname(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int index=m_ListCtr_Staff.GetItemCount();
	for(int i=0;i<m_ListCtr_Staff.GetItemCount();i++)
	{
		if(m_ListCtr_Staff.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			SLZStaff staffinfo=m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(i));
			m_cs_StaffName=staffinfo.GetStaffName();
			m_cs_StaffId=staffinfo.GetStaffId();
			GENDER staffgender=staffinfo.GetStaffGender();
			if (staffgender==MYGENDER_NONE)m_cs_StaffGender.SetCurSel(-1);
			else m_cs_StaffGender.SetCurSel(staffgender-1);
			CString photoadd=staffinfo.GetStaffPhoto();
			if(staffinfo.GetStaffPhoto()=="") m_ListCtr_Staff.SetItemText(i,4,_T("未配置"));
			ShowPhoto(photoadd);
			UpdateData(FALSE);
			
			break;
		}
	}
	*pResult = 0;
}

void CStaffSetDlg::OnLvnItemchangedListStaffname(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_ListCtr_Staff.GetItemCount();i++)
	{
		if(m_ListCtr_Staff.GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED )
		{
			SLZStaff staffinfo=m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(i));
			m_cs_StaffName=staffinfo.GetStaffName();
			m_cs_StaffId=staffinfo.GetStaffId();
			GENDER staffgender=staffinfo.GetStaffGender();
			if (staffgender==MYGENDER_NONE)m_cs_StaffGender.SetCurSel(-1);
			else m_cs_StaffGender.SetCurSel(staffgender-1);
			CString photoadd=staffinfo.GetStaffPhoto();
			if(staffinfo.GetStaffPhoto()=="") m_ListCtr_Staff.SetItemText(i,4,_T("未配置"));
			ShowPhoto(photoadd);
			UpdateData(FALSE);
			break;
		}
	}
	*pResult = 0;
}

// BOOL CStaffSetDlg::OnApply()
// {
// 	WriteStaffBasicIntoFile();
// 	return CPropertyPage::OnApply();
// }

extern void MyWriteConsole(CString str);

BOOL CStaffSetDlg::WriteStaffBasicIntoFile()
{
// #ifdef _DEBUG
// 	m_ComputerTime.SetStartTime(clock());
// #endif
	CFile file;
	CFileException e;
	if (file.Open(m_infofile_path,CFile::modeCreate|CFile::modeWrite,&e))
	{
		int count=m_List_StaffInfo.GetCount();
		for (int i=0;i<count;i++)
		{
			CArchive ar(&file,CArchive::store);
			SLZStaff staffinfo;
			staffinfo=m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(i));
			ar<<&staffinfo;
			ar.Close();
		}
		file.Close();

// #ifdef _DEBUG
// 		m_ComputerTime.SetFinshTime(clock());
// 		CString MyClock;
// 		MyClock.Format(_T("%f"),m_ComputerTime.GetDuration());
// 		MyWriteConsole(_T("CArchive:")+MyClock);
// #endif
		return TRUE;
	}
	else
	{
		TRACE(_T("File could not be opened %d\n"), e.m_cause);
		return FALSE;
	}
}

BOOL CStaffSetDlg::ReadStaffBasicIntoFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_infofile_path,CFile::modeRead,&e))
	{
		SLZStaff* staffinfo=NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>staffinfo;
				if (staffinfo)
				{
					m_List_StaffInfo.AddTail(*staffinfo);
					int count=m_ListCtr_Staff.GetItemCount()+1;
					CString StaffSerNum;
					convert.intToCString(count,StaffSerNum);
					m_ListCtr_Staff.InsertItem(count-1,StaffSerNum);
					m_ListCtr_Staff.SetItemText(count-1,1,staffinfo->GetStaffName());
					m_ListCtr_Staff.SetItemText(count-1,2,staffinfo->GetStaffId());
					GENDER gender=staffinfo->GetStaffGender();
					switch (gender)
					{
					case GENDER_MALE : m_ListCtr_Staff.SetItemText(count-1,3,_T("男"));
						break;
					case GENDER_FEMALE : m_ListCtr_Staff.SetItemText(count-1,3,_T("女"));
						break;
					}
					
					if(staffinfo->GetStaffPhoto()!="") m_ListCtr_Staff.SetItemText(count-1,4,_T("已配置"));
					else  m_ListCtr_Staff.SetItemText(count-1,4,_T("未配置"));
					delete staffinfo;
					staffinfo = NULL;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
			
	}
	else return FALSE;
	return TRUE;
}

BOOL CStaffSetDlg::RefrushStaffInfo()
{
	return ReadStaffBasicIntoFile();
}
void CStaffSetDlg::OnBnClickedBnOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CCascadeMethod cascade;
	for (int i = 0;i<m_DeleteStaffIdArr.GetCount();i++)
	{
		cascade.DeleteStaffInfo(m_DeleteStaffIdArr[i]);
	}
	WriteStaffBasicIntoFile();
	return CDialog::OnOK();
}

void CStaffSetDlg::OnBnClickedBnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	return CDialog::OnCancel();
}

void CStaffSetDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	for(int i=0;i<m_ListCtr_Staff.GetItemCount();i++)
	{
		if(m_ListCtr_Staff.GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED )
		{
			SLZStaff staffinfo=m_List_StaffInfo.GetAt(m_List_StaffInfo.FindIndex(i));
			CString photoadd=staffinfo.GetStaffPhoto();
			ShowPhoto(photoadd);
			UpdateData(FALSE);
			break;
		}
	}
	// 不为绘图消息调用 CDialog::OnPaint()
}
