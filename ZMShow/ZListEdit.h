#pragma once

#define WM_EDITKILLFOCUS WM_USER+2
// ZListEdit
#include <imm.h>  
#pragma comment (lib ,"imm32.lib") 

class ZListEdit : public CEdit
{
	DECLARE_DYNAMIC(ZListEdit)

public:
	ZListEdit();
	virtual ~ZListEdit();

protected:
	DECLARE_MESSAGE_MAP()
private:
	HIMC m_hIMC;
public:
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnKillfocus();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


