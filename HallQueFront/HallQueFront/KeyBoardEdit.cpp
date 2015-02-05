// KeyBoardEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "KeyBoardEdit.h"
#include "resource.h"
#include "DoFile.h"
#include "HallQueFront.h"
// CKeyBoardEdit
UINT g_clickNum;
extern void MyWriteConsole(CString str);

IMPLEMENT_DYNAMIC(CKeyBoardEdit, CEdit)

CKeyBoardEdit::CKeyBoardEdit()
{
	CDoFile keyboardfile;
	m_strKey = keyboardfile.GetExeFullFilePath();
	m_strKey+=L"\\KeyBoard.exe";
}

CKeyBoardEdit::~CKeyBoardEdit()
{
}


BEGIN_MESSAGE_MAP(CKeyBoardEdit, CEdit)

	ON_CONTROL_REFLECT(EN_KILLFOCUS, &CKeyBoardEdit::OnEnKillfocus)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()



// CKeyBoardEdit 消息处理程序





void CKeyBoardEdit::OnEnKillfocus()
{
	// TODO: 在此添加控件通知处理程序代码
 	
//  ::SendMessage(hwnd,WM_CLOSE,0,0);
	
}

void CKeyBoardEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CEdit::OnLButtonDown(nFlags, point);
	CWnd* pKeypWnd = FindWindow(NULL,L"屏幕键盘");
	if(!pKeypWnd)
	{
		STARTUPINFO si = {sizeof(si)} ;  
		PROCESS_INFORMATION pi ; 
		si.dwFlags = STARTF_USESHOWWINDOW;   
		si.wShowWindow = TRUE; //TRUE表示显示创建的进程的窗口
		TCHAR szCommandLine[] = TEXT("osk.exe") ;  
		BOOL flag = CreateProcess(NULL,szCommandLine,NULL,NULL,  
			FALSE,0,NULL,NULL,&si,&pi) ;
	}
	int x, y; 
	x = GetSystemMetrics(SM_CXSCREEN); //屏幕宽度 
	y = GetSystemMetrics(SM_CYSCREEN);
	pKeypWnd = FindWindow(NULL,L"屏幕键盘");
	if(g_clickNum==0)
	{
		if(pKeypWnd)
		{
			//pKeypWnd->MoveWindow(0,y/2,x/2,y/3,1);
			g_clickNum++;
		}
	}
// 	else
// 	{
// 		if(pKeypWnd)
// 		{
// 			DWORD dStyle = pKeypWnd->GetStyle();
// 			if(dStyle & WS_MINIMIZE)
// 			{
// 				pKeypWnd->ModifyStyle(WS_MINIMIZE,0,0);
// 				pKeypWnd->ShowWindow(SW_SHOWNORMAL);
// 			}
// 		}
// 	}
	//ShellExecute(NULL, L"open", _T("osk.exe"), NULL, NULL,SW_NORMAL);
}
void CKeyBoardEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
// 	g_clickNum--;
// #ifdef _DEBUG
// 	CString str;
// 	str.Format(_T("%d"),g_clickNum);
// 	MyWriteConsole(str);
// #endif
// 	if(!g_clickNum)
// 	{
// 		CWnd* pWnd = FindWindow(NULL,L"屏幕键盘");
// 		if(pWnd)
// 		{
// 			::SendMessage(pWnd->m_hWnd,WM_CLOSE,0,0);
// 		}
// 	}
	// TODO: 在此处添加消息处理程序代码
}

void CKeyBoardEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	
	// TODO: 在此处添加消息处理程序代码
}

void CKeyBoardEdit::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CEdit::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
}
