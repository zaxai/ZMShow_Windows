// ZComboBox.cpp: 实现文件
//

#include "stdafx.h"
#include "ZComboBox.h"


// ZComboBox

IMPLEMENT_DYNAMIC(ZComboBox, CComboBox)

ZComboBox::ZComboBox()
	: m_bIsMouseOnButton(false)
	, m_nIconIn(0)
	, m_nIconOut(0)
{

}

ZComboBox::~ZComboBox()
{
}


BEGIN_MESSAGE_MAP(ZComboBox, CComboBox)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// ZComboBox 消息处理程序




void ZComboBox::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_brushBkGnd.CreateSolidBrush(m_crClient);
	COMBOBOXINFO comboInfo;
	comboInfo.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(&comboInfo);
	if (comboInfo.hwndItem != NULL)
	{
		m_edit.SubclassWindow(comboInfo.hwndItem);
		COMBOBOXINFO comboInfo;
		comboInfo.cbSize = sizeof(COMBOBOXINFO);
		GetComboBoxInfo(&comboInfo);
		CRect rcEdit;
		rcEdit = CRect(comboInfo.rcItem);
		rcEdit.left += 8;
		m_edit.MoveWindow(rcEdit);
	}
	if (comboInfo.hwndList != NULL)
	{
		m_listbox.SubclassWindow(comboInfo.hwndList);
		m_listbox.ModifyStyle(WS_BORDER, 0);
		m_listbox.SetItemHeight(0, 15);
	}
	CComboBox::PreSubclassWindow();
}


BOOL ZComboBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CComboBox::PreTranslateMessage(pMsg);
}


HBRUSH ZComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
	{
		pDC->SetTextColor(m_crText);
		pDC->SetBkColor(m_crClient);
		return m_brushBkGnd;
	}
	break;
	}
	return hbr;
}


BOOL ZComboBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CComboBox::OnEraseBkgnd(pDC);
}


void ZComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tm;
	tm.cbSize = sizeof(TRACKMOUSEEVENT);
	tm.dwFlags = TME_LEAVE;
	tm.dwHoverTime = 3000;
	tm.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tm);
	COMBOBOXINFO comboInfo;
	comboInfo.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(&comboInfo);
	CRect rcBtn = CRect(comboInfo.rcButton);
	if (rcBtn.PtInRect(point))
		m_bIsMouseOnButton = true;
	else
		m_bIsMouseOnButton = false;
	Invalidate();
	CComboBox::OnMouseMove(nFlags, point);
}


void ZComboBox::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bIsMouseOnButton)
	{
		m_bIsMouseOnButton = false;
		Invalidate();
	}
	if (GetParent())
		GetParent()->PostMessage(WM_MOUSELEAVE);
	CComboBox::OnMouseLeave();
}


void ZComboBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CComboBox::OnPaint()
	CustDraw(&dc);
}


void ZComboBox::CustDraw(CDC * pDC)
{
	HINSTANCE hInstResource = NULL;
	HICON hIcon;
	if (m_bIsMouseOnButton)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(m_nIconIn), RT_GROUP_ICON);
		hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(m_nIconIn), IMAGE_ICON, 0, 0, 0);
	}
	else
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(m_nIconOut), RT_GROUP_ICON);
		hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(m_nIconOut), IMAGE_ICON, 0, 0, 0);
	}
	COMBOBOXINFO comboInfo;
	comboInfo.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(&comboInfo);
	CRect rcBkGnd,rcClient, rcBtn, rcLeft, rcRight, rcMid;
	GetClientRect(&rcBkGnd);
	rcBtn = CRect(comboInfo.rcButton);
	rcClient = rcBkGnd;
	rcClient.DeflateRect(0,1,0,1);
	rcMid = rcLeft = rcRight = rcClient;
	rcLeft.right = rcLeft.left + rcLeft.Height();
	rcRight.left = rcRight.right - rcRight.Height();
	rcMid.left += rcMid.Height()  / 2;
	rcMid.right -= rcMid.Height() / 2;
	CDC dcMem;                                                   //用于缓冲作图的内存DC
	CBitmap bmp;                                                 //内存中承载临时图象的位图
	dcMem.CreateCompatibleDC(pDC);                               //依附窗口DC创建兼容内存DC
	bmp.CreateCompatibleBitmap(pDC, rcBkGnd.Width(), rcBkGnd.Height());  //创建兼容位图
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	Gdiplus::SolidBrush brushBkGnd(Gdiplus::Color(GetRValue(m_crBkGnd), GetGValue(m_crBkGnd), GetBValue(m_crBkGnd)));
	Gdiplus::SolidBrush brushClient(Gdiplus::Color(GetRValue(m_crClient), GetGValue(m_crClient), GetBValue(m_crClient)));
	Gdiplus::Graphics graphics(dcMem);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//抗锯齿
	graphics.FillRectangle(&brushBkGnd, Gdiplus::Rect(rcBkGnd.left, rcBkGnd.top, rcBkGnd.Width(), rcBkGnd.Height()));
	graphics.FillEllipse(&brushClient, Gdiplus::Rect(rcLeft.left, rcLeft.top, rcLeft.Width(), rcLeft.Height()));
	graphics.FillEllipse(&brushClient, Gdiplus::Rect(rcRight.left, rcRight.top, rcRight.Width(), rcRight.Height()));
	graphics.FillRectangle(&brushClient, Gdiplus::Rect(rcMid.left, rcMid.top, rcMid.Width(), rcMid.Height()));
	::DrawIconEx(dcMem.GetSafeHdc(), rcBtn.left, rcBtn.top+2, hIcon, 0, 0, 0, NULL, DI_NORMAL | DI_COMPAT);
	::DestroyIcon(hIcon);
	pDC->BitBlt(0, 0, rcBkGnd.Width(), rcBkGnd.Height(), &dcMem, 0, 0, SRCCOPY);//将内存DC上的图象拷贝到前台
	//pDC->TransparentBlt(0,0,rcClient.Width(),rcClient.Height(),&dcMem,0,0,rcClient.Width(),rcClient.Height(),g_crBlack));//将内存DC上的图象拷贝到前台
	dcMem.SelectObject(p_bmpOld);
}


void ZComboBox::SetColor(const COLORREF & crBkGnd, const COLORREF & crClient, const COLORREF & crText)
{
	m_crBkGnd = crBkGnd;
	m_crClient = crClient;
	m_crText = crText;
	if(m_brushBkGnd.DeleteObject())
		m_brushBkGnd.CreateSolidBrush(m_crClient);
	Invalidate();
}

void ZComboBox::SetIcon(const unsigned int & nIconIn, const unsigned int & nIconOut)
{
	m_nIconIn = nIconIn;
	m_nIconOut = nIconOut;
	Invalidate();
}
