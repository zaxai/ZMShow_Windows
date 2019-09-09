// OperateBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZMShow.h"
#include "OperateBar.h"
#include "afxdialogex.h"


// COperateBar �Ի���

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


// COperateBar ��Ϣ�������


BOOL COperateBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	g_sz_p_wnd[WND_OPE] = this;
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	m_brushBkGnd.CreateSolidBrush(g_crBlack);
	InitButton();
	InitSlider();
	m_bIsInitFinish = true;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH COperateBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return m_brushBkGnd;
	return hbr;
}

void COperateBar::InitButton(void)
{
	SetBtnColor(m_btnStop, _T("ֹͣ"));
	SetBtnColor(m_btnPrev, _T("��һ��"));
	SetBtnColor(m_btnPlay, _T("����"));
	SetBtnColor(m_btnNext, _T("��һ��"));
	SetBtnColor(m_btnVolume, _T("����"));
	SetBtnColor(m_btnFull, _T("ȫ��"));
	SetBtnColor(m_btnTimeInfo, _T("�л�����ǰʱ��/ʣ��ʱ����"));
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

	// TODO: �ڴ˴������Ϣ����������
	if (m_bIsInitFinish&&nType != SIZE_MINIMIZED)  //��ʼ�����˺���ܱ任
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
	if (pWnd != NULL)     //�ж��Ƿ�Ϊ�գ���Ϊ�ڴ��ڴ�����ʱ��Ҳ�����OnSize���������Ǵ�ʱ�����ؼ���û�д�����PwndΪ��
	{
		CRect rcSon;
		pWnd->GetWindowRect(&rcSon);  //��ȡ�ؼ��仯ǰ�Ĵ�С
		ScreenToClient(&rcSon);   //���ؼ���Сװ��λ�ڶԻ����е��������� 
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
		pWnd->MoveWindow(rcSon);   //�����ؼ�
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
			m_btnPlay.SetTooltipText(_T("��ͣ"));
			m_sliderTime.ShowWindow(SW_SHOW);
			m_btnTimeInfo.ShowWindow(SW_SHOW);
		}
		break;
		default:
		{
			m_btnPlay.SetIcon(IDI_ICON_PLAYON, IDI_ICON_PLAY);
			m_btnPlay.SetTooltipText(_T("����"));
		}
		}
	}
	break;
	case MSGUSER_STOPBTN:
	{
		m_btnPlay.SetIcon(IDI_ICON_PLAYON, IDI_ICON_PLAY);
		m_btnPlay.SetTooltipText(_T("����"));
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
			m_btnVolume.SetTooltipText(_T("ȡ������"));
		}
		else
		{
			m_btnVolume.SetIcon(IDI_ICON_VOLUMEON, IDI_ICON_VOLUME);
			m_btnVolume.SetTooltipText(_T("����"));
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
			m_strToolTip.Format(_T("���� %d"), nPos);
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
			m_strToolTip.Format(_T("���� %d"), nPos);
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
			m_strToolTip.Format(_T("��� %02d:%02d"), nPos / 1000 / 60, (nPos / 1000) % 60);
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
			m_strToolTip.Format(_T("���� %02d:%02d"), nPos / 1000 / 60, (nPos / 1000) % 60);
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
			m_btnFull.SetTooltipText(_T("ȫ��"));
		}
		break;
		case WND_STATE_FULL:
		{
			m_btnFull.SetIcon(IDI_ICON_ORIGINON, IDI_ICON_ORIGIN);
			m_btnFull.SetTooltipText(_T("�˳�ȫ��"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_bIsTimeInfoTotal = !g_bIsTimeInfoTotal;
	if (g_bIsTimeInfoTotal)
	{
		m_btnTimeInfo.SetTooltipText(_T("�л�����ǰʱ��/ʣ��ʱ����"));
	}
	else
	{
		m_btnTimeInfo.SetTooltipText(_T("�л�����ǰʱ��/��ʱ����"));
	}
}


void COperateBar::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_STOPBTN);
}


void COperateBar::OnBnClickedButtonPrev()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_PREVBTN);
}


void COperateBar::OnBnClickedButtonPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_PLAYBTN);
}


void COperateBar::OnBnClickedButtonNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_NEXTBTN);
}


void COperateBar::OnBnClickedButtonVolume()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PostMessage(WM_MSGRECVPRO, 0, MSGUSER_VOLUMEBTN);
}


void COperateBar::OnBnClickedButtonFull()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MOUSELEAVE);
	CDialogEx::OnMouseLeave();
}


BOOL COperateBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MOUSEWHEEL, MAKEWPARAM(nFlags, zDelta), MAKELPARAM(pt.x, pt.y));
	return TRUE;
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
