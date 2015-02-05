#pragma once
#include "SwingCard.h"
#include "afxwin.h"
#include "QueueInfo.h"
#include "CommonConvert.h"
#include "afxcmn.h"
#include "CardLevel.h"
#include "ListCtrlCl.h"
#include "atlimage.h"
#include <map>
using namespace std;


// CCardConfigQue 对话框

class CCardConfigQue : public CDialog
{
	DECLARE_DYNAMIC(CCardConfigQue)

public:
	CCardConfigQue(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCardConfigQue();

// 对话框数据
	enum { IDD = IDD_DLG_CARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CArray<CSwingCard*,CSwingCard*> m_list_swingCard;//刷卡识别信息数组
	CArray<CCardLevel*,CCardLevel*> m_list_cardLev;//卡的级别信息数组
	afx_msg void OnBnClickedBnAddcard();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBnDelcard();
	CListBox m_listbox_card;
	CComboBox m_com_attchQueID;
	afx_msg void OnLbnSelchangeListboxCard();
	CEdit m_ed_cardName;
	CEdit m_ed_cardID;
	CEdit m_ed_config1;
	CEdit m_ed_postion1;
	CEdit m_ed_config2;
	CEdit m_ed_postion2;
private:
	BOOL WriteConfigCardInfoToFile();
	BOOL ReadConfigCardInfoFromFile();
	BOOL WriteConnectInfoToFile();
	BOOL ReadConnectInfoFromFile();
	CMap<int,int,CQueueInfo,CQueueInfo&> m_map_que;
	map<int,UINT> m_map_pageID;
	BOOL ReadQueInfoFromFile();
	CString m_queinfofile_path;//队列信息文件路径
	CCommonConvert m_convert;
	CString m_cardcofinfo_path;//刷卡信息文件路径
	CString m_connect_path;//对接信息文件路径
	CARDCONNECTINFO m_cardConnectInfo;//对接信息
	void GetCardConnectInfo();
	void ClearSwingCardInfo();//清空刷卡信息
	void ClearCardLevInfo();//清空卡级别信息
	CString m_cardLev_path;//卡的等级信息文件路径
public:
	afx_msg void OnCbnSelchangeComAttchque();
	afx_msg void OnEnChangeEdCardtypename();
	CString m_cs_cardName;
	CString m_cs_cardID;
	CString m_cs_config1;
	CString m_cs_postion1;
	CString m_cs_config2;
	CString m_cs_postion2;
	CString m_cs_config3;
	CString m_cs_postion3;
	CString m_cs_config4;
	CString m_cs_postion4;
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdConfig1();
	afx_msg void OnEnChangeEdPostion1();
	afx_msg void OnEnChangeEdConfig3();
	afx_msg void OnEnChangeEdPostion3();
	afx_msg void OnEnChangeEdConfig2();
	afx_msg void OnEnChangeEdPostion2();
	afx_msg void OnEnChangeEdConfig4();
	afx_msg void OnEnChangeEdPostion4();
	CButton m_check_connect;
	CEdit m_ed_serverip;
	CEdit m_ed_serverPort;
	CEdit m_ed_serverTime;
	afx_msg void OnBnClickedCheckConnect();
	CEdit m_ed_config3;
	CEdit m_ed_postion3;
	CEdit m_ed_config4;
	CEdit m_ed_postion4;
	CComboBox m_com_regAttchQueID;
	CListCtrlCl m_listctrl_level;
	CEdit m_ed_levName;
	CEdit m_ed_levID;
	CComboBox m_com_levToQueID;
	afx_msg void OnBnClickedBnAddlev();
	afx_msg void OnBnClickedBnDellev();
	afx_msg void OnLvnItemchangedListLevel(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int m_currSelectItem;//当前选中的item
	BOOL WriteCardLevelToFile();
	BOOL ReadCardLevelFromFile();
public:
	afx_msg void OnEnChangeEdLevelname();
	afx_msg void OnEnChangeEdTolevel();
	afx_msg void OnCbnSelchangeComLevtoqueid();
	CString m_cs_levName;
	CString m_cs_levID;
	CStatic m_sta_bankPic;
	afx_msg void OnPaint();
private:
	CImage m_picImage;
	void AddPageIDInfo();//添加界面信息到内存
};
