
// ZMShow.cpp: 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ZMShow.h"
#include "ZMShowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CZMShowApp

BEGIN_MESSAGE_MAP(CZMShowApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CZMShowApp 构造

CZMShowApp::CZMShowApp()
	: gdiplusToken(0)
	, m_hMutex(NULL)
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CZMShowApp 对象

CZMShowApp theApp;
CWnd * g_sz_p_wnd[10] = { NULL };
COLORREF g_crBlack = RGB(0, 0, 0);
COLORREF g_crWhite = RGB(255, 255, 255);
COLORREF g_crBlue = RGB(19, 190, 236);
COLORREF g_crGrey11 = RGB(11, 11, 11);
COLORREF g_crGrey55 = RGB(55, 55, 55);
COLORREF g_crGrey100 = RGB(100, 100, 100);
COLORREF g_crGrey200 = RGB(200, 200, 200);
COLORREF g_crGrey230 = RGB(230, 230, 230);
COLORREF g_crRed = RGB(255, 0, 0);
COLORREF g_crTransparent = RGB(1, 0, 1);

bool g_bIsTimeInfoTotal = true;
bool g_bIsMute = false;
bool g_bIsFocusCombo = false;
bool g_bIsOnSetting = false;
bool g_bIsDeskLrc = false;
std::vector<CString> g_vec_strHotKey;

// CZMShowApp 初始化

BOOL CZMShowApp::InitInstance()
{
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	m_hMutex = CreateMutex(NULL, TRUE, _T("ZMShow-Mutex"));//控制唯一实例
	if (m_hMutex == NULL)
	{
		return FALSE;
	}
	else
	{
		if (ERROR_ALREADY_EXISTS == GetLastError())
		{
			HWND hWndZMS = ::FindWindow(_T("CZMShowMainWnd"), NULL);
			if (hWndZMS)
			{
				COPYDATASTRUCT copyData = { 0 };
				copyData.lpData = (void *)m_lpCmdLine;
				copyData.cbData = (_tcslen(m_lpCmdLine) + 1) * sizeof(TCHAR);
				::SendMessage(hWndZMS, WM_COPYDATA, 0, (LPARAM)&copyData);
			}
			return FALSE;
		}
	}
	WNDCLASS wndMyClass;
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wndMyClass);
	wndMyClass.lpszClassName = _T("CZMShowMainWnd");//需同步修改rc文件中窗口类名
	AfxRegisterClass(&wndMyClass);
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CZMShowDlg dlg;
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
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}



int CZMShowApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	Gdiplus::GdiplusShutdown(gdiplusToken);
	if (m_hMutex)
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
	return CWinApp::ExitInstance();
}
