// HallQueFrontServer.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "HallQueFrontServer.h"
#include "HallQueFrontServerDlg.h"
#include "..\HallQueFront\DoFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW

#pragma comment(lib,"C:/Program Files/Visual Leak Detector/lib/Win32/vld.lib")
#include "C:\Program Files\Visual Leak Detector\include\vld.h"

HANDLE g_hStdOutput;
void MyWriteConsole(CString str)
{
	::WriteConsole(g_hStdOutput, str, str.GetLength(), NULL, NULL);
	::WriteConsole(g_hStdOutput, "\n", 1, NULL, NULL);
}
#endif


// CHallQueFrontServerApp

BEGIN_MESSAGE_MAP(CHallQueFrontServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHallQueFrontServerApp 构造

CHallQueFrontServerApp::CHallQueFrontServerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CHallQueFrontServerApp 对象

CHallQueFrontServerApp theApp;


// CHallQueFrontServerApp 初始化

BOOL CHallQueFrontServerApp::InitInstance()
{
#ifdef _DEBUG
	::AllocConsole();
	g_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
//	AddAutoRun(_T("HallQueFrontServer"));
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CHallQueFrontServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

BOOL CHallQueFrontServerApp::AddAutoRun(CString strValueName)
{
	HKEY hKey;
	LONG ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE, &hKey);
	if(ERROR_SUCCESS != ret)
	{

		LPCTSTR lpSubKey = _T("Software\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegCreateKey:HKEY_LOCAL_MACHINE\\Software"));
			return FALSE;
		}
		lpSubKey = _T("Microsoft\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegCreateKey:HKEY_LOCAL_MACHINE\\Software\\Microsoft"));
			return FALSE;
		}
		lpSubKey = _T("Windows\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegCreateKey:HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows"));
			return FALSE;
		}
		lpSubKey = _T("CurrentVersion\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegCreateKey:HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion"));
			return FALSE;
		}
		lpSubKey = _T("Run\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegCreateKey:HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"));
			return FALSE;
		}
	}

	//WCHAR wszModulePath[256] = {0};
	//::GetModuleFileName(NULL, wszModulePath, MAX_PATH);
	//ret = ::RegSetValueEx(hKey, _T("NetServer"), 0, REG_SZ,
	//	(const BYTE*)wszModulePath, wcslen(wszModulePath)*2);
	CDoFile file;
	CString strPath = file.GetExeFullFilePath();
	strPath+=_T("\\HallQueFrontServer.exe");
	// #ifdef _DEBUG
	// 	MyWriteConsole(strPath);
	// #endif
	ret = ::RegSetValueEx(hKey, strValueName, 0, REG_SZ,
		(const BYTE*)strPath.GetBuffer(), strPath.GetLength() * 2);

	if(ERROR_SUCCESS != ret)
	{
		//AfxMessageBox(_T("Error:RegSetValue:Write DevDriver"));
		return FALSE;
	}

	return TRUE;
}