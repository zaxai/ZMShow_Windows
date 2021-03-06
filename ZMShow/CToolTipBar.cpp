// CToolTipBar.cpp: 实现文件
//

#include "stdafx.h"
#include "ZMShow.h"
#include "CToolTipBar.h"
#include "afxdialogex.h"


// CToolTipBar 对话框
//Timer
const int nUpdateDelayToolTip = 0;
const int nUpdateOnTimeToolTip = 1;

IMPLEMENT_DYNAMIC(CToolTipBar, CDialogEx)

CToolTipBar::CToolTipBar(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TOOLTIPBAR, pParent)
	, m_nCountDown(0)
	, m_strToolTip(_T(""))
	, m_strShow(_T(""))
	, m_bIsInDelayTimer(false)
{
	m_logfont.lfHeight = -16;
	m_logfont.lfWidth = 0;
	m_logfont.lfEscapement = 0;
	m_logfont.lfOrientation = 0;
	m_logfont.lfWeight = FW_NORMAL;
	m_logfont.lfItalic = FALSE;
	m_logfont.lfUnderline = FALSE;
	m_logfont.lfStrikeOut = 0;
	m_logfont.lfCharSet = ANSI_CHARSET;
	m_logfont.lfOutPrecision = OUT_STROKE_PRECIS;
	m_logfont.lfClipPrecision = CLIP_STROKE_PRECIS;
	m_logfont.lfQuality = DRAFT_QUALITY;
	m_logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	_tcscpy_s(m_logfont.lfFaceName, _T("微软雅黑"));
}

CToolTipBar::~CToolTipBar()
{
}

void CToolTipBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CToolTipBar, CDialogEx)
	ON_MESSAGE(WM_MSGRECVPRO, &CToolTipBar::OnMsgrecvpro)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CToolTipBar 消息处理程序


BOOL CToolTipBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_sz_p_wnd[WND_TOOLTIP] = this;
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


afx_msg LRESULT CToolTipBar::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_DELAYTOOLTIP:
	{
		CString * p_str = (CString *)wParam;
		m_nCountDown = 5;
		m_strToolTip = *p_str;
		m_bIsInDelayTimer = true;
		SetTimer(nUpdateDelayToolTip, 1000, NULL);
	}
	break;
	case MSGUSER_ONTIMETOOLTIP:
	{
		CString * p_str = (CString *)wParam;
		m_strShow = *p_str;
		Invalidate();
		ShowWindow(SW_SHOW);
		SetTimer(nUpdateOnTimeToolTip, 1000, NULL);
	}
	break;
	case MSGUSER_KILLDELAYTOOLTIP:
	{
		if (m_bIsInDelayTimer)
		{
			KillTimer(nUpdateDelayToolTip);
			ShowWindow(SW_HIDE);
			m_bIsInDelayTimer = false;
		}
	}
	break;
	}
	return 0;
}


void CToolTipBar::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case nUpdateDelayToolTip:
	{
		if (m_nCountDown == 0)
		{
			KillTimer(nUpdateDelayToolTip);
			ShowWindow(SW_HIDE);
			g_sz_p_wnd[WND_TIT]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_NEXTBTN);
			m_bIsInDelayTimer = false;
		}
		m_strShow.Format(_T("%s  %ds"), m_strToolTip, m_nCountDown);
		Invalidate();
		if (m_nCountDown >0 && g_sz_p_wnd[WND_ZMS]->IsWindowVisible() && !g_sz_p_wnd[WND_ZMS]->IsIconic()&& !IsWindowVisible())
			ShowWindow(SW_SHOW);
		--m_nCountDown;
	}
	break;
	case nUpdateOnTimeToolTip:
	{
		KillTimer(nUpdateOnTimeToolTip);
		if (!m_bIsInDelayTimer)
			ShowWindow(SW_HIDE);
	}
	break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CToolTipBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CToolTipBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CustDraw(&dc);
}


void CToolTipBar::CustDraw(CDC * pDC)
{
	CBrush brushClient(g_crBlack);
	CRect rcClient;
	GetClientRect(&rcClient);
	CDC dcMem;
	CBitmap bmp;
	dcMem.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	//绘制背景
	dcMem.FillRect(&rcClient, &brushClient);
	//绘制文字
	int R = 0, G = 0, B = 0;
	R = GetRValue(g_crBlue);
	G = GetGValue(g_crBlue);
	B = GetBValue(g_crBlue);
	Gdiplus::SolidBrush brush(Gdiplus::Color(R, G, B));
	Gdiplus::Font font(dcMem.GetSafeHdc(), &m_logfont);
	Gdiplus::FontFamily fontFamily(m_logfont.lfFaceName);
	Gdiplus::StringFormat strformat;
	Gdiplus::GraphicsPath path;
	Gdiplus::Pen pen(Gdiplus::Color(R, G, B), 5);
	Gdiplus::Graphics graphics(dcMem.GetSafeHdc());
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//抗锯齿
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	strformat.SetAlignment(Gdiplus::StringAlignmentNear);
	strformat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	strformat.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);
	strformat.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
	path.AddString(m_strShow, m_strShow.GetLength(), &fontFamily, Gdiplus::FontStyleRegular, font.GetSize(), Gdiplus::RectF((float)rcClient.left+10, (float)rcClient.top, (float)rcClient.Width(), (float)rcClient.Height()), &strformat);
	pen.SetLineJoin(Gdiplus::LineJoinRound);
	for (int i = 3; i > 1; --i)
	{
		pen.SetWidth(Gdiplus::REAL(i));
		pen.SetColor(Gdiplus::Color(12 * i, R, G, B));
		graphics.DrawPath(&pen, &path);
	}
	graphics.FillPath(&brush, &path);
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(p_bmpOld);
}