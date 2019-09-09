#pragma once
#include "ZButton.h"
class ZCircleButton :public ZButton
{
private:
	COLORREF m_crTransparent;
public:
	ZCircleButton();
	~ZCircleButton();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void CustDraw(CDC * pDC);
	void DrawBackground(CDC * pDC, const CRect & rc);
	void SetTransparentColor(COLORREF crColor);
};

