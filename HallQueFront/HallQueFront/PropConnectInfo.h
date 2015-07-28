#pragma once
#include "afxwin.h"
#include "ConnectConfig.h"
#include "CommonConvert.h"
#include "ComplSocketClient.h"
#include "ComInit.h"
#include "afxdtctl.h"
#include "DataDef.h"
#include "atlimage.h"
#include "afxcmn.h"
#include "ShortMsgModem.h"
// CPropConnectInfo �Ի���

class CPropConnectInfo : public CDialog
{
	DECLARE_DYNAMIC(CPropConnectInfo)

public:
	CPropConnectInfo(CWnd* pParent = NULL);
	virtual ~CPropConnectInfo();

// �Ի�������
	enum { IDD = IDD_PROPCONNECTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ed_romoteIP;
	CEdit m_ed_romotePort;
	afx_msg void OnEnChangeEdRomoteip();
	afx_msg void OnEnChangeEdRomoteport();
//	virtual BOOL OnApply();
	CConnectConfig m_connect;
	virtual BOOL OnInitDialog();
//	afx_msg void OnBnClickedBnSendtest();

private:
//	CComplSocketClient m_client;
	CComInit* m_pComInit;
	CShortMsgModem* m_pShortMsg;
public:
	CComboBox m_com_caller;
	CComboBox m_com_readcard;
	CComboBox m_com_msg;
	afx_msg void OnCbnSelchangeComCaller();
	afx_msg void OnCbnSelchangeComReadcard();
	afx_msg void OnBnOk();
	afx_msg void OnBnCancel();
	CEdit m_ed_voiceTimes;
private:
	BOOL WritePlayViceTimes();//��д������������
	BOOL ReadPlayViceTimes();
	BOOL WriteSysLogicVaribiles();//дϵͳ�߼��������ļ�
	BOOL ReadSysLogicVaribiles();//��ϵͳ�߼��������ļ�
public:
	CButton m_checkk_lackPaper;
	CButton m_check_closeCmputer;
	CButton m_check_clearData;
	CButton m_check_useJtts;
	CButton m_Sel_CallType1;
	CButton m_Sel_CallType2;
	CDateTimeCtrl m_date_closeCmputer;
	CDateTimeCtrl m_date_clearData;

	LogicVariables m_logicVariables;//ϵͳ�߼�����
public:
	CEdit m_ed_organName;
	CEdit m_ed_organID;
	CButton m_check_send;
	//CStatic m_sta_set;
private:
	CImage m_picImage;
public:
	afx_msg void OnPaint();
	CEdit m_ed_timeOut;
	CSliderCtrl m_slider_speed;
	afx_msg void OnBnClickedCheckUsejtts();
	CButton m_check_changePage;
	CComboBox m_combox_parentOrg;
private:
	std::map<int,CommDaoOrg> m_map_commDaoOrg;
public:
	afx_msg void OnBnClickedBnFlushorg();
	afx_msg void OnBnClickedBnSavecon();
	afx_msg void OnBnClickedButtonMsgset();
	afx_msg void OnCbnSelchangeComboMsg();
	afx_msg void OnBnClickedBnTestinternet();
	CEdit m_ed_interIP;
//	CEdit m_ed_interPort;
	CButton m_check_inter;
//	CComboBox m_combo_newcard;
//	afx_msg void OnCbnSelchangeComboNewcard();
};
