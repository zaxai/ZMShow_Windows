// ListBar.cpp : 实现文件
//

#include "stdafx.h"
#include "ZMShow.h"
#include "ListBar.h"
#include "afxdialogex.h"


// CListBar 对话框

IMPLEMENT_DYNAMIC(CListBar, CDialogEx)

CListBar::CListBar(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LISTBAR, pParent)
	, m_bIsInitFinish(false)
	, m_nListID(0)
{

}

CListBar::~CListBar()
{
	ZSetting setting;
	if (setting.CheckClearHistory())
	{
		m_playlist.DeleteAllItem();
	}
}

void CListBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_LIST, m_treeList);
}


BEGIN_MESSAGE_MAP(CListBar, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_MESSAGE(WM_MSGRECVPRO, &CListBar::OnMsgrecvpro)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_LIST, &CListBar::OnNMDblclkTreeList)
	ON_WM_CONTEXTMENU()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


// CListBar 消息处理程序


BOOL CListBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_sz_p_wnd[WND_LIS] = this;
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	m_brushBkGnd.CreateSolidBrush(g_crBlack);
	InitTree();
	InitMenu();
	m_bIsInitFinish = true;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CListBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return m_brushBkGnd;
	return hbr;
}


void CListBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_bIsInitFinish&&nType != SIZE_MINIMIZED)  //初始化好了后才能变换
	{
		CRect rcTree;
		GetClientRect(&rcTree);
		m_treeList.MoveWindow(rcTree);
		//RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_ALLCHILDREN);
	}
}

void CListBar::InitTree(void)
{
	const int nIconNum = 2;
	HICON hIcon[nIconNum];
	HTREEITEM hRoot=NULL, hFirst=NULL, hSecond=NULL;
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_ICON_COLLAPSE);
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON_EXPAND);
	m_imageList.Create(16, 16, ILC_COLOR32, nIconNum, nIconNum);
	for (int i = 0; i < nIconNum; ++i)
		m_imageList.Add(hIcon[i]);
	m_treeList.SetImageList(&m_imageList, TVSIL_NORMAL);
	m_treeList.SetColor(g_crBlack, g_crGrey55, g_crBlue, g_crWhite);
	m_treeList.SetItemHeight(25);
	m_nListID = 0;
	if (m_playlist.IsExist())
	{
		std::vector<std::vector <CString>>  vec2_strData;
		m_playlist.GetAllItem(vec2_strData);
		int nRow = vec2_strData.size();
		if (nRow)
		{
			int nColumn = vec2_strData[0].size();
			if (nColumn == 6)
			{
				for (int i = 1; i < nRow; ++i)
				{
					int nID, nParentID, nClass;
					CString strName;
					nID = _ttoi(vec2_strData[i][0]);
					nParentID = _ttoi(vec2_strData[i][1]);
					nClass = _ttoi(vec2_strData[i][2]);
					strName = vec2_strData[i][3];
					m_nListID = m_nListID > nID ? m_nListID : nID;
					switch (nClass)
					{
					case 0:
					{
						hRoot = m_treeList.InsertItem(strName, 2, 2);
						m_treeList.SetItemData(hRoot, nID);
					}
					break;
					case 1:
					{
						hFirst = m_treeList.InsertItem(strName, 0, 1, hRoot);
						m_treeList.SetItemData(hFirst, nID);
					}
					break;
					case 2:
					{
						hFirst = m_treeList.GetItemByData(m_treeList.GetRootItem(), nParentID);
						if (hFirst)
						{
							hSecond = m_treeList.InsertItem(strName, 2, 2, hFirst);
							m_treeList.SetItemData(hSecond, nID);
						}
					}
					break;
					}
				}
				++m_nListID;
			}
		}
		else
			hRoot = InsertItem(NULL, 0, _T("播放列表"), _T("播放列表"));
	}
	else
	{
		hRoot=InsertItem(NULL, 0, _T("播放列表"), _T("播放列表"));
	}
	ExpandTree(m_treeList, m_treeList.GetRootItem());
}

void CListBar::ExpandTree(CTreeCtrl & Tree, HTREEITEM hTreeItem)
{
	if (!hTreeItem)
		return;
	if (!Tree.ItemHasChildren(hTreeItem))
		return;
	HTREEITEM hNextItem = Tree.GetChildItem(hTreeItem);
	while (hNextItem != NULL)
	{
		ExpandTree(Tree, hNextItem);
		hNextItem = Tree.GetNextItem(hNextItem, TVGN_NEXT);
	}
	Tree.Expand(hTreeItem, TVE_EXPAND);
}

afx_msg LRESULT CListBar::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_PREVBTN:
	{
		HTREEITEM hItem = m_treeList.GetPlayItem();
		if (hItem)
		{
			int nID = m_treeList.GetItemData(hItem);
			int nNextID = m_playlist.GetNextID(nID,wParam,true);
			if (nNextID != -1)
			{
				m_treeList.SetRedraw(FALSE);
				HTREEITEM hNextItem = m_treeList.GetItemByData(m_treeList.GetRootItem(), nNextID);
				m_treeList.EnsureVisible(hNextItem);
				m_treeList.SetPlayItem(hNextItem);
				CString strPath;
				strPath = m_playlist.GetPath(nNextID);
				g_sz_p_wnd[WND_ZMS]->SendMessage(WM_MSGRECVPRO, (WPARAM)&strPath, MSGUSER_PLAYFILE);
				m_treeList.SetRedraw(TRUE);
			}
		}
	}
	break;
	case MSGUSER_NEXTBTN:
	{
		HTREEITEM hItem = m_treeList.GetPlayItem();
		if (hItem)
		{
			int nID = m_treeList.GetItemData(hItem);
			int nNextID = m_playlist.GetNextID(nID, wParam, false);
			if (nNextID != -1)
			{
				m_treeList.SetRedraw(FALSE);
				HTREEITEM hNextItem=m_treeList.GetItemByData(m_treeList.GetRootItem(), nNextID);
				m_treeList.EnsureVisible(hNextItem);
				m_treeList.SetPlayItem(hNextItem);
				CString strPath;
				strPath = m_playlist.GetPath(nNextID);
				g_sz_p_wnd[WND_ZMS]->SendMessage(WM_MSGRECVPRO, (WPARAM)&strPath, MSGUSER_PLAYFILE);
				m_treeList.SetRedraw(TRUE);
			}
		}
	}
	break;
	case MSGUSER_FILEINSERT:
	{
		CString *p_str = (CString *)wParam;
		HTREEITEM hRoot = NULL, hFirst = NULL, hSecond = NULL;
		int nID = m_playlist.GetIDByPath(*p_str);
		hRoot = m_treeList.GetRootItem();
		if (nID != -1)
		{
			hSecond = m_treeList.GetItemByData(hRoot, nID);
		}
		else
		{
			int nDefaultID= m_playlist.GetIDByPath(_T("默认目录"));
			if (nDefaultID != -1)
			{
				hFirst = m_treeList.GetItemByData(hRoot, nDefaultID);
			}
			else
			{
				hFirst = InsertItem(hRoot, 1, _T("默认目录"), _T("默认目录"));
			}
			hSecond = InsertItem(hFirst, 2, p_str->Mid(p_str->ReverseFind(_T('\\')) + 1), *p_str);
		}
		m_treeList.SetPlayItem(hSecond);
		m_treeList.EnsureVisible(hSecond);
	}
	break;
	case MSGUSER_FOLDERINSERT:
	{
		CString *p_str = (CString *)wParam;
		HTREEITEM hRoot = NULL, hFirst = NULL, hSecond = NULL;
		int nID = m_playlist.GetIDByPath(*p_str);
		hRoot = m_treeList.GetRootItem();
		if (nID != -1)
		{
			hFirst = m_treeList.GetItemByData(hRoot, nID);
		}
		else
		{
			hFirst = InsertItem(hRoot, 1, p_str->Mid(p_str->ReverseFind(_T('\\')) + 1), *p_str);
			std::vector<CString> vec_strPaths;
			BrowseCurrentDir(*p_str, vec_strPaths);
			int nSize = vec_strPaths.size();
			ZSetting setting;
			for (int i = 0; i < nSize; ++i)
			{
				if (setting.CheckFolderAutoSel())
				{
					if (ZPlayer::GetFileType(vec_strPaths[i]) == ZPlayer::TYPE_OTHER)
						continue;
				}
				vec_strPaths[i] = _T("file:///") + vec_strPaths[i];
				hSecond = InsertItem(hFirst, 2, vec_strPaths[i].Mid(vec_strPaths[i].ReverseFind(_T('\\')) + 1), vec_strPaths[i]);
			}
		}
		m_treeList.Expand(hFirst, TVE_EXPAND);
		m_treeList.EnsureVisible(hFirst);
	}
	break;
	case MSGUSER_ITEMPLAY:
	{
		HTREEITEM hItem = NULL;
		hItem = m_treeList.GetSelectedItem();
		if (hItem)
		{
			m_treeList.SetPlayItem(hItem);
			CString strPath;
			strPath = m_playlist.GetPath(m_treeList.GetItemData(hItem));
			g_sz_p_wnd[WND_ZMS]->SendMessage(WM_MSGRECVPRO, (WPARAM)&strPath, MSGUSER_PLAYFILE);
		}
	}
	break;
	case MSGUSER_ITEMDELETE:
	{
		HTREEITEM hItem = NULL;
		hItem = m_treeList.GetSelectedItem();
		DeleteItem(hItem);
	}
	break;
	case MSGUSER_OPENFILECATALOG:
	{
		HTREEITEM hItem = NULL;
		hItem = m_treeList.GetSelectedItem();
		if (hItem)
		{
			int nID = m_treeList.GetItemData(hItem);
			CString strPath= m_playlist.GetPath(nID);
			CFileFind finder;
			if (m_treeList.GetParentItem(hItem) == m_treeList.GetRootItem())
			{
				if (finder.FindFile(strPath))
					ShellExecute(NULL, _T("open"), _T("explorer.exe"), strPath, NULL, SW_SHOWNORMAL);
				else
					AfxMessageBox(_T("失效目录，可能已被删除！"));
			}
			else
			{
				strPath = strPath.Mid(8);
				if (finder.FindFile(strPath))
					ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T("/n,/select,") + strPath, NULL, SW_SHOWNORMAL);
				else
					AfxMessageBox(_T("失效文件，可能已被删除！"));
			}
			finder.Close();
		}
	}
	break;
	case MSGUSER_REFRESHCATALOG:
	{
		HTREEITEM hItem = NULL;
		hItem = m_treeList.GetSelectedItem();
		RefreshCatalog(hItem);
	}
	break;
	case MSGUSER_EMPTYLIST:
	{
		if (m_treeList.DeleteAllItems())
		{
			m_playlist.DeleteAllItem();
			m_nListID = 0;
			InsertItem(NULL, 0, _T("播放列表"), _T("播放列表"));
			m_treeList.SetSelectedItem(NULL);
			m_treeList.SetPlayItem(NULL);
			g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_ITEMDELETE);
		}
	}
	break;
	case MSGUSER_REFRESHLIST:
	{
		HTREEITEM hRoot = NULL, hFirst = NULL;
		hRoot = m_treeList.GetRootItem();
		hFirst = m_treeList.GetChildItem(hRoot);
		while (hFirst)
		{
			RefreshCatalog(hFirst);
			hFirst = m_treeList.GetNextItem(hFirst, TVGN_NEXT);
		}
	}
	break;
	case MSGUSER_SETSEARCHITEM:
	{
		HTREEITEM hItem = m_treeList.GetItemByData(m_treeList.GetRootItem(), wParam);
		if (hItem)
		{
			m_treeList.SetSelectedItem(hItem);
			m_treeList.EnsureVisible(hItem);
		}
	}
	break;
	}
	return 0;
}


void CListBar::OnNMDblclkTreeList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CString strPath;
	strPath = m_playlist.GetPath(m_treeList.GetItemData(m_treeList.GetPlayItem()));
	g_sz_p_wnd[WND_ZMS]->SendMessage(WM_MSGRECVPRO, (WPARAM)&strPath, MSGUSER_PLAYFILE);
}


void CListBar::BrowseCurrentDir(const CString & strFolder,std::vector<CString> & vec_strPaths)
{
	vec_strPaths.clear();
	CFileFind finder;
	CString strPath;
	BOOL bWorking = finder.FindFile(strFolder+_T("\\*.*"));
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(!finder.IsDirectory())
		{
			strPath=finder.GetFilePath();
			vec_strPaths.push_back(strPath);
		}
	}
	finder.Close();
}


void CListBar::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	if (m_menuList.m_hMenu)
	{
		CMenu* pPopup = m_menuList.GetSubMenu(0);
		if (pPopup)
		{
			pPopup->EnableMenuItem(ID_ITEM_PLAY, MF_BYCOMMAND | MF_ENABLED);
			pPopup->EnableMenuItem(ID_ITEM_DELETE, MF_BYCOMMAND | MF_ENABLED);
			pPopup->EnableMenuItem(ID_OPEN_FILECATALOG, MF_BYCOMMAND | MF_ENABLED);
			pPopup->EnableMenuItem(ID_REFRESH_CATALOG, MF_BYCOMMAND | MF_ENABLED);
			CPoint ptCur = point;
			UINT nFlags;
			HTREEITEM hItem = NULL;
			m_treeList.ScreenToClient(&ptCur);
			hItem = m_treeList.HitTest(ptCur, &nFlags);
			if (hItem&&hItem != m_treeList.GetRootItem() && m_treeList.GetParentItem(hItem) != m_treeList.GetRootItem())
			{
				m_treeList.SetSelectedItem(hItem);
				pPopup->EnableMenuItem(ID_REFRESH_CATALOG, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			}
			else if (hItem&&m_treeList.GetParentItem(hItem) == m_treeList.GetRootItem())
			{
				m_treeList.SetSelectedItem(hItem);
				pPopup->EnableMenuItem(ID_ITEM_PLAY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
				int nDefaultID = m_playlist.GetIDByPath(_T("默认目录"));
				if (m_treeList.GetItemData(hItem) == nDefaultID)
				{
					pPopup->EnableMenuItem(ID_OPEN_FILECATALOG, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
				}
			}
			else
			{
				pPopup->EnableMenuItem(ID_ITEM_PLAY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
				pPopup->EnableMenuItem(ID_ITEM_DELETE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
				pPopup->EnableMenuItem(ID_OPEN_FILECATALOG, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
				pPopup->EnableMenuItem(ID_REFRESH_CATALOG, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			}
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		}
	}
}


void CListBar::InitMenu()
{
	m_menuList.LoadMenu(IDR_MENU_LIST);
}


HTREEITEM CListBar::InsertItem(HTREEITEM hParentItem, int nClass, const CString & strName, const CString & strPath)
{
	HTREEITEM hItem = NULL;
	int nImage = 2, nSelectedImage = 2;
	if (nClass == 1)
	{
		nImage = 0;
		nSelectedImage = 1;
	}
	if (hParentItem == NULL)
	{
		hItem = m_treeList.InsertItem(strName, nImage, nSelectedImage);
		m_treeList.SetItemData(hItem, m_nListID);
		m_playlist.InsertItem(m_nListID, m_nListID, nClass, strName, strPath, 0);
		++m_nListID;
	}
	else
	{
		hItem = m_treeList.InsertItem(strName, nImage, nSelectedImage, hParentItem);
		m_treeList.SetItemData(hItem, m_nListID);
		m_playlist.InsertItem(m_nListID, m_treeList.GetItemData(hParentItem), nClass, strName, strPath, 0);
		++m_nListID;
	}
	return hItem;
}

void CListBar::DeleteItem(HTREEITEM hItem)
{
	if (hItem)
	{
		if (hItem == m_treeList.GetPlayItem() || hItem == m_treeList.GetParentItem(m_treeList.GetPlayItem()))
		{
			m_treeList.SetPlayItem(NULL);
			g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_ITEMDELETE);
		}
		int nID = m_treeList.GetItemData(hItem);
		m_treeList.DeleteItem(hItem);
		m_playlist.DeleteItem(nID);
	}
}


void CListBar::DeleteItem(int nID)
{
	HTREEITEM hItem = NULL;
	hItem = m_treeList.GetItemByData(m_treeList.GetRootItem(), nID);
	DeleteItem(hItem);
}


void CListBar::RefreshCatalog(HTREEITEM hItem)
{
	if (hItem)
	{
		int nID = m_treeList.GetItemData(hItem);
		std::vector<std::vector <CString>>  vec2_strData;
		//剔除失效文件
		m_playlist.GetChildPath(nID, vec2_strData);
		CString strPath;
		CFileFind finder;
		int nRow = vec2_strData.size();
		if (nRow)
		{
			int nColumn = vec2_strData[0].size();
			if (nColumn == 2)
			{
				for (int i = 1; i < nRow; ++i)
				{
					strPath = vec2_strData[i][0].Mid(8);
					if (!finder.FindFile(strPath))
					{
						int nChildID = _ttoi(vec2_strData[i][1]);
						DeleteItem(nChildID);
					}
					finder.Close();
				}
			}
		}
		//增加新文件
		int nDefaultID = m_playlist.GetIDByPath(_T("默认目录"));
		if (nID != nDefaultID)
		{
			std::vector<CString> vec_strPaths;
			BrowseCurrentDir(m_playlist.GetPath(nID), vec_strPaths);
			int nSize = vec_strPaths.size();
			ZSetting setting;
			for (int i = 0; i < nSize; ++i)
			{
				if (setting.CheckFolderAutoSel())
				{
					if (ZPlayer::GetFileType(vec_strPaths[i]) == ZPlayer::TYPE_OTHER)
						continue;
				}
				vec_strPaths[i] = _T("file:///") + vec_strPaths[i];
				int nTempID = m_playlist.GetIDByPath(vec_strPaths[i]);
				if (nTempID == -1)
				{
					InsertItem(hItem, 2, vec_strPaths[i].Mid(vec_strPaths[i].ReverseFind(_T('\\')) + 1), vec_strPaths[i]);
				}
			}
		}
	}
}


void CListBar::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tm;
	tm.cbSize = sizeof(TRACKMOUSEEVENT);
	tm.dwFlags = TME_LEAVE;
	tm.dwHoverTime = 500;
	tm.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tm);
	CDialogEx::OnMouseMove(nFlags, point);
}


void CListBar::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MOUSELEAVE);
	CDialogEx::OnMouseLeave();
}
