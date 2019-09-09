// Relation.cpp : 实现文件
//

#include "stdafx.h"
#include "ZMShow.h"
#include "Relation.h"
#include "afxdialogex.h"


// CRelation 对话框

IMPLEMENT_DYNAMIC(CRelation, CDialogEx)

CRelation::CRelation(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RELATION, pParent)
	, m_nIsVideoRelated(FALSE)
	, m_nIsAudioRelated(FALSE)
	, m_nIsPicRelated(FALSE)
	, m_strVideoExt(_T(""))
	, m_strAudioExt(_T(""))
	, m_strPicExt(_T(""))
{

}

CRelation::~CRelation()
{
}

void CRelation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_VIDEO, m_checkVideo);
	DDX_Control(pDX, IDC_CHECK_AUDIO, m_checkAudio);
	DDX_Control(pDX, IDC_CHECK_PIC, m_checkPic);
	DDX_Control(pDX, IDC_EDIT_VIDEO, m_editVideo);
	DDX_Control(pDX, IDC_EDIT_AUDIO, m_editAudio);
	DDX_Control(pDX, IDC_EDIT_PIC, m_editPic);
	DDX_Control(pDX, IDC_BUTTON_REMINDVIDEO, m_btnRemindVideo);
	DDX_Control(pDX, IDC_BUTTON_REMINDAUDIO, m_btnRemindAudio);
	DDX_Control(pDX, IDC_BUTTON_REMINDPIC, m_btnRemindPic);
}


BEGIN_MESSAGE_MAP(CRelation, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MSGRECVPRO, &CRelation::OnMsgrecvpro)
END_MESSAGE_MAP()


// CRelation 消息处理程序


BOOL CRelation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_sz_p_wnd[WND_RELATION] = this;
	m_brushBkGnd.CreateSolidBrush(g_crWhite);
	InitCheckBox();
	InitEdit();
	InitVariables();
	Check();
	InitButton();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CRelation::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return m_brushBkGnd;
	return hbr;
}


afx_msg LRESULT CRelation::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_SETTING_CANCEL:
	{
		m_checkVideo.SetCheck(m_nIsVideoRelated);
		m_checkAudio.SetCheck(m_nIsAudioRelated);
		m_checkPic.SetCheck(m_nIsPicRelated);
		m_editVideo.SetWindowText(m_strVideoExt);
		m_editAudio.SetWindowText(m_strAudioExt);
		m_editPic.SetWindowText(m_strPicExt);
	}
	break;
	case MSGUSER_SETTING_APPLY:
	case MSGUSER_SETTING_OK:
	{
		CString strExtNew, strIconPath;
		m_editVideo.GetWindowText(strExtNew);
		strIconPath = ZUtil::GetExeCatalogPath() + _T("\\icons\\VideoIcon.ico");
		if (m_setting.SetRelationVideo(m_checkVideo.GetCheck(), strExtNew, strIconPath))
		{
			m_nIsVideoRelated = m_checkVideo.GetCheck();
			m_editVideo.GetWindowText(m_strVideoExt);
		}
		else
		{
			if(m_checkVideo.GetCheck()!= m_nIsVideoRelated)
				m_btnRemindVideo.ShowWindow(SW_SHOW);
			m_checkVideo.SetCheck(m_nIsVideoRelated);
			m_editVideo.SetWindowText(m_strVideoExt);
		}
		m_editAudio.GetWindowText(strExtNew);
		strIconPath = ZUtil::GetExeCatalogPath() + _T("\\icons\\AudioIcon.ico");
		if (m_setting.SetRelationAudio(m_checkAudio.GetCheck(), strExtNew, strIconPath))
		{
			m_nIsAudioRelated = m_checkAudio.GetCheck();
			m_editAudio.GetWindowText(m_strAudioExt);
		}
		else
		{
			if (m_checkAudio.GetCheck() != m_nIsAudioRelated)
				m_btnRemindAudio.ShowWindow(SW_SHOW);
			m_checkAudio.SetCheck(m_nIsAudioRelated);
			m_editAudio.SetWindowText(m_strAudioExt);
		}
		m_editPic.GetWindowText(strExtNew);
		strIconPath = ZUtil::GetExeCatalogPath() + _T("\\icons\\PicIcon.ico");
		if (m_setting.SetRelationPic(m_checkPic.GetCheck(), strExtNew, strIconPath))
		{
			m_nIsPicRelated = m_checkPic.GetCheck();
			m_editPic.GetWindowText(m_strPicExt);
		}
		else
		{
			if (m_checkPic.GetCheck() != m_nIsPicRelated)
				m_btnRemindPic.ShowWindow(SW_SHOW);
			m_checkPic.SetCheck(m_nIsPicRelated);
			m_editPic.SetWindowText(m_strPicExt);
		}
		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	}
	break;
	case MSGUSER_SETTING_RECOVERPAGE:
	{
		InitCheckBox();
		InitEdit();
	}
	break;
	}
	return 0;
}


void CRelation::InitCheckBox(void)
{
	m_checkVideo.SetCheck(0);
	m_checkAudio.SetCheck(0);
	m_checkPic.SetCheck(0);
}


void CRelation::InitEdit(void)
{
	const CString sz_strVideoExt[] = { _T(".avi"),_T(".wm"),_T(".wmv"),_T(".rm"),_T(".rmvb"),_T(".mpeg"),_T(".mpg"),_T(".mp4"),_T(".mpeg4"),_T(".3gp"),_T(".mov"),_T(".qt"),_T(".flv"),_T(".mkv") };
	const CString sz_strAudioExt[] = { _T(".mp2"),_T(".mp3"),_T(".wav"),_T(".wma"),_T(".mid") };
	const CString sz_strPicExt[] = { _T(".bmp"),_T(".dib"),_T(".gif"),_T(".jpg"),_T(".jpeg"),_T(".jpe"),_T(".png"),_T(".tif"),_T(".tiff") };
	m_editVideo.SetWindowText(_T(""));
	m_editAudio.SetWindowText(_T(""));
	m_editPic.SetWindowText(_T(""));
	int nSize = 0;
	nSize = sizeof(sz_strVideoExt) / sizeof(CString);
	for (int i = 0; i<nSize; ++i)
	{
		m_editVideo.SetSel(-1, -1);
		m_editVideo.ReplaceSel(sz_strVideoExt[i]);
	}
	nSize = sizeof(sz_strAudioExt) / sizeof(CString);
	for (int i = 0; i<nSize; ++i)
	{
		m_editAudio.SetSel(-1, -1);
		m_editAudio.ReplaceSel(sz_strAudioExt[i]);
	}
	nSize = sizeof(sz_strPicExt) / sizeof(CString);
	for (int i = 0; i<nSize; ++i)
	{
		m_editPic.SetSel(-1, -1);
		m_editPic.ReplaceSel(sz_strPicExt[i]);
	}
}


void CRelation::InitVariables(void)
{
	m_editVideo.GetWindowText(m_strVideoExt);
	m_editAudio.GetWindowText(m_strAudioExt);
	m_editPic.GetWindowText(m_strPicExt);
}


void CRelation::Check(void)
{
	m_nIsVideoRelated = m_setting.CheckRelationVideo();
	m_checkVideo.SetCheck(m_nIsVideoRelated);
	if (m_nIsVideoRelated)
	{
		m_editVideo.SetWindowText(m_setting.GetVideoExt());
	}
	m_nIsAudioRelated = m_setting.CheckRelationAudio();
	m_checkAudio.SetCheck(m_nIsAudioRelated);
	if (m_nIsAudioRelated)
	{
		m_editAudio.SetWindowText(m_setting.GetAudioExt());
	}
	m_nIsPicRelated = m_setting.CheckRelationPic();
	m_checkPic.SetCheck(m_nIsPicRelated);
	if (m_nIsPicRelated)
	{
		m_editPic.SetWindowText(m_setting.GetPicExt());
	}
}


void CRelation::InitButton(void)
{
	SetBtnColor(m_btnRemindVideo, _T("需要管理员权限才能设置，请关闭程序，右键“以管理员身份运行”"));
	m_btnRemindVideo.SetIcon(IDI_ICON_REMIND, IDI_ICON_REMIND);
	m_btnRemindVideo.ShowWindow(SW_HIDE);
	SetBtnColor(m_btnRemindAudio, _T("需要管理员权限才能设置，请关闭程序，右键“以管理员身份运行”"));
	m_btnRemindAudio.SetIcon(IDI_ICON_REMIND, IDI_ICON_REMIND);
	m_btnRemindAudio.ShowWindow(SW_HIDE);
	SetBtnColor(m_btnRemindPic, _T("需要管理员权限才能设置，请关闭程序，右键“以管理员身份运行”"));
	m_btnRemindPic.SetIcon(IDI_ICON_REMIND, IDI_ICON_REMIND);
	m_btnRemindPic.ShowWindow(SW_HIDE);
}


void CRelation::SetBtnColor(ZButton & btn, const CString & strTip)
{
	//btn.DrawTransparent(TRUE);
	btn.DrawBorder(FALSE);
	btn.SetFlat(TRUE);
	btn.SetColor(ZButton::COLOR_BK_IN, g_crWhite);
	btn.SetColor(ZButton::COLOR_BK_OUT, g_crWhite);
	btn.SetColor(ZButton::COLOR_TEXT_IN, g_crWhite);
	btn.SetColor(ZButton::COLOR_TEXT_OUT, g_crWhite);
	btn.SetTooltipText(strTip);
	//btn.SetBtnCursor(IDC_CURSOR_INBTN);
}