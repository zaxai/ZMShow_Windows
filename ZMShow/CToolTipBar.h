#pragma once


// CToolTipBar 对话框

class CToolTipBar : public CDialogEx
{
	DECLARE_DYNAMIC(CToolTipBar)

public:
	CToolTipBar(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CToolTipBar();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLTIPBAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	int m_nCountDown;
	CString m_strToolTip;
	CString m_strShow;
	bool m_bIsInDelayTimer;
	LOGFONT m_logfont;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	void CustDraw(CDC * pDC);
};
