// CLyricBar.cpp: 实现文件
//

#include "stdafx.h"
#include "ZMShow.h"
#include "CLyricBar.h"
#include "afxdialogex.h"


// CLyricBar 对话框

IMPLEMENT_DYNAMIC(CLyricBar, CDialogEx)

CLyricBar::CLyricBar(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LYRICBAR, pParent)
	, m_bIsInitFinish(false)
	, m_nHitTest(0)
{

}

CLyricBar::~CLyricBar()
{
}

void CLyricBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LRC, m_listLrc);
	DDX_Control(pDX, IDC_BUTTON_DESKLRC, m_btnDeskLrc);
	DDX_Control(pDX, IDC_BUTTON_FONT, m_btnFont);
	DDX_Control(pDX, IDC_BUTTON_SIZEUP, m_btnSizeUp);
	DDX_Control(pDX, IDC_BUTTON_SIZEDOWN, m_btnSizeDown);
	DDX_Control(pDX, IDC_BUTTON_COLORBK, m_btnColrBk);
	DDX_Control(pDX, IDC_BUTTON_COLORFG, m_btnColrFg);
}


BEGIN_MESSAGE_MAP(CLyricBar, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MSGRECVPRO, &CLyricBar::OnMsgrecvpro)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_DESKLRC, &CLyricBar::OnBnClickedButtonDesklrc)
	ON_BN_CLICKED(IDC_BUTTON_FONT, &CLyricBar::OnBnClickedButtonFont)
	ON_BN_CLICKED(IDC_BUTTON_SIZEUP, &CLyricBar::OnBnClickedButtonSizeup)
	ON_BN_CLICKED(IDC_BUTTON_SIZEDOWN, &CLyricBar::OnBnClickedButtonSizedown)
	ON_BN_CLICKED(IDC_BUTTON_COLORBK, &CLyricBar::OnBnClickedButtonColorbk)
	ON_BN_CLICKED(IDC_BUTTON_COLORFG, &CLyricBar::OnBnClickedButtonColorfg)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()


// CLyricBar 消息处理程序


BOOL CLyricBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_sz_p_wnd[WND_LRC] = this;
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	m_brushBkGnd.CreateSolidBrush(g_crBlack);
	InitListBox();
	InitButton();
	m_bIsInitFinish = true;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CLyricBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return m_brushBkGnd;
	return hbr;
}


afx_msg LRESULT CLyricBar::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_INSERTLRC:
	{
		CString * p_str = (CString *)wParam;
		m_listLrc.SetSelectedItem(-1);
		m_listLrc.SetTopIndex(0);
		ProLyric(*p_str);
		InsertListBox();
	}
	break;
	case MSGUSER_SETLRCPOS:
	{
		static int iLast = -1;
		int i = 0;
		std::map<int, CString>::iterator iter;
		std::map<int, CString>::iterator iterUp;
		iterUp = m_map_Lrc.upper_bound(wParam);
		if (iterUp != m_map_Lrc.begin() && iterUp != m_map_Lrc.end())
		{
			for (iter = m_map_Lrc.begin(); iter != m_map_Lrc.end(); ++iter)
			{
				if (iter == iterUp && iLast != i)
				{
					int nPageCount = m_listLrc.GetPageCount();
					m_listLrc.SetSelectedItem(nPageCount / 2 + i - 1);
					m_listLrc.SetTopIndex(i - 1);
					iLast = i;
					break;
				}
				++i;
			}
		}
		if (iterUp == m_map_Lrc.end() && iLast < (int)m_map_Lrc.size())
		{
			int nPageCount = m_listLrc.GetPageCount();
			m_listLrc.SetSelectedItem(nPageCount / 2 + m_map_Lrc.size() - 1);
			m_listLrc.SetTopIndex(m_map_Lrc.size() - 1);
			++iLast;
		}
	}
	break;
	case MSGUSER_DESKLRCBTN:
	{
		g_bIsDeskLrc = !g_bIsDeskLrc;
		g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_DESKLRCBTN);
		COLORREF crItem, crSelectedItem;
		crItem = m_listLrc.GetItemColor();
		crSelectedItem = m_listLrc.GetSelectedItemColor();
		if (g_bIsDeskLrc)
		{
			m_btnDeskLrc.SetTooltipText(_T("关闭桌面歌词"));
			m_listLrc.SetColor(g_crBlack, crItem, crSelectedItem);
			m_listLrc.SetBmpBk();
		}
		else
		{
			m_btnDeskLrc.SetTooltipText(_T("桌面歌词"));
			m_listLrc.SetColor(g_crTransparent, crItem, crSelectedItem);
			m_listLrc.SetBmpBk(IDB_BITMAP_BKGND);
		}
	}
	break;
	}
	return 0;
}


void CLyricBar::InitListBox(void)
{
	int nFontHeight = -20;
	LOGFONT logfont;
	logfont = m_listLrc.GetFont();
	logfont.lfHeight = nFontHeight;
	m_listLrc.SetFont(logfont);
	m_listLrc.SetItemHeight(0, abs(nFontHeight) + abs(nFontHeight) * 2 / 3);
	m_listLrc.SetColor(g_crTransparent, g_crWhite, g_crBlue);
	m_listLrc.SetBmpBk(IDB_BITMAP_BKGND);
	m_listLrc.ModifyStyle(0, WS_CLIPSIBLINGS);
	m_listLrc.SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	m_listLrc.EnableWindow(FALSE);
}

void CLyricBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_bIsInitFinish&&nType != SIZE_MINIMIZED)
	{
		CRect rcDlg;
		GetClientRect(&rcDlg);
		ChangeSize(rcDlg, IDC_LIST_LRC);
		ChangeSize(rcDlg, IDC_BUTTON_SIZEUP);
		ChangeSize(rcDlg, IDC_BUTTON_FONT);
		ChangeSize(rcDlg, IDC_BUTTON_DESKLRC);
		ChangeSize(rcDlg, IDC_BUTTON_SIZEDOWN);
		ChangeSize(rcDlg, IDC_BUTTON_COLORBK);
		ChangeSize(rcDlg, IDC_BUTTON_COLORFG);
		InsertListBox();
	}
}


void CLyricBar::ChangeSize(const CRect & rcParent, const UINT & nID)
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
		case IDC_LIST_LRC:
		{
			rcSon = rcParent;
		}
		break;
		case IDC_BUTTON_SIZEUP:
		{
			rcSon.bottom = rcParent.Height() / 2;
			rcTemp.top = rcSon.top = rcSon.bottom - nHeight;
		}
		break;
		case IDC_BUTTON_FONT:
		case IDC_BUTTON_DESKLRC:
		{
			rcSon.bottom = rcTemp.top;
			rcTemp.top = rcSon.top = rcSon.bottom - nHeight;
		}
		break;
		case IDC_BUTTON_SIZEDOWN:
		{
			rcSon.top = rcParent.Height() / 2;
			rcTemp.bottom = rcSon.bottom = rcSon.top + nHeight;
		}
		break;
		case IDC_BUTTON_COLORBK:
		case IDC_BUTTON_COLORFG:
		{
			rcSon.top = rcTemp.bottom;
			rcTemp.bottom = rcSon.bottom = rcSon.top + nHeight;
		}
		}
		pWnd->MoveWindow(rcSon);   //伸缩控件
	}
}


void CLyricBar::ProLyric(const CString & strLyric)
{
	m_map_Lrc.clear();
	std::vector<CString> vec_strLines;
	ZUtil::StrSplit(strLyric, vec_strLines, _T('\n'), strLyric.Right(1) == _T('\n'));
	int nSize = vec_strLines.size();
	for (int i = 0; i<nSize; ++i)
	{
		vec_strLines[i].Remove(_T('['));
		vec_strLines[i].Remove(_T('\r'));
		vec_strLines[i].Remove(_T('\n'));
		std::vector<CString> vec_strDatas;
		ZUtil::StrSplit(vec_strLines[i], vec_strDatas, _T(']'), false);
		int nDataSize = vec_strDatas.size();
		switch (nDataSize)
		{
		case 0:
		case 1:
			break;
		default:
		{
			for (int j = 0; j<nDataSize - 1; ++j)
			{
				int nMinWidth = vec_strDatas[j].Find(_T(':'));
				int nSecWidth = -1;
				if (nMinWidth == 1 && IsDigidit(vec_strDatas[j][0]) && IsDigidit(vec_strDatas[j][2]) && !IsDigidit(vec_strDatas[j][3]))
					nSecWidth = 1;
				if (nMinWidth == 1 && IsDigidit(vec_strDatas[j][0]) && IsDigidit(vec_strDatas[j][2]) && IsDigidit(vec_strDatas[j][3]))
					nSecWidth = 2;
				if (nMinWidth == 2 && IsDigidit(vec_strDatas[j][0]) && IsDigidit(vec_strDatas[j][1]) && IsDigidit(vec_strDatas[j][3]) && !IsDigidit(vec_strDatas[j][4]))
					nSecWidth = 1;
				if (nMinWidth == 2 && IsDigidit(vec_strDatas[j][0]) && IsDigidit(vec_strDatas[j][1]) && IsDigidit(vec_strDatas[j][3]) && IsDigidit(vec_strDatas[j][4]))
					nSecWidth = 2;
				if (nMinWidth != -1 && nSecWidth != -1)
				{
					int nMin, nSec, nMSec, nKey;
					nMin = _ttoi(vec_strDatas[j].Left(nMinWidth));
					nSec = _ttoi(vec_strDatas[j].Mid(nMinWidth + 1, nSecWidth));
					nMSec = _ttoi(vec_strDatas[j].Mid(nMinWidth + nSecWidth + 2, 1)) * 100 + _ttoi(vec_strDatas[j].Mid(nMinWidth + nSecWidth + 3, 1)) * 10 + _ttoi(vec_strDatas[j].Mid(nMinWidth + nSecWidth + 4, 1));
					nKey = nMin * 60 * 1000 + nSec * 1000 + nMSec;
					m_map_Lrc.insert(std::pair<int, CString>(nKey, vec_strDatas[nDataSize - 1]));
				}
			}
		}
		}
	}
}


void CLyricBar::InsertListBox(void)
{
	m_listLrc.SetRedraw(FALSE);
	int nSelectedItem = m_listLrc.GetSelectedItem();
	int nTopIndex = m_listLrc.GetTopIndex();
	m_listLrc.ResetContent();
	int nPageCount = m_listLrc.GetPageCount();
	for (int i = 0; i<nPageCount / 2; ++i)
	{
		m_listLrc.AddString(_T(""));
	}
	std::map<int, CString>::iterator  iter;
	for (iter = m_map_Lrc.begin(); iter != m_map_Lrc.end(); ++iter)
	{
		m_listLrc.AddString(iter->second);
	}
	for (int i = 0; i<nPageCount / 2; ++i)
	{
		m_listLrc.AddString(_T(""));
	}
	if (nTopIndex != LB_ERR)
		m_listLrc.SetTopIndex(nTopIndex);
	if (nSelectedItem != -1)
		m_listLrc.SetSelectedItem(nPageCount / 2 + nTopIndex);
	m_listLrc.SetRedraw(TRUE);
}


BOOL CLyricBar::IsDigidit(TCHAR ch)
{
	BOOL nRtn = FALSE;
	if (ch >= _T('0') && ch <= _T('9'))
		nRtn = TRUE;
	return nRtn;
}


void CLyricBar::InitButton(void)
{
	SetBtnColor(m_btnDeskLrc, _T("桌面歌词"));
	SetBtnColor(m_btnFont, _T("字体"));
	SetBtnColor(m_btnSizeUp, _T("Size↑"));
	SetBtnColor(m_btnSizeDown, _T("Size↓"));
	m_btnDeskLrc.SetIcon(IDI_ICON_DESKLRCON, IDI_ICON_DESKLRC);
	m_btnFont.SetIcon(IDI_ICON_FONTON, IDI_ICON_FONT);
	m_btnSizeUp.SetIcon(IDI_ICON_SIZEUPON, IDI_ICON_SIZEUP);
	m_btnSizeDown.SetIcon(IDI_ICON_SIZEDOWNON, IDI_ICON_SIZEDOWN);
	m_btnColrBk.SetColor(ZButton::COLOR_BK_IN, g_crWhite);
	m_btnColrBk.SetColor(ZButton::COLOR_BK_OUT, g_crBlack);
	m_btnColrBk.SetIcon(-1, IDI_ICON_COLORBK);
	m_btnColrBk.SetFlat(TRUE);
	m_btnColrBk.SetTooltipText(_T("背景色"));
	m_btnColrFg.SetColor(ZButton::COLOR_BK_IN, g_crBlue);
	m_btnColrFg.SetColor(ZButton::COLOR_BK_OUT, g_crBlack);
	m_btnColrFg.SetIcon(-1, IDI_ICON_COLORFG);
	m_btnColrFg.SetFlat(TRUE);
	m_btnColrFg.SetTooltipText(_T("前景色"));
}


void CLyricBar::SetBtnColor(ZButton & btn, const CString & strTip)
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


void CLyricBar::OnBnClickedButtonDesklrc()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_MSGRECVPRO, 0, MSGUSER_DESKLRCBTN);
}


void CLyricBar::OnBnClickedButtonFont()
{
	// TODO: 在此添加控件通知处理程序代码
	LOGFONT logfont;
	logfont = m_listLrc.GetFont();
	CFontDialog fontDlg(&logfont);
	if (IDOK == fontDlg.DoModal())
	{
		if (logfont.lfHeight<-2 && logfont.lfHeight>-100)
		{
			m_listLrc.SetFont(logfont);
			m_listLrc.SetItemHeight(0, abs(logfont.lfHeight) + abs(logfont.lfHeight) * 2 / 3);
			InsertListBox();
		}
	}
	m_btnFont.SetTooltipText(_T("字体"));
	PostMessage(WM_MOUSELEAVE);
}


void CLyricBar::OnBnClickedButtonSizeup()
{
	// TODO: 在此添加控件通知处理程序代码
	LOGFONT logfont;
	logfont = m_listLrc.GetFont();
	if (logfont.lfHeight>-100)
	{
		logfont.lfHeight -= 2;
		if (m_listLrc.SetItemHeight(0, abs(logfont.lfHeight) + abs(logfont.lfHeight) * 2 / 3) != LB_ERR)
		{
			m_listLrc.SetFont(logfont);
			InsertListBox();
		}
		else
			logfont.lfHeight += 2;
	}
	m_btnSizeUp.SetTooltipText(_T("Size↑"));
}


void CLyricBar::OnBnClickedButtonSizedown()
{
	// TODO: 在此添加控件通知处理程序代码
	LOGFONT logfont;
	logfont = m_listLrc.GetFont();
	if (logfont.lfHeight<-2)
	{
		logfont.lfHeight += 2;
		if (m_listLrc.SetItemHeight(0, abs(logfont.lfHeight) + abs(logfont.lfHeight) * 2 / 3) != LB_ERR)
		{
			m_listLrc.SetFont(logfont);
			InsertListBox();
		}
		else
			logfont.lfHeight -= 2;
	}
	m_btnSizeDown.SetTooltipText(_T("Size↓"));
}


void CLyricBar::OnBnClickedButtonColorbk()
{
	// TODO: 在此添加控件通知处理程序代码
	COLORREF color = m_listLrc.GetItemColor();
	CColorDialog colorDlg(color);
	if (IDOK == colorDlg.DoModal())
	{
		color = colorDlg.GetColor();
		m_listLrc.SetItemColor(color);
		m_btnColrBk.SetColor(ZButton::COLOR_BK_IN, color);
	}
	m_btnColrBk.SetTooltipText(_T("背景色"));
	PostMessage(WM_MOUSELEAVE);
}


void CLyricBar::OnBnClickedButtonColorfg()
{
	// TODO: 在此添加控件通知处理程序代码
	COLORREF color = m_listLrc.GetSelectedItemColor();
	CColorDialog colorDlg(color);
	if (IDOK == colorDlg.DoModal())
	{
		color = colorDlg.GetColor();
		m_listLrc.SetSelectedItemColor(color);
		m_btnColrFg.SetColor(ZButton::COLOR_BK_IN, color);
	}
	m_btnColrFg.SetTooltipText(_T("前景色"));
	PostMessage(WM_MOUSELEAVE);
}


void CLyricBar::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tm;
	tm.cbSize = sizeof(TRACKMOUSEEVENT);
	tm.dwFlags = TME_LEAVE | TME_HOVER;
	tm.dwHoverTime = 3000;
	tm.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tm);
	ShowButton(SW_SHOW);
	if (g_bIsDeskLrc)
	{
		m_nHitTest = HitTest(point);
		SetLayeredWindowAttributes(g_crBlack, 180, LWA_ALPHA);
	}
	else
	{
		CPoint pt(point);
		ClientToScreen(&pt);
		g_sz_p_wnd[WND_ZMS]->ScreenToClient(&pt);
		g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MOUSEMOVE, nFlags, MAKELPARAM(pt.x, pt.y));
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CLyricBar::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt;
	CRect rc;
	GetCursorPos(&pt);
	GetWindowRect(&rc);
	if (!rc.PtInRect(pt))
	{
		ShowButton(SW_HIDE);
		if (g_bIsDeskLrc)
			SetLayeredWindowAttributes(g_crBlack, 180, LWA_COLORKEY);
		else
			g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MOUSELEAVE);
	}
	CDialogEx::OnMouseLeave();
}


void CLyricBar::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt(point);
	ClientToScreen(&pt);
	g_sz_p_wnd[WND_ZMS]->ScreenToClient(&pt);
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MOUSEHOVER, nFlags, MAKELPARAM(pt.x, pt.y));
	CDialogEx::OnMouseHover(nFlags, point);
}


void CLyricBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (g_bIsDeskLrc)
	{
		ScaleWnd(m_nHitTest, point);
	}
	else
	{
		CPoint pt(point);
		ClientToScreen(&pt);
		g_sz_p_wnd[WND_ZMS]->ScreenToClient(&pt);
		g_sz_p_wnd[WND_ZMS]->PostMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(pt.x, pt.y));
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CLyricBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (g_bIsDeskLrc)
	{

	}
	else
	{
		CPoint pt(point);
		ClientToScreen(&pt);
		g_sz_p_wnd[WND_ZMS]->ScreenToClient(&pt);
		g_sz_p_wnd[WND_ZMS]->PostMessage(WM_LBUTTONDBLCLK, nFlags, MAKELPARAM(pt.x, pt.y));
	}
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


int CLyricBar::HitTest(CPoint point)
{
	static CRect rect;
	GetClientRect(&rect);
	int nHitTest = HTNOWHERE;
	if (point.x <= rect.left + 3 && point.y <= rect.top + 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		nHitTest = HTTOPLEFT;
	}
	else if (point.x >= rect.right - 3 && point.y <= rect.top + 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
		nHitTest = HTTOPRIGHT;
	}
	else if (point.x <= rect.left + 3 && point.y >= rect.bottom - 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
		nHitTest = HTBOTTOMLEFT;
	}
	else if (point.x >= rect.right - 3 && point.y >= rect.bottom - 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		nHitTest = HTBOTTOMRIGHT;
	}
	else if (point.x <= rect.left + 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		nHitTest = HTLEFT;
	}
	else if (point.x >= rect.right - 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		nHitTest = HTRIGHT;
	}
	else if (point.y <= rect.top + 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		nHitTest = HTTOP;
	}
	else if (point.y >= rect.bottom - 3)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		nHitTest = HTBOTTOM;
	}
	else
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		nHitTest = HTNOWHERE;
	}
	return nHitTest;
}


void CLyricBar::ScaleWnd(int nHitTest, CPoint point)
{
	if (nHitTest == HTTOP)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTBOTTOM)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTLEFT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTRIGHT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTTOPLEFT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTTOPRIGHT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTBOTTOMLEFT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));
	}
	else if (nHitTest == HTBOTTOMRIGHT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		PostMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));
	}
	else
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		//SendMessage(WM_SYSCOMMAND,SC_MOVE + HTCAPTION,0);  
	}
}


void CLyricBar::ShowButton(const int & nShowFlag)
{
	m_btnDeskLrc.ShowWindow(nShowFlag);
	m_btnFont.ShowWindow(nShowFlag);
	m_btnSizeUp.ShowWindow(nShowFlag);
	m_btnSizeDown.ShowWindow(nShowFlag);
	m_btnColrBk.ShowWindow(nShowFlag);
	m_btnColrFg.ShowWindow(nShowFlag);
}
