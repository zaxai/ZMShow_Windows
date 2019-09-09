#pragma once
#include "afxwin.h"


// CRelation 对话框

class CRelation : public CDialogEx
{
	DECLARE_DYNAMIC(CRelation)

public:
	CRelation(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRelation();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RELATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CBrush m_brushBkGnd;
	CButton m_checkVideo;
	CButton m_checkAudio;
	CButton m_checkPic;
	CEdit m_editVideo;
	CEdit m_editAudio;
	CEdit m_editPic;
	BOOL m_nIsVideoRelated;
	BOOL m_nIsAudioRelated;
	BOOL m_nIsPicRelated;
	CString m_strVideoExt;
	CString m_strAudioExt;
	CString m_strPicExt;
	ZSetting m_setting;
	ZButton m_btnRemindVideo;
	ZButton m_btnRemindAudio;
	ZButton m_btnRemindPic;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void InitCheckBox(void);
	void InitEdit(void);
	void InitVariables(void);
	void Check(void);
	void InitButton(void);
	void SetBtnColor(ZButton & btn, const CString & strTip);
};
