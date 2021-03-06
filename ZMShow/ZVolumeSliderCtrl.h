#pragma once


// ZVolumeSliderCtrl

class ZVolumeSliderCtrl : public CSliderCtrl
{
	DECLARE_DYNAMIC(ZVolumeSliderCtrl)

public:
	ZVolumeSliderCtrl();
	virtual ~ZVolumeSliderCtrl();

protected:
	DECLARE_MESSAGE_MAP()
private:
	bool m_bIsMouseMove;
	CToolTipCtrl m_tooltip;
	COLORREF m_crBkGnd;
	COLORREF m_crChannel;
	COLORREF m_crLeftChannel;
	COLORREF m_crThumb;
public:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnPaint();
	void CustDraw(CDC * pDC);
	void SetColor(const COLORREF & crBkGnd, const COLORREF & crChannel, const COLORREF & crLeftChannel, const COLORREF & crThumb);
	void SetPos(int nPos);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


