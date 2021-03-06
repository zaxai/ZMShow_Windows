// ZVolumeSliderCtrl.cpp: 实现文件
//

#include "stdafx.h"
#include "ZVolumeSliderCtrl.h"


// ZVolumeSliderCtrl

IMPLEMENT_DYNAMIC(ZVolumeSliderCtrl, CSliderCtrl)

ZVolumeSliderCtrl::ZVolumeSliderCtrl()
	: m_bIsMouseMove(false)
{

}

ZVolumeSliderCtrl::~ZVolumeSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(ZVolumeSliderCtrl, CSliderCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// ZVolumeSliderCtrl 消息处理程序




void ZVolumeSliderCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_tooltip.Create(this, TTS_ALWAYSTIP);
	m_tooltip.AddTool(this, _T("30"));
	m_tooltip.SetDelayTime(TTDT_AUTOPOP, 3000);
	CSliderCtrl::PreSubclassWindow();
}


BOOL ZVolumeSliderCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_tooltip.m_hWnd)
		m_tooltip.RelayEvent(pMsg);
	return CSliderCtrl::PreTranslateMessage(pMsg);
}


BOOL ZVolumeSliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CSliderCtrl::OnEraseBkgnd(pDC);
}


void ZVolumeSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcChannel, rcThumb;
	GetChannelRect(&rcChannel);
	GetThumbRect(&rcThumb);
	if (!rcThumb.PtInRect(point))
	{
		int nMin, nMax, nPos;
		GetRange(nMin, nMax);
		if (point.x<rcChannel.left)
			nPos = nMin;
		else if (point.x>rcChannel.right)
			nPos = nMax;
		else
		{
			double dRst = 0;
			dRst = point.x - rcChannel.left;
			dRst *= (nMax - nMin);
			dRst /= rcChannel.Width();
			nPos = (int)dRst;
		}
		SetPos(nPos);
		if (GetParent())
			GetParent()->PostMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, nPos), (LPARAM)m_hWnd);
	}
	CSliderCtrl::OnLButtonDown(nFlags, point);
}


void ZVolumeSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tm;
	tm.cbSize = sizeof(TRACKMOUSEEVENT);
	tm.dwFlags = TME_LEAVE;
	tm.dwHoverTime = 3000;
	tm.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tm);
	if (!m_bIsMouseMove)
	{
		m_bIsMouseMove = true;
		Invalidate();
	}
	CString str;
	str.Format(_T("%d"), GetPos());
	CString strLast;
	m_tooltip.GetText(strLast, this);
	if (strLast != str)
		m_tooltip.UpdateTipText(str, this);
	CSliderCtrl::OnMouseMove(nFlags, point);
}


void ZVolumeSliderCtrl::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bIsMouseMove)
	{
		m_bIsMouseMove = false;
		Invalidate();
	}
	if (GetParent())
		GetParent()->PostMessage(WM_MOUSELEAVE);
	CSliderCtrl::OnMouseLeave();
}


void ZVolumeSliderCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CSliderCtrl::OnPaint()
	CustDraw(&dc);
}

void ZVolumeSliderCtrl::CustDraw(CDC * pDC)
{
	CBrush brushLeft(m_crLeftChannel);
	CBrush brushChannel(m_crChannel);
	COLORREF crThumb = m_crThumb;
	CBrush brushClient(m_crBkGnd);
	CRect rcClient, rcThumb, rcChannel, rcLeft;
	CPoint pt;
	GetClientRect(&rcClient);
	GetThumbRect(&rcThumb);
	GetChannelRect(&rcChannel);
	pt = rcThumb.CenterPoint();
	rcLeft.SetRect(rcChannel.left, rcChannel.top, pt.x, rcChannel.bottom);
	rcThumb.SetRect(pt.x - rcThumb.Width() * 2 / 3, pt.y - rcThumb.Width() * 2 / 3, pt.x + rcThumb.Width() * 2 / 3, pt.y + rcThumb.Width() * 2 / 3);
	CDC dcMem;                                                   //用于缓冲作图的内存DC
	CBitmap bmp;                                                 //内存中承载临时图象的位图
	dcMem.CreateCompatibleDC(pDC);                               //依附窗口DC创建兼容内存DC
	bmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());  //创建兼容位图
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	dcMem.FillRect(&rcClient, &brushClient);
	dcMem.FillRect(&rcChannel, &brushChannel);
	dcMem.FillRect(&rcLeft, &brushLeft);
	Gdiplus::SolidBrush brush(Gdiplus::Color(GetRValue(crThumb), GetGValue(crThumb), GetBValue(crThumb)));
	Gdiplus::Graphics graphics(dcMem);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//抗锯齿
	graphics.FillEllipse(&brush, Gdiplus::Rect(rcThumb.left, rcThumb.top, rcThumb.Width(), rcThumb.Height()));
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);//将内存DC上的图象拷贝到前台
	//pDC->TransparentBlt(0,0,rcClient.Width(),rcClient.Height(),&dcMem,0,0,rcClient.Width(),rcClient.Height(),g_crBlack));//将内存DC上的图象拷贝到前台
	dcMem.SelectObject(p_bmpOld);
}

void ZVolumeSliderCtrl::SetColor(const COLORREF & crBkGnd, const COLORREF & crChannel, const COLORREF & crLeftChannel, const COLORREF & crThumb)
{
	m_crBkGnd = crBkGnd;
	m_crChannel = crChannel;
	m_crLeftChannel = crLeftChannel;
	m_crThumb = crThumb;
	Invalidate();
}

void ZVolumeSliderCtrl::SetPos(int nPos)
{
	CSliderCtrl::SetPos(nPos);
	Invalidate();
}

BOOL ZVolumeSliderCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (GetParent())
	{
		GetParent()->PostMessage(WM_MOUSEWHEEL, MAKEWPARAM(nFlags, zDelta), MAKELPARAM(pt.x, pt.y));
		return TRUE;
	}
	return CSliderCtrl::OnMouseWheel(nFlags, zDelta, pt);
}
