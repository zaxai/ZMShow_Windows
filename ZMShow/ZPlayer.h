#pragma once
#include <iostream>
#include "ZUtil.h"
#include "vlc/vlc.h"
#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")
class ZPlayer
{
private:
	libvlc_instance_t * m_p_vlcInst;//VLCÊµÀý
	libvlc_media_t * m_p_vlcMedia;//VLCÃ½Ìå
    libvlc_media_player_t * m_p_vlcPlayer;//VLC²¥·ÅÆ÷
	HWND m_hWnd;
	CString m_strPath;
	bool CreatePlayer(const char * pPath);
public:
	enum STATE
	{
		STATE_NOTHINGSPECIAL = 0,
		STATE_OPENING,
		STATE_BUFFERING,
		STATE_PLAYING,
		STATE_PAUSED,
		STATE_STOPPED,
		STATE_ENDED,
		STATE_ERROR
	};
	enum FILETYPE
	{
		TYPE_VIDEO = 0,
		TYPE_AUDIO,
		TYPE_PIC,
		TYPE_OTHER,
	};
	ZPlayer(void);
	~ZPlayer(void);
	void Init(void);
	void Release(void);
	bool CreatePlayer(const CString & strPath);
	void Stop(void);
	int Play(void);
	void Pause(void);
	void SetHWND(HWND hWnd);
	__int64 GetTotalTime();
	__int64 GetTime();
	void SetTime(__int64 nTime);
	int GetVolume();
	void SetVolume(int nVol);
	void GetVersion(char * pBuf,int nBufLen);
	int GetState(void);
	int GetSize(unsigned int & nWidth,unsigned int & nHeight);
	CString GetPath();
	static int GetFileType(const CString & strFilePath);
	int GetFileType(void);
};

