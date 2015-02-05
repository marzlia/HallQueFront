// PropEditing.cpp : 实现文件
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "PropEditing.h"



// CPropEditing
#ifdef _DEBUG
extern void MyWriteConsole(CString str);
#endif

IMPLEMENT_DYNAMIC(CPropEditing, CPropertySheet)

CPropEditing::CPropEditing(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	this->m_psh.dwFlags |= PSH_NOAPPLYNOW;
	this->m_psh.dwFlags &= ~PSP_HASHELP;
	m_propEdButton.m_psp.dwFlags &= ~PSP_HASHELP;
	m_propEdText.m_psp.dwFlags &= ~PSP_HASHELP;
	m_propEdPic.m_psp.dwFlags &= ~PSP_HASHELP;
	m_propShowTime.m_psp.dwFlags&= ~PSP_HASHELP;
	m_propShowQueNum.m_psp.dwFlags&= ~PSP_HASHELP;
	m_pView = (CHallQueFrontView*)pParentWnd;

	if(m_pView->m_isEdit)
	{
		switch(m_pView->m_pTrackCtrl->m_pRightBnSelect->GetWindowType())
		{
		case enmButton:
			AddPage(&m_propEdButton);
			break;
		case enmStatic:
			if(!m_pView->m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->IsForImage() && m_pView->
				m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->GetIsShowTime())
			{
				AddPage(&m_propShowTime);
			}
			if(!m_pView->m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->IsForImage() && m_pView->
				m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->GetIsShowQueNum())
			{
				AddPage(&m_propShowQueNum);
			}
			if(!m_pView->m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->IsForImage() && !m_pView->
				m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->
				GetIsShowQueNum() && !m_pView->m_pTrackCtrl->
				m_pRightBnSelect->m_pTransStatic->GetIsShowTime())
			{
				AddPage(&m_propEdText);
			}
			if(m_pView->m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->IsForImage())
			{
				AddPage(&m_propEdPic);
			}
			break;
		}
	}
	if(m_pView->m_isAddButton)
	{
		AddPage(&m_propEdButton);
	}
	if(m_pView->m_isAddText)
	{
		AddPage(&m_propEdText);
	}
	if(m_pView->m_isAddPic)
	{
		AddPage(&m_propEdPic);
	}
	if(m_pView->m_isShowTime)
	{
		AddPage(&m_propShowTime);
	}
	if(m_pView->m_isShowQueNum)
	{
		AddPage(&m_propShowQueNum);
	}
}

CPropEditing::CPropEditing(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	this->m_psh.dwFlags |= PSH_NOAPPLYNOW;
	this->m_psh.dwFlags &= ~PSP_HASHELP;
	
	m_propEdButton.m_psp.dwFlags &= ~PSP_HASHELP;
	m_propEdText.m_psp.dwFlags &= ~PSP_HASHELP;
	m_propEdPic.m_psp.dwFlags &= ~PSP_HASHELP;
	m_propShowTime.m_psp.dwFlags&= ~PSP_HASHELP;
	m_propShowQueNum.m_psp.dwFlags&= ~PSP_HASHELP;
	m_pView = (CHallQueFrontView*)pParentWnd;

	if(m_pView->m_isEdit)
	{
		switch(m_pView->m_pTrackCtrl->m_pRightBnSelect->GetWindowType())
		{
		case enmButton:
			AddPage(&m_propEdButton);
			break;
		case enmStatic:
			if(!m_pView->m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->IsForImage() && m_pView->
				m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->GetIsShowTime())
			{
				AddPage(&m_propShowTime);
			}
			if(!m_pView->m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->IsForImage() && m_pView->
				m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->GetIsShowQueNum())
			{
				AddPage(&m_propShowQueNum);
			}
			if(!m_pView->m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->IsForImage() && !m_pView->
				m_pTrackCtrl->m_pRightBnSelect->m_pTransStatic->
				GetIsShowQueNum() && !m_pView->m_pTrackCtrl->
				m_pRightBnSelect->m_pTransStatic->GetIsShowTime())
			{
				AddPage(&m_propEdText);
			}
			if(m_pView->m_pTrackCtrl->m_pRightBnSelect->
				m_pTransStatic->IsForImage())
			{
				AddPage(&m_propEdPic);
			}
			break;
		}
	}
	if(m_pView->m_isAddButton)
	{
		AddPage(&m_propEdButton);
	}
	if(m_pView->m_isAddText)
	{
		AddPage(&m_propEdText);
	}
	if(m_pView->m_isAddPic)
	{
		AddPage(&m_propEdPic);
	}
	if(m_pView->m_isShowTime)
	{
		AddPage(&m_propShowTime);
	}
	if(m_pView->m_isShowQueNum)
	{
		AddPage(&m_propShowQueNum);
	}
}

CPropEditing::~CPropEditing()
{
}


BEGIN_MESSAGE_MAP(CPropEditing, CPropertySheet)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPropEditing 消息处理程序

BOOL CPropEditing::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  在此添加您的专用代码
	return bResult;
}

BOOL CPropEditing::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CPropertySheet::OnEraseBkgnd(pDC);
}
