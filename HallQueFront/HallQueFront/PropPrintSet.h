#pragma once

#include "PrintSetDlg.h"
#include"PropPrintText.h"
#include "PropPrintPic.h"
#include "PropPrintShowTime.h"
// CPropPrintSet

class CPropPrintSet : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropPrintSet)

public:
	CPropPrintSet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropPrintSet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPropPrintSet();
	
	CPropPrintText  m_printText;
	CPropPrintPic m_printPic;
	CPrintSetDlg* m_pPrSetDlg;
	CPropPrintShowTime m_printShowTime;
protected:
	DECLARE_MESSAGE_MAP()
};


