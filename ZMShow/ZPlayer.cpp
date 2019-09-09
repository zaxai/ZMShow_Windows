#include "StdAfx.h"
#include "ZPlayer.h"


ZPlayer::ZPlayer(void)
	: m_p_vlcInst(NULL)
	, m_p_vlcMedia(NULL)
	, m_p_vlcPlayer(NULL)
	, m_hWnd(NULL)
	, m_strPath(_T(""))
{
}


ZPlayer::~ZPlayer(void)
{
	Release();
}


void ZPlayer::Init(void)
{
	if (!m_p_vlcInst)
	{
		m_p_vlcInst=libvlc_new(0, NULL);
	}
}


void ZPlayer::Release(void)
{
	Stop();
    if(m_p_vlcInst)
    {
        libvlc_release (m_p_vlcInst);
        m_p_vlcInst=NULL;
    }
}


bool ZPlayer::CreatePlayer(const char * pPath)
{
	if(!m_p_vlcInst)
	{
		Init();
	}
	if(!m_p_vlcInst)
		return false;
	m_p_vlcMedia=NULL;
    m_p_vlcMedia=libvlc_media_new_location(m_p_vlcInst,pPath);
	if(m_p_vlcMedia)
	{
		m_p_vlcPlayer=libvlc_media_player_new_from_media(m_p_vlcMedia);
		if(m_p_vlcPlayer)
			return true;
	}
	return false;
}

bool ZPlayer::CreatePlayer(const CString & strPath)
{
	m_strPath = strPath;
	char szPath[MAX_PATH] = { 0 };
	if(ZUtil::TtoUTF8(strPath, szPath, MAX_PATH))
		return CreatePlayer(szPath);
	return false;
}

void ZPlayer::Stop(void)
{
	if(m_p_vlcPlayer)
    {
        libvlc_media_player_stop (m_p_vlcPlayer);  
        m_p_vlcPlayer=NULL;
		libvlc_media_release(m_p_vlcMedia);
		m_p_vlcMedia=NULL;
		m_hWnd=NULL;
		m_strPath.Empty();
    }
}


int ZPlayer::Play(void)
{
	int nRtn=-1;
	if(m_p_vlcPlayer)
    {
        nRtn=libvlc_media_player_play(m_p_vlcPlayer);
    }
	return nRtn;
}


void ZPlayer::Pause(void)
{
	if(m_p_vlcPlayer)
    {
        libvlc_media_player_pause(m_p_vlcPlayer);
    }
}


void ZPlayer::SetHWND(HWND hWnd)
{
	if(!m_hWnd)
	{
		m_hWnd=hWnd;
		libvlc_media_player_set_hwnd(m_p_vlcPlayer,m_hWnd);
	}
}

__int64 ZPlayer::GetTotalTime()
{
	if(m_p_vlcPlayer)
	{
		return libvlc_media_player_get_length(m_p_vlcPlayer);
	}
	return 0;
}

__int64 ZPlayer::GetTime()
{
	if(m_p_vlcPlayer)
	{
		return libvlc_media_player_get_time(m_p_vlcPlayer);
	}
	return 0;
}

void ZPlayer::SetTime(__int64 nTime)
{
	if(m_p_vlcPlayer)
	{
		libvlc_media_player_set_time(m_p_vlcPlayer,nTime);
	}
}

int ZPlayer::GetVolume()
{
    if(m_p_vlcPlayer)
    {
        return libvlc_audio_get_volume(m_p_vlcPlayer);
    }
    return 0;
}

void ZPlayer::SetVolume(int nVol)
{
    if(m_p_vlcPlayer)
    {
        libvlc_audio_set_volume(m_p_vlcPlayer,nVol);
    }
}

void ZPlayer::GetVersion(char * pBuf,int nBufLen)
{
	strcpy_s(pBuf,nBufLen,libvlc_get_version()); 
}


int ZPlayer::GetState(void)
{
	if(m_p_vlcPlayer)
    {
        return libvlc_media_get_state(m_p_vlcMedia);
    }
	return 0;
}


int ZPlayer::GetSize(unsigned int & nWidth,unsigned int & nHeight)
{
	if(m_p_vlcPlayer)
    {
        return libvlc_video_get_size(m_p_vlcPlayer,0,&nWidth,&nHeight);
    }
	return -1;
}


CString ZPlayer::GetPath(void)
{
	return m_strPath;
}


int ZPlayer::GetFileType(const CString & strFilePath)
{
	const CString sz_strVideoExt[] = { _T(".avi"),_T(".wm"),_T(".wmv"),_T(".rm"),_T(".rmvb"),_T(".mpeg"),_T(".mpg"),_T(".mp4"),_T(".mpeg4"),_T(".3gp"),_T(".mov"),_T(".qt"),_T(".flv"),_T(".mkv") };
	const CString sz_strAudioExt[] = { _T(".mp2"),_T(".mp3"),_T(".wav"),_T(".wma"),_T(".mid") };
	const CString sz_strPicExt[] = { _T(".bmp"),_T(".dib"),_T(".gif"),_T(".jpg"),_T(".jpeg"),_T(".jpe"),_T(".png"),_T(".tif"),_T(".tiff") };
	CString strFileExt;
	strFileExt = strFilePath.Mid(strFilePath.ReverseFind(_T('.')));
	strFileExt.MakeLower();
	int nSize = 0;
	nSize = sizeof(sz_strVideoExt) / sizeof(CString);
	for (int i = 0; i<nSize; ++i)
	{
		if (strFileExt == sz_strVideoExt[i])
			return TYPE_VIDEO;
	}
	nSize = sizeof(sz_strAudioExt) / sizeof(CString);
	for (int i = 0; i<nSize; ++i)
	{
		if (strFileExt == sz_strAudioExt[i])
			return TYPE_AUDIO;
	}
	nSize = sizeof(sz_strPicExt) / sizeof(CString);
	for (int i = 0; i<nSize; ++i)
	{
		if (strFileExt == sz_strPicExt[i])
			return TYPE_PIC;
	}
	return TYPE_OTHER;
}


int ZPlayer::GetFileType(void)
{
	return GetFileType(m_strPath);
}