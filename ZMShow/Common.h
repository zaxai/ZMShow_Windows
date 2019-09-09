#pragma once
#include "afxwin.h"


// CCommon 对话框

class CCommon : public CDialogEx
{
	DECLARE_DYNAMIC(CCommon)

public:
	CCommon(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCommon();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CBrush m_brushBkGnd;
	CButton m_checkAutoRun;
	CButton m_checkClearHistory;
	CButton m_checkSavePlayProgress;
	CButton m_checkFolderAutoSel;
	BOOL m_nIsAutoRun;
	BOOL m_nIsClearHistory;
	BOOL m_nIsSavePlayProgress;
	BOOL m_nIsFolderAutoSel;
	ZSetting m_setting;
	ZButton m_btnRemind;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void InitCheckBox(void);
	void Check(void);
	void InitButton(void);
	void SetBtnColor(ZButton & btn, const CString & strTip);
};
