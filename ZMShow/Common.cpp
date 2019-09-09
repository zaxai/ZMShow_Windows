// Common.cpp : 实现文件
//

#include "stdafx.h"
#include "ZMShow.h"
#include "Common.h"
#include "afxdialogex.h"


// CCommon 对话框

IMPLEMENT_DYNAMIC(CCommon, CDialogEx)

CCommon::CCommon(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON, pParent)
	, m_nIsAutoRun(FALSE)
	, m_nIsClearHistory(FALSE)
	, m_nIsSavePlayProgress(FALSE)
{

}

CCommon::~CCommon()
{
}

void CCommon::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_AUTORUN, m_checkAutoRun);
	DDX_Control(pDX, IDC_CHECK_CLEARHISTORY, m_checkClearHistory);
	DDX_Control(pDX, IDC_CHECK_SAVEPLAYPROGRESS, m_checkSavePlayProgress);
	DDX_Control(pDX, IDC_CHECK_FOLDERAUTOSEL, m_checkFolderAutoSel);
	DDX_Control(pDX, IDC_BUTTON_REMIND, m_btnRemind);
}


BEGIN_MESSAGE_MAP(CCommon, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MSGRECVPRO, &CCommon::OnMsgrecvpro)
END_MESSAGE_MAP()


// CCommon 消息处理程序


BOOL CCommon::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_sz_p_wnd[WND_COMMON] = this;
	m_brushBkGnd.CreateSolidBrush(g_crWhite);
	InitCheckBox();
	Check();
	InitButton();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CCommon::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC: 
	{
		pDC->SetBkMode(TRANSPARENT);
	}
	break;
	}
	return m_brushBkGnd;
	return hbr;
}


void CCommon::InitCheckBox(void)
{
	m_checkAutoRun.SetCheck(0);
	m_checkClearHistory.SetCheck(0);
	m_checkSavePlayProgress.SetCheck(1);
	m_checkFolderAutoSel.SetCheck(1);
}


void CCommon::Check(void)
{
	m_nIsAutoRun = m_setting.CheckAutoRun();
	m_checkAutoRun.SetCheck(m_nIsAutoRun);
	m_nIsClearHistory = m_setting.CheckClearHistory();
	m_checkClearHistory.SetCheck(m_nIsClearHistory);
	m_nIsSavePlayProgress = m_setting.CheckSavePlayProgress();
	m_checkSavePlayProgress.SetCheck(m_nIsSavePlayProgress);
	m_nIsFolderAutoSel = m_setting.CheckFolderAutoSel();
	m_checkFolderAutoSel.SetCheck(m_nIsFolderAutoSel);
}

afx_msg LRESULT CCommon::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_SETTING_CANCEL:
	{
		m_checkAutoRun.SetCheck(m_nIsAutoRun);
		m_checkClearHistory.SetCheck(m_nIsClearHistory);
		m_checkSavePlayProgress.SetCheck(m_nIsSavePlayProgress);
		m_checkFolderAutoSel.SetCheck(m_nIsFolderAutoSel);
	}
	break;
	case MSGUSER_SETTING_APPLY:
	case MSGUSER_SETTING_OK:
	{
		if(m_setting.SetAutoRun(m_checkAutoRun.GetCheck()))
			m_nIsAutoRun = m_checkAutoRun.GetCheck();
		else
		{
			if(m_checkAutoRun.GetCheck()!= m_nIsAutoRun)
				m_btnRemind.ShowWindow(SW_SHOW);
			m_checkAutoRun.SetCheck(m_nIsAutoRun);
		}
		if (m_setting.SetClearHistory(m_checkClearHistory.GetCheck()))
			m_nIsClearHistory = m_checkClearHistory.GetCheck();
		else
			m_checkClearHistory.SetCheck(m_nIsClearHistory);
		if (m_setting.SetSavePlayProgress(m_checkSavePlayProgress.GetCheck()))
			m_nIsSavePlayProgress = m_checkSavePlayProgress.GetCheck();
		else
			m_checkSavePlayProgress.SetCheck(m_nIsSavePlayProgress);
		if (m_setting.SetFolderAutoSel(m_checkFolderAutoSel.GetCheck()))
			m_nIsFolderAutoSel = m_checkFolderAutoSel.GetCheck();
		else
			m_checkFolderAutoSel.SetCheck(m_nIsFolderAutoSel);
	}
	break;
	case MSGUSER_SETTING_RECOVERPAGE:
	{
		InitCheckBox();
	}
	break;
	}
	return 0;
}


void CCommon::InitButton(void)
{
	SetBtnColor(m_btnRemind, _T("需要管理员权限才能设置，请关闭程序，右键“以管理员身份运行”"));
	m_btnRemind.SetIcon(IDI_ICON_REMIND, IDI_ICON_REMIND);
	m_btnRemind.ShowWindow(SW_HIDE);
}


void CCommon::SetBtnColor(ZButton & btn, const CString & strTip)
{
	//btn.DrawTransparent(TRUE);
	btn.DrawBorder(FALSE);
	btn.SetFlat(TRUE);
	btn.SetColor(ZButton::COLOR_BK_IN, g_crWhite);
	btn.SetColor(ZButton::COLOR_BK_OUT, g_crWhite);
	btn.SetColor(ZButton::COLOR_TEXT_IN, g_crWhite);
	btn.SetColor(ZButton::COLOR_TEXT_OUT, g_crWhite);
	btn.SetTooltipText(strTip);
	//btn.SetBtnCursor(IDC_CURSOR_INBTN);
}