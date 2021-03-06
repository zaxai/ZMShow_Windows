#pragma once
#include "ZComboListBox.h"

// ZComboBox

class ZComboBox : public CComboBox
{
	DECLARE_DYNAMIC(ZComboBox)

public:
	ZComboBox();
	virtual ~ZComboBox();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
private:
	CEdit m_edit;
	ZComboListBox m_listbox;
	bool m_bIsMouseOnButton;
	unsigned int m_nIconIn;
	unsigned int m_nIconOut;
	COLORREF m_crBkGnd;
	COLORREF m_crClient;
	COLORREF m_crText;
	CBrush m_brushBkGnd;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnPaint();
	void CustDraw(CDC * pDC);
	void SetColor(const COLORREF & crBkGnd, const COLORREF & crClient, const COLORREF & crText);
	void SetIcon(const unsigned int & nIconIn, const unsigned int & nIconOut);
};


