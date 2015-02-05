#pragma once

#include "QueueInfo.h"
#include "DoFile.h"
#include "afxwin.h"
#include "CommonConvert.h"
#include "ComputeFuncationTime.h"
#include "afxdlgs.h"
#include "resource.h"
// CQueSetDlg 对话框


class CQueSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CQueSetDlg)

public:
	CQueSetDlg(CWnd* pParent = NULL);
	virtual ~CQueSetDlg();

// 对话框数据
	enum { IDD = IDD_QUESETDLG };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//生成消息映射的函数
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
public:
	CList<CQueueInfo,CQueueInfo&> m_list_queinfo;//存放队列信息的链表
	CDoFile m_dealFile;
	
	/*virtual BOOL OnApply();*/
	CListBox m_listctr_quename; //队列列表listbox控件
private:
	CCommonConvert m_convert; 
	CString m_infofile_path; //文件存放地址
	CString m_maxnumfile_path;
	CStringArray m_DeleteQueidArr;
	BOOL CheckQueNum();
public:
	CString m_cs_quename; //各个控件显示的关联变量
	CString m_cs_callname;
	CString m_cs_queid;
	CString m_cs_frontid;
	CString m_cs_numstart;
	CString m_cs_numend;
	CString m_cs_amlimit;
	CString m_cs_pmlimit;
	CString m_cs_daylimit;
	CDateTimeCtrl m_timectrl_start;
	CDateTimeCtrl m_timectrl_end;
	CComboBox m_cs_stbid; // 机顶盒
	//CString GetQueinfoBasicInfo(){return m_infofile_path;} //获得地址

	BOOL WriteQueBasicInfoToFile(); //写入文件
	BOOL ReadQueBasicInfoFromFile(); //读取文件
	BOOL WriteMaxIdToFile();		//写最大数到文件
	BOOL ReadMaxIdFromFile();		//读最大数从文件
	BOOL RefrushQueInfo();
	void SetEdctrNull(); //控件显示置空

	int m_ReadMaxSerialID;

	CComputeFuncationTime m_ComputerTime;
	afx_msg void OnBnClickedBnAddqueue();
	afx_msg void OnLbnSelchangeListQuename();
	afx_msg void OnBnClickedBnDelqueue();
	afx_msg void OnEnChangeEdAmlimit();
	afx_msg void OnEnChangeEdPmlimit();
	afx_msg void OnEnChangeEdDaylimit();
	afx_msg void OnEnChangeEdQuename();
	afx_msg void OnEnChangeEdCallname();
	afx_msg void OnEnChangeEdFrontid();
	afx_msg void OnEnChangeEdQuenumstart();
	afx_msg void OnEnChangeEdQuenumend();
	afx_msg void OnBnClickedBnOk();
	afx_msg void OnBnClickedBnCancle();
	afx_msg void OnDtnDatetimechangeTimestart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeTimeend(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_ed_queSerialID;
	afx_msg void OnEnChangeEdQueserialid();
	CString m_cs_queManNum;
};
