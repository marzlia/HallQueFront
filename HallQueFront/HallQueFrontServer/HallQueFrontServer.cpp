// HallQueFrontServer.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "HallQueFrontServer.h"
#include "HallQueFrontServerDlg.h"
#include "..\HallQueFront\DoFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW

// #pragma comment(lib,"C:/Program Files/Visual Leak Detector/lib/Win32/vld.lib")
// #include "C:\Program Files\Visual Leak Detector\include\vld.h"

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


// CHallQueFrontServerApp ����

CHallQueFrontServerApp::CHallQueFrontServerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHallQueFrontServerApp ����

CHallQueFrontServerApp theApp;


// CHallQueFrontServerApp ��ʼ��

BOOL CHallQueFrontServerApp::InitInstance()
{
#ifdef _DEBUG
	::AllocConsole();
	g_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
//	AddAutoRun(_T("HallQueFrontServer"));
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CHallQueFrontServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
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