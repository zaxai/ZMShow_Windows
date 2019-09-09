// OperateBar.cpp : 实现文件
//

#include "stdafx.h"
#include "ZMShow.h"
#include "OperateBar.h"
#include "afxdialogex.h"


// COperateBar 对话框

IMPLEMENT_DYNAMIC(COperateBar, CDialogEx)

COperateBar::COperateBar(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPERATEBAR, pParent)
	, m_bIsInitFinish(false)
	, m_strToolTip(_T(""))
{

}

COperateBar::~COperateBar()
{
}

void COperateBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TIMEINFO, m_btnTimeInfo);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
	DDX_Control(pDX, IDC_BUTTON_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BUTTON_VOLUME, m_btnVolume);
	DDX_Control(pDX, IDC_BUTTON_FULL, m_btnFull);
	DDX_Control(pDX, IDC_SLIDER_TIME, m_sliderTime);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_sliderVolume);
}


BEGIN_MESSAGE_MAP(COperateBar, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_MESSAGE(WM_MSGRECVPRO, &COperateBar::OnMsgrecvpro)
	ON_BN_CLICKED(IDC_BUTTON_TIMEINFO, &COperateBar::OnBnClickedButtonTimeinfo)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &COperateBar::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &COperateBar::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &COperateBar::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &COperateBar::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_VOLUME, &COperateBar::OnBnClickedButtonVolume)
	ON_BN_CLICKED(IDC_BUTTON_FULL, &COperateBar::OnBnClickedButtonFull)
	ON_WM_HSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// COperateBar 消息处理程序


BOOL COperateBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_sz_p_wnd[WND_OPE] = this;
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	m_brushBkGnd.CreateSolidBrush(g_crBlack);
	InitButton();
	InitSlider();
	m_bIsInitFinish = true;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH COperateBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return m_brushBkGnd;
	return hbr;
}

void COperateBar::InitButton(void)
{
	SetBtnColor(m_btnStop, _T("停止"));
	SetBtnColor(m_btnPrev, _T("上一个"));
	SetBtnColor(m_btnPlay, _T("播放"));
	SetBtnColor(m_btnNext, _T("下一个"));
	SetBtnColor(m_btnVolume, _T("静音"));
	SetBtnColor(m_btnFull, _T("全屏"));
	SetBtnColor(m_btnTimeInfo, _T("切换“当前时长/剩余时长”"));
	m_btnPlay.SetIcon(IDI_ICON_PLAYON, IDI_ICON_PLAY);
	m_btnStop.SetIcon(IDI_ICON_STOPON, IDI_ICON_STOP);
	m_btnPrev.SetIcon(IDI_ICON_PREVON, IDI_ICON_PREV);
	m_btnNext.SetIcon(IDI_ICON_NEXTON, IDI_ICON_NEXT);
	m_btnVolume.SetIcon(IDI_ICON_VOLUMEON, IDI_ICON_VOLUME);
	m_btnFull.SetIcon(IDI_ICON_FULLON, IDI_ICON_FULL);
	m_btnTimeInfo.ShowWindow(SW_HIDE);
	SetDlgItemText(IDC_BUTTON_TIMEINFO, _T(""));
}

void COperateBar::SetBtnColor(ZButton & btn, const CString & strTip)
{
	//btn.DrawTransparent(TRUE);
	btn.DrawBorder(FALSE);
	btn.SetFlat(TRUE);
	btn.SetColor(ZButton::COLOR_BK_IN, g_crBlack);
	btn.SetColor(ZButton::COLOR_BK_OUT, g_crBlack);
	btn.SetColor(ZButton::COLOR_TEXT_IN, g_crBlue);
	btn.SetColor(ZButton::COLOR_TEXT_OUT, g_crWhite);
	btn.SetTooltipText(strTip);
	//btn.SetBtnCursor(IDC_CURSOR_INBTN);
}

void COperateBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_bIsInitFinish&&nType != SIZE_MINIMIZED)  //初始化好了后才能变换
	{
		CRect rcDlg;
		GetClientRect(&rcDlg);
		ChangeSize(rcDlg, IDC_BUTTON_NEXT);
		ChangeSize(rcDlg, IDC_BUTTON_VOLUME);
		ChangeSize(rcDlg, IDC_SLIDER_VOLUME);
		ChangeSize(rcDlg, IDC_BUTTON_PLAY);
		ChangeSize(rcDlg, IDC_BUTTON_PREV);
		ChangeSize(rcDlg, IDC_BUTTON_STOP);
		ChangeSize(rcDlg, IDC_SLIDER_TIME);
		ChangeSize(rcDlg, IDC_BUTTON_FULL);
		//RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_ALLCHILDREN);
	}
}

void COperateBar::ChangeSize(const CRect & rcParent, const UINT & nID)
{
	static CRect rcTemp;
	CWnd *pWnd = GetDlgItem(nID);
	if (pWnd != NULL)     //判断是否为空，因为在窗口创建的时候也会调用OnSize函数，但是此时各个控件还没有创建，Pwnd为空
	{
		CRect rcSon;
		pWnd->GetWindowRect(&rcSon);  //获取控件变化前的大小
		ScreenToClient(&rcSon);   //将控件大小装换位在对话框中的区域坐标 
		int nHeight = rcSon.Height();
		int nWidth = rcSon.Width();
		switch (nID)
		{
		case IDC_BUTTON_NEXT:
		{
			rcTemp.left = rcSon.left = rcParent.left + rcParent.Width() / 2;
			rcTemp.right = rcSon.right = rcSon.left + nWidth;
		}
		break;
		case IDC_BUTTON_VOLUME:
		case IDC_SLIDER_VOLUME:
		{
			rcSon.left = rcTemp.right + 10;
			rcTemp.right = rcSon.right = rcSon.left + nWidth;
		}
		break;
		case IDC_BUTTON_PLAY:
		case IDC_BUTTON_PREV:
		case IDC_BUTTON_STOP:
		{
			rcSon.right = rcTemp.left - 10;
			rcTemp.left = rcSon.left = rcSon.right - nWidth;
		}
		break;
		case IDC_SLIDER_TIME:
		{
			rcTemp.right = rcSon.right = rcParent.right - (rcSon.left - rcParent.left);
		}
		break;
		case IDC_BUTTON_FULL:
		{
			rcSon.right = rcTemp.right;
			rcSon.left = rcSon.right - nWidth;
		}
		}
		pWnd->MoveWindow(rcSon);   //伸缩控件
	}
}

afx_msg LRESULT COperateBar::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_PLAYBTN:
	{
		switch (wParam)
		{
		case ZPlayer::STATE_PLAYING:
		{
			m_btnPlay.SetIcon(IDI_ICON_PAUSEON, IDI_ICON_PAUSE);
			m_btnPlay.SetTooltipText(_T("暂停"));
			m_sliderTime.ShowWindow(SW_SHOW);
			m_btnTimeInfo.ShowWindow(SW_SHOW);
		}
		break;
		default:
		{
			m_btnPlay.SetIcon(IDI_ICON_PLAYON, IDI_ICON_PLAY);
			m_btnPlay.SetTooltipText(_T("播放"));
		}
		}
	}
	break;
	case MSGUSER_STOPBTN:
	{
		m_btnPlay.SetIcon(IDI_ICON_PLAYON, IDI_ICON_PLAY);
		m_btnPlay.SetTooltipText(_T("播放"));
		m_sliderTime.ShowWindow(SW_HIDE);
		m_sliderTime.SetPos(0);
		m_btnTimeInfo.ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_BUTTON_TIMEINFO, _T(""));
	}
	break;
	case MSGUSER_VOLUMEBTN:
	{
		g_bIsMute = !g_bIsMute;
		if (g_bIsMute)
		{
			m_btnVolume.SetIcon(IDI_ICON_MUTEON, IDI_ICON_MUTE);
			m_btnVolume.SetTooltipText(_T("取消静音"));
		}
		else
		{
			m_btnVolume.SetIcon(IDI_ICON_VOLUMEON, IDI_ICON_VOLUME);
			m_btnVolume.SetTooltipText(_T("静音"));
		}
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETVOLUME);
	}
	break;
	case MSGUSER_SETMAXTIME:
	{
		m_sliderTime.SetRangeMax(wParam);
	}
	break;
	case MSGUSER_SETCURTIME:
	{
		m_sliderTime.SetPos(wParam);
	}
	break;
	case MSGUSER_SETTIMEINFO:
	{
		CString *p_str = (CString *)wParam;
		SetDlgItemText(IDC_BUTTON_TIMEINFO, *p_str);
	}
	break;
	case MSGUSER_SETVOLUME:
	{
		int nPos = 0;
		if (!g_bIsMute)
			nPos = m_sliderVolume.GetPos();
		g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, (WPARAM)nPos, MSGUSER_SETVOLUME);
	}
	break;
	case MSGUSER_SETVOLUMEUP:
	{
		int nPos,nMax;
		nPos= m_sliderVolume.GetPos();
		nMax=m_sliderVolume.GetRangeMax();
		nPos = nPos + 5 > nMax ? nMax : nPos + 5;
		m_sliderVolume.SetPos(nPos);
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETVOLUME);
		if (g_sz_p_wnd[WND_ZMS]->IsWindowVisible())
		{
			m_strToolTip.Format(_T("音量 %d"), nPos);
			g_sz_p_wnd[WND_TOOLTIP]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_strToolTip, MSGUSER_ONTIMETOOLTIP);
		}
	}
	break;
	case MSGUSER_SETVOLUMEDOWN:
	{
		int nPos, nMin;
		nPos = m_sliderVolume.GetPos();
		nMin = m_sliderVolume.GetRangeMin();
		nPos = nPos - 5 < nMin ? nMin : nPos - 5;
		m_sliderVolume.SetPos(nPos);
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETVOLUME);
		if (g_sz_p_wnd[WND_ZMS]->IsWindowVisible())
		{
			m_strToolTip.Format(_T("音量 %d"), nPos);
			g_sz_p_wnd[WND_TOOLTIP]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_strToolTip, MSGUSER_ONTIMETOOLTIP);
		}
	}
	break;
	case MSGUSER_SETTIME:
	{
		int nPos = m_sliderTime.GetPos();
		g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, (WPARAM)nPos, MSGUSER_SETTIME);
	}
	break;
	case MSGUSER_SETTIMEUP:
	{
		int nPos, nMax;
		nPos = m_sliderTime.GetPos();
		nMax = m_sliderTime.GetRangeMax();
		nPos = nPos + 5000 > nMax ? nMax : nPos + 5000;
		m_sliderTime.SetPos(nPos);
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTIME);
		if (g_sz_p_wnd[WND_ZMS]->IsWindowVisible())
		{
			m_strToolTip.Format(_T("快进 %02d:%02d"), nPos / 1000 / 60, (nPos / 1000) % 60);
			g_sz_p_wnd[WND_TOOLTIP]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_strToolTip, MSGUSER_ONTIMETOOLTIP);
		}
	}
	break;
	case MSGUSER_SETTIMEDOWN:
	{
		int nPos, nMin;
		nPos = m_sliderTime.GetPos();
		nMin = m_sliderTime.GetRangeMin();
		nPos = nPos - 5000 < nMin ? nMin : nPos - 5000;
		m_sliderTime.SetPos(nPos);
		PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTIME);
		if (g_sz_p_wnd[WND_ZMS]->IsWindowVisible())
		{
			m_strToolTip.Format(_T("快退 %02d:%02d"), nPos / 1000 / 60, (nPos / 1000) % 60);
			g_sz_p_wnd[WND_TOOLTIP]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_strToolTip, MSGUSER_ONTIMETOOLTIP);
		}
	}
	break;
	case MSGUSER_MAXBTN:
	case MSGUSER_FULLBTN:
	{
		switch (wParam)
		{
		case WND_STATE_NORMAL:
		case WND_STATE_MAX:
		{
			m_btnFull.SetIcon(IDI_ICON_FULLON, IDI_ICON_FULL);
			m_btnFull.SetTooltipText(_T("全屏"));
		}
		break;
		case WND_STATE_FULL:
		{
			m_btnFull.SetIcon(IDI_ICON_ORIGINON, IDI_ICON_ORIGIN);
			m_btnFull.SetTooltipText(_T("退出全屏"));
		}
		break;
		}
	}
	break;
	}
	return 0;
}


void COperateBar::OnBnClickedButtonTimeinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bIsTimeInfoTotal = !g_bIsTimeInfoTotal;
	if (g_bIsTimeInfoTotal)
	{
		m_btnTimeInfo.SetTooltipText(_T("切换“当前时长/剩余时长”"));
	}
	else
	{
		m_btnTimeInfo.SetTooltipText(_T("切换“当前时长/总时长”"));
	}
}


void COperateBar::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_STOPBTN);
}


void COperateBar::OnBnClickedButtonPrev()
{
	// TODO: 在此添加控件通知处理程序代码
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_PREVBTN);
}


void COperateBar::OnBnClickedButtonPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_PLAYBTN);
}


void COperateBar::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_NEXTBTN);
}


void COperateBar::OnBnClickedButtonVolume()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_MSGRECVPRO, 0, MSGUSER_VOLUMEBTN);
}


void COperateBar::OnBnClickedButtonFull()
{
	// TODO: 在此添加控件通知处理程序代码
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_FULLBTN);
}

void COperateBar::InitSlider(void)
{
	m_sliderTime.SetRange(0, 1000);
	m_sliderTime.SetPos(0);
	m_sliderTime.SetPageSize(0);
	m_sliderTime.SetLineSize(0);
	m_sliderTime.SetColor(g_crBlack, g_crGrey100, g_crBlue, g_crGrey200);
	m_sliderTime.ShowWindow(SW_HIDE);
	m_sliderVolume.SetRange(0, 100);
	m_sliderVolume.SetPos(30);
	m_sliderVolume.SetPageSize(0);
	m_sliderVolume.SetLineSize(0);
	m_sliderVolume.SetColor(g_crBlack, g_crGrey100, g_crBlue, g_crGrey200);
}

void COperateBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nSBCode)
	{
	case SB_THUMBTRACK:
	{
		if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_TIME)
		{
			PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTIME);
			pScrollBar->Invalidate();
		}
		if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_VOLUME)
		{
			PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETVOLUME);
			pScrollBar->Invalidate();
		}
	}
	break;
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void COperateBar::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tm;
	tm.cbSize = sizeof(TRACKMOUSEEVENT);
	tm.dwFlags = TME_LEAVE;
	tm.dwHoverTime = 500;
	tm.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tm);
	CDialogEx::OnMouseMove(nFlags, point);
}


void COperateBar::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MOUSELEAVE);
	CDialogEx::OnMouseLeave();
}


BOOL COperateBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MOUSEWHEEL, MAKEWPARAM(nFlags, zDelta), MAKELPARAM(pt.x, pt.y));
	return TRUE;
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
