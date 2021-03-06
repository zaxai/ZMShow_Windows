
// ZMShow.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ZClassLib.h"
#include <GdiPlus.h>
#pragma comment(lib, "Gdiplus.lib")

// CZMShowApp:
// 有关此类的实现，请参阅 ZMShow.cpp
//

class CZMShowApp : public CWinApp
{
public:
	CZMShowApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
private:
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	HANDLE m_hMutex;
public:
	virtual int ExitInstance();
};

extern CZMShowApp theApp;
enum WND_POINT
{
	WND_ZMS = 0,
	WND_TIT,
	WND_OPE,
	WND_LIS,
	WND_SET,
	WND_COMMON,
	WND_RELATION,
	WND_HOTKEY,
	WND_LRC,
	WND_TOOLTIP
};
enum WND_STATE
{
	WND_STATE_NORMAL = 0,
	WND_STATE_MAX,
	WND_STATE_FULL
};

extern CWnd * g_sz_p_wnd[10];
extern COLORREF g_crBlack;
extern COLORREF g_crWhite;
extern COLORREF g_crBlue;
extern COLORREF g_crGrey11;
extern COLORREF g_crGrey55;
extern COLORREF g_crGrey100;
extern COLORREF g_crGrey200;
extern COLORREF g_crGrey230;
extern COLORREF g_crRed;
extern COLORREF g_crTransparent;

extern bool g_bIsTimeInfoTotal;
extern bool g_bIsMute;
extern bool g_bIsFocusCombo;
extern bool g_bIsOnSetting;
extern bool g_bIsDeskLrc;
extern std::vector<CString> g_vec_strHotKey;

#define WM_MSGRECVPRO WM_USER+1
#define WM_TRAYNOTIFY WM_USER+2
enum MSGUSER
{
	MSGUSER_PLAYBTN = 0,
	MSGUSER_STOPBTN,
	MSGUSER_PREVBTN,
	MSGUSER_NEXTBTN,
	MSGUSER_VOLUMEBTN,
	MSGUSER_OPENFILE,
	MSGUSER_OPENFOLDER,
	MSGUSER_SETMAXTIME,
	MSGUSER_SETCURTIME,
	MSGUSER_SETTIMEINFO,
	MSGUSER_SETVOLUME,
	MSGUSER_SETVOLUMEUP,
	MSGUSER_SETVOLUMEDOWN,
	MSGUSER_SETTIME,
	MSGUSER_SETTIMEUP,
	MSGUSER_SETTIMEDOWN,
	MSGUSER_MINBTN,
	MSGUSER_MAXBTN,
	MSGUSER_CLOSEBTN,
	MSGUSER_FULLBTN,
	MSGUSER_LISTBTN,
	MSGUSER_FILEINSERT,
	MSGUSER_FOLDERINSERT,
	MSGUSER_PLAYFILE,
	MSGUSER_ITEMPLAY,
	MSGUSER_ITEMDELETE,
	MSGUSER_OPENFILECATALOG,
	MSGUSER_REFRESHCATALOG,
	MSGUSER_EMPTYLIST,
	MSGUSER_REFRESHLIST,
	MSGUSER_SETLISTMODE,
	MSGUSER_SETSEARCHITEM,
	MSGUSER_CLOSESETTING,
	MSGUSER_SETTING_CANCEL,
	MSGUSER_SETTING_APPLY,
	MSGUSER_SETTING_OK,
	MSGUSER_SETTING_RECOVERPAGE,
	MSGUSER_INSERTLRC,
	MSGUSER_SETLRCPOS,
	MSGUSER_DESKLRCBTN,
	MSGUSER_DELAYTOOLTIP,
	MSGUSER_ONTIMETOOLTIP,
	MSGUSER_KILLDELAYTOOLTIP
};
