#pragma once

#include "SLZStaff.h"
#include "DoFile.h"
#include "CommonConvert.h"
#include "ComputeFuncationTime.h"
#include "afxdlgs.h"
#include "ListCtrlCl.h"
#include "atlimage.h"


/*部门结构*/




// CStaffSet 对话框

class CStaffSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CStaffSetDlg)

public:
	CStaffSetDlg(CWnd* pParent = NULL);
	virtual ~CStaffSetDlg();

// 对话框数据
	enum { IDD = IDD_STAFFSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CList<SLZStaff,SLZStaff&> m_List_StaffInfo; //员工信息链表
//	virtual BOOL OnApply();
	CListCtrlCl m_ListCtr_Staff; //员工列表listctrl控件（已被重写成CListCtrlCl）
	CDoFile m_StaffFile;
private:
	CCommonConvert convert;
	CString m_infofile_path;
	CString strMaxDocID;
	int GetNumFromString(CString strNum);  //从字符串中提取数字 如“B0012”得到12
	CStringArray m_DeleteStaffIdArr;
public:
	CString m_cs_StaffName;
	CString m_cs_StaffId;
	CComboBox m_cs_StaffGender;
	CString m_StaffPhotoPath;
	CString m_strGender;
	CString	 m_strPicFilter;
	CStatic m_picPhoto;
	
	//CString GetStaffInfoBasicInfo(){return m_infofile_path;}

	BOOL WriteStaffBasicIntoFile();
	BOOL ReadStaffBasicIntoFile();
	BOOL RefrushStaffInfo();
	int m_ReadMaxStaffSerNum;
	CComputeFuncationTime m_ComputerTime;
	afx_msg void OnBnClickedBnAddstaff();
	afx_msg void OnBnClickedBnDelstaff();
	afx_msg void OnCbnSelchangeComStaffgender();
	afx_msg void OnEnChangeEdStaffname();
	afx_msg void OnEnChangeEdStaffid();
	afx_msg void OnBnClickedBnSetphoto();

	void ShowPhoto(const CString& strPhotoPath); //照片显示
	BOOL ShowPhoto(const CString& strPhotoPath, CWnd* pWnd);
	BOOL SetStaticPicture(CWnd* pWnd, int iPicture);


	afx_msg void OnNMClickListStaffname(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListStaffname(NMHDR *pNMHDR, LRESULT *pResult);
	
	
	afx_msg void OnBnClickedBnOk();
	afx_msg void OnBnClickedBnCancel();
	afx_msg void OnPaint();
};
