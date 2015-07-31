// HallQueFront.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"

#include "HallQueFront.h"
#include "MainFrm.h"
#include "HallQueFrontDoc.h"
#include "HallQueFrontView.h"
#include "PrintSetDlg.h"
#include "CommonConvert.h"
#include "CommonStrMethod.h"
#include "LicenseMaker.h"

#pragma comment(lib, "QueSystemLicense.lib")

#ifdef _DEBUG
#define new DEBUG_NEW

//#pragma comment(lib,"C:/Program Files/Visual Leak Detector/lib/Win32/vld.lib")
//#include "C:\Program Files\Visual Leak Detector\include\vld.h"

HANDLE g_hStdOutput;
void MyWriteConsole(CString str)
{
	::WriteConsole(g_hStdOutput, str, str.GetLength(), NULL, NULL);
	::WriteConsole(g_hStdOutput, "\n", 1, NULL, NULL);
}
#endif


// CHallQueFrontApp

BEGIN_MESSAGE_MAP(CHallQueFrontApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CHallQueFrontApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CHallQueFrontApp ����

CHallQueFrontApp::CHallQueFrontApp() : m_pView(NULL)
,m_pComInit(NULL)
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	memset(&m_logicVariables,0,sizeof(m_logicVariables));
}


// Ψһ��һ�� CHallQueFrontApp ����

CHallQueFrontApp theApp;


// CHallQueFrontApp ��ʼ��

BOOL CHallQueFrontApp::InitInstance()
{
#ifdef _DEBUG
	::AllocConsole();
	g_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	HWND hWnd = ::FindWindow(NULL, L"�����Ŷӽк�ϵͳ");
	if(hWnd)
	{
		return FALSE;
	}
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

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������

	///////////////////////////////////////////////////////////////////////////
	////////////////////	���ע����Ϣ	///////////////////////////////////
	if(!VerifyLicense())
	{
		STARTUPINFO si = { sizeof(si) };   
		PROCESS_INFORMATION pi;
		CDoFile doFile;
		CString strExePath = doFile.GetExeFullFilePath();
		strExePath += _T("\\QueSystemRegister.exe");
		BOOL flag = ::CreateProcess(strExePath,NULL,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);
		if(flag)
		{
			WaitForSingleObject(pi.hThread,INFINITE);
		}	
	}
	if(!VerifyLicense())
	{
		return FALSE;
	}
	//////////////////		End ���ע����Ϣ	///////////////////////////////
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CHallQueFrontDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CHallQueFrontView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	
	//�ó��򿪻�����
//	AddAutoRun(_T("HallQueFront"));
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->SetWindowText(_T("�����Ŷӽк�ϵͳ"));
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	////////////////////////////////////////////
	
	///////////////////////////////////////////
	AfxInitRichEdit();
	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CHallQueFrontApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
// CHallQueFrontApp ��Ϣ�������

BOOL CHallQueFrontApp::ReadLogicVariablesFromFile()
{
	CString strExePath = CommonStrMethod::GetModuleDir();
	strExePath+=_T("\\SysLogicVaribiles.dat");
	CFile logicFile;
	if(logicFile.Open(strExePath,CFile::modeRead))
	{
		logicFile.Read(&m_logicVariables,sizeof(m_logicVariables));
		logicFile.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CHallQueFrontApp::AddAutoRun(CString strValueName)
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
	strPath+=_T("\\HallQueFront.exe");
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



BOOL CHallQueFrontApp::IsLocal()
{
	if(!theApp.m_logicVariables.IsOpenInterNum)
	{
		return TRUE;
	}
	else
	{
		if(theApp.m_logicVariables.strInterIP[0] == '\0')
		{
			return TRUE;
		}
	}
	return FALSE;
}