#pragma once


// ZStatic

class ZStatic : public CStatic
{
	DECLARE_DYNAMIC(ZStatic)

public:
	ZStatic();
	virtual ~ZStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


