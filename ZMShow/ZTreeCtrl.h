#pragma once


// ZTreeCtrl

class ZTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(ZTreeCtrl)

public:
	ZTreeCtrl();
	virtual ~ZTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
private:
	HTREEITEM m_hPlayItem;
	HTREEITEM m_hSelectedItem;
	HTREEITEM m_hHoverItem;
	LOGFONT m_logfont;
	COLORREF m_crBkGnd;
	COLORREF m_crBkSelected;
	COLORREF m_crTextPlay;
	COLORREF m_crTextNormal;
	CToolTipCtrl m_tooltip;
public:
	afx_msg void OnPaint();
	void CustDraw(CDC * pDC);
	void DrawItem(CDC* pDC, HTREEITEM hItem);
	void SetColor(const COLORREF & crBkGnd, const COLORREF & crBkSelected, const COLORREF & crTextPlay, const COLORREF & crTextNormal);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void SetPlayItem(const HTREEITEM & hItem);
	void SetSelectedItem(const HTREEITEM & hItem);
	HTREEITEM GetSelectedItem(void);
	HTREEITEM GetPlayItem(void);
	HTREEITEM GetItemByData(HTREEITEM hItem, const DWORD & dwData);
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
};


