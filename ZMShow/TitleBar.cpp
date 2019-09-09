// TitleBar.cpp : 实现文件
//

#include "stdafx.h"
#include "ZMShow.h"
#include "TitleBar.h"
#include "afxdialogex.h"


// CTitleBar 对话框

IMPLEMENT_DYNAMIC(CTitleBar, CDialogEx)

CTitleBar::CTitleBar(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TITLEBAR, pParent)
	, m_bIsInitFinish(false)
	, m_strListMode(_T(""))
	, m_nListMode(0)
{

}

CTitleBar::~CTitleBar()
{
}

void CTitleBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TITLE, m_btnTitle);
	DDX_Control(pDX, IDC_BUTTON_MIN, m_btnMin);
	DDX_Control(pDX, IDC_BUTTON_MAX, m_btnMax);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_btnList);
	DDX_Control(pDX, IDC_BUTTON_LISTMODE, m_btnListMode);
	DDX_Control(pDX, IDC_COMBO_SEARCH, m_comboSearch);
}


BEGIN_MESSAGE_MAP(CTitleBar, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MSGRECVPRO, &CTitleBar::OnMsgrecvpro)
	ON_BN_CLICKED(IDC_BUTTON_TITLE, &CTitleBar::OnBnClickedButtonTitle)
	ON_BN_CLICKED(IDC_BUTTON_MIN, &CTitleBar::OnBnClickedButtonMin)
	ON_BN_CLICKED(IDC_BUTTON_MAX, &CTitleBar::OnBnClickedButtonMax)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CTitleBar::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_LISTMODE, &CTitleBar::OnBnClickedButtonListmode)
	ON_BN_CLICKED(IDC_BUTTON_LIST, &CTitleBar::OnBnClickedButtonList)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_CBN_EDITCHANGE(IDC_COMBO_SEARCH, &CTitleBar::OnCbnEditchangeComboSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_SEARCH, &CTitleBar::OnCbnSelchangeComboSearch)
	ON_CBN_SETFOCUS(IDC_COMBO_SEARCH, &CTitleBar::OnCbnSetfocusComboSearch)
	ON_CBN_KILLFOCUS(IDC_COMBO_SEARCH, &CTitleBar::OnCbnKillfocusComboSearch)
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


// CTitleBar 消息处理程序


BOOL CTitleBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_sz_p_wnd[WND_TIT] = this;
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	m_brushBkGnd.CreateSolidBrush(g_crBlack);
	InitButton();
	InitMenu();
	InitComboBox();
	m_bIsInitFinish = true;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

HBRUSH CTitleBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return m_brushBkGnd;
	return hbr;
}

void CTitleBar::InitButton(void)
{
	SetBtnColor(m_btnTitle, _T("主菜单"));
	SetBtnColor(m_btnMin, _T("最小化"));
	SetBtnColor(m_btnMax, _T("最大化"));
	SetBtnColor(m_btnClose, _T("关闭"));
	SetBtnColor(m_btnList, _T("播放列表"));
	m_strListMode = _T("列表模式:列表顺序");
	SetBtnColor(m_btnListMode, m_strListMode);
	m_btnTitle.SetIcon(IDR_MAINFRAME,IDR_MAINFRAME);
	m_btnMin.SetIcon(IDI_ICON_MINON, IDI_ICON_MIN);
	m_btnMax.SetIcon(IDI_ICON_MAXON, IDI_ICON_MAX);
	m_btnClose.SetIcon(IDI_ICON_CLOSEON, IDI_ICON_CLOSE);
	m_btnList.SetIcon(IDI_ICON_LISTON, IDI_ICON_LISTON);
	m_btnListMode.SetIcon(IDI_ICON_LISTMODEON, IDI_ICON_LISTMODE);
}

void CTitleBar::SetBtnColor(ZButton & btn, const CString & strTip)
{
	//btn.DrawTransparent(TRUE);
	btn.DrawBorder(FALSE);
	btn.SetFlat(TRUE);
	btn.SetColor(ZButton::COLOR_BK_IN, g_crBlack);
	btn.SetColor(ZButton::COLOR_BK_OUT, g_crBlack);
	btn.SetColor(ZButton::COLOR_TEXT_IN, g_crBlue);
	btn.SetColor(ZButton::COLOR_TEXT_OUT, g_crWhite);
	btn.SetTooltipText(strTip);
	//btn.SetBtnCursor(IDC_CURSOR_INBTN);
}

void CTitleBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_bIsInitFinish&&nType != SIZE_MINIMIZED)  //初始化好了后才能变换
	{
		CRect rcDlg;
		GetClientRect(&rcDlg);
		ChangeSize(rcDlg, IDC_BUTTON_CLOSE);
		ChangeSize(rcDlg, IDC_BUTTON_MAX);
		ChangeSize(rcDlg, IDC_BUTTON_MIN);
		ChangeSize(rcDlg, IDC_BUTTON_LIST);
		ChangeSize(rcDlg, IDC_BUTTON_LISTMODE);
		ChangeSize(rcDlg, IDC_COMBO_SEARCH);
		//RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_ALLCHILDREN);
	}
}

void CTitleBar::ChangeSize(const CRect & rcParent, const UINT & nID)
{
	static CRect rcTemp;
	CWnd *pWnd = GetDlgItem(nID);
	if (pWnd != NULL)     //判断是否为空，因为在窗口创建的时候也会调用OnSize函数，但是此时各个控件还没有创建，Pwnd为空
	{
		CRect rcSon;
		pWnd->GetWindowRect(&rcSon);  //获取控件变化前的大小
		ScreenToClient(&rcSon);   //将控件大小装换位在对话框中的区域坐标 
		int nHeight = rcSon.Height();
		int nWidth = rcSon.Width();
		switch (nID)
		{
		case IDC_BUTTON_CLOSE:
		{
			rcSon.right = rcParent.right - 5;
			rcTemp.left = rcSon.left = rcSon.right - nWidth;
		}
		break;
		case IDC_BUTTON_MAX:
		case IDC_BUTTON_MIN:
		{
			rcSon.right = rcTemp.left;
			rcTemp.left = rcSon.left = rcSon.right - nWidth;
		}
		break;
		case IDC_BUTTON_LIST:
		{
			rcSon.right = rcParent.right - 5;
			rcTemp.left = rcSon.left = rcSon.right - nWidth;
		}
		break;
		case IDC_BUTTON_LISTMODE:
		case IDC_COMBO_SEARCH:
		{
			rcSon.right = rcTemp.left;
			rcTemp.left = rcSon.left = rcSon.right - nWidth;
		}
		break;
		}
		pWnd->MoveWindow(rcSon);   //伸缩控件
	}
}


afx_msg LRESULT CTitleBar::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_PREVBTN:
	{
		g_sz_p_wnd[WND_LIS]->PostMessage(WM_MSGRECVPRO, (WPARAM)m_nListMode, MSGUSER_PREVBTN);
	}
	break;
	case MSGUSER_NEXTBTN:
	{
		g_sz_p_wnd[WND_LIS]->PostMessage(WM_MSGRECVPRO, (WPARAM)m_nListMode, MSGUSER_NEXTBTN);
	}
	break;
	case MSGUSER_MAXBTN:
	case MSGUSER_FULLBTN:
	{
		switch (wParam)
		{
		case WND_STATE_NORMAL:
		{
			m_btnMax.SetIcon(IDI_ICON_MAXON, IDI_ICON_MAX);
			m_btnMax.SetTooltipText(_T("最大化"));
		}
		break;
		case WND_STATE_MAX:
		{
			m_btnMax.SetIcon(IDI_ICON_NORMALON, IDI_ICON_NORMAL);
			m_btnMax.SetTooltipText(_T("恢复"));
		}
		break;
		case WND_STATE_FULL:
		{
			m_btnMax.SetTooltipText(_T("退出全屏"));
		}
		break;
		}
	}
	break;
	case MSGUSER_LISTBTN:
	{
		if(wParam)
			m_btnList.SetIcon(IDI_ICON_LISTON, IDI_ICON_LISTON);
		else
			m_btnList.SetIcon(IDI_ICON_LIST, IDI_ICON_LIST);
	}
	break;
	case MSGUSER_SETLISTMODE:
	{
		m_nListMode = wParam;
		const int nList = 0, nCatalog = 1, nSingle = 2;
		const int nSeq = 0, nLoop = 1, nRand = 2;
		if (CheckListModeMenu(m_nListMode / 3, m_nListMode % 3, 0, 2, 0, 2))
		{
			m_strListMode = _T("列表模式:");
			switch (m_nListMode / 3)
			{
			case nList:
			{
				m_strListMode += _T("列表");
			}
			break;
			case nCatalog:
			{
				m_strListMode += _T("目录");
			}
			break;
			case nSingle:
			{
				m_strListMode += _T("单个");
			}
			break;
			}
			switch (m_nListMode % 3)
			{
			case nSeq:
			{
				if (m_nListMode / 3 != 2)
					m_strListMode += _T("顺序");
				else
					m_strListMode += _T("播放");
			}
			break;
			case nLoop:
			{
				m_strListMode += _T("循环");
			}
			break;
			case nRand:
			{
				m_strListMode += _T("随机");
			}
			break;
			}
		}
		m_btnListMode.SetTooltipText(m_strListMode);
	}
	break;
	}
	return 0;
}


void CTitleBar::OnBnClickedButtonTitle()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rc;
	GetDlgItem(IDC_BUTTON_TITLE)->GetWindowRect(&rc);
	CMenu menu;
	if (menu.LoadMenu(IDR_MENU_MAIN))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		if (pPopup)
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rc.left, rc.bottom, this); //在鼠标当前位置显示指定菜单
	}
	m_btnTitle.SetTooltipText(_T("主菜单"));
}


void CTitleBar::OnBnClickedButtonMin()
{
	// TODO: 在此添加控件通知处理程序代码
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_MINBTN);
}


void CTitleBar::OnBnClickedButtonMax()
{
	// TODO: 在此添加控件通知处理程序代码
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_MAXBTN);
}


void CTitleBar::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_CLOSEBTN);
}


void CTitleBar::OnBnClickedButtonListmode()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rc;
	GetDlgItem(IDC_BUTTON_LISTMODE)->GetWindowRect(&rc);
	if (m_menuListMode.m_hMenu)
	{
		CMenu* pPopup = m_menuListMode.GetSubMenu(0);
		if (pPopup)
			pPopup->TrackPopupMenu(TPM_RIGHTALIGN | TPM_LEFTBUTTON, rc.right, rc.bottom, this); //在鼠标当前位置显示指定菜单
	}
	m_btnListMode.SetTooltipText(m_strListMode);
}


void CTitleBar::OnBnClickedButtonList()
{
	// TODO: 在此添加控件通知处理程序代码
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_LISTBTN);
}


void CTitleBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt(point);
	ClientToScreen(&pt);
	g_sz_p_wnd[WND_ZMS]->ScreenToClient(&pt);
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(pt.x, pt.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CTitleBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt(point);
	ClientToScreen(&pt);
	g_sz_p_wnd[WND_ZMS]->ScreenToClient(&pt);
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_LBUTTONDBLCLK, nFlags, MAKELPARAM(pt.x, pt.y));
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CTitleBar::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tm;
	tm.cbSize = sizeof(TRACKMOUSEEVENT);
	tm.dwFlags = TME_LEAVE;
	tm.dwHoverTime = 500;
	tm.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tm);
	CPoint pt(point);
	ClientToScreen(&pt);
	g_sz_p_wnd[WND_ZMS]->ScreenToClient(&pt);
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MOUSEMOVE, nFlags, MAKELPARAM(pt.x, pt.y));
	CDialogEx::OnMouseMove(nFlags, point);
}


void CTitleBar::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MOUSELEAVE);
	CDialogEx::OnMouseLeave();
}


void CTitleBar::InitMenu(void)
{
	m_menuListMode.LoadMenu(IDR_MENU_LISTMODE);
	CheckListModeMenu(m_nListMode / 3, m_nListMode % 3, 0, 2, 0, 2);
}


bool CTitleBar::CheckListModeMenu(int nFirst, int nSecond, int nMinFirst, int nMaxFirst, int nMinSecond, int nMaxSecond)
{
	static int nLastFirst = 0, nLastSecond = 0;
	CMenu* p_menuFirst = m_menuListMode.GetSubMenu(0);
	if (p_menuFirst)
	{
		CMenu* p_menuLastSecond = p_menuFirst->GetSubMenu(nLastFirst);
		if (p_menuLastSecond)
		{
			p_menuLastSecond->CheckMenuItem(nLastSecond, MF_UNCHECKED | MF_BYPOSITION);
		}
		CMenu* p_menuSecond = p_menuFirst->GetSubMenu(nFirst);
		if (p_menuSecond)
		{
			p_menuSecond->CheckMenuRadioItem(nMinSecond, nMaxSecond, nSecond, MF_CHECKED | MF_BYPOSITION);
			p_menuFirst->CheckMenuRadioItem(nMinFirst, nMaxFirst, nFirst, MF_CHECKED | MF_BYPOSITION);
			nLastFirst = nFirst;
			nLastSecond = nSecond;
			return true;
		}
	}
	return false;
}


void CTitleBar::OnCbnEditchangeComboSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSize = m_comboSearch.GetCount();
	for (int i = 0; i<nSize; ++i)
	{
		m_comboSearch.DeleteString(0);
	}
	CString strKey;
	m_comboSearch.GetWindowText(strKey);
	ZPlayList playlist;
	std::vector<std::vector <CString>>  vec2_strData;
	playlist.SearchKey(strKey, vec2_strData);
	int nRow = vec2_strData.size();
	if (nRow > 1)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 2)
		{
			for (int i = 1; i<nRow; ++i)
			{
				int nIndex = m_comboSearch.AddString(vec2_strData[i][0]);
				if (nIndex >= 0)
					m_comboSearch.SetItemData(nIndex, _ttoi(vec2_strData[i][1]));
			}
		}
	}
}


void CTitleBar::OnCbnSelchangeComboSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel;
	nSel = m_comboSearch.GetCurSel();
	if (nSel != CB_ERR)
	{
		DWORD dwID = m_comboSearch.GetItemData(nSel);
		if(dwID != CB_ERR)
			g_sz_p_wnd[WND_LIS]->PostMessage(WM_MSGRECVPRO, dwID, MSGUSER_SETSEARCHITEM);
	}
}


void CTitleBar::InitComboBox()
{
	m_comboSearch.SetColor(g_crBlack,g_crGrey55,g_crWhite);
	m_comboSearch.SetIcon(IDI_ICON_SEARCHON, IDI_ICON_SEARCH);
}


void CTitleBar::OnCbnSetfocusComboSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CPoint pt;
	::GetCursorPos(&pt);
	m_comboSearch.ScreenToClient(&pt);
	COMBOBOXINFO comboInfo;
	comboInfo.cbSize = sizeof(COMBOBOXINFO);
	m_comboSearch.GetComboBoxInfo(&comboInfo);
	CRect rcBtn;
	rcBtn = CRect(comboInfo.rcButton);
	if (!rcBtn.PtInRect(pt))
	{
		m_comboSearch.ShowDropDown(TRUE);
	}
	g_bIsFocusCombo = true;
}


void CTitleBar::OnCbnKillfocusComboSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bIsFocusCombo = false;
}
