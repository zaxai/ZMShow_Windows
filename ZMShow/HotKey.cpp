// HotKey.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZMShow.h"
#include "HotKey.h"
#include "afxdialogex.h"


// CHotKey �Ի���

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


// CHotKey ��Ϣ�������


BOOL CHotKey::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	g_sz_p_wnd[WND_HOTKEY] = this;
	m_brushBkGnd.CreateSolidBrush(g_crWhite);
	InitList();
	InsertListData();
	InitVariables();
	Check();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CHotKey::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
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
	m_listHotKey.InsertColumn(0, _T("����"), LVCFMT_CENTER, rc.Width() * 3 / 10, 0);
	m_listHotKey.InsertColumn(1, _T("�ȼ�(˫���޸�)"), LVCFMT_CENTER, rc.Width() * 6 / 10, 1);
	m_listHotKey.EnableEdit(TRUE);
}


void CHotKey::InsertListData(void)
{
	m_listHotKey.DeleteAllItems();
	m_listHotKey.InsertItem(0, _T("����/��ͣ"));
	m_listHotKey.SetItemText(0, 1, _T("�ո�"));
	m_listHotKey.InsertItem(1, _T("ֹͣ"));
	m_listHotKey.SetItemText(1, 1, _T("S"));
	m_listHotKey.InsertItem(2, _T("�ϸ��ļ�"));
	m_listHotKey.SetItemText(2, 1, _T("PageUp"));
	m_listHotKey.InsertItem(3, _T("�¸��ļ�"));
	m_listHotKey.SetItemText(3, 1, _T("PageDown"));
	m_listHotKey.InsertItem(4, _T("����/ȡ������"));
	m_listHotKey.SetItemText(4, 1, _T("M"));
	m_listHotKey.InsertItem(5, _T("�������"));
	m_listHotKey.SetItemText(5, 1, _T("��"));
	m_listHotKey.InsertItem(6, _T("��������"));
	m_listHotKey.SetItemText(6, 1, _T("��"));
	m_listHotKey.InsertItem(7, _T("���"));
	m_listHotKey.SetItemText(7, 1, _T("��"));
	m_listHotKey.InsertItem(8, _T("����"));
	m_listHotKey.SetItemText(8, 1, _T("��"));
	m_listHotKey.InsertItem(9, _T("ȫ��/�˳�ȫ��"));
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
	// TODO: �ڴ˴����ʵ�ִ���.
	int nSize = g_vec_strHotKey.size();
	nSize = nSize > nIndex ? nIndex : nSize;
	for (int i = 0; i < nSize; ++i)
	{
		if (g_vec_strHotKey[i] == strHotKey)
			return true;
	}
	return false;
}
