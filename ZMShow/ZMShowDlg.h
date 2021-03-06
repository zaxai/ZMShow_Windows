
// ZMShowDlg.h: 头文件
//

#pragma once
#include "afxwin.h"
#include "ZPlayer.h"
#include "atltypes.h"

class CTitleBar;
class COperateBar;
class CListBar;
class CSetting;
class CLyricBar;
class CToolTipBar;

// CZMShowDlg 对话框
class CZMShowDlg : public CDialog
{
// 构造
public:
	CZMShowDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CZMShowDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZMSHOW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	ZStatic m_staticPlayWnd;//视频播放窗体
	CLyricBar *m_p_LyricBar;//歌词窗体
	CTitleBar * m_p_TitleBar;//标题栏
	COperateBar *m_p_OpeBar;//操作栏
	CListBar *m_p_ListBar;//列表栏
	CToolTipBar *m_p_ToolTipBar;//信息提示栏
	CSetting *m_p_Setting;//设置窗体
	int m_nShowLyricBar;
	int m_nShowTitleBar;
	int m_nShowOpeBar;
	int m_nShowListBar;
	bool m_bIsInitFinish;
	CBrush m_brushBkGnd;
	ZPlayer m_player;//播放器对象
	CString m_strFilePath;//当前文件路径
	CString m_strFolderPath;//当前文件夹路径
	int m_nWndState;//当前窗体尺寸状态，参考enum WND_STATE
	int m_nLastWndState;//上一次窗体尺寸状态，参考enum WND_STATE
	int m_nHitTest;//鼠标移动点测试，用于窗体缩放
	CMenu m_menuPlay;//视频播放窗体的右键菜单
	bool m_bIsModeMini;
	bool m_bIsModeFull;
	bool m_bIsDblClk;
	CPoint m_ptDown;
	NOTIFYICONDATA m_nid;
	bool m_bIsExit;
	int m_nTopMostState;//窗口最前状态，参考enum TOPMOST_STATE
	CString m_strToolTip;//信息提示栏数据
	static HHOOK m_hKeyboard;//热键hook
	enum TOPMOST_STATE
	{
		TOPMOST_STATE_NO = 0,
		TOPMOST_STATE_ALWAYS,
		TOPMOST_STATE_PLAY
	};
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTraynotify(WPARAM wParam, LPARAM lParam);
public:
	void InitLyricBar(void);
	void InitTitleBar(void);
	void InitOperateBar(void);
	void InitListBar(void);
	void InitToolTipBar(void);
	void InitSetting(void);
	void InitMenu();
	void InitTray(void);
	void SetBarPos(void);//窗体大小显示布局
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void OperateWnd(int nWndState, int nTgtWndState);//窗体大小变化操作
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	int HitTest(CPoint point);
	void ShowBar(CPoint pt);//鼠标移动时的各栏显示操作
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void ScaleWnd(int nHitTest, CPoint point);//窗体缩放
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point);
	afx_msg void OnOpenFile();
	afx_msg void OnOpenFolder();
	afx_msg void OnSetting();
	afx_msg void OnExit();
	afx_msg void OnModeFull();
	afx_msg void OnModeMini();
	afx_msg void OnModeNoneblack();
	afx_msg void OnDeskLrc();
	afx_msg void OnItemPlay();
	afx_msg void OnItemDelete();
	afx_msg void OnOpenFilecatalog();
	afx_msg void OnRefreshCatalog();
	afx_msg void OnEmptyList();
	afx_msg void OnRefreshList();
	afx_msg void OnListmodeListseq();
	afx_msg void OnListmodeListloop();
	afx_msg void OnListmodeListrand();
	afx_msg void OnListmodeCatalogseq();
	afx_msg void OnListmodeCatalogloop();
	afx_msg void OnListmodeCatalogrand();
	afx_msg void OnListmodeSingleseq();
	afx_msg void OnListmodeSingleloop();
	void ProBar(void);//迷你模式，全屏模式和常规模式间的切换操作
	void ProLrcBar(void);//桌面歌词模式的切换操作
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	void SetLastPlayProgress();//设置播放进度
	void GetLastPlayProgress();//获取播放进度
	void ProCmdPath(CString strCmdPath);//处理命令行，鼠标双击影音文件操作
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnClose();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void FileTypeIsAudioByPlay();//音频特殊处理，歌词调用等
	void FileTypeIsAudioByStop();//音频特殊处理，歌词关闭等
	static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	static void ProHotKey(UINT nKey);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	void ChangeListBar();
	afx_msg void OnModeNotopmost();
	afx_msg void OnModeAlwaystopmost();
	afx_msg void OnModePlaytopmost();
	void ProTopMost(int nPlayerState = ZPlayer::STATE_NOTHINGSPECIAL);//处理窗口最前操作
	void CheckTopMostMenu();
};
