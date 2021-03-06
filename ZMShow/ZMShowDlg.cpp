
// ZMShowDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ZMShow.h"
#include "ZMShowDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CZMShowDlg 对话框

#include "TitleBar.h"
#include "OperateBar.h"
#include "ListBar.h"
#include "Setting.h"
#include "CLyricBar.h"
#include "CToolTipBar.h"

#define ID_MODE_NONEBLACK 2000
#define ID_MODE_DESKLRC 2001

const int nTitleHeight = 50;
const int nOpeBarHeight = 70;
const int nListBarWidth = 180;
const int nToolTipBarHeight = 20;
//Timer
const int nUpdatePro = 0;
const int nIsDblClk = 1;

//static
HHOOK CZMShowDlg::m_hKeyboard = NULL;

CZMShowDlg::CZMShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ZMSHOW_DIALOG, pParent)
	, m_p_LyricBar(NULL)
	, m_p_TitleBar(NULL)
	, m_p_OpeBar(NULL)
	, m_p_ListBar(NULL)
	, m_p_ToolTipBar(NULL)
	, m_p_Setting(NULL)
	, m_nShowLyricBar(SW_HIDE)
	, m_nShowTitleBar(SW_SHOW)
	, m_nShowOpeBar(SW_SHOW)
	, m_nShowListBar(SW_SHOW)
	, m_bIsInitFinish(false)
	, m_strFilePath(_T(""))
	, m_strFolderPath(_T(""))
	, m_nWndState(WND_STATE_NORMAL)
	, m_nLastWndState(WND_STATE_NORMAL)
	, m_nHitTest(0)
	, m_bIsDblClk(false)
	, m_ptDown(0)
	, m_bIsModeMini(false)
	, m_bIsModeFull(false)
	, m_bIsExit(false)
	, m_nTopMostState(TOPMOST_STATE_NO)
	, m_strToolTip(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CZMShowDlg::~CZMShowDlg()
{
	if (m_hKeyboard)
		::UnhookWindowsHookEx(m_hKeyboard);
	SetLastPlayProgress();
	if (m_p_Setting != NULL)
	{
		delete m_p_Setting;
		m_p_Setting = NULL;
	}
	if (m_p_ToolTipBar != NULL)
	{
		delete m_p_ToolTipBar;
		m_p_ToolTipBar = NULL;
	}
	if (m_p_ListBar != NULL)
	{
		delete m_p_ListBar;
		m_p_ListBar = NULL;
	}
	if (m_p_OpeBar != NULL)
	{
		delete m_p_OpeBar;
		m_p_OpeBar = NULL;
	}
	if (m_p_TitleBar != NULL)
	{
		delete m_p_TitleBar;
		m_p_TitleBar = NULL;
	}
	if (m_p_LyricBar != NULL)
	{
		delete m_p_LyricBar;
		m_p_LyricBar = NULL;
	}
}

void CZMShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PLAYWND, m_staticPlayWnd);
}

BEGIN_MESSAGE_MAP(CZMShowDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MSGRECVPRO, &CZMShowDlg::OnMsgrecvpro)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_OPEN_FILE, &CZMShowDlg::OnOpenFile)
	ON_COMMAND(ID_OPEN_FOLDER, &CZMShowDlg::OnOpenFolder)
	ON_COMMAND(ID_SETTING, &CZMShowDlg::OnSetting)
	ON_COMMAND(ID_EXIT, &CZMShowDlg::OnExit)
	ON_COMMAND(ID_MODE_FULL, &CZMShowDlg::OnModeFull)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ITEM_PLAY, &CZMShowDlg::OnItemPlay)
	ON_COMMAND(ID_ITEM_DELETE, &CZMShowDlg::OnItemDelete)
	ON_COMMAND(ID_OPEN_FILECATALOG, &CZMShowDlg::OnOpenFilecatalog)
	ON_COMMAND(ID_REFRESH_CATALOG, &CZMShowDlg::OnRefreshCatalog)
	ON_COMMAND(ID_EMPTY_LIST, &CZMShowDlg::OnEmptyList)
	ON_COMMAND(ID_REFRESH_LIST, &CZMShowDlg::OnRefreshList)
	ON_COMMAND(ID_MODE_MINI, &CZMShowDlg::OnModeMini)
	ON_COMMAND(ID_MODE_NONEBLACK, &CZMShowDlg::OnModeNoneblack)
	ON_COMMAND(ID_MODE_DESKLRC, &CZMShowDlg::OnDeskLrc)
	ON_COMMAND(ID_LISTMODE_LISTSEQ, &CZMShowDlg::OnListmodeListseq)
	ON_COMMAND(ID_LISTMODE_LISTLOOP, &CZMShowDlg::OnListmodeListloop)
	ON_COMMAND(ID_LISTMODE_LISTRAND, &CZMShowDlg::OnListmodeListrand)
	ON_COMMAND(ID_LISTMODE_CATALOGSEQ, &CZMShowDlg::OnListmodeCatalogseq)
	ON_COMMAND(ID_LISTMODE_CATALOGLOOP, &CZMShowDlg::OnListmodeCatalogloop)
	ON_COMMAND(ID_LISTMODE_CATALOGRAND, &CZMShowDlg::OnListmodeCatalogrand)
	ON_COMMAND(ID_LISTMODE_SINGLESEQ, &CZMShowDlg::OnListmodeSingleseq)
	ON_COMMAND(ID_LISTMODE_SINGLELOOP, &CZMShowDlg::OnListmodeSingleloop)
	ON_WM_MOVING()
	ON_WM_COPYDATA()
	ON_MESSAGE(WM_TRAYNOTIFY, &CZMShowDlg::OnTraynotify)
	ON_WM_CLOSE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEHOVER()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_MODE_NOTOPMOST, &CZMShowDlg::OnModeNotopmost)
	ON_COMMAND(ID_MODE_ALWAYSTOPMOST, &CZMShowDlg::OnModeAlwaystopmost)
	ON_COMMAND(ID_MODE_PLAYTOPMOST, &CZMShowDlg::OnModePlaytopmost)
END_MESSAGE_MAP()


// CZMShowDlg 消息处理程序

BOOL CZMShowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	g_sz_p_wnd[WND_ZMS] = this;
	ModifyStyle(0, WS_SYSMENU | WS_MINIMIZEBOX);
	SetWindowText(_T("Zaxai Media Show - ZMS播放器"));
	m_brushBkGnd.CreateSolidBrush(g_crBlack);
	InitLyricBar();
	InitTitleBar();
	InitOperateBar();
	InitListBar();
	InitToolTipBar();
	InitSetting();
	ProBar();
	ProLrcBar();
	InitMenu();
	InitTray();
	m_staticPlayWnd.EnableWindow(FALSE);//vlc会拦截鼠标消息，故禁用绑定窗体，鼠标消息放父窗处理
	m_staticPlayWnd.ModifyStyle(0, WS_CLIPSIBLINGS);
	m_staticPlayWnd.SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);//视频窗口置于z序最底
	ProCmdPath(AfxGetApp()->m_lpCmdLine);//处理命令行，鼠标双击影音文件操作
	m_hKeyboard = ::SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, NULL, ::GetCurrentThreadId());//键盘钩子，实现热键
	DragAcceptFiles(TRUE);//实现文件拖拽
	m_bIsInitFinish = true;
	PostMessage(WM_SIZE, 0, 0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CZMShowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if (nID == SC_MINIMIZE)
			m_p_ToolTipBar->ShowWindow(SW_HIDE);
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CZMShowDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CZMShowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CZMShowDlg::InitLyricBar(void)
{
	m_p_LyricBar = new CLyricBar();
	m_p_LyricBar->Create(IDD_LYRICBAR, this);
}


void CZMShowDlg::InitTitleBar(void)
{
	m_p_TitleBar = new CTitleBar();
	m_p_TitleBar->Create(IDD_TITLEBAR, m_p_LyricBar);
}


void CZMShowDlg::InitOperateBar(void)
{
	m_p_OpeBar = new COperateBar();
	m_p_OpeBar->Create(IDD_OPERATEBAR, m_p_TitleBar);
}


void CZMShowDlg::InitListBar(void)
{
	m_p_ListBar = new CListBar();
	m_p_ListBar->Create(IDD_LISTBAR, m_p_OpeBar);
}


void CZMShowDlg::InitToolTipBar(void)
{
	m_p_ToolTipBar = new CToolTipBar();
	m_p_ToolTipBar->Create(IDD_TOOLTIPBAR, m_p_ListBar);
	//m_p_ToolTipBar->ShowWindow(SW_SHOW);
	m_p_ToolTipBar->ModifyStyleEx(0, WS_EX_LAYERED);
	m_p_ToolTipBar->SetLayeredWindowAttributes(g_crBlack, 180, LWA_COLORKEY);
}


void CZMShowDlg::InitSetting(void)
{
	m_p_Setting = new CSetting();
	m_p_Setting->Create(IDD_SETTING, m_p_ToolTipBar);
	//m_p_Setting->ShowWindow(SW_SHOW);
}


void CZMShowDlg::InitMenu()
{
	m_menuPlay.LoadMenu(IDR_MENU_PLAY);
	CheckTopMostMenu();
}


void CZMShowDlg::InitTray(void)
{
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDR_MAINFRAME;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nid.uCallbackMessage = WM_TRAYNOTIFY;
	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	_tcscpy_s(m_nid.szTip, _tcslen(_T("ZMS播放器")) + 1, _T("ZMS播放器"));
	Shell_NotifyIcon(NIM_ADD, &m_nid);
}


void CZMShowDlg::SetBarPos(void)
{
	CRect rectClient, rectPlay, rectTitle, rectOpe, rectList, rectLyric, rectToolTip;
	GetClientRect(&rectClient);
	rectPlay = rectClient;
	rectClient.DeflateRect(3, 3, 3, 3);
	rectTitle = rectOpe = rectList = rectClient;

	rectTitle.bottom = rectClient.top + nTitleHeight;
	if (rectTitle.bottom>rectClient.bottom)
		rectTitle.bottom = rectClient.bottom;
	if (!m_nShowTitleBar)
		ClientToScreen(&rectTitle);
	m_p_TitleBar->MoveWindow(rectTitle);
	m_p_TitleBar->ShowWindow(m_nShowTitleBar);

	rectOpe.top = rectClient.bottom - nOpeBarHeight;
	if (rectOpe.top<rectClient.top)
		rectOpe.top = rectClient.top;
	if (!m_nShowOpeBar)
		ClientToScreen(&rectOpe);
	m_p_OpeBar->MoveWindow(rectOpe);
	m_p_OpeBar->ShowWindow(m_nShowOpeBar);

	rectList.left = rectClient.right - nListBarWidth;
	if (rectList.left<rectClient.left)
		rectList.left = rectClient.left;
	rectList.top = rectClient.top + nTitleHeight;
	if (rectList.top>rectClient.bottom)
		rectList.top = rectClient.top;
	rectList.bottom = rectClient.bottom - nOpeBarHeight;
	if (rectList.bottom<rectClient.top)
		rectList.bottom = rectClient.bottom;
	if (m_bIsModeMini|| m_bIsModeFull)
		ClientToScreen(&rectList);
	m_p_ListBar->MoveWindow(rectList);
	m_p_ListBar->ShowWindow(m_nShowListBar);

	if (m_nShowTitleBar)
		rectPlay.top = rectTitle.bottom;
	if (m_nShowOpeBar)
		rectPlay.bottom = rectOpe.top;
	if (m_nShowListBar)
	{
		if (m_bIsModeMini || m_bIsModeFull)
			ScreenToClient(&rectList);
		rectPlay.right = rectList.left;
	}
	m_staticPlayWnd.MoveWindow(rectPlay);

	if (!g_bIsDeskLrc)
	{
		rectLyric = rectPlay;
		m_p_LyricBar->MoveWindow(rectLyric);
	}
	m_p_LyricBar->ShowWindow(m_nShowLyricBar);

	rectToolTip = rectPlay;
	if (m_bIsModeMini || m_bIsModeFull)
		rectToolTip.top = rectPlay.top + nTitleHeight;
	if (rectToolTip.Height()>nToolTipBarHeight)
		rectToolTip.bottom = rectToolTip.top + nToolTipBarHeight;
	ClientToScreen(&rectToolTip);
	m_p_ToolTipBar->MoveWindow(rectToolTip);

	BringWindowToTop();
}


void CZMShowDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_bIsInitFinish&&nType != SIZE_MINIMIZED)  //初始化好了后才能变换
	{
		SetBarPos();
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
	}
}


HBRUSH CZMShowDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return m_brushBkGnd;
	return hbr;
}


afx_msg LRESULT CZMShowDlg::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_PLAYBTN:
	{
		int nState = m_player.GetState();
		switch (nState)
		{
		case ZPlayer::STATE_PLAYING:
		{
			m_player.Pause();
			nState = ZPlayer::STATE_PAUSED;
		}
		break;
		case ZPlayer::STATE_PAUSED:
		{
			m_player.Play();
			nState = ZPlayer::STATE_PLAYING;
		}
		break;
		default:
		{
			if (m_player.CreatePlayer(m_strFilePath))
			{
				m_player.SetHWND(m_staticPlayWnd.GetSafeHwnd());
				m_player.Play();
				nState = ZPlayer::STATE_PLAYING;
				GetLastPlayProgress();
				g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETVOLUME);
				FileTypeIsAudioByPlay();
				if (m_nShowLyricBar)
					SetTimer(nUpdatePro, 100, NULL);
				else
					SetTimer(nUpdatePro, 1000, NULL);
				CString strTitle;
				strTitle = m_strFilePath.Mid(m_strFilePath.ReverseFind(_T('\\')) + 1) + _T(" - ZMS");
				SetWindowText(strTitle);
				_tcscpy_s(m_nid.szTip, strTitle.GetLength() + 1, strTitle);
				Shell_NotifyIcon(NIM_MODIFY, &m_nid);
			}
		}
		}
		g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, (WPARAM)nState, MSGUSER_PLAYBTN);
		ProTopMost(nState);
	}
	break;
	case MSGUSER_STOPBTN:
	{
		KillTimer(nUpdatePro);
		FileTypeIsAudioByStop();
		SetLastPlayProgress();
		m_player.Stop();
		SetWindowText(_T("Zaxai Media Show - ZMS播放器"));
		_tcscpy_s(m_nid.szTip, _tcslen(_T("ZMS播放器")) + 1, _T("ZMS播放器"));
		Shell_NotifyIcon(NIM_MODIFY, &m_nid);
		g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_STOPBTN);
		ProTopMost(ZPlayer::STATE_STOPPED);
		g_sz_p_wnd[WND_TOOLTIP]->SendMessage(WM_MSGRECVPRO, 0, MSGUSER_KILLDELAYTOOLTIP);
	}
	break;
	case MSGUSER_OPENFILE:
	{
		TCHAR szFilter[] = _T("所有文件(*.*)|*.*||");
		CFileDialog fileDlg(TRUE, _T("选择文件"), NULL, 0, szFilter, this);
		if (IDOK == fileDlg.DoModal())
		{
			m_strFilePath = fileDlg.GetPathName();
			m_strFilePath = _T("file:///") + m_strFilePath;
			PostMessage(WM_MSGRECVPRO, 0, MSGUSER_STOPBTN);
			PostMessage(WM_MSGRECVPRO, 0, MSGUSER_PLAYBTN);
			g_sz_p_wnd[WND_LIS]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_strFilePath, MSGUSER_FILEINSERT);
		}
	}
	break;
	case MSGUSER_OPENFOLDER:
	{
		BROWSEINFO  bi;
		bi.hwndOwner = this->m_hWnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = NULL;
		bi.lpszTitle = NULL;
		bi.ulFlags = 0;
		bi.lpfn = NULL;
		bi.iImage = 0;
		LPCITEMIDLIST pidl = SHBrowseForFolder(&bi);
		if (pidl)
		{
			TCHAR  szDisplayName[255];
			SHGetPathFromIDList(pidl, szDisplayName);
			m_strFolderPath = szDisplayName;
			g_sz_p_wnd[WND_LIS]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_strFolderPath, MSGUSER_FOLDERINSERT);
		}
	}
	break;
	case MSGUSER_SETVOLUME:
	{
		m_player.SetVolume(wParam * 2);
	}
	break;
	case MSGUSER_SETTIME:
	{
		m_player.SetTime(wParam);
	}
	break;
	case MSGUSER_MINBTN:
	{
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	break;
	case MSGUSER_MAXBTN:
	{
		int nTgtWndState = 0;
		switch (m_nWndState)
		{
		case WND_STATE_NORMAL:
		{
			nTgtWndState = WND_STATE_MAX;
		}
		break;
		case WND_STATE_MAX:
		{
			nTgtWndState = WND_STATE_NORMAL;
		}
		break;
		case WND_STATE_FULL:
		{
			nTgtWndState = m_nLastWndState;
			m_bIsModeFull = false;
		}
		break;
		}
		ProBar();
		m_nLastWndState = m_nWndState;
		OperateWnd(m_nWndState, nTgtWndState);
		m_nWndState = nTgtWndState;
		g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, (WPARAM)m_nWndState, MSGUSER_MAXBTN);
		g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, (WPARAM)m_nWndState, MSGUSER_MAXBTN);
	}
	break;
	case MSGUSER_CLOSEBTN:
	{
		PostMessage(WM_CLOSE);
	}
	break;
	case MSGUSER_FULLBTN:
	{
		int nTgtWndState = 0;
		switch (m_nWndState)
		{
		case WND_STATE_NORMAL:
		case WND_STATE_MAX:
		{
			nTgtWndState = WND_STATE_FULL;
			m_bIsModeFull = true;
		}
		break;
		case WND_STATE_FULL:
		{
			nTgtWndState = m_nLastWndState;
			m_bIsModeFull = false;
		}
		break;
		}
		ProBar();
		m_nLastWndState = m_nWndState;
		OperateWnd(m_nWndState, nTgtWndState);
		m_nWndState = nTgtWndState;
		g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, (WPARAM)m_nWndState, MSGUSER_FULLBTN);
		g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, (WPARAM)m_nWndState, MSGUSER_FULLBTN);
		ProTopMost(m_player.GetState());
	}
	break;
	case MSGUSER_LISTBTN:
	{
		ChangeListBar();
		PostMessage(WM_SIZE, 0, 0);
	}
	break;
	case MSGUSER_PLAYFILE:
	{
		CString *p_str = (CString *)wParam;
		m_strFilePath = *p_str;
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_STOPBTN);
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_PLAYBTN);
	}
	break;
	case MSGUSER_ITEMDELETE:
	{
		m_strFilePath.Empty();
	}
	break;
	case MSGUSER_CLOSESETTING:
	{
		g_bIsOnSetting = false;
	}
	break;
	case MSGUSER_DESKLRCBTN:
	{
		ProLrcBar();
		if (g_bIsDeskLrc)
		{
			CRect rectLyric;
			m_staticPlayWnd.GetWindowRect(&rectLyric);
			m_p_LyricBar->MoveWindow(rectLyric);
			if(m_menuPlay.GetSubMenu(0))
				m_menuPlay.GetSubMenu(0)->ModifyMenu(ID_MODE_DESKLRC, MF_BYCOMMAND | MF_STRING, ID_MODE_DESKLRC, _T("关闭桌面歌词"));
		}
		else
		{
			PostMessage(WM_SIZE, 0, 0);
			if (m_menuPlay.GetSubMenu(0))
				m_menuPlay.GetSubMenu(0)->ModifyMenu(ID_MODE_DESKLRC, MF_BYCOMMAND | MF_STRING, ID_MODE_DESKLRC, _T("桌面歌词"));
		}
		ProTopMost(m_player.GetState());
	}
	break;
	case MSGUSER_SETTIMEUP:
	{
		if (m_player.GetState() == ZPlayer::STATE_PLAYING || m_player.GetState() == ZPlayer::STATE_PAUSED)
			g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTIMEUP);
	}
	break;
	case MSGUSER_SETTIMEDOWN:
	{
		if (m_player.GetState() == ZPlayer::STATE_PLAYING || m_player.GetState() == ZPlayer::STATE_PAUSED)
			g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTIMEDOWN);
	}
	break;
	}
	return 0;
}


void CZMShowDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case nUpdatePro:
	{
		static CString strTimeInfo, strTemp;
		static int nTotTimeLast = 0, nTotTime = 0, nCurTime = 0;
		switch (m_player.GetState())
		{
		case ZPlayer::STATE_ENDED:
		{
			KillTimer(nUpdatePro);
			PostMessage(WM_MSGRECVPRO, 0, MSGUSER_STOPBTN);
			g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_NEXTBTN);
			//if(!m_player.GetTotalTime())
				//AfxMessageBox(_T("文件播放出错!"));
		}
		break;
		case ZPlayer::STATE_ERROR:
		{
			KillTimer(nUpdatePro);
			PostMessage(WM_MSGRECVPRO, 0, MSGUSER_STOPBTN);
			m_strToolTip = _T("文件打开失败或者不存在!");
			g_sz_p_wnd[WND_TOOLTIP]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_strToolTip, MSGUSER_DELAYTOOLTIP);
		}
		break;
		default:
		{
			nTotTime = (int)m_player.GetTotalTime();
			nCurTime = (int)m_player.GetTime();
			if(nTotTime!= nTotTimeLast)
			{
				nTotTimeLast = nTotTime;
				g_sz_p_wnd[WND_OPE]->SendMessage(WM_MSGRECVPRO, (WPARAM)nTotTime, MSGUSER_SETMAXTIME);
			}
			g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, (WPARAM)nCurTime, MSGUSER_SETCURTIME);
			strTimeInfo.Format(_T("%02d:%02d"), nCurTime / 1000 / 60, (nCurTime / 1000) % 60);
			if(g_bIsTimeInfoTotal)
				strTemp.Format(_T("%02d:%02d"), nTotTime / 1000 / 60, (nTotTime / 1000) % 60);
			else
				strTemp.Format(_T("%02d:%02d"), (nTotTime- nCurTime) / 1000 / 60, ((nTotTime- nCurTime) / 1000) % 60);
			strTimeInfo += _T("/") + strTemp;
			g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, (WPARAM)&strTimeInfo, MSGUSER_SETTIMEINFO);
			if (m_nShowLyricBar)
				g_sz_p_wnd[WND_LRC]->PostMessage(WM_MSGRECVPRO, (WPARAM)nCurTime, MSGUSER_SETLRCPOS);
		}
		}
	}
	break;
	case nIsDblClk:
	{
		KillTimer(nIsDblClk);
		if (!m_bIsDblClk)
		{
			CRect rcPlayWnd;;
			m_staticPlayWnd.GetWindowRect(&rcPlayWnd);
			CPoint pt;
			GetCursorPos(&pt);
			if (rcPlayWnd.PtInRect(m_ptDown) && m_ptDown == pt && m_nHitTest== HTNOWHERE)
			{
				int nState = m_player.GetState();
				if (nState == ZPlayer::STATE_PLAYING || nState == ZPlayer::STATE_PAUSED)
					PostMessage(WM_MSGRECVPRO, 0, MSGUSER_PLAYBTN);
				else
					PostMessage(WM_MSGRECVPRO, 0, MSGUSER_OPENFILE);
			}
		}
	}
	break;
	}
	CDialog::OnTimer(nIDEvent);
}


void CZMShowDlg::OperateWnd(int nWndState, int nTgtWndState)
{
	static CRect rcNormal;
	switch (nTgtWndState)
	{
	case WND_STATE_NORMAL:
	{
		switch (nWndState)
		{
		case WND_STATE_MAX:
		{
			MoveWindow(rcNormal);
		}
		break;
		case WND_STATE_FULL:
		{
			if (m_menuPlay.GetSubMenu(0))
				m_menuPlay.GetSubMenu(0)->ModifyMenu(ID_MODE_FULL, MF_BYCOMMAND | MF_STRING, ID_MODE_FULL, _T("全屏"));
			if (!m_bIsModeMini)
			{
				m_nShowTitleBar = m_nShowOpeBar = SW_SHOW;
			}
			MoveWindow(rcNormal);
		}
		break;
		}
	}
	break;
	case WND_STATE_MAX:
	{
		switch (nWndState)
		{
		case WND_STATE_NORMAL:
		{
			GetWindowRect(&rcNormal);
			CRect rc;
			::SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
			MoveWindow(rc);
		}
		break;
		case WND_STATE_FULL:
		{
			if (m_menuPlay.GetSubMenu(0))
				m_menuPlay.GetSubMenu(0)->ModifyMenu(ID_MODE_FULL, MF_BYCOMMAND | MF_STRING, ID_MODE_FULL, _T("全屏"));
			if (!m_bIsModeMini)
			{
				m_nShowTitleBar = m_nShowOpeBar = SW_SHOW;
			}
			CRect rc;
			::SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
			MoveWindow(rc);
		}
		break;
		}
	}
	break;
	case WND_STATE_FULL:
	{
		if (m_menuPlay.GetSubMenu(0))
			m_menuPlay.GetSubMenu(0)->ModifyMenu(ID_MODE_FULL, MF_BYCOMMAND | MF_STRING, ID_MODE_FULL, _T("退出全屏"));
		if (!m_bIsModeMini)
		{
			m_nShowTitleBar = m_nShowOpeBar = SW_HIDE;
		}
		if(m_nShowListBar)
			ChangeListBar();
		switch (nWndState)
		{
		case WND_STATE_NORMAL:
		{
			GetWindowRect(&rcNormal);
			CRect rc;
			GetDesktopWindow()->GetWindowRect(&rc);
			MoveWindow(rc);
		}
		break;
		case WND_STATE_MAX:
		{
			CRect rc;
			GetDesktopWindow()->GetWindowRect(&rc);
			MoveWindow(rc);
		}
		break;
		}
	}
	break;
	}
}

void CZMShowDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tm;
	tm.cbSize = sizeof(TRACKMOUSEEVENT);
	tm.dwFlags = TME_LEAVE| TME_HOVER;
	tm.dwHoverTime = 3000;
	tm.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tm);
	switch (m_nWndState)
	{
	case WND_STATE_MAX:
	case WND_STATE_FULL:
		break;
	case WND_STATE_NORMAL:
	{
		m_nHitTest=HitTest(point);
	}
	break;
	}
	CPoint pt(point);
	ClientToScreen(&pt);
	ShowBar(pt);
	CDialog::OnMouseMove(nFlags, point);
}


void CZMShowDlg::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt;
	CRect rc;
	GetCursorPos(&pt);
	GetWindowRect(&rc);
	if (!rc.PtInRect(pt))
	{
		ShowBar(pt);
	}
	CDialog::OnMouseLeave();
}


void CZMShowDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_player.GetState() == ZPlayer::STATE_PLAYING || m_player.GetState() == ZPlayer::STATE_PAUSED)
		::SetCursor(::LoadCursor(NULL, NULL));
	CDialog::OnMouseHover(nFlags, point);
}


int CZMShowDlg::HitTest(CPoint point)
{
	static CRect rect;
	GetClientRect(&rect);
	int nHitTest = HTNOWHERE;
	if (point.x <= rect.left + 3 && point.y <= rect.top + 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		nHitTest = HTTOPLEFT;
	}
	else if (point.x >= rect.right - 3 && point.y <= rect.top + 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
		nHitTest = HTTOPRIGHT;
	}
	else if (point.x <= rect.left + 3 && point.y >= rect.bottom - 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
		nHitTest = HTBOTTOMLEFT;
	}
	else if (point.x >= rect.right - 3 && point.y >= rect.bottom - 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		nHitTest = HTBOTTOMRIGHT;
	}
	else if (point.x <= rect.left + 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		nHitTest = HTLEFT;
	}
	else if (point.x >= rect.right - 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		nHitTest = HTRIGHT;
	}
	else if (point.y <= rect.top + 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		nHitTest = HTTOP;
	}
	else if (point.y >= rect.bottom - 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		nHitTest = HTBOTTOM;
	}
	else
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		nHitTest = HTNOWHERE;
	}
	return nHitTest;
}


void CZMShowDlg::ShowBar(CPoint pt)
{
	CRect rectTitle, rectOpe, rectList;
	m_p_TitleBar->GetWindowRect(&rectTitle);
	m_p_OpeBar->GetWindowRect(&rectOpe);
	m_p_ListBar->GetWindowRect(&rectList);
	if (!m_nShowTitleBar)
	{
		if (rectTitle.PtInRect(pt))
		{
			if (!(m_p_TitleBar->IsWindowVisible()) && !IsIconic())
			{
				m_p_TitleBar->ShowWindow(SW_SHOW);
			}
		}
		else
			m_p_TitleBar->ShowWindow(SW_HIDE);
	}
	if (!m_nShowOpeBar)
	{
		if (rectOpe.PtInRect(pt))
		{
			if (!(m_p_OpeBar->IsWindowVisible()) && !IsIconic())
			{
				m_p_OpeBar->ShowWindow(SW_SHOW);
			}
		}
		else
			m_p_OpeBar->ShowWindow(SW_HIDE);
	}
	if (!m_nShowListBar)
	{
		if (rectList.PtInRect(pt))
		{
			if (!(m_p_ListBar->IsWindowVisible()) && !IsIconic())
			{
				m_p_ListBar->ShowWindow(SW_SHOW);
			}
		}
		else
			m_p_ListBar->ShowWindow(SW_HIDE);
	}
	BringWindowToTop();
}


void CZMShowDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bIsDblClk = false;
	m_ptDown = point;
	ClientToScreen(&m_ptDown);
	if (m_nWndState == WND_STATE_NORMAL)
	{
		ScaleWnd(m_nHitTest,point);
	}
	SetTimer(nIsDblClk, 300, NULL);
	CDialog::OnLButtonDown(nFlags, point);
}


void CZMShowDlg::ScaleWnd(int nHitTest, CPoint point)
{
	if (nHitTest == HTTOP)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTBOTTOM)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTLEFT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTRIGHT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTTOPLEFT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTTOPRIGHT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTBOTTOMLEFT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTBOTTOMRIGHT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));
	}
	else
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		//SendMessage(WM_SYSCOMMAND,SC_MOVE + HTCAPTION,0);  
	}
}


void CZMShowDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bIsDblClk = true;
	CRect rcPlayWnd,rcZMSWnd;
	GetClientRect(&rcZMSWnd);
	m_staticPlayWnd.GetWindowRect(&rcPlayWnd);
	ScreenToClient(&rcPlayWnd);
	if (rcPlayWnd.PtInRect(point))
	{
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_FULLBTN);
	}
	if (!rcPlayWnd.PtInRect(point)&& rcZMSWnd.PtInRect(point))
	{
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_MAXBTN);
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}


void CZMShowDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	if (m_menuPlay.m_hMenu)
	{
		CMenu* pPopup = m_menuPlay.GetSubMenu(0);
		if (pPopup)
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}


void CZMShowDlg::OnOpenFile()
{
	// TODO: 在此添加命令处理程序代码
	PostMessage(WM_MSGRECVPRO, 0, MSGUSER_OPENFILE);
}


void CZMShowDlg::OnOpenFolder()
{
	// TODO: 在此添加命令处理程序代码
	PostMessage(WM_MSGRECVPRO, 0, MSGUSER_OPENFOLDER);
}


void CZMShowDlg::OnSetting()
{
	// TODO: 在此添加命令处理程序代码
	m_p_Setting->CenterWindow(this);
	m_p_Setting->ShowWindow(SW_SHOW);
	g_bIsOnSetting = true;
}


void CZMShowDlg::OnExit()
{
	// TODO: 在此添加命令处理程序代码
	m_bIsExit = true;
	PostMessage(WM_CLOSE);
}


void CZMShowDlg::OnModeFull()
{
	// TODO: 在此添加命令处理程序代码
	PostMessage(WM_MSGRECVPRO, 0, MSGUSER_FULLBTN);
}


void CZMShowDlg::OnModeMini()
{
	// TODO: 在此添加命令处理程序代码
	m_bIsModeMini = !m_bIsModeMini;
	ProBar();
	if (m_bIsModeMini)
	{
		m_nShowTitleBar = m_nShowOpeBar = SW_HIDE;
		if (m_nShowListBar)
			ChangeListBar();
		if (m_menuPlay.GetSubMenu(0))
		{
			m_menuPlay.GetSubMenu(0)->ModifyMenu(ID_MODE_MINI, MF_BYCOMMAND | MF_STRING, ID_MODE_MINI, _T("普通模式"));
			m_menuPlay.GetSubMenu(0)->InsertMenu(4, MF_BYPOSITION, ID_MODE_NONEBLACK, _T("去视频黑边"));
		}
	}
	else
	{
		if (m_nWndState != WND_STATE_FULL)
		{
			m_nShowTitleBar = m_nShowOpeBar = SW_SHOW;
		}
		if (m_menuPlay.GetSubMenu(0))
		{
			m_menuPlay.GetSubMenu(0)->ModifyMenu(ID_MODE_MINI, MF_BYCOMMAND | MF_STRING, ID_MODE_MINI, _T("迷你模式"));
			m_menuPlay.GetSubMenu(0)->RemoveMenu(ID_MODE_NONEBLACK, MF_BYCOMMAND);
		}
	}
	PostMessage(WM_SIZE, 0, 0);
	ProTopMost(m_player.GetState());
}


void CZMShowDlg::OnModeNoneblack()
{
	if (m_nWndState == WND_STATE_NORMAL&&(m_player.GetState()==ZPlayer::STATE_PLAYING|| m_player.GetState() == ZPlayer::STATE_PAUSED))
	{
		unsigned int nWidth = 0, nHeight = 0;
		if (m_player.GetSize(nWidth, nHeight) == 0)
		{
			if (nWidth&&nHeight)
			{
				CRect rc;
				GetWindowRect(&rc);
				rc.bottom = rc.top + rc.Width()*nHeight / nWidth;
				MoveWindow(rc);
			}
		}
	}
}


void CZMShowDlg::OnDeskLrc()
{
	g_sz_p_wnd[WND_LRC]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_DESKLRCBTN);
}


void CZMShowDlg::OnItemPlay()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_LIS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_ITEMPLAY);
}


void CZMShowDlg::OnItemDelete()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_LIS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_ITEMDELETE);
}


void CZMShowDlg::OnOpenFilecatalog()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_LIS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_OPENFILECATALOG);
}


void CZMShowDlg::OnRefreshCatalog()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_LIS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_REFRESHCATALOG);
}


void CZMShowDlg::OnEmptyList()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_LIS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_EMPTYLIST);
}


void CZMShowDlg::OnRefreshList()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_LIS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_REFRESHLIST);
}


void CZMShowDlg::OnListmodeListseq()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETLISTMODE);
}


void CZMShowDlg::OnListmodeListloop()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 1, MSGUSER_SETLISTMODE);
}


void CZMShowDlg::OnListmodeListrand()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 2, MSGUSER_SETLISTMODE);
}


void CZMShowDlg::OnListmodeCatalogseq()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 3, MSGUSER_SETLISTMODE);
}


void CZMShowDlg::OnListmodeCatalogloop()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 4, MSGUSER_SETLISTMODE);
}


void CZMShowDlg::OnListmodeCatalogrand()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 5, MSGUSER_SETLISTMODE);
}


void CZMShowDlg::OnListmodeSingleseq()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 6, MSGUSER_SETLISTMODE);
}


void CZMShowDlg::OnListmodeSingleloop()
{
	// TODO: 在此添加命令处理程序代码
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 7, MSGUSER_SETLISTMODE);
}


void CZMShowDlg::ProBar(void)
{
	static bool bChild = true;
	m_p_TitleBar->ShowWindow(SW_HIDE);
	m_p_OpeBar->ShowWindow(SW_HIDE);
	m_p_ListBar->ShowWindow(SW_HIDE);
	if (bChild&&!m_bIsModeMini&&!m_bIsModeFull)
	{
		m_p_TitleBar->ModifyStyleEx(WS_EX_LAYERED, 0);
		m_p_TitleBar->ModifyStyle(WS_POPUP, WS_CHILD);
		m_p_TitleBar->SetParent(this);
		m_p_OpeBar->ModifyStyleEx(WS_EX_LAYERED, 0);
		m_p_OpeBar->ModifyStyle(WS_POPUP, WS_CHILD);
		m_p_OpeBar->SetParent(this);
		m_p_ListBar->ModifyStyleEx(WS_EX_LAYERED, 0);
		m_p_ListBar->ModifyStyle(WS_POPUP, WS_CHILD);
		m_p_ListBar->SetParent(this);
		bChild = false;
	}
	if (!bChild && (m_bIsModeMini || m_bIsModeFull))
	{
		m_p_TitleBar->ModifyStyle(WS_CHILD, WS_POPUP);
		m_p_TitleBar->SetParent(GetDesktopWindow());
		m_p_TitleBar->ModifyStyleEx(0, WS_EX_LAYERED);
		m_p_TitleBar->SetLayeredWindowAttributes(g_crBlack, 180, LWA_ALPHA);
		m_p_OpeBar->ModifyStyle(WS_CHILD, WS_POPUP);
		m_p_OpeBar->SetParent(GetDesktopWindow());
		m_p_OpeBar->ModifyStyleEx(0, WS_EX_LAYERED);
		m_p_OpeBar->SetLayeredWindowAttributes(g_crBlack, 180, LWA_ALPHA);
		m_p_ListBar->ModifyStyle(WS_CHILD, WS_POPUP);
		m_p_ListBar->SetParent(GetDesktopWindow());
		m_p_ListBar->ModifyStyleEx(0, WS_EX_LAYERED);
		m_p_ListBar->SetLayeredWindowAttributes(g_crBlack, 180, LWA_ALPHA);
		bChild = true;
	}
}


void CZMShowDlg::ProLrcBar(void)
{
	if (g_bIsDeskLrc)
	{
		m_p_LyricBar->ModifyStyle(WS_CHILD, WS_POPUP);
		m_p_LyricBar->SetParent(GetDesktopWindow());
		m_p_LyricBar->ModifyStyleEx(0, WS_EX_LAYERED);
		m_p_LyricBar->SetLayeredWindowAttributes(g_crBlack, 180, LWA_ALPHA);
	}
	else
	{
		m_p_LyricBar->ModifyStyleEx(WS_EX_LAYERED, 0);
		m_p_LyricBar->ModifyStyle(WS_POPUP, WS_CHILD);
		m_p_LyricBar->SetParent(this);
		m_p_LyricBar->ModifyStyle(0, WS_CLIPSIBLINGS);
		m_p_LyricBar->SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		m_staticPlayWnd.SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
}


void CZMShowDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialog::OnMoving(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
	SetBarPos();
}


void CZMShowDlg::SetLastPlayProgress()
{
	ZSetting setting;
	if (setting.CheckSavePlayProgress())
	{
		CString strPath = m_player.GetPath();
		if (!strPath.IsEmpty())
		{
			int nCurTime = 0;
			switch (m_player.GetState())
			{
			case ZPlayer::STATE_PLAYING:
			case ZPlayer::STATE_PAUSED:
			{
				nCurTime = (int)m_player.GetTime();
				ZPlayList playlist;
				playlist.SetLastPlayProgress(playlist.GetIDByPath(strPath), nCurTime);
			}
			break;
			case ZPlayer::STATE_ENDED:
			{
				ZPlayList playlist;
				playlist.SetLastPlayProgress(playlist.GetIDByPath(strPath), nCurTime);
			}
			break;
			}
		}
	}
}


void CZMShowDlg::GetLastPlayProgress()
{
	ZSetting setting;
	if (setting.CheckSavePlayProgress())
	{
		int nCurTime = 0;
		ZPlayList playlist;
		nCurTime = playlist.GetLastPlayProgress(playlist.GetIDByPath(m_strFilePath));
		m_player.SetTime(nCurTime);
	}
}


void CZMShowDlg::ProCmdPath(CString strCmdPath)
{
	if (!strCmdPath.IsEmpty())
	{
		strCmdPath.Remove(_T('\"'));
		m_strFilePath = _T("file:///") + strCmdPath;
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_STOPBTN);
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_PLAYBTN);
		g_sz_p_wnd[WND_LIS]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_strFilePath, MSGUSER_FILEINSERT);
	}
}


BOOL CZMShowDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_SHOWNORMAL);
	PostMessage(WM_SIZE, 0, 0);
	if (pCopyDataStruct->cbData > 0)
	{
		CString strCmdPath;
		strCmdPath = (TCHAR *)pCopyDataStruct->lpData;
		ProCmdPath(strCmdPath);
	}
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}


afx_msg LRESULT CZMShowDlg::OnTraynotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam)
	{
	case WM_RBUTTONUP:
	{
		SetForegroundWindow();
		CPoint pt;
		GetCursorPos(&pt);
		CMenu menu;
		menu.LoadMenu(IDR_MENU_MAIN);
		CMenu* pPopup = menu.GetSubMenu(0);
		if (pPopup)
			pPopup->TrackPopupMenu(TPM_LEFTALIGN| TPM_LEFTBUTTON, pt.x, pt.y, this); //在鼠标当前位置显示指定菜单
	}
	break;
	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow();
		ShowWindow(SW_SHOWNORMAL);
		PostMessage(WM_SIZE, 0, 0);
	}
	break;
	}
	return 0;
}


void CZMShowDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bIsExit)
	{
		m_p_TitleBar->ShowWindow(SW_HIDE);
		m_p_OpeBar->ShowWindow(SW_HIDE);
		//m_p_ListBar->ShowWindow(SW_HIDE); //注释后迷你模式下可显示
		m_p_ToolTipBar->ShowWindow(SW_HIDE);
		ShowWindow(SW_HIDE);
		return;
	}
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
	if (m_p_Setting&&m_p_Setting->m_hWnd)
		m_p_Setting->DestroyWindow();
	if (m_p_ToolTipBar&&m_p_ToolTipBar->m_hWnd)
		m_p_ToolTipBar->DestroyWindow();
	if (m_p_ListBar&&m_p_ListBar->m_hWnd)
		m_p_ListBar->DestroyWindow();
	if (m_p_OpeBar&&m_p_OpeBar->m_hWnd)
		m_p_OpeBar->DestroyWindow();
	if (m_p_TitleBar&&m_p_TitleBar->m_hWnd)
		m_p_TitleBar->DestroyWindow();
	if (m_p_LyricBar&&m_p_LyricBar->m_hWnd)
		m_p_LyricBar->DestroyWindow();
	CDialog::OnClose();
}


BOOL CZMShowDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (zDelta > 0)
		g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETVOLUMEUP);
	else
		g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETVOLUMEDOWN);
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CZMShowDlg::FileTypeIsAudioByPlay()
{
	if (m_player.GetFileType() == ZPlayer::TYPE_AUDIO)
	{
		static CString strLrc;
		CString strPath(m_strFilePath);
		strPath.Replace(_T("file:///"),_T(""));
		strPath = strPath.Left(strPath.ReverseFind(_T('.')));
		strPath += _T(".lrc");
		ZUtil::ReadFile2CString(strPath, strLrc);
		if (strLrc.IsEmpty())
		{
			m_nShowLyricBar = SW_HIDE;
		}
		else
		{
			m_nShowLyricBar = SW_SHOW;
			g_sz_p_wnd[WND_LRC]->PostMessage(WM_MSGRECVPRO, (WPARAM)&strLrc, MSGUSER_INSERTLRC);
			if (m_menuPlay.GetSubMenu(0))
			{
				if (g_bIsDeskLrc)
					m_menuPlay.GetSubMenu(0)->InsertMenu(ID_SETTING, MF_BYCOMMAND, ID_MODE_DESKLRC, _T("关闭桌面歌词"));
				else
					m_menuPlay.GetSubMenu(0)->InsertMenu(ID_SETTING, MF_BYCOMMAND, ID_MODE_DESKLRC, _T("桌面歌词"));
			}
		}
	}
	else
	{
		m_nShowLyricBar = SW_HIDE;
	}
	PostMessage(WM_SIZE, 0, 0);
}


void CZMShowDlg::FileTypeIsAudioByStop()
{
	if (m_player.GetFileType() == ZPlayer::TYPE_AUDIO)
	{
		m_nShowLyricBar = SW_HIDE;
		PostMessage(WM_SIZE, 0, 0);
		if(m_menuPlay.GetSubMenu(0))
			m_menuPlay.GetSubMenu(0)->RemoveMenu(ID_MODE_DESKLRC, MF_BYCOMMAND);
	}
}


LRESULT CALLBACK CZMShowDlg::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		if (!g_bIsFocusCombo && !g_bIsOnSetting)
		{
			if (1 == (lParam >> 30 & 1))
				ProHotKey(wParam);
			return TRUE;
		}
		else if (g_bIsFocusCombo)
		{
			if (wParam == VK_RETURN || wParam == VK_ESCAPE)
				return TRUE;
		}
	}
	return ::CallNextHookEx(m_hKeyboard,nCode,wParam,lParam);
}

void CZMShowDlg::ProHotKey(UINT nKey)
{
	// TODO: 在此处添加实现代码.
	CString str, strChar;
	if (::GetKeyState(VK_CONTROL)<0)
		str += _T("Ctrl+");
	if (::GetKeyState(VK_MENU)<0)
		str += _T("Alt+");
	if (::GetKeyState(VK_SHIFT)<0)
		str += _T("Shift+");
	if (nKey == VK_PRIOR || nKey == VK_NEXT || nKey == VK_SPACE || nKey == VK_RETURN || nKey == VK_LEFT || nKey == VK_RIGHT || nKey == VK_UP || nKey == VK_DOWN || (nKey >= '0'&&nKey <= '9') || (nKey >= 'A'&&nKey <= 'Z') || (nKey >= VK_NUMPAD0 && nKey <= VK_NUMPAD9))
	{
		if (nKey >= VK_NUMPAD0 && nKey <= VK_NUMPAD9)
		{
			strChar.Format(_T("小键盘%d"), nKey - VK_NUMPAD0);
		}
		else
		{
			switch (nKey)
			{
			case VK_PRIOR:
				strChar = _T("PageUp");
				break;
			case VK_NEXT:
				strChar = _T("PageDown");
				break;
			case VK_SPACE:
				strChar = _T("空格");
				break;
			case VK_RETURN:
				strChar = _T("回车");
				break;
			case VK_LEFT:
				strChar = _T("←");
				break;
			case VK_RIGHT:
				strChar = _T("→");
				break;
			case VK_UP:
				strChar = _T("↑");
				break;
			case VK_DOWN:
				strChar = _T("↓");
				break;
			default:
				strChar = (char)nKey;
			}
		}
		str += strChar;
		int nSize = g_vec_strHotKey.size();
		int i = 0;
		for (i = 0; i < nSize; ++i)
		{
			if (g_vec_strHotKey[i] == str)
				break;
		}
		switch (i)
		{
		case 0:
		{
			g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_PLAYBTN);
		}
		break;
		case 1:
		{
			g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_STOPBTN);
		}
		break;
		case 2:
		{
			g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_PREVBTN);
		}
		break;
		case 3:
		{
			g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_NEXTBTN);
		}
		break;
		case 4:
		{
			g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_VOLUMEBTN);
		}
		break;
		case 5:
		{
			g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETVOLUMEUP);
		}
		break;
		case 6:
		{
			g_sz_p_wnd[WND_OPE]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETVOLUMEDOWN);
		}
		break;
		case 7:
		{
			g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTIMEUP);
		}
		break;
		case 8:
		{
			g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTIMEDOWN);
		}
		break;
		case 9:
		{
			g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_FULLBTN);
		}
		break;
		}
	}
}


void CZMShowDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strFilePath, strFolderPath;
	int nDropCount = DragQueryFile(hDropInfo, -1, NULL, 0);
	for (int i = 0; i < nDropCount; ++i)
	{
		TCHAR sz_tc[MAX_PATH];
		DragQueryFile(hDropInfo, i, sz_tc, MAX_PATH);
		if (GetFileAttributes(sz_tc)&FILE_ATTRIBUTE_DIRECTORY)
		{
			strFolderPath = sz_tc;
			g_sz_p_wnd[WND_LIS]->SendMessage(WM_MSGRECVPRO, (WPARAM)&strFolderPath, MSGUSER_FOLDERINSERT);
		}
		else
		{
			strFilePath = _T("file:///") + CString(sz_tc);
			g_sz_p_wnd[WND_LIS]->SendMessage(WM_MSGRECVPRO, (WPARAM)&strFilePath, MSGUSER_FILEINSERT);
		}
	}
	if (!strFilePath.IsEmpty())
	{
		m_strFilePath = strFilePath;
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_STOPBTN);
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_PLAYBTN);
	}
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}


void CZMShowDlg::ChangeListBar()
{
	// TODO: 在此处添加实现代码.
	if (m_nShowListBar)
		m_nShowListBar = SW_HIDE;
	else
		m_nShowListBar = SW_SHOW;
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, (WPARAM)m_nShowListBar, MSGUSER_LISTBTN);
}


void CZMShowDlg::OnModeNotopmost()
{
	// TODO: 在此添加命令处理程序代码
	m_nTopMostState = TOPMOST_STATE_NO;
	ProTopMost();
	CheckTopMostMenu();
}


void CZMShowDlg::OnModeAlwaystopmost()
{
	// TODO: 在此添加命令处理程序代码
	m_nTopMostState = TOPMOST_STATE_ALWAYS;
	ProTopMost();
	CheckTopMostMenu();
}


void CZMShowDlg::OnModePlaytopmost()
{
	// TODO: 在此添加命令处理程序代码
	m_nTopMostState = TOPMOST_STATE_PLAY;
	ProTopMost(m_player.GetState());
	CheckTopMostMenu();
}


void CZMShowDlg::ProTopMost(int nPlayerState)
{
	// TODO: 在此处添加实现代码.
	switch (m_nTopMostState)
	{
	case TOPMOST_STATE_NO:
	{
		SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	break;
	case TOPMOST_STATE_ALWAYS:
	{
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	break;
	case TOPMOST_STATE_PLAY:
	{
		if (nPlayerState == ZPlayer::STATE_PLAYING)
			SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	break;
	}
}


void CZMShowDlg::CheckTopMostMenu()
{
	CMenu * p_menu = m_menuPlay.GetSubMenu(0);
	if (p_menu)
	{
		CMenu * p_menuTopMost = NULL;
		CString str;
		int nSize = p_menu->GetMenuItemCount();
		for (int i = 0; i < nSize; ++i)
		{
			p_menu->GetMenuString(i, str, MF_BYPOSITION);
			if (str == _T("窗口最前"))
			{
				p_menuTopMost = p_menu->GetSubMenu(i);
				p_menuTopMost->CheckMenuRadioItem(TOPMOST_STATE_NO, TOPMOST_STATE_PLAY, m_nTopMostState, MF_CHECKED | MF_BYPOSITION);
				break;
			}
		}
	}
}