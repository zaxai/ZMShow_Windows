// HotKey.cpp : 实现文件
//

#include "stdafx.h"
#include "ZMShow.h"
#include "HotKey.h"
#include "afxdialogex.h"


// CHotKey 对话框

IMPLEMENT_DYNAMIC(CHotKey, CDialogEx)

CHotKey::CHotKey(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HOTKEY, pParent)
{

}

CHotKey::~CHotKey()
{
}

void CHotKey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HOTKEY, m_listHotKey);
}


BEGIN_MESSAGE_MAP(CHotKey, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MSGRECVPRO, &CHotKey::OnMsgrecvpro)
END_MESSAGE_MAP()


// CHotKey 消息处理程序


BOOL CHotKey::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_sz_p_wnd[WND_HOTKEY] = this;
	m_brushBkGnd.CreateSolidBrush(g_crWhite);
	InitList();
	InsertListData();
	InitVariables();
	Check();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CHotKey::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return m_brushBkGnd;
	return hbr;
}


afx_msg LRESULT CHotKey::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_SETTING_CANCEL:
	{
		int nRow = m_listHotKey.GetItemCount();
		for (int i = 0; i < nRow; ++i)
		{
			m_listHotKey.SetItemText(i, 1, g_vec_strHotKey[i]);
		}
	}
	break;
	case MSGUSER_SETTING_APPLY:
	case MSGUSER_SETTING_OK:
	{
		int nRow = m_listHotKey.GetItemCount();
		CString strHotKey;
		for (int i = 0; i < nRow; ++i)
		{
			strHotKey = m_listHotKey.GetItemText(i, 1);
			m_setting.SetHotKey(i, strHotKey);
			g_vec_strHotKey[i] = strHotKey;
			if (IsHotKeyRepeat(i, g_vec_strHotKey[i]))
				m_listHotKey.SetItemBkColor(i, 1, g_crRed);
			else
				m_listHotKey.SetItemBkColor(i, 1, g_crWhite);
		}
	}
	break;
	case MSGUSER_SETTING_RECOVERPAGE:
	{
		InsertListData();
	}
	break;
	}
	return 0;
}


void CHotKey::InitList(void)
{
	CRect rc;
	m_listHotKey.GetClientRect(&rc);
	m_listHotKey.SetExtendedStyle(m_listHotKey.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listHotKey.InsertColumn(0, _T("操作"), LVCFMT_CENTER, rc.Width() * 3 / 10, 0);
	m_listHotKey.InsertColumn(1, _T("热键(双击修改)"), LVCFMT_CENTER, rc.Width() * 6 / 10, 1);
	m_listHotKey.EnableEdit(TRUE);
}


void CHotKey::InsertListData(void)
{
	m_listHotKey.DeleteAllItems();
	m_listHotKey.InsertItem(0, _T("播放/暂停"));
	m_listHotKey.SetItemText(0, 1, _T("空格"));
	m_listHotKey.InsertItem(1, _T("停止"));
	m_listHotKey.SetItemText(1, 1, _T("S"));
	m_listHotKey.InsertItem(2, _T("上个文件"));
	m_listHotKey.SetItemText(2, 1, _T("PageUp"));
	m_listHotKey.InsertItem(3, _T("下个文件"));
	m_listHotKey.SetItemText(3, 1, _T("PageDown"));
	m_listHotKey.InsertItem(4, _T("静音/取消静音"));
	m_listHotKey.SetItemText(4, 1, _T("M"));
	m_listHotKey.InsertItem(5, _T("提高音量"));
	m_listHotKey.SetItemText(5, 1, _T("↑"));
	m_listHotKey.InsertItem(6, _T("降低音量"));
	m_listHotKey.SetItemText(6, 1, _T("↓"));
	m_listHotKey.InsertItem(7, _T("快进"));
	m_listHotKey.SetItemText(7, 1, _T("→"));
	m_listHotKey.InsertItem(8, _T("快退"));
	m_listHotKey.SetItemText(8, 1, _T("←"));
	m_listHotKey.InsertItem(9, _T("全屏/退出全屏"));
	m_listHotKey.SetItemText(9, 1, _T("F"));
}


void CHotKey::InitVariables()
{
	g_vec_strHotKey.clear();
	int nRow = m_listHotKey.GetItemCount();
	CString strHotKey;
	for (int i = 0; i < nRow; ++i)
	{
		strHotKey = m_listHotKey.GetItemText(i, 1);
		g_vec_strHotKey.push_back(strHotKey);
	}
}


void CHotKey::Check()
{
	int nRow = m_listHotKey.GetItemCount();
	CString strHotKey;
	for (int i = 0; i < nRow; ++i)
	{
		strHotKey = m_setting.GetHotKey(i);
		if (!strHotKey.IsEmpty())
		{
			g_vec_strHotKey[i] = strHotKey;
			m_listHotKey.SetItemText(i, 1, strHotKey);
		}
		else
		{
			m_setting.SetHotKey(i, g_vec_strHotKey[i]);
		}
	}
	for (int i = 0; i < nRow; ++i)
	{
		if (IsHotKeyRepeat(i, g_vec_strHotKey[i]))
			m_listHotKey.SetItemBkColor(i, 1, g_crRed);
		else
			m_listHotKey.SetItemBkColor(i, 1, g_crWhite);
	}
}

bool CHotKey::IsHotKeyRepeat(int nIndex,const CString & strHotKey)
{
	// TODO: 在此处添加实现代码.
	int nSize = g_vec_strHotKey.size();
	nSize = nSize > nIndex ? nIndex : nSize;
	for (int i = 0; i < nSize; ++i)
	{
		if (g_vec_strHotKey[i] == strHotKey)
			return true;
	}
	return false;
}
