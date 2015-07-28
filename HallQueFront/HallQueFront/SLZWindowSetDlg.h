#pragma once
#include "SLZWindow.h"
#include "ListCtrlCl.h"
#include "QueueInfo.h"
//#include "SLZWindowManager.h"
// SLZWindowSetDlg �Ի���



class SLZWindowSetDlg : public CDialog
{
	DECLARE_DYNAMIC(SLZWindowSetDlg)

public:
	SLZWindowSetDlg(CWnd* pParent = NULL);
	virtual ~SLZWindowSetDlg();

// �Ի�������
	enum { IDD = IDD_WINDOWSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CList<SLZWindow,SLZWindow&> m_List_WindowInfo;
//	virtual BOOL OnApply();
	CListCtrlCl m_ListCtr_Window;
//	CDoFile m_WindowFile;
	CStatic m_cs_AbleQue;
	CString m_cs_WindowId;
	CString m_cs_WindowName;
	CString m_cs_WndCallName;
	CString m_cs_CallerID;
	CString m_cs_EvalId;
//	CString m_cs_WndScrName;
//	CString m_cs_ComScrName;
// 	CString m_cs_LEDPhyId;
// 	CString m_cs_LEDIPId;
// 	CString m_cs_LEDPipeId;
	CString m_LogStaffName;
	CString m_cs_EvalTimeOut;
	CComboBox m_cs_LogStaff;
	int m_ReadMaxSerialID;
//	CComputeFuncationTime m_ComputerTime;
	BOOL WriteWindowBasicIntoFile();
	BOOL ReadWindowBasicIntoFile();
	BOOL RefrushWindowInfo();		//ˢ����Ϣ
	int QueIdStringToArray(CString strSrc,CArray<CString,CString&>& arrStr);
	CString ArrayToQueIdString(CStringArray& arrStr);
private:
//	CCommonConvert convert;
	CString m_infofile_path;
	CString m_staffinfo_path;
	CString m_Queinfo_path;
	CString m_maxnumfile_path;
	CMap<int,int,SLZStaff,SLZStaff&> m_map_staff;
	CMap<int,int,CQueueInfo,CQueueInfo&> m_map_UndstQueInfo;
	CArray<CString,CString&> arrQueId;
	BOOL ReadStaffInfoFromFile();		// ��ȡԱ����Ϣ
	BOOL ReadQueInfoFromFile();			//��ȡ������Ϣ
	BOOL WriteMaxIdToFile();		//д��������ļ�
	BOOL ReadMaxIdFromFile();		//����������ļ�
	CString GetStaffNameFromMap(CString StaffId);
	CString GetQueNameFromMap(CString QueId);
//	SLZWindowManager WindowManage;
	int GetNumFromString(CString strNum);
public:
	afx_msg void OnBnClickedAddwnd();
	afx_msg void OnBnClickedDelwnd();
	afx_msg void OnEnChangeEditWndname();
	afx_msg void OnEnChangeEditWndcallname();
	afx_msg void OnEnChangeEditCallid();
	afx_msg void OnEnChangeEditEvaid();
//	afx_msg void OnEnChangeEditWndscrid();
//	afx_msg void OnEnChangeEditComscrid();
	//afx_msg void OnBnClickedAutologin();
	afx_msg void OnCbnSelchangeComboLogstaff();
	afx_msg void OnNMClickWndlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedWndlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnChooseque();
	afx_msg void OnBnClickedWndOk();
	afx_msg void OnBnClickedWndCancel();
	afx_msg void OnBnClickedBtnCallsound();
	afx_msg void OnBnClickedBtnWaitsound();
	afx_msg void OnBnClickedBtnCallshow();
	afx_msg void OnBnClickedBtnWaitshow();
	afx_msg void OnEnChangeEditWndid();
	afx_msg void OnEnChangeEditEvaltimeout();
//	afx_msg void OnEnChangeEditLedphyid();
//	afx_msg void OnEnChangeEditLedipid();
//	afx_msg void OnEnChangeEditLedpipe();
	afx_msg void OnBnClickedAutologin();
	afx_msg void OnBnClickedCancelautologin();
	afx_msg void OnBnClickedBnWndscrset();
public:
	int m_nCurSelectItem;
	int GetAllMaxWndScreenID();
};
