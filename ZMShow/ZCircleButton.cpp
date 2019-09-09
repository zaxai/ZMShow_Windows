#include "stdafx.h"
#include "ZCircleButton.h"


ZCircleButton::ZCircleButton()
{
	m_crTransparent = RGB(0, 0, 0);
}


ZCircleButton::~ZCircleButton()
{
}


void ZCircleButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项
	CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	m_nIsFocused = (lpDrawItemStruct->itemState & ODS_FOCUS);
	m_nIsDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED);
	CustDraw(pDC);
}


void ZCircleButton::CustDraw(CDC * pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	CDC dcMem;
	CBitmap bmp;
	dcMem.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	DrawBackground(&dcMem, rcClient);
	DrawBorder(&dcMem, rcClient);
	DrawText(&dcMem, rcClient);
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(p_bmpOld);
}

void ZCircleButton::DrawBackground(CDC * pDC, const CRect & rc)
{
	COLORREF crBk;
	HINSTANCE hInstResource = NULL;
	HICON hIcon = NULL;
	CBitmap bmpBk;
	BOOL nBmpRslt = FALSE;
	if (m_nMouseOnButton)
	{
		crBk = m_crColors[COLOR_BK_IN];
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(m_nIconIn), RT_GROUP_ICON);
		hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(m_nIconIn), IMAGE_ICON, 0, 0, 0);
		nBmpRslt = bmpBk.LoadBitmap(m_nBmpIn);
	}
	else
	{
		crBk = m_crColors[COLOR_BK_OUT];
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(m_nIconOut), RT_GROUP_ICON);
		hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(m_nIconOut), IMAGE_ICON, 0, 0, 0);
		nBmpRslt = bmpBk.LoadBitmap(m_nBmpOut);
	}
	CBrush brushTran(m_crTransparent);
	pDC->FillRect(&rc, &brushTran);
	if (m_nDrawTransparent)
	{
		if (GetParent())
		{
			CRect rcInParent(rc);
			CClientDC dcParent(GetParent());
			ClientToScreen(&rcInParent);
			GetParent()->ScreenToClient(&rcInParent);
			if (m_dcBk.m_hDC == NULL)
			{
				m_dcBk.CreateCompatibleDC(&dcParent);
				m_bmpBk.CreateCompatibleBitmap(&dcParent, rc.Width(), rc.Height());
				m_p_bmpBkOld = m_dcBk.SelectObject(&m_bmpBk);
				m_dcBk.BitBlt(0, 0, rc.Width(), rc.Height(), &dcParent, rcInParent.left, rcInParent.top, SRCCOPY);
			}
			pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &m_dcBk, 0, 0, SRCCOPY);
		}
	}
	if (m_nMouseOnButton)
	{
		CRect rcBk(rc);
		rcBk.right -= 7;
		rcBk.bottom -= 9;
		if (m_nIsPressed)
			rcBk.OffsetRect(1, 1);
		Gdiplus::SolidBrush solidBrush(Gdiplus::Color(GetRValue(crBk), GetGValue(crBk), GetBValue(crBk)));
		Gdiplus::Graphics graphics(*pDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//抗锯齿
		graphics.FillEllipse(&solidBrush, Gdiplus::Rect(rcBk.left, rcBk.top, rcBk.Width(), rcBk.Height()));
	}
	if (hIcon)
	{
		CRect rcIcon(rc);
		if (m_nIsPressed)
			rcIcon.OffsetRect(1, 1);
		//pDC->DrawIcon(rcIcon.left,rcIcon.top,hIcon);
		::DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, hIcon, 0, 0, 0, NULL, DI_NORMAL | DI_COMPAT);
		::DestroyIcon(hIcon);
	}
	if (nBmpRslt)
	{
		CRect rcBmp(rc);
		if (m_nIsPressed)
			rcBmp.OffsetRect(1, 1);
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap * p_bmpBkOld = dcMem.SelectObject(&bmpBk);
		BITMAP bmpInfo;
		bmpBk.GetBitmap(&bmpInfo);
		pDC->SetStretchBltMode(HALFTONE);
		pDC->StretchBlt(rcBmp.left, rcBmp.top, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMem, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
		dcMem.SelectObject(p_bmpBkOld);
	}
}

void ZCircleButton::SetTransparentColor(COLORREF crColor)
{
	m_crTransparent = crColor;
	Invalidate();
}