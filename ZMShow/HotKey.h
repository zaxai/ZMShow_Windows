#pragma once
#include "afxcmn.h"


// CHotKey �Ի���

class CHotKey : public CDialogEx
{
	DECLARE_DYNAMIC(CHotKey)

public:
	CHotKey(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHotKey();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOTKEY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CBrush m_brushBkGnd;
	ZListCtrl m_listHotKey;
	ZSetting m_setting;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void InitList(void);
	void InsertListData(void);
	void InitVariables();
	void Check();
	bool IsHotKeyRepeat(int nIndex, const CString & strHotKey);
};
