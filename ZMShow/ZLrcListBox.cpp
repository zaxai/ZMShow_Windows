// ZLrcListBox.cpp: 实现文件
//

#include "stdafx.h"
#include "ZLrcListBox.h"


// ZLrcListBox

IMPLEMENT_DYNAMIC(ZLrcListBox, CListBox)

ZLrcListBox::ZLrcListBox()
	: m_nSecletedItem(-1)
	, m_nBmpBk(-1)
{
	m_logfont.lfHeight = -20;
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

ZLrcListBox::~ZLrcListBox()
{
}


BEGIN_MESSAGE_MAP(ZLrcListBox, CListBox)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// ZLrcListBox 消息处理程序




BOOL ZLrcListBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nBmpBk != -1)
	{
		CBitmap bmp;
		bmp.LoadBitmap(m_nBmpBk);
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);
		CRect rect;
		GetClientRect(&rect);
		//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);
		pDC->SetStretchBltMode(HALFTONE);
		pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
		dcMem.SelectObject(p_bmpOld);
	}
	return TRUE;
	return CListBox::OnEraseBkgnd(pDC);
}


void ZLrcListBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CListBox::OnPaint()
	CustDraw(&dc);
}


void ZLrcListBox::CustDraw(CDC * pDC)
{
	CBrush brushClient(m_crBkGnd);
	CRect rcClient;
	GetClientRect(&rcClient);
	CDC dcMem;
	CBitmap bmp;
	dcMem.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	dcMem.FillRect(&rcClient, &brushClient);
	int nPageCount = GetPageCount();
	int nTotalCount = GetCount();
	int nTopIndex = GetTopIndex();
	if (nTopIndex != LB_ERR)
	{
		for (int i = 0; i<nPageCount; ++i)
		{
			if (nTopIndex + i<nTotalCount)
				DrawItem(&dcMem, nTopIndex + i);
		}
	}
	if (m_nBmpBk == -1)
		pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
	else
		pDC->TransparentBlt(0,0,rcClient.Width(),rcClient.Height(),&dcMem,0,0,rcClient.Width(),rcClient.Height(), m_crBkGnd);//将内存DC上的图象拷贝到前台
	dcMem.SelectObject(p_bmpOld);
}

void ZLrcListBox::DrawItem(CDC* pDC, int nItem)
{
	CBrush brushClient(m_crBkGnd);
	CRect rcItem;
	GetItemRect(nItem, &rcItem);
	//绘制背景
	pDC->FillRect(&rcItem, &brushClient);
	//绘制文字
	CString strItem;
	GetText(nItem, strItem);
	COLORREF cr;
	LOGFONT logfont;
	if (GetSelectedItem() == nItem)
	{
		cr = m_crSelectedItem;
		logfont = m_logfont;
		logfont.lfHeight = -min(GetItemHeight(0), abs(logfont.lfHeight) * 6 / 5);
	}
	else
	{
		cr = m_crItem;
		logfont = m_logfont;
	}
	int R = 0, G = 0, B = 0;
	R = GetRValue(cr);
	G = GetGValue(cr);
	B = GetBValue(cr);
	Gdiplus::SolidBrush brush(Gdiplus::Color(R, G, B));
	Gdiplus::Font font(pDC->GetSafeHdc(), &logfont);
	Gdiplus::FontFamily fontFamily(logfont.lfFaceName);
	Gdiplus::StringFormat strformat;
	Gdiplus::GraphicsPath path;
	Gdiplus::Pen pen(Gdiplus::Color(R, G, B), 5);
	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//抗锯齿
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	strformat.SetAlignment(Gdiplus::StringAlignmentCenter);
	strformat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	strformat.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);
	strformat.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
	path.AddString(strItem, strItem.GetLength(), &fontFamily, Gdiplus::FontStyleRegular, font.GetSize(), Gdiplus::RectF((float)rcItem.left, (float)rcItem.top, (float)rcItem.Width(), (float)rcItem.Height()), &strformat);
	pen.SetLineJoin(Gdiplus::LineJoinRound);
	for (int i = 5; i > 3; --i)
	{
		pen.SetWidth(Gdiplus::REAL(i));
		pen.SetColor(Gdiplus::Color(12 * i, R, G, B));
		graphics.DrawPath(&pen, &path);
	}
	graphics.FillPath(&brush, &path);
}


void ZLrcListBox::SetSelectedItem(const int & nItem)
{
	m_nSecletedItem = nItem;
	Invalidate();
}


int ZLrcListBox::GetSelectedItem(void)
{
	return m_nSecletedItem;
}


int ZLrcListBox::GetPageCount(void)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int nHeight = GetItemHeight(0);
	int nCount = rcClient.Height() / nHeight;
	return nCount;
}


void ZLrcListBox::SetFont(const LOGFONT & logfont)
{
	m_logfont = logfont;
}


LOGFONT ZLrcListBox::GetFont(void)
{
	return m_logfont;
}


void ZLrcListBox::SetItemColor(const COLORREF & cr)
{
	m_crItem = cr;
	Invalidate();
}


COLORREF ZLrcListBox::GetItemColor(void)
{
	return m_crItem;
}


void ZLrcListBox::SetSelectedItemColor(const COLORREF & cr)
{
	m_crSelectedItem = cr;
	Invalidate();
}


COLORREF ZLrcListBox::GetSelectedItemColor(void)
{
	return m_crSelectedItem;
}


void ZLrcListBox::SetColor(const COLORREF & crBkGnd, const COLORREF & crItem, const COLORREF & crSelectedItem)
{
	m_crBkGnd = crBkGnd;
	m_crItem = crItem;
	m_crSelectedItem = crSelectedItem;
	Invalidate();
}


void ZLrcListBox::SetBmpBk(const unsigned int & nBmp)
{
	m_nBmpBk = nBmp;
	Invalidate();
}