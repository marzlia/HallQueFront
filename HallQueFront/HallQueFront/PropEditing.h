#pragma once
#include "PropEdButton.h"
#include "PropEdText.h"
#include "HallQueFrontDoc.h"
#include "HallQueFrontView.h"
#include "PropEdPic.h"
#include "PropShowTime.h"
#include "PropShowQueNum.h"

// CPropEditing

class CPropEditing : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropEditing)

public:
	CPropEditing(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropEditing(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPropEditing();
	
	CPropEdButton m_propEdButton;
	CPropEdText   m_propEdText;
	CPropEdPic    m_propEdPic;
	CPropShowTime m_propShowTime;
	CPropShowQueNum m_propShowQueNum;
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CHallQueFrontView* m_pView;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


