#pragma once
#include <map>

// CLyricBar 对话框

class CLyricBar : public CDialogEx
{
	DECLARE_DYNAMIC(CLyricBar)

public:
	CLyricBar(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLyricBar();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LYRICBAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CBrush m_brushBkGnd;
	ZLrcListBox m_listLrc;
	bool m_bIsInitFinish;
	std::map<int, CString> m_map_Lrc;
	ZButton m_btnDeskLrc;
	ZButton m_btnFont;
	ZButton m_btnSizeUp;
	ZButton m_btnSizeDown;
	ZCircleButton m_btnColrBk;
	ZCircleButton m_btnColrFg;
	int m_nHitTest;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void InitListBox(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ProLyric(const CString & strLyric);
	void InsertListBox(void);
	BOOL IsDigidit(TCHAR ch);
	void InitButton(void);
	void SetBtnColor(ZButton & btn, const CString & strTip);
	afx_msg void OnBnClickedButtonDesklrc();
	afx_msg void OnBnClickedButtonFont();
	afx_msg void OnBnClickedButtonSizeup();
	afx_msg void OnBnClickedButtonSizedown();
	afx_msg void OnBnClickedButtonColorbk();
	afx_msg void OnBnClickedButtonColorfg();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	int HitTest(CPoint point);
	void ScaleWnd(int nHitTest, CPoint point);
	void ShowButton(const int & nShowFlag);
	void ChangeSize(const CRect & rcParent, const UINT & nID);
};
