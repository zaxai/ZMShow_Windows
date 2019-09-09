#pragma once
#include "afxcmn.h"
#include "ZButton.h"
#include "Common.h"
#include "Relation.h"
#include "HotKey.h"
// CSetting 对话框

class CSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CSetting)

public:
	CSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	ZTabCtrl m_tabSetting;
	ZButton m_btnClose;
	ZButton m_btnCancel;
	ZButton m_btnApply;
	ZButton m_btnOK;
	ZButton m_btnRecoverPage;
	CCommon m_common;
	CRelation m_relation;
	CHotKey m_hotkey;
	CWnd * m_sz_p_wndTab[3];
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitButton(void);
	void SetBtnColor(ZButton & btn, const CString & strTip);
	void InitTab(void);
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonRecoverpage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTcnSelchangeTabSetting(NMHDR *pNMHDR, LRESULT *pResult);
};
