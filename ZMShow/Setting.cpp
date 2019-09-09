// Setting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZMShow.h"
#include "Setting.h"
#include "afxdialogex.h"


// CSetting �Ի���
const int nTabWidth = 90;
const int nTabHeight = 30;

IMPLEMENT_DYNAMIC(CSetting, CDialogEx)

CSetting::CSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTING, pParent)
{
	for (int i = 0; i < sizeof(m_sz_p_wndTab) / sizeof(CWnd *); ++i)
	{
		m_sz_p_wndTab[i] = NULL;
	}
}

CSetting::~CSetting()
{
}

void CSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SETTING, m_tabSetting);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_btnApply);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_BUTTON_RECOVERPAGE, m_btnRecoverPage);
}


BEGIN_MESSAGE_MAP(CSetting, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetting::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDCANCEL, &CSetting::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSetting::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDOK, &CSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RECOVERPAGE, &CSetting::OnBnClickedButtonRecoverpage)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETTING, &CSetting::OnTcnSelchangeTabSetting)
END_MESSAGE_MAP()


// CSetting ��Ϣ�������


BOOL CSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	g_sz_p_wnd[WND_SET] = this;
	InitButton();
	InitTab();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CSetting::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CBrush brushClient(g_crWhite);
	CBrush brushLeft(g_crGrey230);
	CRect rcClient, rcItem, rcLeft;
	GetClientRect(&rcClient);
	m_tabSetting.GetItemRect(0, &rcItem);
	rcLeft = rcClient;
	rcLeft.right = rcItem.right;
	CDC dcMem;                                                           //���ڻ�����ͼ���ڴ�DC
	CBitmap bmp;                                                         //�ڴ��г�����ʱͼ���λͼ
	dcMem.CreateCompatibleDC(pDC);                                       //��������DC���������ڴ�DC
	bmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());  //��������λͼ
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	dcMem.FillRect(&rcClient, &brushClient);
	dcMem.FillRect(&rcLeft, &brushLeft);
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨
	//pDC->TransparentBlt(0,0,rcClient.Width(),rcClient.Height(),&dcMem,0,0,rcClient.Width(),rcClient.Height(),crBlack);//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨
	dcMem.SelectObject(p_bmpOld);
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CSetting::InitButton(void)
{
	SetBtnColor(m_btnOK, _T(""));
	SetBtnColor(m_btnApply, _T(""));
	SetBtnColor(m_btnCancel, _T(""));
	SetBtnColor(m_btnClose, _T("�ر�"));
	SetBtnColor(m_btnRecoverPage, _T(""));
	m_btnClose.DrawBorder(FALSE);
	m_btnClose.SetIcon(IDI_ICON_CLOSEON, IDI_ICON_CLOSE);
}


void CSetting::SetBtnColor(ZButton & btn, const CString & strTip)
{
	//btn.DrawTransparent(TRUE);
	btn.DrawBorder(TRUE);
	btn.SetFlat(TRUE);
	btn.SetColor(ZButton::COLOR_BK_IN, g_crWhite);
	btn.SetColor(ZButton::COLOR_BK_OUT, g_crWhite);
	btn.SetColor(ZButton::COLOR_TEXT_IN, g_crBlue);
	btn.SetColor(ZButton::COLOR_TEXT_OUT, g_crGrey55);
	btn.SetTooltipText(strTip);
	//btn.SetBtnCursor(IDC_CURSOR_INBTN);
}


void CSetting::InitTab(void)
{
	CSize size(nTabHeight, nTabWidth);
	CRect rcTab, rcItem;
	m_tabSetting.InsertItem(0, _T("����"));
	m_tabSetting.InsertItem(1, _T("����"));
	m_tabSetting.InsertItem(3, _T("�ȼ�"));
	m_tabSetting.SetItemSize(size);
	m_tabSetting.SetColor(g_crWhite, g_crGrey230, g_crBlue, g_crGrey200, g_crGrey55);
	m_tabSetting.GetClientRect(&rcTab);
	m_tabSetting.GetItemRect(0, &rcItem);
	rcTab.left = rcItem.right;
	m_common.Create(IDD_COMMON, &m_tabSetting);
	m_common.SetWindowPos(NULL, rcTab.left, rcTab.top, rcTab.Width(), rcTab.Height(), SWP_SHOWWINDOW);
	m_relation.Create(IDD_RELATION, &m_tabSetting);
	m_relation.SetWindowPos(NULL, rcTab.left, rcTab.top, rcTab.Width(), rcTab.Height(), SWP_HIDEWINDOW);
	m_hotkey.Create(IDD_HOTKEY, &m_tabSetting);
	m_hotkey.SetWindowPos(NULL, rcTab.left, rcTab.top, rcTab.Width(), rcTab.Height(), SWP_HIDEWINDOW);
	m_sz_p_wndTab[0] = &m_common;
	m_sz_p_wndTab[1] = &m_relation;
	m_sz_p_wndTab[2] = &m_hotkey;
}

void CSetting::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PostMessage(WM_CLOSE);
}


void CSetting::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < sizeof(m_sz_p_wndTab) / sizeof(CWnd *); ++i)
	{
		if (m_sz_p_wndTab[i])
		{
			m_sz_p_wndTab[i]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTING_CANCEL);
		}
	}
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_CLOSESETTING);
	CDialogEx::OnCancel();
}


void CSetting::OnBnClickedButtonApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < sizeof(m_sz_p_wndTab) / sizeof(CWnd *); ++i)
	{
		if (m_sz_p_wndTab[i])
		{
			m_sz_p_wndTab[i]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTING_APPLY);
		}
	}
}


void CSetting::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < sizeof(m_sz_p_wndTab) / sizeof(CWnd *); ++i)
	{
		if (m_sz_p_wndTab[i])
		{
			m_sz_p_wndTab[i]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTING_OK);
		}
	}
	g_sz_p_wnd[WND_ZMS]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_CLOSESETTING);
	CDialogEx::OnOK();
}


void CSetting::OnBnClickedButtonRecoverpage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < sizeof(m_sz_p_wndTab) / sizeof(CWnd *); ++i)
	{
		if (m_sz_p_wndTab[i] && i == m_tabSetting.GetSelectedItem())
		{
			m_sz_p_wndTab[i]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SETTING_RECOVERPAGE);
		}
	}
}


void CSetting::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CSetting::OnTcnSelchangeTabSetting(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	for (int i = 0; i < sizeof(m_sz_p_wndTab) / sizeof(CWnd *); ++i)
	{
		if (m_sz_p_wndTab[i])
		{
			if (i == m_tabSetting.GetSelectedItem())
				m_sz_p_wndTab[i]->ShowWindow(SW_SHOW);
			else
				m_sz_p_wndTab[i]->ShowWindow(SW_HIDE);
		}
	}
}
