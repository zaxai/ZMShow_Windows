// ZTabCtrl.cpp: 实现文件
//

#include "stdafx.h"
#include "ZTabCtrl.h"


// ZTabCtrl

IMPLEMENT_DYNAMIC(ZTabCtrl, CTabCtrl)

ZTabCtrl::ZTabCtrl()
	: m_nSelectedItem(0)
	, m_nHoverItem(-1)
{
	m_logfont.lfHeight = -14;
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
	_tcscpy_s(m_logfont.lfFaceName, _T("宋体"));
}

ZTabCtrl::~ZTabCtrl()
{
}


BEGIN_MESSAGE_MAP(ZTabCtrl, CTabCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// ZTabCtrl 消息处理程序




void ZTabCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CTabCtrl::OnPaint()
	CustDraw(&dc);
}


void ZTabCtrl::CustDraw(CDC * pDC)
{
	CBrush brushClient(m_crBkGnd);
	CBrush brushLeft(m_crBkLeft);
	CRect rcClient, rcItem, rcLeft;
	GetClientRect(&rcClient);
	GetItemRect(0, &rcItem);
	rcLeft = rcClient;
	rcLeft.right = rcItem.right;
	CDC dcMem;                                                           //用于缓冲作图的内存DC
	CBitmap bmp;                                                         //内存中承载临时图象的位图
	dcMem.CreateCompatibleDC(pDC);                                       //依附窗口DC创建兼容内存DC
	bmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());  //创建兼容位图
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	dcMem.FillRect(&rcClient, &brushClient);
	dcMem.FillRect(&rcLeft, &brushLeft);
	int nCount = GetItemCount();
	for (int i = 0; i<nCount; ++i)
	{
		DrawItem(&dcMem, i);
	}
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);//将内存DC上的图象拷贝到前台
	//pDC->TransparentBlt(0,0,rcClient.Width(),rcClient.Height(),&dcMem,0,0,rcClient.Width(),rcClient.Height(),crBlack);//将内存DC上的图象拷贝到前台
	dcMem.SelectObject(p_bmpOld);
}


void ZTabCtrl::DrawItem(CDC* pDC, int nItem)
{
	CBrush brushClient(m_crBkLeft);
	CBrush brushSelected(m_crBkSelected);
	CBrush brushHover(m_crBkHover);
	CRect rcItem;
	GetItemRect(nItem, &rcItem);
	//绘制背景
	if (nItem == m_nSelectedItem)
		pDC->FillRect(&rcItem, &brushSelected);
	else if (nItem == m_nHoverItem)
		pDC->FillRect(&rcItem, &brushHover);
	else
		pDC->FillRect(&rcItem, &brushClient);
	//绘制文字
	CString strText;
	TC_ITEM tcItem;
	memset(&tcItem, 0, sizeof(tcItem));
	TCHAR szText[100] = { 0 };
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = szText;
	tcItem.cchTextMax = sizeof(szText) / sizeof(TCHAR);
	if (GetItem(nItem, &tcItem))
		strText = tcItem.pszText;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_crText);
	CFont font;
	font.CreateFontIndirect(&m_logfont);
	CFont * p_fontOld = pDC->SelectObject(&font);
	pDC->DrawText(strText, rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	pDC->SelectObject(p_fontOld);
}


void ZTabCtrl::SetColor(const COLORREF & crBkGnd, const COLORREF & crBkLeft, const COLORREF & crBkSelected, const COLORREF & crBkHover, const COLORREF & crText)
{
	m_crBkGnd = crBkGnd;
	m_crBkLeft = crBkLeft;
	m_crBkSelected = crBkSelected;
	m_crBkHover = crBkHover;
	m_crText = crText;
	Invalidate();
}

void ZTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TCHITTESTINFO tcInfo;
	tcInfo.pt = point;
	int nItem = HitTest(&tcInfo);
	if (nItem != -1)
	{
		m_nSelectedItem = nItem;
		Invalidate();
	}
	CTabCtrl::OnLButtonDown(nFlags, point);
}


void ZTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tm;
	tm.cbSize = sizeof(TRACKMOUSEEVENT);
	tm.dwFlags = TME_LEAVE;
	tm.dwHoverTime = 500;
	tm.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tm);
	TCHITTESTINFO tcInfo;
	tcInfo.pt = point;
	int nItem = HitTest(&tcInfo);
	if (nItem != -1)
	{
		m_nHoverItem = nItem;
	}
	else
		m_nHoverItem = -1;
	CRect rcItem, rcLeft;
	GetClientRect(&rcLeft);
	GetItemRect(0, &rcItem);
	rcLeft.right = rcItem.right;
	InvalidateRect(&rcLeft);
	CTabCtrl::OnMouseMove(nFlags, point);
}


void ZTabCtrl::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nHoverItem = -1;
	Invalidate();
	CTabCtrl::OnMouseLeave();
}


BOOL ZTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CTabCtrl::OnEraseBkgnd(pDC);
}


int ZTabCtrl::GetSelectedItem(void)
{
	return m_nSelectedItem;
}