#pragma once


// ZTabCtrl

class ZTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(ZTabCtrl)

public:
	ZTabCtrl();
	virtual ~ZTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_crBkGnd;
	COLORREF m_crBkLeft;
	COLORREF m_crBkSelected;
	COLORREF m_crBkHover;
	COLORREF m_crText;
	int m_nSelectedItem;
	int m_nHoverItem;
	LOGFONT m_logfont;
public:
	afx_msg void OnPaint();
	void CustDraw(CDC * pDC);
	void DrawItem(CDC* pDC, int nItem);
	void SetColor(const COLORREF & crBkGnd, const COLORREF & crBkLeft, const COLORREF & crBkSelected, const COLORREF & crBkHover, const COLORREF & crText);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int GetSelectedItem(void);
};


