#pragma once


// ZComboListBox

class ZComboListBox : public CListBox
{
	DECLARE_DYNAMIC(ZComboListBox)

public:
	ZComboListBox();
	virtual ~ZComboListBox();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CToolTipCtrl m_tooltip;
	virtual void PreSubclassWindow();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


