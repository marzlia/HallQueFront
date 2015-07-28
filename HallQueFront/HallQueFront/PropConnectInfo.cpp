// PropConnectInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "PropConnectInfo.h"
#include "CommonStrMethod.h"
#include "DoFile.h"
#include "PropertyShortMsg.h"
#include "DealInterMsg.h"



// CPropConnectInfo �Ի���

IMPLEMENT_DYNAMIC(CPropConnectInfo, CDialog)

CPropConnectInfo::CPropConnectInfo(CWnd* pParent /* = NULL */)
	: CDialog(CPropConnectInfo::IDD,pParent)
{
	m_pComInit = CComInit::GetInstance();
	m_pShortMsg = CShortMsgModem::GetInstance();
	CDoFile doFile;
	CString exePath = doFile.GetExeFullFilePath();
	exePath+=_T("\\SYS");
	doFile.CreateMyDirectory(exePath);
	memset(&m_logicVariables,0,sizeof(m_logicVariables));
}

CPropConnectInfo::~CPropConnectInfo()
{
}

void CPropConnectInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_ROMOTEIP, m_ed_romoteIP);
	DDX_Control(pDX, IDC_ED_ROMOTEPORT, m_ed_romotePort);
	DDX_Control(pDX, IDC_COM_CALLER, m_com_caller);
	DDX_Control(pDX, IDC_COM_READCARD, m_com_readcard);
	DDX_Control(pDX,IDC_COMBO_MSG,m_com_msg);
	DDX_Control(pDX, IDC_ED_VOICETIMES, m_ed_voiceTimes);
	DDX_Control(pDX, IDC_CHECK_LACKPAPER, m_checkk_lackPaper);
	DDX_Control(pDX, IDC_CHECK_CLOSECMP, m_check_closeCmputer);
	DDX_Control(pDX, IDC_CHECK_CLEARDATA, m_check_clearData);
	DDX_Control(pDX, IDC_CHECK_USEJTTS, m_check_useJtts);
	DDX_Control(pDX, IDC_DATE_CLOSE, m_date_closeCmputer);
	DDX_Control(pDX, IDC_DATE_CLEARDATA, m_date_clearData);
	DDX_Control(pDX, IDC_ED_ORGANNAME, m_ed_organName);
	DDX_Control(pDX, IDC_ED_ORGANID, m_ed_organID);
	DDX_Control(pDX, IDC_CHECK_SEND, m_check_send);
	DDX_Control(pDX, IDC_ED_ROMOTEPORT2, m_ed_timeOut);
	DDX_Control(pDX, IDC_SLID_SPEED, m_slider_speed);
	DDX_Control(pDX, IDC_RADIO_TYPE1, m_Sel_CallType1);
	DDX_Control(pDX, IDC_RADIO_TYPE2, m_Sel_CallType2);
	DDX_Control(pDX, IDC_CHECK4, m_check_changePage);
	DDX_Control(pDX, IDC_COM_PARENTORG, m_combox_parentOrg);
	DDX_Control(pDX, IDC_ED_INTERIP, m_ed_interIP);
	//	DDX_Control(pDX, IDC_ED_INTERPORT, m_ed_interPort);
	DDX_Control(pDX, IDC_CHECK_INTER, m_check_inter);
//	DDX_Control(pDX, IDC_COMBO_NEWCARD, m_combo_newcard);
}


BEGIN_MESSAGE_MAP(CPropConnectInfo, CDialog)
	ON_EN_CHANGE(IDC_ED_ROMOTEIP, &CPropConnectInfo::OnEnChangeEdRomoteip)
	ON_EN_CHANGE(IDC_ED_ROMOTEPORT, &CPropConnectInfo::OnEnChangeEdRomoteport)
//	ON_BN_CLICKED(IDC_BN_SENDTEST, &CPropConnectInfo::OnBnClickedBnSendtest)
ON_CBN_SELCHANGE(IDC_COM_CALLER, &CPropConnectInfo::OnCbnSelchangeComCaller)
ON_CBN_SELCHANGE(IDC_COM_READCARD, &CPropConnectInfo::OnCbnSelchangeComReadcard)
ON_BN_CLICKED(IDC_BN_OK,&CPropConnectInfo::OnBnOk)
ON_BN_CLICKED(IDC_BN_CANCEL,&CPropConnectInfo::OnBnCancel)
ON_WM_PAINT()
ON_BN_CLICKED(IDC_CHECK_USEJTTS, &CPropConnectInfo::OnBnClickedCheckUsejtts)
ON_BN_CLICKED(IDC_BN_FLUSHORG, &CPropConnectInfo::OnBnClickedBnFlushorg)
ON_BN_CLICKED(IDC_BN_SAVECON, &CPropConnectInfo::OnBnClickedBnSavecon)
ON_BN_CLICKED(IDC_BUTTON_MSGSET, &CPropConnectInfo::OnBnClickedButtonMsgset)
ON_CBN_SELCHANGE(IDC_COMBO_MSG, &CPropConnectInfo::OnCbnSelchangeComboMsg)
ON_BN_CLICKED(IDC_BN_TESTINTERNET, &CPropConnectInfo::OnBnClickedBnTestinternet)
//ON_CBN_SELCHANGE(IDC_COMBO_NEWCARD, &CPropConnectInfo::OnCbnSelchangeComboNewcard)
END_MESSAGE_MAP()


// CPropConnectInfo ��Ϣ�������

void CPropConnectInfo::OnEnChangeEdRomoteip()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	SetModified();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CPropConnectInfo::OnEnChangeEdRomoteport()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	SetModified();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

// BOOL CPropConnectInfo::OnApply()
// {
// 	// TODO: �ڴ����ר�ô����/����û���
// 	CString port,ip;
// 	CCommonConvert convert;
// 	m_ed_romotePort.GetWindowText(port);
// 	m_ed_romoteIP.GetWindowText(ip);
// 	
// 	int iPort=0;
// 	convert.CStringToint(iPort,port);
// 	m_connect.SetRomoteIP(ip);
// 	m_connect.SetRomotePort(iPort);
// 	m_connect.SaveInfo();
// 	//���洮��
// 	m_comInit.SaveComm();
// 	return CPropertyPage::OnApply();
// }

BOOL CPropConnectInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	/*
	���ݿ�Ļ���
	*/
	list<CommDaoOrg>::const_iterator itera = theApp.m_list_comOrg.begin();
	for(itera;itera!=theApp.m_list_comOrg.end();itera++)
	{
		CString wOrgID(itera->curOrgID.c_str());
		if(wOrgID != theApp.m_logicVariables.strOrganID)//��ӵ�combox��
		{
			CString curOrgName(itera->curOrgName.c_str());
			int index = m_combox_parentOrg.AddString(curOrgName);
			m_map_commDaoOrg[index] = *itera;
		}
	}
	int count = m_combox_parentOrg.GetCount();
	for(int i=0;i<count;i++)
	{
		CString wOrgID(m_map_commDaoOrg[i].curOrgID.c_str());
		CString localParentOrgID(theApp.m_logicVariables.strParOrgID);
		if(wOrgID == localParentOrgID && !wOrgID.IsEmpty())
		{
			m_combox_parentOrg.SetCurSel(i);
			break;
		}
	}
	///////////////////////////////////////////////////////
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Sel_CallType1.SetCheck(BST_CHECKED);
	CString ip = m_connect.GetRomoteIP();
	UINT uPort = m_connect.GetRomotePort();
	UINT uTimeOut = m_connect.GetTimeOut();
	CString cPort;
	CCommonConvert convert;
	convert.intToCString(uPort,cPort);
	m_ed_romoteIP.SetWindowText(ip);
	m_ed_romotePort.SetWindowText(cPort);
	CString cTimeOut;
	cTimeOut.Format(_T("%d"),uTimeOut);
	m_ed_timeOut.SetWindowText(cTimeOut);

	///////////////////////////////////////////
	CString readCardCom = m_pComInit->GetCardComm();
	CString callerCom = m_pComInit->GetWndComm();
	CString MsgCom = m_pComInit->GetMsgComm();
//	CString newCardCom = m_pComInit->GetNewCardComm();

	m_com_caller.AddString(_T("0"));
	m_com_readcard.AddString(_T("0"));
	m_com_msg.AddString(_T("0"));

//	m_combo_newcard.AddString(_T("0"));
//	m_combo_newcard.AddString(_T("USB"));

	for(int i=0;i<10;i++)
	{
		if(m_pComInit->m_canUse[i]>0)
		{
			CString comm;
			comm.Format(_T("%d"),m_pComInit->m_canUse[i]);
			m_com_caller.AddString(comm);
			m_com_readcard.AddString(comm);
			m_com_msg.AddString(comm);
//			m_combo_newcard.AddString(comm);
		}
	}
	////////////////////////////////////////////
	for(int i=0;i<m_com_caller.GetCount();i++)
	{
		CString content;
		m_com_caller.GetLBText(i,content);
		if(callerCom == content)
		{
			m_com_caller.SetCurSel(i);
			break;
		}
	}
	///////////////////////////////////////
	for(int i=0;i<m_com_readcard.GetCount();i++)
	{
		CString content;
		m_com_readcard.GetLBText(i,content);
		if(readCardCom == content)
		{
			m_com_readcard.SetCurSel(i);
			break;
		}
	}

	for(int i=0;i<m_com_msg.GetCount();i++)
	{
		CString content;
		m_com_msg.GetLBText(i,content);
		if(MsgCom == content)
		{
			m_com_msg.SetCurSel(i);
			break;
		}
	}
	////////////////////////////////////////
// 	for(int i=0;i<m_combo_newcard.GetCount();i++)
// 	{
// 		CString content;
// 		m_combo_newcard.GetLBText(i,content);
// 		if(newCardCom == content)
// 		{
// 			m_combo_newcard.SetCurSel(i);
// 			break;
// 		}
// 	}
	///////////////////////////////Ĭ��ת�ƶ���
	CButton* pButton = (CButton*)GetDlgItem(IDC_RA_EXQUE);
	ASSERT(pButton);
	pButton->SetCheck(BST_CHECKED);
	///////////////////////////////
	///////////////����������
	ReadPlayViceTimes();
	//////////////���߼�����
	CTime currTime = CTime::GetCurrentTime();
	if(ReadSysLogicVaribiles())
	{
		if(m_logicVariables.IsAutoClearData)
		{
			m_check_clearData.SetCheck(BST_CHECKED);
			CTime nowTime(currTime.GetYear(),currTime.GetMonth(),currTime.GetDay(),
				m_logicVariables.ClearDataTime.GetHour(),
				m_logicVariables.ClearDataTime.GetMinute(),
				m_logicVariables.ClearDataTime.GetSecond());
			m_date_clearData.SetTime(&nowTime);
		}
		if(m_logicVariables.IsAutoCloseComputer)
		{
			m_check_closeCmputer.SetCheck(BST_CHECKED);
			CTime nowTime(currTime.GetYear(),currTime.GetMonth(),currTime.GetDay(),
				m_logicVariables.CloseComputerTime.GetHour(),
				m_logicVariables.CloseComputerTime.GetMinute(),
				m_logicVariables.CloseComputerTime.GetSecond());
			m_date_closeCmputer.SetTime(&nowTime);
		}
		if(m_logicVariables.IsLackPaperAlarm)
		{
			m_checkk_lackPaper.SetCheck(BST_CHECKED);
		}
		if(m_logicVariables.IsAutoSendToServer)
		{
			m_check_send.SetCheck(BST_CHECKED);
		}
		//////////////////////////
		if (m_logicVariables.IsUseJtts)
		{
			m_check_useJtts.SetCheck(BST_CHECKED);
			m_slider_speed.EnableWindow(TRUE);
		}

		if (m_logicVariables.iNumberCallType ==2)
		{
			m_Sel_CallType2.SetCheck(BST_CHECKED);
			m_Sel_CallType1.SetCheck(BST_UNCHECKED);
		}
		if(m_logicVariables.ExchangeQue)
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_RA_EXQUE);
			ASSERT(pButton);
			pButton->SetCheck(BST_CHECKED);
			//////////////////////////////////////
			pButton = (CButton*)GetDlgItem(IDC_RA_EXWINDOW);
			ASSERT(pButton);
			pButton->SetCheck(BST_UNCHECKED);
		}
		if(m_logicVariables.ExchangeWindow)
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_RA_EXWINDOW);
			ASSERT(pButton);
			pButton->SetCheck(BST_CHECKED);
			//////////////////////////////////////
			pButton = (CButton*)GetDlgItem(IDC_RA_EXQUE);
			ASSERT(pButton);
			pButton->SetCheck(BST_UNCHECKED);
		}
		if(m_logicVariables.IsOpenInterNum)
		{
			m_check_inter.SetCheck(BST_CHECKED);
		}
		else
		{
			m_check_inter.SetCheck(BST_UNCHECKED);
		}
		m_ed_interIP.SetWindowText(m_logicVariables.strInterIP);
		m_ed_organID.SetWindowText(m_logicVariables.strOrganID);
		m_ed_organName.SetWindowText(m_logicVariables.strOrganNmae);
	}
	m_picImage.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_SET));
	m_slider_speed.SetRange(1,10);
	if(m_logicVariables.playSpeed==0)
	{
		m_slider_speed.SetPos(6);
	}
	else
	{
		m_slider_speed.SetPos(m_logicVariables.playSpeed);
	}


	if(m_logicVariables.IsAutoChangePage)
	{
		m_check_changePage.SetCheck(BST_CHECKED);
	}
	else
	{
		m_check_changePage.SetCheck(BST_UNCHECKED);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
/*
void CPropConnectInfo::OnBnClickedBnSendtest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_client.SendData(_T("hello bitch"));
}
*/
void CPropConnectInfo::OnCbnSelchangeComCaller()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCommonConvert convert;
	int index=m_com_caller.GetCurSel();
	if(index == CB_ERR)
	{
		return;
	}
	CString wndScreenCom=_T("");
	m_com_caller.GetLBText(index,wndScreenCom);
	int i_wndsScreenCom=0;
	convert.CStringToint(i_wndsScreenCom,wndScreenCom);

	if(m_pComInit->OpenWndScreen(i_wndsScreenCom) == -1)
	{
		m_com_caller.SetCurSel(0);		
		MessageBox(_T("���������ڴ�ʧ�ܻ�ռ��"),_T("ע��"),MB_OK|MB_ICONINFORMATION);
	}
	
//	SetModified();
}

void CPropConnectInfo::OnCbnSelchangeComReadcard()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCommonConvert convert;
	int index=m_com_readcard.GetCurSel();
	if(index == CB_ERR)
	{
		return;
	}
	CString readCardCom=_T("");
	m_com_readcard.GetLBText(index,readCardCom);
	int i_readCardCom=0;
	convert.CStringToint(i_readCardCom,readCardCom);

	if(m_pComInit->OpenCardComm(i_readCardCom) == -1)
	{
		m_com_readcard.SetCurSel(0);
		MessageBox(_T("ˢ�����ڴ�ʧ�ܻ�ռ��"),_T("ע��"),MB_OK|MB_ICONINFORMATION);
	}
}

void CPropConnectInfo::OnBnOk()
{
	CString port,ip;
	CCommonConvert convert;
 	m_ed_romotePort.GetWindowText(port);
	m_ed_romoteIP.GetWindowText(ip);
 	
	int iPort=0;
 	convert.CStringToint(iPort,port);
	m_connect.SetRomoteIP(ip);
	m_connect.SetRomotePort(iPort);

	CString cTimeOut;
	m_ed_timeOut.GetWindowText(cTimeOut);
	int iTimeOut = 0;
	convert.CStringToint(iTimeOut,cTimeOut);
	m_connect.SetTimeOut(iTimeOut);
	
	m_connect.SaveInfo();
 	//���洮��
	m_pComInit->SaveComm();
	//������д���
	WritePlayViceTimes();
	//дϵͳ�߼�����
	WriteSysLogicVaribiles();
	return CDialog::OnOK();
}

void CPropConnectInfo::OnBnCancel()
{
	return CDialog::OnCancel();
}

BOOL CPropConnectInfo::WritePlayViceTimes()
{
	CString strExePath = CommonStrMethod::GetModuleDir();
	strExePath += _T("\\SYS\\sysconfig.ini");
	CString voicesTimes;
	m_ed_voiceTimes.GetWindowText(voicesTimes);
	WritePrivateProfileString(_T("sys"),_T("SOUND_REPLAY_TIMES"),voicesTimes,strExePath);
	return TRUE;
}

BOOL CPropConnectInfo::ReadPlayViceTimes()
{
	CString strExePath = CommonStrMethod::GetModuleDir();
	strExePath += _T("\\SYS\\sysconfig.ini");
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("sys"),_T("SOUND_REPLAY_TIMES"),NULL,wbuf,255,strExePath);
	CString viceTimes;
	viceTimes.Format(_T("%s"),wbuf);
	if(viceTimes.IsEmpty())
		m_ed_voiceTimes.SetWindowText(_T("1"));
	else
		m_ed_voiceTimes.SetWindowText(viceTimes);
	return TRUE;
}

BOOL CPropConnectInfo::ReadSysLogicVaribiles()
{
	CString strExePath = CommonStrMethod::GetModuleDir();
	strExePath+=_T("\\SysLogicVaribiles.dat");
	CFile logicFile;
	if(logicFile.Open(strExePath,CFile::modeRead))
	{
		logicFile.Read(&m_logicVariables,sizeof(m_logicVariables));
		logicFile.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CPropConnectInfo::WriteSysLogicVaribiles()
{
	if(BST_CHECKED == m_check_closeCmputer.GetCheck())
	{
		m_logicVariables.IsAutoCloseComputer=TRUE;
		CTime closeTime;
		m_date_closeCmputer.GetTime(closeTime);
		m_logicVariables.CloseComputerTime = closeTime;
	}
	else
	{
		m_logicVariables.IsAutoCloseComputer=FALSE;
	}
	if(BST_CHECKED == m_check_clearData.GetCheck())
	{
		m_logicVariables.IsAutoClearData = TRUE;
		CTime clearTime;
		m_date_clearData.GetTime(clearTime);
		m_logicVariables.ClearDataTime = clearTime;
	}
	else
	{
		m_logicVariables.IsAutoClearData = FALSE;
	}
	if(BST_CHECKED == m_checkk_lackPaper.GetCheck())
	{
		m_logicVariables.IsLackPaperAlarm = TRUE;
	}
	else
	{
		m_logicVariables.IsLackPaperAlarm = FALSE;
	}
	if(BST_CHECKED == m_check_useJtts.GetCheck())
	{
		m_logicVariables.IsUseJtts = TRUE;
	}
	else
	{
		m_logicVariables.IsUseJtts = FALSE;
	}
	if(BST_CHECKED == m_check_send.GetCheck())
	{
		m_logicVariables.IsAutoSendToServer = TRUE;
	}
	else
	{
		m_logicVariables.IsAutoSendToServer = FALSE;
	}
	CButton* pQueButton = (CButton*)GetDlgItem(IDC_RA_EXQUE);
	if(BST_CHECKED == pQueButton->GetCheck())
	{
		m_logicVariables.ExchangeQue = TRUE;
	}
	else
	{
		m_logicVariables.ExchangeQue = FALSE;
	}
	CButton* pWindowButton = (CButton*)GetDlgItem(IDC_RA_EXWINDOW);
	if(BST_CHECKED == pWindowButton->GetCheck())
	{
		m_logicVariables.ExchangeWindow = TRUE;
	}
	else
	{
		m_logicVariables.ExchangeWindow = FALSE;
	}
	if(BST_CHECKED == m_check_changePage.GetCheck())
	{
		m_logicVariables.IsAutoChangePage = TRUE;
	}
	else
	{
		m_logicVariables.IsAutoChangePage = FALSE;
	}
	if(BST_CHECKED == m_check_inter.GetCheck())
	{
		m_logicVariables.IsOpenInterNum = TRUE;
	}
	else
	{
		m_logicVariables.IsOpenInterNum = FALSE;
	}
	CString wstrOrganID;
	m_ed_organID.GetWindowText(wstrOrganID);
	CString wstrOrganName;
	m_ed_organName.GetWindowText(wstrOrganName);
	wcscpy_s(m_logicVariables.strOrganID,MYBUFLEN,
		wstrOrganID.GetBuffer(0));
	wstrOrganID.ReleaseBuffer(0);
	wcscpy_s(m_logicVariables.strOrganNmae,MYBUFLEN,
		wstrOrganName.GetBuffer(0));
	wstrOrganName.ReleaseBuffer(0);
	CString wstrInterIP;
	m_ed_interIP.GetWindowText(wstrInterIP);
	wcscpy_s(m_logicVariables.strInterIP,MYBUFLEN,
		wstrInterIP.GetBuffer(0));
	wstrInterIP.ReleaseBuffer(0);

	int nSpeed = m_slider_speed.GetPos();
	m_logicVariables.playSpeed = nSpeed;
	
	if (m_Sel_CallType1.GetCheck()==BST_CHECKED)
	{
		m_logicVariables.iNumberCallType = 1;
	}
	if (m_Sel_CallType2.GetCheck()==BST_CHECKED)
	{
		m_logicVariables.iNumberCallType = 2;
	}
	///////////////////////////////////////////
	int index = m_combox_parentOrg.GetCurSel();
	if(index!=CB_ERR)
	{
		CString parOrgName(m_map_commDaoOrg[index].curOrgName.c_str());
		CString parOrgID(m_map_commDaoOrg[index].curOrgID.c_str());
		wcscpy_s(m_logicVariables.strParOrgID,MYBUFLEN,parOrgID.GetBuffer(0));
		parOrgID.ReleaseBuffer();
		wcscpy_s(m_logicVariables.strParOrgName,MYBUFLEN,parOrgName.GetBuffer(0));
		parOrgID.ReleaseBuffer();
	}
	/////////////////////////////////
	CString strExePath = CommonStrMethod::GetModuleDir();
	strExePath+=_T("\\SysLogicVaribiles.dat");
	CFile logicFile;
	if(logicFile.Open(strExePath,CFile::modeCreate | CFile::modeWrite))
	{
		logicFile.Write(&m_logicVariables,sizeof(m_logicVariables));
		logicFile.Close();
		return TRUE;
	}
	return FALSE;
}
void CPropConnectInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	//CRect rect;
	//m_sta_set.GetWindowRect(rect);
	//ScreenToClient(rect);
	//SetStretchBltMode(dc, COLORONCOLOR);//������ɫʧ��
	//if(!m_picImage.IsNull())
	//	m_picImage.StretchBlt(dc,rect.left,rect.top,rect.Width(),rect.Height(),SRCCOPY);
}

void CPropConnectInfo::OnBnClickedCheckUsejtts()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(BST_CHECKED == m_check_useJtts.GetCheck())
	{
		GetDlgItem(IDC_SLID_SPEED)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_SLID_SPEED)->EnableWindow(FALSE);
	}
}
void CPropConnectInfo::OnBnClickedBnFlushorg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetCursor(LoadCursor(NULL,IDC_WAIT));
	if(BST_CHECKED == m_check_send.GetCheck())
//	if(theApp.m_logicVariables.IsAutoSendToServer)
		theApp.m_Controller.ReadCommDaoOrgInfo();
	///////////////////////////////////////////////////
	int count	= m_combox_parentOrg.GetCount();
	for(int i=count-1;i>=0;i--)
	{
		m_combox_parentOrg.DeleteString(i);
	}
	m_map_commDaoOrg.clear();
	//////////////////////////////////////////////////
	list<CommDaoOrg>::const_iterator itera = theApp.m_list_comOrg.begin();
	for(itera;itera!=theApp.m_list_comOrg.end();itera++)
	{
		CString wOrgID(itera->curOrgID.c_str());
		if(wOrgID != theApp.m_logicVariables.strOrganID)//��ӵ�combox��
		{
			CString curOrgName(itera->curOrgName.c_str());
			m_map_commDaoOrg[m_combox_parentOrg.AddString(curOrgName)]
			= *itera;
		}
	}
	SetCursor(LoadCursor(NULL,IDC_ARROW));
}

void CPropConnectInfo::OnBnClickedBnSavecon()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString port,ip;
	CCommonConvert convert;
	m_ed_romotePort.GetWindowText(port);
	m_ed_romoteIP.GetWindowText(ip);

	int iPort=0;
	convert.CStringToint(iPort,port);
	m_connect.SetRomoteIP(ip);
	m_connect.SetRomotePort(iPort);

	CString cTimeOut;
	m_ed_timeOut.GetWindowText(cTimeOut);
	int iTimeOut = 0;
	convert.CStringToint(iTimeOut,cTimeOut);
	m_connect.SetTimeOut(iTimeOut);

	m_connect.SaveInfo();
}

void CPropConnectInfo::OnBnClickedButtonMsgset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPropertyShortMsg PropertyShortMsg(_T("��Ϣ����"),this);
	PropertyShortMsg.DoModal();
}



void CPropConnectInfo::OnCbnSelchangeComboMsg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCommonConvert convert;
	int index=m_com_msg.GetCurSel();
	if(index == CB_ERR)
	{
		return;
	}
	CString MsgCom=_T("");
	m_com_msg.GetLBText(index,MsgCom);
	int i_MsgCom=0;
	convert.CStringToint(i_MsgCom,MsgCom);
	m_pShortMsg->CloseMsgComm();
	if (i_MsgCom!=0)
	{
		if(!m_pShortMsg->OpenMsgComm(i_MsgCom))
		{
			MessageBox(_T("����è���ڴ�ʧ�ܻ�ռ��"),_T("ע��"),MB_OK|MB_ICONINFORMATION);
			m_com_msg.SetCurSel(0);
		}
		else 
		{
			CString strCom;
			CCommonConvert::intToCString(i_MsgCom,strCom);
			m_pComInit->SetMsgComm(strCom);
		}
	}
	else m_pComInit->SetMsgComm(L"0");
}

void CPropConnectInfo::OnBnClickedBnTestinternet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(theApp.IsLocal())
		return;
	
	CComplSocketClient client;

	string sendMsg,recvMsg;
	int actRecvSize = 0;
	CDealInterMsg::ProduceSendInNumMsg(_T("1"),sendMsg);
	if(client.SendData(INTERPORT,theApp.m_logicVariables.strInterIP,
		sendMsg,sendMsg.size(),recvMsg,actRecvSize) && actRecvSize)
	{
		MessageBox(_T("���ӳɹ�"),_T("ע��"),MB_OK | MB_ICONINFORMATION);
	}
}

// void CPropConnectInfo::OnCbnSelchangeComboNewcard()
// {
// 	// TODO: �ڴ���ӿؼ�֪ͨ����������
// 	CCommonConvert convert;
// 	int index=m_combo_newcard.GetCurSel();
// 	if(index == CB_ERR)
// 	{
// 		return;
// 	}
// 
// 	CComInit* pComInit = CComInit::GetInstance();
// 	CString newCardCom;
// 	m_combo_newcard.GetLBText(index,newCardCom);
// 
// //	char* pErrInfo = new char[256];
// 
// 	pComInit->OpenNewCardComm(newCardCom);
// 
// 	pComInit->SetNewCardComm(newCardCom);
// }
