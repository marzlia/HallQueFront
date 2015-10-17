// HallQueFrontView.h : CHallQueFrontView ��Ľӿ�
//


#pragma once
#include "TrackContrl.h"
#include "TrackDef.h"
#include "SLZController.h"
#include <afxmt.h>
#include "WaringDlg.h"
#include "ShowPageDlg.h"
#include <list>
using namespace std;

#define  FORSHOWTIME 9568
#define  WORKTIME 9578
#define  DOINITThROUGHSCREEN 9579
#define  BUTTONNUM 200


class CHallQueFrontView : public CView
{
protected: // �������л�����
	CHallQueFrontView();
	DECLARE_DYNCREATE(CHallQueFrontView)

// ����
public:
	CHallQueFrontDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CHallQueFrontView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnQuitmanage();
private:
	//�жϿؼ��Ƿ��ڱ༭״̬
	BOOL m_isManage;
public:
	afx_msg void OnManage();
	afx_msg void OnAddbutton();
	afx_msg void OnAddtext();
	afx_msg void OnAddpic();
	afx_msg void OnQuithallque();
	afx_msg void OnQuitsys();
	///trackContrl������ؼ����ƶ�����С�Ȳ���
	list<CTrackContrl*> m_list_trackCtrl;//�༶���洦��
	CTrackContrl* m_pTrackCtrl;//��ʾ�ĵ�ǰҳ��
	//////////////////////////////////////////////////
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnErasebackpic();
public:
//	CString m_backPicpath;
	CImage m_backImage;
	CRect m_clientRect;
	//SLZController control;
private:
	CString m_picType;
	CString m_strPicFilter;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	///������
	void MyDraw(CDC* pDC);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//�༭�ؼ�
	afx_msg void OnEditctr();
	//�ÿؼ����ӵ����幦��
	afx_msg void OnLinkctr();
	//ɾ��һ���ؼ�
	afx_msg void OnDeletectr();
	//���ر���ͼƬ
	BOOL MyLoadBackImage(CString path);
	BOOL MyLoadBackImage(UINT nPageID);

	//�жϿؼ��ĸ���״̬
	BOOL m_isEdit;//�ж��Ƿ�༭���пؼ�������ӿؼ�
	BOOL m_isAddButton;//��Ӱ�ť
	BOOL m_isAddText;//����ı�
	BOOL m_isAddPic;//���ͼƬ
	BOOL m_isShowTime;//��ʾʱ��
	BOOL m_isShowQueNum;//��ʾ�Ŷ�����
	///����д�ļ��Ľṹ��
	CList<WINDOWCTRINFO,WINDOWCTRINFO&> m_list_allCtrInfo;
	//�ؼ���Ϣд��/��ȡ �ļ�
	BOOL WriteCtrInfoToFile();
	BOOL ReadCtrInfoFromFile();
	virtual void OnInitialUpdate();
	//�ѱ���ͼƬ��·��д��\�����ļ�
//	BOOL WriteBackPicPathToFile();
//	BOOL ReadBackPicPathFromFile();
	//���ļ��������ݺ󣬴������еĿؼ�
	void CreateAllCtr();
	//���汳��DC
	void GetBackDC();
//	afx_msg void OnSysseting();
	afx_msg void OnShowTime();
	afx_msg void OnShowQuenum();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//��Ӧ��ť
	afx_msg void OnButtonClick(UINT uID);
public:
	//////////////////////////////////////
	//����ӿ�
	BOOL HasData();
	CString GetData();
	void ShowWaitNum(const CString& queID,const int waitNum );//��ʾ�ȴ�����
	//////////////////////////////////////
private:
	CList<CString> m_list_address;
	CMutex m_Mlock;
	const CString m_workTimeOut;
	void WriteCtrlDataToList();
public:
	afx_msg void OnQueset();
	afx_msg void OnStaffset();
	afx_msg void OnWindowset();
	afx_msg void OnPrintset();
	afx_msg void OnCardset();
	afx_msg void OnComprehset();
private:
	void JudgeButtonWorkOut();
	CWaringDlg* m_pWaringDlg;
	CRect m_rWaringRect;
	CRect m_rShowPageRect;
	///д�м������浽�ļ�
	CString m_strPageFilePath;
	BOOL WritePageToFile();
	BOOL ReadPageFromFile();
	//��������ڴ�
	void ClearPage();
public:
//	afx_msg void OnStbset();
	afx_msg LRESULT OnMyShowMessage(WPARAM wParam, LPARAM lParam);//���ܽ�����ʾȱֽ��Ϣ����
	afx_msg LRESULT OnMyShowPage(WPARAM wParam,LPARAM lParam);//���ܽ����ػ���Ϣ,����slzcontrol�̵߳���
	ShowVariables m_showVaria;
	afx_msg void OnViewminsize();
public:
	CShowPageDlg* m_pShowPageDlg;
	void ShowPage(int nPageID);
	afx_msg void OnLeadinwav();
	afx_msg void OnHidemain();
};

#ifndef _DEBUG  // HallQueFrontView.cpp �еĵ��԰汾
inline CHallQueFrontDoc* CHallQueFrontView::GetDocument() const
   { return reinterpret_cast<CHallQueFrontDoc*>(m_pDocument); }
#endif