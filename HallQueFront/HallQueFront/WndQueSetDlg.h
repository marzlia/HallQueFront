#pragma once
#include "CommonConvert.h"
#include "ListBoxPlus.h"
#include "afxwin.h"
// CWndQueSetDlg �Ի���
//�ɴ�����жԻ���
class CWndQueSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CWndQueSetDlg)

public:
	CWndQueSetDlg(CWnd* pParent );   // ��׼���캯��
	virtual ~CWndQueSetDlg();

// �Ի�������
	enum { IDD = IDD_WNDQUEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list_undstque;
	CListBox m_list_ableque;
	CMap<int,int,CQueueInfo,CQueueInfo&> m_map_UndstQueInfo;
	CList<CQueueInfo,CQueueInfo&> m_list_UndstQueInfo;
	CList<CQueueInfo,CQueueInfo&> m_list_AbleQueInfo;
	CStringArray m_AbleQueId;			//�ɴ������ID ����
	CString m_AbleQueName;			//�ɴ����������
	BOOL m_bMakeAll;		//�Ƿ�Ӧ�õ����д���
	BOOL m_bIsUsePower;  //�Ƿ�ʹ�����ȼ�
private:
	BOOL ReadQueInfoFromFile();
	BOOL ReadWndQueInfo();
	CString m_Queinfo_path;
	CCommonConvert convert;
	void ArrayOrder(CArray<int,int>& iArray);		//��������
	CButton m_check_makeall;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAddque();
	afx_msg void OnBnClickedBtnDelque();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
	afx_msg void OnBnClickedCheckMkall();
	CButton m_check_usepower;
};
