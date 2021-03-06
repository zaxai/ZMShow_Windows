#pragma once


// ZLrcListBox

class ZLrcListBox : public CListBox
{
	DECLARE_DYNAMIC(ZLrcListBox)

public:
	ZLrcListBox();
	virtual ~ZLrcListBox();

protected:
	DECLARE_MESSAGE_MAP()
private:
	int m_nSecletedItem;
	LOGFONT m_logfont;
	COLORREF m_crBkGnd;
	COLORREF m_crItem;
	COLORREF m_crSelectedItem;
	unsigned int m_nBmpBk;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	void CustDraw(CDC * pDC);
	void DrawItem(CDC* pDC, int nItem);
	void SetSelectedItem(const int & nItem);
	int GetSelectedItem(void);
	int GetPageCount(void);
	void SetFont(const LOGFONT & logfont);
	LOGFONT GetFont(void);
	void SetItemColor(const COLORREF & cr);
	COLORREF GetItemColor(void);
	void SetSelectedItemColor(const COLORREF & cr);
	COLORREF GetSelectedItemColor(void);
	void SetColor(const COLORREF & crBkGnd, const COLORREF & crItem, const COLORREF & crSelectedItem);
	void SetBmpBk(const unsigned int & nBmp = -1);
};


