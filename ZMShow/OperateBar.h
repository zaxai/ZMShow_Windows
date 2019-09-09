#pragma once
#include "afxwin.h"
#include "ZButton.h"
#include "afxcmn.h"


// COperateBar 对话框

class COperateBar : public CDialogEx
{
	DECLARE_DYNAMIC(COperateBar)

public:
	COperateBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COperateBar();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPERATEBAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CBrush m_brushBkGnd;
	ZButton m_btnTimeInfo;
	ZButton m_btnStop;
	ZButton m_btnPrev;
	ZButton m_btnPlay;
	ZButton m_btnNext;
	ZButton m_btnVolume;
	ZButton m_btnFull;
	bool m_bIsInitFinish;
	ZTimeSliderCtrl m_sliderTime;
	ZVolumeSliderCtrl m_sliderVolume;
	CString m_strToolTip;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void InitButton(void);
	void SetBtnColor(ZButton & btn, const CString & strTip);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(const CRect & rcParent, const UINT & nID);
	afx_msg void OnBnClickedButtonTimeinfo();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonVolume();
	afx_msg void OnBnClickedButtonFull();
	void InitSlider(void);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
