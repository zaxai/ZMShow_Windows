#pragma once
#include "ZUtil.h"
#include "ZOperateRegedit.h"
#include "ZSqlite3.h"

class ZSetting
{
private:
	CString m_strPathDB;
	BOOL InitTable(ZSqlite3 & zsql);
	BOOL SetRegRelation(BOOL nIsRelated, CString & strExtOld, CString & strExtNew, const CString & strIconPath);
public:
	enum ID
	{
		IsClearHistory = 0,
		IsSavePlayProgress,
		IsFolderAutoSel,
		IsVideoRelated,
		IsAudioRelated,
		IsPicRelated,
		VideoExt,
		AudioExt,
		PicExt,
		HotKey0,
		HotKey1,
		HotKey2,
		HotKey3,
		HotKey4,
		HotKey5,
		HotKey6,
		HotKey7,
		HotKey8,
		HotKey9
	};
	ZSetting();
	~ZSetting();
	BOOL CheckAutoRun();
	BOOL SetAutoRun(BOOL nIsAutoRun);
	BOOL CheckClearHistory();
	BOOL SetClearHistory(BOOL nIsClearHistory);
	BOOL CheckSavePlayProgress();
	BOOL SetSavePlayProgress(BOOL nIsSavePlayProgress);
	BOOL CheckFolderAutoSel();
	BOOL SetFolderAutoSel(BOOL nIsFolderAutoSel);
	BOOL CheckRelationVideo();
	BOOL SetRelationVideo(BOOL nIsRelated, CString strExtNew, const CString & strIconPath);
	BOOL CheckRelationAudio();
	BOOL SetRelationAudio(BOOL nIsRelated, CString strExtNew, const CString & strIconPath);
	BOOL CheckRelationPic();
	BOOL SetRelationPic(BOOL nIsRelated, CString strExtNew, const CString & strIconPath);
	CString GetVideoExt();
	BOOL SetVideoExt(CString strVideoExt);
	CString GetAudioExt();
	BOOL SetAudioExt(CString strAudioExt);
	CString GetPicExt();
	BOOL SetPicExt(CString strPicExt);
	CString GetHotKey(int nIndex);
	BOOL SetHotKey(int nIndex, const CString & strHotKey);
	BOOL CheckFileRelation(const CString & strExt, const CString & strExePath);
	BOOL SetFileRelation(const CString & strExt, const CString & strExePath, const CString & strDefaultIcon);
	BOOL BackupFileRelation(const CString & strExt);
	BOOL RecoverFileRelation(const CString & strExt);
};

