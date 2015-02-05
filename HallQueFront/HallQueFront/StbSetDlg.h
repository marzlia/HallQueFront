#pragma once


// CStbSetDlg 对话框

class CStbSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CStbSetDlg)

public:
	CStbSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStbSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_STBSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	
};
