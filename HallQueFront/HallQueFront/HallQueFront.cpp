// HallQueFront.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CHallQueFrontApp 构造

CHallQueFrontApp::CHallQueFrontApp() : m_pView(NULL)
,m_pComInit(NULL)
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	memset(&m_logicVariables,0,sizeof(m_logicVariables));
}


// 唯一的一个 CHallQueFrontApp 对象

CHallQueFrontApp theApp;


// CHallQueFrontApp 初始化

BOOL CHallQueFrontApp::InitInstance()
{
#ifdef _DEBUG
	::AllocConsole();
	g_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	HWND hWnd = ::FindWindow(NULL, L"大厅排队叫号系统");
	if(hWnd)
	{
		return FALSE;
	}
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

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接

	///////////////////////////////////////////////////////////////////////////
	////////////////////	检测注册信息	///////////////////////////////////
	if(!VerifyLicense())
	{
		AfxMessageBox(_T("该软件未注册，请先运行注册程序进行注册"));
		return FALSE;
	}
	//////////////////		End 检测注册信息	///////////////////////////////
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CHallQueFrontDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CHallQueFrontView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	
	//让程序开机启动
//	AddAutoRun(_T("HallQueFront"));
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->SetWindowText(_T("大厅排队叫号系统"));
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	////////////////////////////////////////////
	
	///////////////////////////////////////////
	AfxInitRichEdit();
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CHallQueFrontApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
// CHallQueFrontApp 消息处理程序

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