// ZTreeCtrl.cpp: 实现文件
//

#include "stdafx.h"
#include "ZTreeCtrl.h"


// ZTreeCtrl

IMPLEMENT_DYNAMIC(ZTreeCtrl, CTreeCtrl)

ZTreeCtrl::ZTreeCtrl()
	: m_hPlayItem(NULL)
	, m_hSelectedItem(NULL)
	, m_hHoverItem(NULL)
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
	_tcscpy_s(m_logfont.lfFaceName, _T("微软雅黑"));
}

ZTreeCtrl::~ZTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(ZTreeCtrl, CTreeCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_VSCROLL()
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()



// ZTreeCtrl 消息处理程序




void ZTreeCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CTreeCtrl::OnPaint()
	CustDraw(&dc);
}


void ZTreeCtrl::CustDraw(CDC * pDC)
{
	CBrush brushClient(m_crBkGnd);
	CRect rcClient;
	GetClientRect(&rcClient);
	CDC dcMem;                                                           //用于缓冲作图的内存DC
	CBitmap bmp;                                                         //内存中承载临时图象的位图
	dcMem.CreateCompatibleDC(pDC);                                       //依附窗口DC创建兼容内存DC
	bmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());  //创建兼容位图
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	dcMem.FillRect(&rcClient, &brushClient);
	HTREEITEM hItem;
	hItem = GetFirstVisibleItem();
	while (hItem)
	{
		DrawItem(&dcMem, hItem);
		hItem = GetNextVisibleItem(hItem);
	}
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);//将内存DC上的图象拷贝到前台
	//pDC->TransparentBlt(0,0,rcClient.Width(),rcClient.Height(),&dcMem,0,0,rcClient.Width(),rcClient.Height(),g_crBlack);//将内存DC上的图象拷贝到前台
	dcMem.SelectObject(p_bmpOld);
}

void ZTreeCtrl::DrawItem(CDC* pDC, HTREEITEM hItem)
{
	CBrush brushClient(m_crBkGnd);
	CBrush brushSelected(m_crBkSelected);
	CRect rcItem, rcImage, rcText;
	GetItemRect(hItem, &rcItem, FALSE);
	//绘制背景
	if (hItem == m_hSelectedItem || hItem == m_hHoverItem)
		pDC->FillRect(&rcItem, &brushSelected);
	else
		pDC->FillRect(&rcItem, &brushClient);
	//绘制图片
	int nImage, nSelectedImage;
	CImageList* pImageList = GetImageList(TVSIL_NORMAL);
	if (pImageList)
	{
		GetItemImage(hItem, nImage, nSelectedImage);
		IMAGEINFO imageInfo;
		pImageList->GetImageInfo(0, &imageInfo);
		rcImage = rcItem;
		rcImage.left += 3;
		rcImage.top += (rcItem.Height() - CRect(imageInfo.rcImage).Height()) / 2;
		rcImage.right = rcImage.left + CRect(imageInfo.rcImage).Width();
		if (TVIS_EXPANDED&GetItemState(hItem, TVIS_EXPANDED))
			pImageList->Draw(pDC, nSelectedImage, rcImage.TopLeft(), ILD_TRANSPARENT);
		else
			pImageList->Draw(pDC, nImage, rcImage.TopLeft(), ILD_TRANSPARENT);
	}
	//绘制文字
	CString strText;
	strText = GetItemText(hItem);
	pDC->SetBkMode(TRANSPARENT);
	if (m_hPlayItem == hItem)
		pDC->SetTextColor(m_crTextPlay);
	else
		pDC->SetTextColor(m_crTextNormal);
	rcText = rcItem;
	rcText.left = rcImage.right;
	CFont font;
	font.CreateFontIndirect(&m_logfont);
	CFont * p_fontOld = pDC->SelectObject(&font);
	pDC->DrawText(strText, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	pDC->SelectObject(p_fontOld);
}

void ZTreeCtrl::SetColor(const COLORREF & crBkGnd, const COLORREF & crBkSelected, const COLORREF & crTextPlay, const COLORREF & crTextNormal)
{
	m_crBkGnd = crBkGnd;
	m_crBkSelected = crBkSelected;
	m_crTextPlay = crTextPlay;
	m_crTextNormal = crTextNormal;
	Invalidate();
}

void ZTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HTREEITEM hItem;
	hItem = HitTest(point);
	if (hItem&&hItem != GetRootItem())
	{
		m_hSelectedItem = hItem;
		if (GetParentItem(hItem) == GetRootItem())
		{
			SetRedraw(FALSE);
			Expand(hItem, TVE_TOGGLE);
			SetRedraw(TRUE);
		}
		Invalidate();
	}
	CTreeCtrl::OnLButtonDown(nFlags, point);
}


void ZTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tm;
	tm.cbSize = sizeof(TRACKMOUSEEVENT);
	tm.dwFlags = TME_LEAVE;
	tm.dwHoverTime = 500;
	tm.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tm);
	static HTREEITEM hItemLast = NULL;
	HTREEITEM hItem;
	hItem = HitTest(point);
	if (hItem&&hItem != GetRootItem())
	{
		m_hHoverItem = hItem;
		CString str;
		str=GetItemText(hItem);
		if (hItem != hItemLast)
		{
			m_tooltip.UpdateTipText(str, this);
			hItemLast = hItem;
		}
	}
	else
		m_hHoverItem = NULL;
	Invalidate();
	CTreeCtrl::OnMouseMove(nFlags, point);
}


void ZTreeCtrl::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_hHoverItem = NULL;
	Invalidate();
	if (GetParent())
		GetParent()->PostMessage(WM_MOUSELEAVE);
	CTreeCtrl::OnMouseLeave();
}


BOOL ZTreeCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CTreeCtrl::OnEraseBkgnd(pDC);
}


void ZTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HTREEITEM hItem;
	hItem = HitTest(point);
	if (hItem&&hItem != GetRootItem()&& GetParentItem(hItem) != GetRootItem())
	{
		m_hPlayItem = hItem;
		Invalidate();
		CTreeCtrl::OnLButtonDblClk(nFlags, point);
	}
}


void ZTreeCtrl::SetPlayItem(const HTREEITEM & hItem)
{
	m_hPlayItem = hItem;
	Invalidate();
}


void ZTreeCtrl::SetSelectedItem(const HTREEITEM & hItem)
{
	m_hSelectedItem = hItem;
	Invalidate();
}


HTREEITEM ZTreeCtrl::GetPlayItem(void)
{
	return m_hPlayItem;
}


HTREEITEM ZTreeCtrl::GetSelectedItem(void)
{
	return m_hSelectedItem;
}


HTREEITEM ZTreeCtrl::GetItemByData(HTREEITEM hItem, const DWORD & dwData)
{
	DWORD dw = GetItemData(hItem);
	if (dwData == dw)
		return hItem;
	else
	{
		if (ItemHasChildren(hItem))
		{
			HTREEITEM hNextItem = GetChildItem(hItem);
			while (hNextItem != NULL)
			{
				hItem = GetItemByData(hNextItem, dwData);
				if (hItem)
					return hItem;
				hNextItem = GetNextItem(hNextItem, TVGN_NEXT);
			}
		}
	}
	return HTREEITEM(NULL);
}

void ZTreeCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_tooltip.Create(this, TTS_ALWAYSTIP);
	m_tooltip.AddTool(this, _T(""));
	m_tooltip.SetDelayTime(TTDT_AUTOPOP, 3000);
	CTreeCtrl::PreSubclassWindow();
}


BOOL ZTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_tooltip.m_hWnd)
		m_tooltip.RelayEvent(pMsg);
	return CTreeCtrl::PreTranslateMessage(pMsg);
}


void ZTreeCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetRedraw(FALSE);
	SetRedraw(TRUE);
	CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}


void ZTreeCtrl::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetRedraw(FALSE);
	EnsureVisible(m_hPlayItem);
	SetRedraw(TRUE);
	CTreeCtrl::OnMButtonDown(nFlags, point);
}
