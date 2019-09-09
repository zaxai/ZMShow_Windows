// Setting.cpp : 实现文件
//

#include "stdafx.h"
#include "ZMShow.h"
#include "Setting.h"
#include "afxdialogex.h"


// CSetting 对话框
const int nTabWidth = 90;
const int nTabHeight = 30;

IMPLEMENT_DYNAMIC(CSetting, CDialogEx)

CSetting::CSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTING, pParent)
{
	for (int i = 0; i < sizeof(m_sz_p_wndTab) / sizeof(CWnd *); ++i)
	{
		m_sz_p_wndTab[i] = NULL;
	}
}

CSetting::~CSetting()
{
}

void CSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SETTING, m_tabSetting);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_btnApply);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_BUTTON_RECOVERPAGE, m_btnRecoverPage);
}


BEGIN_MESSAGE_MAP(CSetting, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetting::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDCANCEL, &CSetting::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSetting::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDOK, &CSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RECOVERPAGE, &CSetting::OnBnClickedButtonRecoverpage)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETTING, &CSetting::OnTcnSelchangeTabSetting)
END_MESSAGE_MAP()


// CSetting 消息处理程序


BOOL CSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_sz_p_wnd[WND_SET] = this;
	InitButton();
	InitTab();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CSetting::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBrush brushClient(g_crWhite);
	CBrush brushLeft(g_crGrey230);
	CRect rcClient, rcItem, rcLeft;
	GetClientRect(&rcClient);
	m_tabSetting.GetItemRect(0, &rcItem);
	rcLeft = rcClient;
	rcLeft.right = rcItem.right;
	CDC dcMem;                                                           //用于缓冲作图的内存DC
	CBitmap bmp;                                                         //内存中承载临时图象的位图
	dcMem.CreateCompatibleDC(pDC);                                       //依附窗口DC创建兼容内存DC
	bmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());  //创建兼容位图
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	dcMem.FillRect(&rcClient, &brushClient);
	dcMem.FillRect(&rcLeft, &brushLeft);
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);//将内存DC上的图象拷贝到前台
	//pDC->TransparentBlt(0,0,rcClient.Width(),rcClient.Height(),&dcMem,0,0,rcClient.Width(),rcClient.Height(),crBlack);//将内存DC上的图象拷贝到前台
	dcMem.SelectObject(p_bmpOld);
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CSetting::InitButton(void)
{
	SetBtnColor(m_btnOK, _T(""));
	SetBtnColor(m_btnApply, _T(""));
	SetBtnColor(m_btnCancel, _T(""));
	SetBtnColor(m_btnClose, _T("关闭"));
	SetBtnColor(m_btnRecoverPage, _T(""));
	m_btnClose.DrawBorder(FALSE);
	m_btnClose.SetIcon(IDI_ICON_CLOSEON, IDI_ICON_CLOSE);
}


void CSetting::SetBtnColor(ZButton & btn, const CString & strTip)
{
	//btn.DrawTransparent(TRUE);
	btn.DrawBorder(TRUE);
	btn.SetFlat(TRUE);
	btn.SetColor(ZButton::COLOR_BK_IN, g_crWhite);
	btn.SetColor(ZButton::COLOR_BK_OUT, g_crWhite);
	btn.SetColor(ZButton::COLOR_TEXT_IN, g_crBlue);
	btn.SetColor(ZButton::COLOR_TEXT_OUT, g_crGrey55);
	btn.SetTooltipText(strTip);
	//btn.SetBtnCursor(IDC_CURSOR_INBTN);
}


void CSetting::InitTab(void)
{
	CSize size(nTabHeight, nTabWidth);
	CRect rcTab, rcItem;
	m_tabSetting.InsertItem(0, _T("常规"));
	m_tabSetting.InsertItem(1, _T("关联"));
	m_tabSetting.InsertItem(3, _T("热键"));
	m_tabSetting.SetItemSize(size);
	m_tabSetting.SetColor(g_crWhite, g_crGrey230, g_crBlue, g_crGrey200, g_crGrey55);
	m_tabSetting.GetClientRect(&rcTab);
	m_tabSetting.GetItemRect(0, &rcItem);
	rcTab.left = rcItem.right;
	m_common.Create(IDD_COMMON, &m_tabSetting);
	m_common.SetWindowPos(NULL, rcTab.left, rcTab.top, rcTab.Width(), rcTab.Height(), SWP_SHOWWINDOW);
	m_relation.Create(IDD_RELATION, &m_tabSetting);
	m_relation.SetWindowPos(NULL, rcTab.left, rcTab.top, rcTab.Width(), rcTab.Height(), SWP_HIDEWINDOW);
	m_hotkey.Create(IDD_HOTKEY, &m_tabSetting);
	m_hotkey.SetWindowPos(NULL, rcTab.left, rcTab.top, rcTab.Width(), rcTab.Height(), SWP_HIDEWINDOW);
	m_sz_p_wndTab[0] = &m_common;
	m_sz_p_wndTab[1] = &m_relation;
	m_sz_p_wndTab[2] = &m_hotkey;
}

void CSetting::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_CLOSE);
}


void CSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < sizeof(m_sz_p_wndTab) / sizeof(CWnd *); ++i)
	{
		if (m_sz_p_wndTab[i])
		{
			m_sz_p_wndTab[i]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTING_CANCEL);
		}
	}
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_CLOSESETTING);
	CDialogEx::OnCancel();
}


void CSetting::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < sizeof(m_sz_p_wndTab) / sizeof(CWnd *); ++i)
	{
		if (m_sz_p_wndTab[i])
		{
			m_sz_p_wndTab[i]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTING_APPLY);
		}
	}
}


void CSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < sizeof(m_sz_p_wndTab) / sizeof(CWnd *); ++i)
	{
		if (m_sz_p_wndTab[i])
		{
			m_sz_p_wndTab[i]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTING_OK);
		}
	}
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_CLOSESETTING);
	CDialogEx::OnOK();
}


void CSetting::OnBnClickedButtonRecoverpage()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < sizeof(m_sz_p_wndTab) / sizeof(CWnd *); ++i)
	{
		if (m_sz_p_wndTab[i] && i == m_tabSetting.GetSelectedItem())
		{
			m_sz_p_wndTab[i]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTING_RECOVERPAGE);
		}
	}
}


void CSetting::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CSetting::OnTcnSelchangeTabSetting(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	for (int i = 0; i < sizeof(m_sz_p_wndTab) / sizeof(CWnd *); ++i)
	{
		if (m_sz_p_wndTab[i])
		{
			if (i == m_tabSetting.GetSelectedItem())
				m_sz_p_wndTab[i]->ShowWindow(SW_SHOW);
			else
				m_sz_p_wndTab[i]->ShowWindow(SW_HIDE);
		}
	}
}
