// CardConfigQue.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "CardConfigQue.h"
#include "Dofile.h"
#include "CommonConvert.h"


//extern g_swingCardInfoID;
// CCardConfigQue 对话框
extern UINT g_swingCardInfoID;
extern UINT g_cardLevelSerialID;

IMPLEMENT_DYNAMIC(CCardConfigQue, CDialog)

CCardConfigQue::CCardConfigQue(CWnd* pParent /*=NULL*/)
	: CDialog(CCardConfigQue::IDD, pParent)
	, m_cs_cardName(_T(""))
	, m_cs_cardID(_T(""))
	, m_cs_config1(_T(""))
	, m_cs_postion1(_T(""))
	, m_cs_config2(_T(""))
	, m_cs_postion2(_T(""))
	, m_cs_config3(_T(""))
	, m_cs_postion3(_T(""))
	, m_cs_config4(_T(""))
	, m_cs_postion4(_T(""))
	, m_cs_levName(_T(""))
	, m_cs_levID(_T(""))
{
	////////队列信息文件路径
	m_queinfofile_path = m_convert.GetExeFullFilePath();
	m_queinfofile_path += _T("\\QueBasicInfo\\QueBasicInfo.dat");
	////////卡识别信息文件路径
	m_cardcofinfo_path = m_convert.GetExeFullFilePath();
	m_cardcofinfo_path += _T("\\CardConfigInfo");
	////////卡对接信息文件路径
	m_connect_path = m_convert.GetExeFullFilePath();
	m_connect_path += _T("\\CardConfigInfo");
	////////卡等级信息文件路径
	m_cardLev_path = m_convert.GetExeFullFilePath();
	m_cardLev_path += _T("\\CardConfigInfo");
	///////////////////////////////////////////////
	CDoFile doFile;
	doFile.CreateMyDirectory(m_cardcofinfo_path);
	m_cardcofinfo_path += _T("\\CardConfigInfo.dat");
	m_connect_path += _T("\\CardConnectInfo.dat");
	m_cardLev_path += _T("\\CardLevInfo.dat");
//	memset(&m_cardConnectInfo,0,sizeof(m_cardConnectInfo));
}

CCardConfigQue::~CCardConfigQue()
{
	ClearSwingCardInfo();
	ClearCardLevInfo();
}

void CCardConfigQue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX_CARD, m_listbox_card);
	DDX_Control(pDX, IDC_COM_ATTCHQUE, m_com_attchQueID);
	DDX_Control(pDX, IDC_ED_CARDTYPENAME, m_ed_cardName);
	DDX_Control(pDX, IDC_ED_CARDID, m_ed_cardID);
	DDX_Control(pDX, IDC_ED_CONFIG1, m_ed_config1);
	DDX_Control(pDX, IDC_ED_POSTION1, m_ed_postion1);
	DDX_Control(pDX, IDC_ED_CONFIG2, m_ed_config2);
	DDX_Control(pDX, IDC_ED_POSTION2, m_ed_postion2);

	DDX_Text(pDX, IDC_ED_CARDTYPENAME, m_cs_cardName);
	DDX_Text(pDX, IDC_ED_CARDID, m_cs_cardID);
	DDX_Text(pDX, IDC_ED_CONFIG1, m_cs_config1);
	DDX_Text(pDX, IDC_ED_POSTION1, m_cs_postion1);
	DDX_Text(pDX, IDC_ED_CONFIG2, m_cs_config2);
	DDX_Text(pDX, IDC_ED_POSTION2, m_cs_postion2);
	DDX_Text(pDX, IDC_ED_CONFIG3, m_cs_config3);
	DDX_Text(pDX, IDC_ED_POSTION3, m_cs_postion3);
	DDX_Text(pDX, IDC_ED_CONFIG4, m_cs_config4);
	DDX_Text(pDX, IDC_ED_POSTION4, m_cs_postion4);
	DDX_Control(pDX, IDC_CHECK_CONNECT, m_check_connect);
	DDX_Control(pDX, IDC_ED_SERVERIP, m_ed_serverip);
	DDX_Control(pDX, IDC_ED_SERVERPORT, m_ed_serverPort);
	DDX_Control(pDX, IDC_ED_SERVERTIME, m_ed_serverTime);
	DDX_Control(pDX, IDC_ED_CONFIG3, m_ed_config3);
	DDX_Control(pDX, IDC_ED_POSTION3, m_ed_postion3);
	DDX_Control(pDX, IDC_ED_CONFIG4, m_ed_config4);
	DDX_Control(pDX, IDC_ED_POSTION4, m_ed_postion4);
	DDX_Control(pDX, IDC_COM_SFZATTCHQUE, m_com_regAttchQueID);
	DDX_Control(pDX, IDC_LIST_LEVEL, m_listctrl_level);
	DDX_Control(pDX, IDC_ED_LEVELNAME, m_ed_levName);
	DDX_Control(pDX, IDC_ED_TOLEVEL, m_ed_levID);
	DDX_Control(pDX, IDC_COM_LEVTOQUEID, m_com_levToQueID);
	DDX_Text(pDX, IDC_ED_LEVELNAME, m_cs_levName);
	DDX_Text(pDX, IDC_ED_TOLEVEL, m_cs_levID);
	DDX_Control(pDX, IDC_STA_CARDPIC, m_sta_bankPic);
}


BEGIN_MESSAGE_MAP(CCardConfigQue, CDialog)
	ON_BN_CLICKED(IDC_BN_ADDCARD, &CCardConfigQue::OnBnClickedBnAddcard)
	ON_BN_CLICKED(IDC_BN_DELCARD, &CCardConfigQue::OnBnClickedBnDelcard)
	ON_LBN_SELCHANGE(IDC_LISTBOX_CARD, &CCardConfigQue::OnLbnSelchangeListboxCard)
	ON_CBN_SELCHANGE(IDC_COM_ATTCHQUE, &CCardConfigQue::OnCbnSelchangeComAttchque)
	ON_EN_CHANGE(IDC_ED_CARDTYPENAME, &CCardConfigQue::OnEnChangeEdCardtypename)
	ON_BN_CLICKED(IDOK, &CCardConfigQue::OnBnClickedOk)
	ON_EN_CHANGE(IDC_ED_CONFIG1, &CCardConfigQue::OnEnChangeEdConfig1)
	ON_EN_CHANGE(IDC_ED_POSTION1, &CCardConfigQue::OnEnChangeEdPostion1)
	ON_EN_CHANGE(IDC_ED_CONFIG3, &CCardConfigQue::OnEnChangeEdConfig3)
	ON_EN_CHANGE(IDC_ED_POSTION3, &CCardConfigQue::OnEnChangeEdPostion3)
	ON_EN_CHANGE(IDC_ED_CONFIG2, &CCardConfigQue::OnEnChangeEdConfig2)
	ON_EN_CHANGE(IDC_ED_POSTION2, &CCardConfigQue::OnEnChangeEdPostion2)
	ON_EN_CHANGE(IDC_ED_CONFIG4, &CCardConfigQue::OnEnChangeEdConfig4)
	ON_EN_CHANGE(IDC_ED_POSTION4, &CCardConfigQue::OnEnChangeEdPostion4)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CCardConfigQue::OnBnClickedCheckConnect)
	ON_BN_CLICKED(IDC_BN_ADDLEV, &CCardConfigQue::OnBnClickedBnAddlev)
	ON_BN_CLICKED(IDC_BN_DELLEV, &CCardConfigQue::OnBnClickedBnDellev)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LEVEL, &CCardConfigQue::OnLvnItemchangedListLevel)
	ON_EN_CHANGE(IDC_ED_LEVELNAME, &CCardConfigQue::OnEnChangeEdLevelname)
	ON_EN_CHANGE(IDC_ED_TOLEVEL, &CCardConfigQue::OnEnChangeEdTolevel)
	ON_CBN_SELCHANGE(IDC_COM_LEVTOQUEID, &CCardConfigQue::OnCbnSelchangeComLevtoqueid)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CCardConfigQue 消息处理程序

void CCardConfigQue::OnBnClickedBnAddcard()
{
	// TODO: 在此添加控件通知处理程序代码
	CSwingCard *pCSwingCard=new CSwingCard;
	CString swingCardName=_T("卡类型");
	swingCardName.AppendFormat(_T("%d"),pCSwingCard->GetCardInfoID());
	pCSwingCard->SetCardName(swingCardName);
	int index = m_listbox_card.AddString(swingCardName);
	m_listbox_card.SetCurSel(index);
	m_list_swingCard.Add(pCSwingCard);
	OnLbnSelchangeListboxCard();
}

BOOL CCardConfigQue::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	ReadQueInfoFromFile();
	ReadConfigCardInfoFromFile();
	ReadConnectInfoFromFile();
	/////////////////////////////
	AddPageIDInfo();
	/////////////////////////////
	if(m_cardConnectInfo.IsConnect)
	{
		m_check_connect.SetCheck(BST_CHECKED);
	}
	else
	{
		m_check_connect.SetCheck(BST_UNCHECKED);
	}
	m_ed_serverip.SetWindowText(m_cardConnectInfo.ServerIP);
	CString port;
	port.Format(_T("%d"),m_cardConnectInfo.ServerPort);
	m_ed_serverPort.SetWindowText(port);
	CString overTime;
	overTime.Format(_T("%d"),m_cardConnectInfo.OverTime);
	m_ed_serverTime.SetWindowText(overTime);
	///////////////////////////////////////////////处理身份证对应
	CString regAttchQueID(m_cardConnectInfo.RegAttchQueID);//身份证对应ID
	int nRegAttchPageID = m_cardConnectInfo.RegAttchPageID;//身份证对应界面ID 
	for(int i=0;i<m_com_regAttchQueID.GetCount();i++)
	{
		CQueueInfo queInfo;
		if(m_map_que.Lookup(i,queInfo))
		{
			if(regAttchQueID == m_map_que[i].GetQueID())
			{
				m_com_regAttchQueID.SetCurSel(i);
				break;
			}
		}
		map<int,UINT>::const_iterator itera = m_map_pageID.find(i);
		if(itera!=m_map_pageID.end())
		{
			if(nRegAttchPageID == (int)itera->second)
			{
				m_com_regAttchQueID.SetCurSel(i);
				break;
			}
		}
	}
	///////////////////////////
	OnBnClickedCheckConnect();
	////////////////////////////////
	CRect listCtrlLevRect;
	m_listctrl_level.GetClientRect(listCtrlLevRect);
	int nCol = 0;
	m_listctrl_level.InsertColumn(nCol++,_T("序号"),LVCFMT_CENTER,20*listCtrlLevRect.Width()/100,-1);
	m_listctrl_level.InsertColumn(nCol++,_T("级别名称"),LVCFMT_CENTER,30*listCtrlLevRect.Width()/100,-1);
	m_listctrl_level.InsertColumn(nCol++,_T("对应级别"),LVCFMT_CENTER,30*listCtrlLevRect.Width()/100,-1);
	m_listctrl_level.InsertColumn(nCol++,_T("关联队列"),LVCFMT_CENTER,20*listCtrlLevRect.Width()/100,-1);
	m_listctrl_level.ModifyStyle(0,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	m_listctrl_level.SetExtendedStyle(LVS_EX_GRIDLINES);//添加网格风格
	//读卡等级信息
	ReadCardLevelFromFile();
	int count = m_listctrl_level.GetItemCount();
	m_listctrl_level.SetItemState(count-1, LVIS_SELECTED, LVIS_SELECTED);//让最后一项被选中
	////////////////////////////////////////
//	m_sta_bankPic.ModifyStyle(SS_BITMAP,SS_ICON|SS_CENTERIMAGE);
//	m_sta_bankPic.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_BANKCARD));
	try
	{
		m_picImage.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_CARD));
	}
	catch(...)
	{
		
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCardConfigQue::OnBnClickedBnDelcard() 
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_listbox_card.GetCurSel();
	if(index != CB_ERR)
	{
		delete m_list_swingCard[index];
		m_list_swingCard[index] = NULL;
		m_list_swingCard.RemoveAt(index);
		m_listbox_card.DeleteString(index);
		if(index-1>=0)
		{
			m_listbox_card.SetCurSel(index-1);
			OnLbnSelchangeListboxCard();
		}
	}
}

void CCardConfigQue::OnLbnSelchangeListboxCard()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_listbox_card.GetCurSel();
	if(index != CB_ERR)
	{
		m_cs_cardName = m_list_swingCard[index]->GetCardName();
		CString cardID;
		cardID.Format(_T("%d"),m_list_swingCard[index]->GetCardInfoID());
		m_cs_cardID = cardID;
		/////////////////////////
		//读出关联队列
		//m_list_swingCard[index]->GetAttchQueID();
		BOOL flag = FALSE;
		for(int i=0;i<m_com_attchQueID.GetCount();i++)
		{
			CQueueInfo qutInfo;
			if(m_map_que.Lookup(i,qutInfo))
			{
				if(qutInfo.GetQueID() == m_list_swingCard[index]->GetAttchQueID())
				{
					m_com_attchQueID.SetCurSel(i);
					flag = TRUE;
				}
			}
			map<int,UINT>::const_iterator itera = m_map_pageID.find(i);
			if(itera!=m_map_pageID.end())
			{
				if(m_list_swingCard[index]->GetAttchPageID() == itera->second)
				{
					m_com_attchQueID.SetCurSel(i);
					flag = TRUE;
				}
			}
		}
		if(!flag)
		{
			m_com_attchQueID.SetCurSel(CB_ERR);
		}
		/////////////////////////
		/////卡识别信息
		///////////////////////////////////////
	   ///////////////////////////////////////
//		int count = m_list_swingCard[index]->m_disTinguishCard.GetCount();
	   for(int i=0;i<4;i++)
	   {
		   CString c_postion,c_value;
		   c_postion.Format(_T("%d"),m_list_swingCard[index]->
			   m_disTinguishCard[i].postion);
		   c_value.Format(_T("%d"),m_list_swingCard[index]->
			   m_disTinguishCard[i].toValue);
		   switch(i)
		   {
		   case 0:
				m_cs_postion1 = c_postion;m_cs_config1 = c_value;
				break;
		   case 1:
			   m_cs_postion2 = c_postion;m_cs_config2 = c_value;
			   break;
		   case 2:
			   m_cs_postion3 = c_postion;m_cs_config3 = c_value;
			   break;
		   case 3:
			   m_cs_postion4 = c_postion;m_cs_config4 = c_value;
			   break;
		   }
	   }
	   ////////////////////////////////////////
	   UpdateData(FALSE);
	}
}

BOOL CCardConfigQue::ReadQueInfoFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_queinfofile_path,CFile::modeRead,&e))
	{
		CQueueInfo* queinfo=NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>queinfo;
				if (queinfo)
				{
					m_map_que[m_com_attchQueID.AddString(queinfo->GetBussName())]
					= *queinfo;
					m_com_regAttchQueID.AddString(queinfo->GetBussName());
					m_com_levToQueID.AddString(queinfo->GetBussName());
					delete queinfo;
					queinfo = NULL;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
	}
	return TRUE;
}
void CCardConfigQue::OnCbnSelchangeComAttchque()
{
	// TODO: 在此添加控件通知处理程序代码
	int listboxIndex = m_listbox_card.GetCurSel();
	if(listboxIndex!=CB_ERR)
	{
		int index = m_com_attchQueID.GetCurSel();
		if(index!=CB_ERR)
		{
			/*
			m_list_swingCard[listboxIndex]->SetAttchQueID(
				m_map_que[index].GetQueID());
				*/
			CQueueInfo queInfo;
			if(m_map_que.Lookup(index,queInfo))
			{
				m_list_swingCard[listboxIndex]->SetAttchQueID(
					m_map_que[index].GetQueID());
				m_list_swingCard[listboxIndex]->SetAttchPageID(-1);
			}
			map<int,UINT>::const_iterator itera = m_map_pageID.find(index);
			if(itera!=m_map_pageID.end())
			{
				m_list_swingCard[listboxIndex]->SetAttchPageID(itera->second);
				m_list_swingCard[listboxIndex]->SetAttchQueID(_T(""));//置空
			}
		}
	}
}

void CCardConfigQue::OnEnChangeEdCardtypename()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
//	m_ed_cardName.GetWindowText(content);
	int index = m_listbox_card.GetCurSel();
	if(index != CB_ERR)
	{
		m_list_swingCard[index]->SetCardName(m_cs_cardName);
		m_listbox_card.DeleteString(index);
		m_listbox_card.InsertString(index,m_cs_cardName);	
		m_listbox_card.SetCurSel(index);
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CCardConfigQue::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	WriteConfigCardInfoToFile();
	WriteConnectInfoToFile();
	WriteCardLevelToFile();
	OnOK();
}
BOOL CCardConfigQue::WriteConfigCardInfoToFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_cardcofinfo_path,CFile::modeCreate|CFile::modeWrite,&e))
	{
		int count=m_list_swingCard.GetCount();
		for (int i=0;i<count;i++)
		{
			CArchive ar(&file,CArchive::store);
			ar<<m_list_swingCard[i];
			ar.Close();
		}
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CCardConfigQue::ReadConfigCardInfoFromFile()
{
	ClearSwingCardInfo();
	CFile file;
	CFileException e;
	if (file.Open(m_cardcofinfo_path,CFile::modeRead,&e))
	{
		////注意序列化出来的指针变量已经分配了内存
		CSwingCard* cardConfInfo = NULL;
		UINT cardConfigID = 0;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
		{
			do
			{
				ar>>cardConfInfo;
				if (cardConfInfo)
				{
					m_listbox_card.AddString(cardConfInfo->GetCardName());
					m_list_swingCard.Add(cardConfInfo);
					cardConfigID = cardConfInfo->GetCardInfoID() > cardConfigID
						? cardConfInfo->GetCardInfoID() : cardConfigID;
				}
			}while(!ar.IsBufferEmpty());
		}
		ar.Close();
		file.Close();
		g_swingCardInfoID = cardConfigID;
	}
	else return FALSE;
	return TRUE;
}
void CCardConfigQue::OnEnChangeEdConfig1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	int index = m_listbox_card.GetCurSel();
	if(index != CB_ERR)
	{
		int i_value = 0;
		m_convert.CStringToint(i_value,m_cs_config1);
		m_list_swingCard[index]->m_disTinguishCard[0].toValue = i_value;
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CCardConfigQue::OnEnChangeEdPostion1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	int index = m_listbox_card.GetCurSel();
	if(index != CB_ERR)
	{
		int i_postion = 0;
		m_convert.CStringToint(i_postion,m_cs_postion1);
		m_list_swingCard[index]->m_disTinguishCard[0].postion = i_postion;
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CCardConfigQue::OnEnChangeEdConfig3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	int index = m_listbox_card.GetCurSel();
	if(index != CB_ERR)
	{
		int i_value = 0;
		m_convert.CStringToint(i_value,m_cs_config3);
		m_list_swingCard[index]->m_disTinguishCard[2].toValue = i_value;
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CCardConfigQue::OnEnChangeEdPostion3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	int index = m_listbox_card.GetCurSel();
	if(index != CB_ERR)
	{
		int i_postion = 0;
		m_convert.CStringToint(i_postion,m_cs_postion3);
		m_list_swingCard[index]->m_disTinguishCard[2].postion = i_postion;
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CCardConfigQue::OnEnChangeEdConfig2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	int index = m_listbox_card.GetCurSel();
	if(index != CB_ERR)
	{
		int i_value = 0;
		m_convert.CStringToint(i_value,m_cs_config2);
		m_list_swingCard[index]->m_disTinguishCard[1].toValue = i_value;
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CCardConfigQue::OnEnChangeEdPostion2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	int index = m_listbox_card.GetCurSel();
	if(index != CB_ERR)
	{
		int i_postion = 0;
		m_convert.CStringToint(i_postion,m_cs_postion2);
		m_list_swingCard[index]->m_disTinguishCard[1].postion = i_postion;
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CCardConfigQue::OnEnChangeEdConfig4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	int index = m_listbox_card.GetCurSel();
	if(index != CB_ERR)
	{
		int i_value = 0;
		m_convert.CStringToint(i_value,m_cs_config4);
		m_list_swingCard[index]->m_disTinguishCard[3].toValue = i_value;
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CCardConfigQue::OnEnChangeEdPostion4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	int index = m_listbox_card.GetCurSel();
	if(index != CB_ERR)
	{
		int i_postion = 0;
		m_convert.CStringToint(i_postion,m_cs_postion4);
		m_list_swingCard[index]->m_disTinguishCard[3].postion = i_postion;
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CCardConfigQue::OnBnClickedCheckConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_check_connect.GetCheck()==BST_CHECKED)
	{
		m_ed_cardID.EnableWindow(FALSE);
		m_ed_cardName.EnableWindow(FALSE);
		m_ed_config1.EnableWindow(FALSE);
		m_ed_config2.EnableWindow(FALSE);
		m_ed_postion1.EnableWindow(FALSE);
		m_ed_postion2.EnableWindow(FALSE);
		m_ed_config3.EnableWindow(FALSE);
		m_ed_config4.EnableWindow(FALSE);
		m_ed_postion3.EnableWindow(FALSE);
		m_ed_postion4.EnableWindow(FALSE);
		m_com_attchQueID.EnableWindow(FALSE);
		m_listbox_card.EnableWindow(FALSE);
		CButton* pButton = (CButton*)GetDlgItem(IDC_BN_ADDCARD);
		pButton->EnableWindow(FALSE);
		pButton = (CButton*)GetDlgItem(IDC_BN_DELCARD);
		pButton->EnableWindow(FALSE);
		////////////////////////////////////////
		m_ed_serverip.EnableWindow();
		m_ed_serverPort.EnableWindow();
		m_ed_serverTime.EnableWindow();
		m_com_regAttchQueID.EnableWindow(FALSE);
		m_listctrl_level.EnableWindow();
		m_ed_levName.EnableWindow();
		m_ed_levID.EnableWindow();
		pButton = (CButton*)GetDlgItem(IDC_BN_ADDLEV);
		pButton->EnableWindow();
		pButton = (CButton*)GetDlgItem(IDC_BN_DELLEV);
		pButton->EnableWindow();
		m_com_levToQueID.EnableWindow();
	}
	else if(m_check_connect.GetCheck()==BST_UNCHECKED)
	{
		m_ed_cardID.EnableWindow();
		m_ed_cardName.EnableWindow();
		m_ed_config1.EnableWindow();
		m_ed_config2.EnableWindow();
		m_ed_postion1.EnableWindow();
		m_ed_postion2.EnableWindow();
		m_ed_config3.EnableWindow();
		m_ed_config4.EnableWindow();
		m_ed_postion3.EnableWindow();
		m_ed_postion4.EnableWindow();
		m_com_attchQueID.EnableWindow();
		m_listbox_card.EnableWindow();
		CButton* pButton = (CButton*)GetDlgItem(IDC_BN_ADDCARD);
		pButton->EnableWindow();
		pButton = (CButton*)GetDlgItem(IDC_BN_DELCARD);
		pButton->EnableWindow();
		////////////////////////////////////////
		m_ed_serverip.EnableWindow(FALSE);
		m_ed_serverPort.EnableWindow(FALSE);
		m_ed_serverTime.EnableWindow(FALSE);
		m_listctrl_level.EnableWindow(FALSE);
		m_ed_levName.EnableWindow(FALSE);
		m_ed_levID.EnableWindow(FALSE);
		pButton = (CButton*)GetDlgItem(IDC_BN_ADDLEV);
		pButton->EnableWindow(FALSE);
		pButton = (CButton*)GetDlgItem(IDC_BN_DELLEV);
		pButton->EnableWindow(FALSE);
		m_com_regAttchQueID.EnableWindow();
		m_com_levToQueID.EnableWindow(FALSE);
	}
}

BOOL CCardConfigQue::WriteConnectInfoToFile()
{
	GetCardConnectInfo();
	CFile file;
	CFileException e;
	if (file.Open(m_connect_path,CFile::modeCreate|CFile::modeWrite,&e))
	{
		file.Write(&m_cardConnectInfo,sizeof(CARDCONNECTINFO));
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CCardConfigQue::ReadConnectInfoFromFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_connect_path,CFile::modeRead,&e))
	{
		file.Read(&m_cardConnectInfo,sizeof(CARDCONNECTINFO));
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void CCardConfigQue::GetCardConnectInfo()
{
//	memset(&m_cardConnectInfo,0,sizeof(m_cardConnectInfo));
	if(m_check_connect.GetCheck() == BST_CHECKED)
	{
		m_cardConnectInfo.IsConnect = TRUE;
	}
	else
	{
		m_cardConnectInfo.IsConnect = FALSE;
	}
	CString serverIP;
	m_ed_serverip.GetWindowText(serverIP);
	_tcsncpy_s(m_cardConnectInfo.ServerIP, addNum, serverIP,serverIP.GetLength());
	CString serverPort;
	m_ed_serverPort.GetWindowText(serverPort);
	int nPort=0;
	m_convert.CStringToint(nPort,serverPort);
	m_cardConnectInfo.ServerPort = nPort;
	
	CString overTime;
	m_ed_serverTime.GetWindowText(overTime);
	int nTime=0;
	m_convert.CStringToint(nTime,overTime);
	m_cardConnectInfo.OverTime = nTime;
	////////////////////////////////
	int index = m_com_regAttchQueID.GetCurSel();
	if(index != CB_ERR)
	{
		CQueueInfo queInfo;
		if(m_map_que.Lookup(index,queInfo))
		{
			CString queID = queInfo.GetQueID();
			_tcsncpy_s(m_cardConnectInfo.RegAttchQueID, addNum, queID,queID.GetLength());
			m_cardConnectInfo.RegAttchPageID = -1;
		}
		map<int,UINT>::const_iterator itera = m_map_pageID.find(index);
		if(itera!=m_map_pageID.end())
		{
			m_cardConnectInfo.RegAttchPageID = itera->second;
			memset(m_cardConnectInfo.RegAttchQueID,0,addNum);//置空
		}
	}
}

void CCardConfigQue::ClearSwingCardInfo()
{
	for(int i=0;i<m_list_swingCard.GetCount();i++)
	{
		delete m_list_swingCard[i];
		m_list_swingCard[i] = NULL;
	}
	m_list_swingCard.RemoveAll();
}

void CCardConfigQue::ClearCardLevInfo()
{
	for(int i=0;i<m_list_cardLev.GetCount();i++)
	{
		delete m_list_cardLev[i];
		m_list_cardLev[i]=NULL;
	}
	m_list_cardLev.RemoveAll();
}
void CCardConfigQue::OnBnClickedBnAddlev()
{
	// TODO: 在此添加控件通知处理程序代码
	CCardLevel* pCardLev = new CCardLevel;
	ASSERT(pCardLev);
	UINT serialID = pCardLev->GetCardLecSerialID();
	CString cardLevName;
	cardLevName.Format(_T("卡等级%d"),serialID);
	pCardLev->SetCardLevelName(cardLevName);
	m_list_cardLev.Add(pCardLev);//添加到数组

	/////////////////////////////////界面显示
	CString strSerialID;
	strSerialID.Format(_T("%d"),pCardLev->GetCardLecSerialID());
	int count = m_listctrl_level.GetItemCount();
	m_listctrl_level.InsertItem(count,strSerialID);
	int nItem = 1;
	m_listctrl_level.SetItemText(count,nItem++,pCardLev->GetCardLevelName());//卡等级
	m_listctrl_level.SetItemState(count, LVIS_SELECTED, LVIS_SELECTED);//让最后一项被选中
}

void CCardConfigQue::OnBnClickedBnDellev()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos=m_listctrl_level.GetFirstSelectedItemPosition();
	int index=m_listctrl_level.GetNextSelectedItem(pos);

	if(index<0)
	{
		AfxMessageBox(_T("请选中卡等级"));
		return;
	}
	int seralID = 0;
	CString strSerialID = m_listctrl_level.GetItemText(m_currSelectItem,0);
	CCommonConvert convert;
	convert.CStringToint(seralID,strSerialID);
	int count = m_list_cardLev.GetCount();
	for(int i=0;i<count;i++)
	{
		if(seralID == m_list_cardLev[i]->GetCardLecSerialID())
		{
			delete m_list_cardLev[i];
			m_list_cardLev[i] = NULL;
			m_list_cardLev.RemoveAt(i);//从数组中删除
			break;
		}
	}
	//界面
	m_listctrl_level.DeleteItem(m_currSelectItem);//界面删除
	//删到顶端时把焦点设置在最上边
	if(index>0)
	{
		m_listctrl_level.SetItemState(index-1,LVIS_SELECTED,LVIS_SELECTED);
	}
	else
	{
		m_listctrl_level.SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);
	}
}

void CCardConfigQue::OnLvnItemchangedListLevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_currSelectItem = pNMLV->iItem;
	CString levName=m_listctrl_level.GetItemText(m_currSelectItem,1);
	m_cs_levName = levName;
	CString levID = m_listctrl_level.GetItemText(m_currSelectItem,2);
	m_cs_levID = levID;
	UpdateData(FALSE);
	////////////////////////////////////
	CString queID;//关联的队列ID
	int nPageID=-1;//关联的界面ID
	int seralID = 0;
	CString strSerialID = m_listctrl_level.GetItemText(m_currSelectItem,0);
	CCommonConvert convert;
	convert.CStringToint(seralID,strSerialID);
	int count = m_list_cardLev.GetCount();
	for(int i=0;i<count;i++)
	{
		if(seralID == m_list_cardLev[i]->GetCardLecSerialID())
		{
			queID = m_list_cardLev[i]->GetCardLevAttchToQue();
			nPageID = m_list_cardLev[i]->GetCardLevAttchToPageID();
			break;
		}
	}
	/////////////////////
	BOOL flag = FALSE;
	if(!queID.IsEmpty())
	{
		for(int i=0;i<m_com_levToQueID.GetCount();i++)
		{
			CQueueInfo queInfo;
			if(m_map_que.Lookup(i,queInfo))
			{
				if(queInfo.GetQueID()==queID)
				{
					flag = TRUE;
					m_com_levToQueID.SetCurSel(i);
					break;
				}
			}
		}
	}
	if(nPageID!=-1)
	{
		for(int i=0;i<m_com_levToQueID.GetCount();i++)
		{
			map<int,UINT>::const_iterator itera = m_map_pageID.find(i);
			if(itera != m_map_pageID.end())
			{
				if(nPageID == (int)itera->second)
				{
					flag = TRUE;
					m_com_levToQueID.SetCurSel(i);
					break;
				}
			}
		}
	}
	if(!flag)
	{
		m_com_levToQueID.SetCurSel(CB_ERR);
	}
	*pResult = 0;
}

void CCardConfigQue::OnEnChangeEdLevelname()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if(m_currSelectItem<0)return;
	CString strLevName;
	m_ed_levName.GetWindowText(strLevName);
	if(strLevName.IsEmpty())return;
	m_listctrl_level.SetItemText(m_currSelectItem,1,strLevName);
	///////////////////////////////////修改内存
	int seralID = 0;
	CString strSerialID = m_listctrl_level.GetItemText(m_currSelectItem,0);
	CCommonConvert convert;
	convert.CStringToint(seralID,strSerialID);
	int count = m_list_cardLev.GetCount();
	for(int i=0;i<count;i++)
	{
		if(seralID == m_list_cardLev[i]->GetCardLecSerialID())
		{
			m_list_cardLev[i]->SetCardLevelName(strLevName);
			break;
		}
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CCardConfigQue::OnEnChangeEdTolevel()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if(m_currSelectItem<0)return;
	CString strLevID;
	m_ed_levID.GetWindowText(strLevID);
	if(strLevID.IsEmpty())return;
	///////////////////////////////////修改内存
	int i_levID = -1;
	int seralID = 0;
	CString strSerialID = m_listctrl_level.GetItemText(m_currSelectItem,0);
	CCommonConvert convert;
	convert.CStringToint(seralID,strSerialID);//转换序号
	convert.CStringToint(i_levID,strLevID);//转换等级
	int count = m_list_cardLev.GetCount();
	for(int i=0;i<count;i++)
	{
		if(i_levID == m_list_cardLev[i]->GetCardLevel()
			&& seralID != m_list_cardLev[i]->GetCardLecSerialID())//重复等级
		{
			MessageBox(_T("等级重复,请重新填写!"),_T("注意"),MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	for(int i=0;i<count;i++)
	{
		if(seralID == m_list_cardLev[i]->GetCardLecSerialID())
		{
			m_list_cardLev[i]->SetCardLevel(i_levID);
			break;
		}
	}
	m_listctrl_level.SetItemText(m_currSelectItem,2,strLevID);
	// TODO:  在此添加控件通知处理程序代码
}

void CCardConfigQue::OnCbnSelchangeComLevtoqueid()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_com_levToQueID.GetCurSel();
	if(index==CB_ERR)return;
	if(m_currSelectItem<0)return;
	CString strQueName;
	m_com_levToQueID.GetLBText(index,strQueName);
	m_listctrl_level.SetItemText(m_currSelectItem,3,strQueName);
	///////////////////////////////////修改内存
	int seralID = 0;
	CString strSerialID = m_listctrl_level.GetItemText(m_currSelectItem,0);
	CCommonConvert convert;
	convert.CStringToint(seralID,strSerialID);//转换序号
	int count = m_list_cardLev.GetCount();
	for(int i=0;i<count;i++)
	{
		if(seralID == m_list_cardLev[i]->GetCardLecSerialID())
		{
			CQueueInfo queInfo;
			if(m_map_que.Lookup(index,queInfo))
			{
				m_list_cardLev[i]->SetCardLevAttchToQue(queInfo.GetQueID());
				m_list_cardLev[i]->SetCardLevAttchToPageID(-1);
			}
			map<int,UINT>::const_iterator itera = m_map_pageID.find(index);
			if(itera!=m_map_pageID.end())
			{
				m_list_cardLev[i]->SetCardLevAttchToPageID(itera->second);
				m_list_cardLev[i]->SetCardLevAttchToQue(_T(""));
			}
			break;
		}
	}
}


BOOL CCardConfigQue::WriteCardLevelToFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_cardLev_path,CFile::modeCreate|CFile::modeWrite,&e))
	{
		int count=m_list_cardLev.GetCount();
		for (int i=0;i<count;i++)
		{
			CArchive ar(&file,CArchive::store);
			ar<<m_list_cardLev[i];
			ar.Close();
		}
		file.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCardConfigQue::ReadCardLevelFromFile()
{
	ClearCardLevInfo();//清空卡等级信息
	CFile file;
	CFileException e;
	if (file.Open(m_cardLev_path,CFile::modeRead,&e))
	{
		////注意序列化出来的指针变量已经分配了内存
		CCardLevel* pCardLevel = NULL;
		UINT cardLevelSerialID = 0;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
		{
			do
			{
				ar>>pCardLevel;
				if (pCardLevel)
				{
					//界面显示
					CString strSerialID;
					CString strCardLevID;
					strSerialID.Format(_T("%d"),pCardLevel->GetCardLecSerialID());
					strCardLevID.Format(_T("%d"),pCardLevel->GetCardLevel());
					///////////////////////////
					int count = m_listctrl_level.GetItemCount();
					m_listctrl_level.InsertItem(count,strSerialID);
					int nItem = 1;
					m_listctrl_level.SetItemText(count,nItem++,pCardLevel->GetCardLevelName());//卡等级名称
					m_listctrl_level.SetItemText(count,nItem++,strCardLevID);
					for(int i=0;i<m_com_levToQueID.GetCount();i++)
					{
						CQueueInfo queInfo;
						if(m_map_que.Lookup(i,queInfo))
						{
							if(m_map_que[i].GetQueID() == pCardLevel->GetCardLevAttchToQue())
							{
								m_listctrl_level.SetItemText(count,nItem++,m_map_que[i].GetBussName());
								break;
							}
						}
						map<int,UINT>::const_iterator itera = m_map_pageID.find(i);
						if(itera!=m_map_pageID.end())
						{
							if(pCardLevel->GetCardLevAttchToPageID() == (int)itera->second)
							{
								CString pageName;
								if(itera->second==0)
									pageName=_T("主页面");
								else
									pageName.Format(_T("页面%d"),itera->second);
								m_listctrl_level.SetItemText(count,nItem++,pageName);
								break;
							}
						}
					}
					m_list_cardLev.Add(pCardLevel);
					cardLevelSerialID = pCardLevel->GetCardLecSerialID() > cardLevelSerialID
						? pCardLevel->GetCardLecSerialID() : cardLevelSerialID;
				}
			}while(!ar.IsBufferEmpty());
		}
		ar.Close();
		file.Close();
		g_cardLevelSerialID = cardLevelSerialID;
	}
	else return FALSE;
	return TRUE;
}
void CCardConfigQue::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rect;
	m_sta_bankPic.GetWindowRect(rect);
	ScreenToClient(rect);
	SetStretchBltMode(dc, COLORONCOLOR);//避免颜色失真
	if(!m_picImage.IsNull())
		m_picImage.StretchBlt(dc,rect.left,rect.top,rect.Width(),rect.Height(),SRCCOPY);
}

void CCardConfigQue::AddPageIDInfo()
{
	list<CTrackContrl*>::const_iterator itera = theApp.m_pView->m_list_trackCtrl.begin();
	for(itera;itera!=theApp.m_pView->m_list_trackCtrl.end();itera++)
	{
		CTrackContrl* pTrackContrl = *itera;
		CString pageName;
		int nPageID = pTrackContrl->GetSerialID();
		if(nPageID==0)
			pageName=_T("主页面");
		else
			pageName.Format(_T("页面%d"),pTrackContrl->GetSerialID());
		m_map_pageID[m_com_attchQueID.AddString(pageName)]=pTrackContrl->GetSerialID();
		m_com_regAttchQueID.AddString(pageName);
		m_com_levToQueID.AddString(pageName);
	}
}