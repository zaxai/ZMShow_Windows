#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ZPlayList.h"


// CListBar 对话框

class CListBar : public CDialogEx
{
	DECLARE_DYNAMIC(CListBar)

public:
	CListBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListBar();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTBAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CBrush m_brushBkGnd;
	bool m_bIsInitFinish;
	ZTreeCtrl m_treeList;
	CImageList m_imageList;
	int m_nListID;
	CMenu m_menuList;
	ZPlayList m_playlist;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void InitTree(void);
	void ExpandTree(CTreeCtrl & Tree, HTREEITEM hTreeItem);
	afx_msg void OnNMDblclkTreeList(NMHDR *pNMHDR, LRESULT *pResult);
	void BrowseCurrentDir(const CString & strFolder, std::vector<CString> & vec_strPaths);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point);
	void InitMenu();
	HTREEITEM InsertItem(HTREEITEM hParentItem, int nClass, const CString & strName, const CString & strPath);
	void DeleteItem(HTREEITEM hItem);
	void DeleteItem(int nID);
	void RefreshCatalog(HTREEITEM hItem);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};
