#pragma once
#include "ZButton.h"
#include "afxwin.h"


// CTitleBar 对话框

class CTitleBar : public CDialogEx
{
	DECLARE_DYNAMIC(CTitleBar)

public:
	CTitleBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTitleBar();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TITLEBAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	bool m_bIsInitFinish;
	ZButton m_btnTitle;
	ZButton m_btnMin;
	ZButton m_btnMax;
	ZButton m_btnClose;
	ZButton m_btnList;
	ZButton m_btnListMode;
	CBrush m_brushBkGnd;
	CMenu m_menuListMode;
	CString m_strListMode;
	int m_nListMode;
	ZComboBox m_comboSearch;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void InitButton(void);
	void SetBtnColor(ZButton & btn, const CString & strTip);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(const CRect & rcParent, const UINT & nID);
	afx_msg void OnBnClickedButtonTitle();
	afx_msg void OnBnClickedButtonMin();
	afx_msg void OnBnClickedButtonMax();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonListmode();
	afx_msg void OnBnClickedButtonList();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	void InitMenu(void);
	bool CheckListModeMenu(int nFirst, int nSecond, int nMinFirst, int nMaxFirst, int nMinSecond, int nMaxSecond);
	afx_msg void OnCbnEditchangeComboSearch();
	afx_msg void OnCbnSelchangeComboSearch();
	void InitComboBox();
	afx_msg void OnCbnSetfocusComboSearch();
	afx_msg void OnCbnKillfocusComboSearch();
};
