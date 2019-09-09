#include "stdafx.h"
#include "ZSetting.h"


ZSetting::ZSetting()
	: m_strPathDB(_T(""))
{
	m_strPathDB = ZUtil::GetExeCatalogPath() + _T("\\Data.db3");
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	InitTable(zsql);
}


ZSetting::~ZSetting()
{
}


BOOL ZSetting::InitTable(ZSqlite3 & zsql)
{
	const CString sz_strIDInfo[] =
	{
		_T("IsClearHistory"),
		_T("IsSavePlayProgress"),
		_T("IsFolderAutoSel"),
		_T("IsVideoRelated"),
		_T("IsAudioRelated"),
		_T("IsPicRelated"),
		_T("VideoExt"),
		_T("AudioExt"),
		_T("PicExt"),
		_T("HotKey0"),
		_T("HotKey1"),
		_T("HotKey2"),
		_T("HotKey3"),
		_T("HotKey4"),
		_T("HotKey5"),
		_T("HotKey6"),
		_T("HotKey7"),
		_T("HotKey8"),
		_T("HotKey9")
	};
	const CString sz_strValue[] =
	{
		_T(""),
		_T("1"),
		_T("1"),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T("")
	};
	CString strSql;
	strSql = _T("CREATE TABLE Setting (ID int PRIMARY KEY NOT NULL,IDInfo nvarchar(100) NOT NULL,Value nvarchar(1000) NOT NULL)");
	if (zsql.ExecSQL(strSql) == 0)
	{
		for (int i = 0; i < sizeof(sz_strIDInfo) / sizeof(CString); ++i)
		{
			strSql.Format(_T("INSERT INTO Setting VALUES ( %d ,'%s','%s')"), i, sz_strIDInfo[i], sz_strValue[i]);
			if (zsql.ExecSQL(strSql))
			{
				zsql.ExecSQL(_T("DROP TABLE Setting"));
				return FALSE;
			}
		}
	}
	return TRUE;
}


BOOL ZSetting::SetRegRelation(BOOL nIsRelated, CString & strExtOld, CString & strExtNew, const CString & strIconPath)
{
	double dVersion = _ttof(ZUtil::GetWinVersion());
	if (dVersion >= 6.0)
	{
		if (!ZUtil::IsRunasAdmin())
			return FALSE;
	}
	std::vector<CString> vec_strExt;
	ZUtil::StrSplit(strExtOld, vec_strExt, _T('.'), strExtOld.Right(1) == _T('.'));
	int nSize = vec_strExt.size();
	for (int i = 1; i < nSize; ++i)
	{
		vec_strExt[i] = _T(".") + vec_strExt[i];
		RecoverFileRelation(vec_strExt[i]);
	}
	if (nIsRelated)
	{
		ZUtil::StrSplit(strExtNew, vec_strExt, _T('.'), strExtNew.Right(1) == _T('.'));
		nSize = vec_strExt.size();
		for (int i = 1; i < nSize; ++i)
		{
			vec_strExt[i] = _T(".") + vec_strExt[i];
			BackupFileRelation(vec_strExt[i]);
			SetFileRelation(vec_strExt[i], ZUtil::GetExePath(), strIconPath);
		}
	}
	else
	{
		strExtNew.Empty();
	}
	return TRUE;
}


BOOL ZSetting::CheckAutoRun()
{
	return ZOperateRegedit::CheckAutoRun(ZUtil::GetExePath(), _T("ZMS"));
}


BOOL ZSetting::SetAutoRun(BOOL nIsAutoRun)
{
	return ZOperateRegedit::SetAutoRun(ZUtil::GetExePath(), _T("ZMS"), nIsAutoRun);
}


BOOL ZSetting::CheckClearHistory()
{
	std::vector<std::vector <CString>>  vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Value FROM Setting WHERE ID = %d"), IsClearHistory);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			return _ttoi(vec2_strData[nRow-1][0]);
		}
	}
	return FALSE;
}


BOOL ZSetting::SetClearHistory(BOOL nIsClearHistory)
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	if (InitTable(zsql) == FALSE)
		return FALSE;
	strSql.Format(_T("UPDATE Setting SET Value = '%d' WHERE ID = %d"), nIsClearHistory, IsClearHistory);
	if (zsql.ExecSQL(strSql, &strError) == 0)
		return TRUE;
	else
		return FALSE;
}


BOOL ZSetting::CheckSavePlayProgress()
{
	std::vector<std::vector <CString>>  vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Value FROM Setting WHERE ID = %d"), IsSavePlayProgress);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			return _ttoi(vec2_strData[nRow - 1][0]);
		}
	}
	return FALSE;
}


BOOL ZSetting::SetSavePlayProgress(BOOL nIsSavePlayProgress)
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	if (InitTable(zsql) == FALSE)
		return FALSE;
	strSql.Format(_T("UPDATE Setting SET Value = '%d' WHERE ID = %d"), nIsSavePlayProgress, IsSavePlayProgress);
	if (zsql.ExecSQL(strSql, &strError) == 0)
		return TRUE;
	else
		return FALSE;
}


BOOL ZSetting::CheckFolderAutoSel()
{
	std::vector<std::vector <CString>>  vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Value FROM Setting WHERE ID = %d"), IsFolderAutoSel);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			return _ttoi(vec2_strData[nRow - 1][0]);
		}
	}
	return FALSE;
}


BOOL ZSetting::SetFolderAutoSel(BOOL nIsFolderAutoSel)
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	if (InitTable(zsql) == FALSE)
		return FALSE;
	strSql.Format(_T("UPDATE Setting SET Value = '%d' WHERE ID = %d"), nIsFolderAutoSel, IsFolderAutoSel);
	if (zsql.ExecSQL(strSql, &strError) == 0)
		return TRUE;
	else
		return FALSE;
}


BOOL ZSetting::CheckRelationVideo()
{
	std::vector<std::vector <CString>>  vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Value FROM Setting WHERE ID = %d"), IsVideoRelated);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			if (_ttoi(vec2_strData[nRow - 1][0]))
			{
				CString strExt = GetVideoExt();
				std::vector<CString> vec_strExt;
				ZUtil::StrSplit(strExt, vec_strExt, _T('.'), strExt.Right(1) == _T('.'));
				if (vec_strExt.size()>1)
				{
					if (CheckFileRelation(_T(".") + vec_strExt[1], ZUtil::GetExePath()))
						return TRUE;
				}
			}
		}
	}
	return FALSE;
}


BOOL ZSetting::SetRelationVideo(BOOL nIsRelated, CString strExtNew, const CString & strIconPath)
{
	CString strExtOld = GetVideoExt();
	if (!SetRegRelation(nIsRelated,strExtOld,strExtNew,strIconPath))
		return FALSE;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	if (InitTable(zsql) == FALSE)
		return FALSE;
	strSql.Format(_T("UPDATE Setting SET Value = '%d' WHERE ID = %d"), nIsRelated, IsVideoRelated);
	if (zsql.ExecSQL(strSql, &strError) == 0)
	{
		strSql.Format(_T("UPDATE Setting SET Value = '%s' WHERE ID = %d"), strExtNew, VideoExt);
		zsql.ExecSQL(strSql, &strError);
		return TRUE;
	}
	else
		return FALSE;
}


BOOL ZSetting::CheckRelationAudio()
{
	std::vector<std::vector <CString>>  vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Value FROM Setting WHERE ID = %d"), IsAudioRelated);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			if (_ttoi(vec2_strData[nRow - 1][0]))
			{
				CString strExt = GetAudioExt();
				std::vector<CString> vec_strExt;
				ZUtil::StrSplit(strExt, vec_strExt, _T('.'), strExt.Right(1) == _T('.'));
				if (vec_strExt.size()>1)
				{
					if (CheckFileRelation(_T(".") + vec_strExt[1], ZUtil::GetExePath()))
						return TRUE;
				}
			}
		}
	}
	return FALSE;
}


BOOL ZSetting::SetRelationAudio(BOOL nIsRelated, CString strExtNew, const CString & strIconPath)
{
	CString strExtOld = GetAudioExt();
	if (!SetRegRelation(nIsRelated, strExtOld, strExtNew, strIconPath))
		return FALSE;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	if (InitTable(zsql) == FALSE)
		return FALSE;
	strSql.Format(_T("UPDATE Setting SET Value = '%d' WHERE ID = %d"), nIsRelated, IsAudioRelated);
	if (zsql.ExecSQL(strSql, &strError) == 0)
	{
		strSql.Format(_T("UPDATE Setting SET Value = '%s' WHERE ID = %d"), strExtNew, AudioExt);
		zsql.ExecSQL(strSql, &strError);
		return TRUE;
	}
	else
		return FALSE;
}


BOOL ZSetting::CheckRelationPic()
{
	std::vector<std::vector <CString>>  vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Value FROM Setting WHERE ID = %d"), IsPicRelated);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			if (_ttoi(vec2_strData[nRow - 1][0]))
			{
				CString strExt = GetPicExt();
				std::vector<CString> vec_strExt;
				ZUtil::StrSplit(strExt, vec_strExt, _T('.'), strExt.Right(1) == _T('.'));
				if (vec_strExt.size()>1)
				{
					if (CheckFileRelation(_T(".") + vec_strExt[1], ZUtil::GetExePath()))
						return TRUE;
				}
			}
		}
	}
	return FALSE;
}


BOOL ZSetting::SetRelationPic(BOOL nIsRelated, CString strExtNew, const CString & strIconPath)
{
	CString strExtOld = GetPicExt();
	if (!SetRegRelation(nIsRelated, strExtOld, strExtNew, strIconPath))
		return FALSE;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	if (InitTable(zsql) == FALSE)
		return FALSE;
	strSql.Format(_T("UPDATE Setting SET Value = '%d' WHERE ID = %d"), nIsRelated, IsPicRelated);
	if (zsql.ExecSQL(strSql, &strError) == 0)
	{
		strSql.Format(_T("UPDATE Setting SET Value = '%s' WHERE ID = %d"), strExtNew, PicExt);
		zsql.ExecSQL(strSql, &strError);
		return TRUE;
	}
	else
		return FALSE;
}


CString ZSetting::GetVideoExt()
{
	std::vector<std::vector <CString>>  vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Value FROM Setting WHERE ID = %d"), VideoExt);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			return  vec2_strData[nRow - 1][0];
		}
	}
	return CString(_T(""));
}


BOOL ZSetting::SetVideoExt(CString strVideoExt)
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	if (InitTable(zsql) == FALSE)
		return FALSE;
	strSql.Format(_T("UPDATE Setting SET Value = '%s' WHERE ID = %d"), strVideoExt, VideoExt);
	if (zsql.ExecSQL(strSql, &strError) == 0)
		return TRUE;
	else
		return FALSE;
}


CString ZSetting::GetAudioExt()
{
	std::vector<std::vector <CString>>  vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Value FROM Setting WHERE ID = %d"), AudioExt);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			return  vec2_strData[nRow - 1][0];
		}
	}
	return CString(_T(""));
}


BOOL ZSetting::SetAudioExt(CString strAudioExt)
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	if (InitTable(zsql) == FALSE)
		return FALSE;
	strSql.Format(_T("UPDATE Setting SET Value = '%s' WHERE ID = %d"), strAudioExt, AudioExt);
	if (zsql.ExecSQL(strSql, &strError) == 0)
		return TRUE;
	else
		return FALSE;
}


CString ZSetting::GetPicExt()
{
	std::vector<std::vector <CString>>  vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Value FROM Setting WHERE ID = %d"), PicExt);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			return  vec2_strData[nRow - 1][0];
		}
	}
	return CString(_T(""));
}


BOOL ZSetting::SetPicExt(CString strPicExt)
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	if (InitTable(zsql) == FALSE)
		return FALSE;
	strSql.Format(_T("UPDATE Setting SET Value = '%s' WHERE ID = %d"), strPicExt, PicExt);
	if (zsql.ExecSQL(strSql, &strError) == 0)
		return TRUE;
	else
		return FALSE;
}


CString ZSetting::GetHotKey(int nIndex)
{
	std::vector<std::vector <CString>>  vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Value FROM Setting WHERE ID = %d"), HotKey0 + nIndex);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			return  vec2_strData[nRow - 1][0];
		}
	}
	return CString(_T(""));
}


BOOL ZSetting::SetHotKey(int nIndex, const CString & strHotKey)
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	if (InitTable(zsql) == FALSE)
		return FALSE;
	strSql.Format(_T("UPDATE Setting SET Value = '%s' WHERE ID = %d"), strHotKey, HotKey0 + nIndex);
	if (zsql.ExecSQL(strSql, &strError) == 0)
		return TRUE;
	else
		return FALSE;
}


BOOL ZSetting::CheckFileRelation(const CString & strExt, const CString & strExePath)
{
	BOOL nRtn = FALSE;
	HKEY hKeyExt, hKeyExtFile;
	TCHAR szPath[_MAX_PATH] = { 0 };
	DWORD dwSize = sizeof(szPath);
	CString strExtFile, strValue;
	if (RegOpenKey(HKEY_CLASSES_ROOT, strExt, &hKeyExt) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKeyExt, NULL, NULL, NULL, (LPBYTE)szPath, &dwSize) == ERROR_SUCCESS)
		{
			strExtFile = szPath;
			strExtFile += _T("\\shell\\open\\command");
			dwSize = sizeof(szPath);
			if (RegOpenKey(HKEY_CLASSES_ROOT, strExtFile, &hKeyExtFile) == ERROR_SUCCESS)
			{
				if (RegQueryValueEx(hKeyExtFile, NULL, NULL, NULL, (LPBYTE)szPath, &dwSize) == ERROR_SUCCESS)
				{
					strValue = szPath;
					if (strValue.Find(strExePath) != -1)
						nRtn = TRUE;
				}
				RegCloseKey(hKeyExtFile);
			}
		}
		RegCloseKey(hKeyExt);
	}
	return nRtn;
}


BOOL ZSetting::SetFileRelation(const CString & strExt, const CString & strExePath, const CString & strDefaultIcon)
{
	if (strExt.IsEmpty() || strExePath.IsEmpty() || strDefaultIcon.IsEmpty())
		return FALSE;
	CString strTemp, strExeFileKey, strExeFullPath, strDescribe;
	HKEY hKey;
	long lRtn = ERROR_SUCCESS;
	lRtn = RegCreateKey(HKEY_CLASSES_ROOT, strExt, &hKey);
	strExeFileKey = _T("zms") + strExt;//根据项目修改
	lRtn += RegSetValue(hKey, NULL, REG_SZ, strExeFileKey, strExeFileKey.GetLength());
	lRtn += RegCloseKey(hKey);

	lRtn += RegCreateKey(HKEY_CLASSES_ROOT, strExeFileKey, &hKey);
	strDescribe.Format(_T("媒体文件(%s)"), strExt);//根据项目修改
	lRtn += RegSetValue(hKey, NULL, REG_SZ, strDescribe, strDescribe.GetLength());
	lRtn += RegCloseKey(hKey);

	strTemp.Format(_T("%s\\DefaultIcon"), strExeFileKey);
	lRtn += RegCreateKey(HKEY_CLASSES_ROOT, strTemp, &hKey);
	lRtn += RegSetValue(hKey, NULL, REG_SZ, strDefaultIcon, strDefaultIcon.GetLength());
	lRtn += RegCloseKey(hKey);

	strTemp.Format(_T("%s\\shell\\open"), strExeFileKey);
	lRtn += RegCreateKey(HKEY_CLASSES_ROOT, strTemp, &hKey);
	strTemp = _T("使用 ZMShow 打开");//鼠标右键双击显示项，根据项目修改
	lRtn += RegSetValue(hKey, NULL, REG_SZ, strTemp, strTemp.GetLength());
	lRtn += RegCloseKey(hKey);

	strTemp.Format(_T("%s\\shell\\open\\command"), strExeFileKey);
	lRtn += RegCreateKey(HKEY_CLASSES_ROOT, strTemp, &hKey);
	strExeFullPath.Format(_T("\"%s\" \"%%1\""), strExePath);
	lRtn += RegSetValue(hKey, NULL, REG_SZ, strExeFullPath, strExeFullPath.GetLength());
	lRtn += RegCloseKey(hKey);
	if (lRtn == ERROR_SUCCESS)
		return TRUE;
	else
		return FALSE;
}


BOOL ZSetting::BackupFileRelation(const CString & strExt)
{
	BOOL nRtn = FALSE;
	if (strExt.IsEmpty())
		return nRtn;
	HKEY hKey;
	TCHAR szNullValue[_MAX_PATH] = { 0 };
	TCHAR szBakValue[_MAX_PATH] = { 0 };
	DWORD dwSize = sizeof(szNullValue);
	if (RegOpenKey(HKEY_CLASSES_ROOT, strExt, &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)szNullValue, &dwSize) == ERROR_SUCCESS)
		{
			dwSize = sizeof(szBakValue);
			if (RegQueryValueEx(hKey, _T("zmsbak"), NULL, NULL, (LPBYTE)szBakValue, &dwSize) == ERROR_SUCCESS)
			{
				CString strNullValue, strBakValue;
				strNullValue = szNullValue;
				strBakValue = szBakValue;
				if (strNullValue != strBakValue && strNullValue != CString(_T("zms") + strExt))
				{
					if (RegSetValueEx(hKey, _T("zmsbak"), 0, REG_SZ, (CONST BYTE *)szNullValue, _tcslen(szNullValue) * sizeof(TCHAR)) == ERROR_SUCCESS)
						nRtn = TRUE;
				}
				else
				{
					nRtn = TRUE;
				}
			}
			else
			{
				if (RegSetValueEx(hKey, _T("zmsbak"), 0, REG_SZ, (CONST BYTE *)szNullValue, _tcslen(szNullValue) * sizeof(TCHAR)) == ERROR_SUCCESS)
					nRtn = TRUE;
			}
		}
		RegCloseKey(hKey);
	}
	return nRtn;
}


BOOL ZSetting::RecoverFileRelation(const CString & strExt)
{
	BOOL nRtn = FALSE;
	if (strExt.IsEmpty())
		return nRtn;
	HKEY hKey;
	TCHAR szPath[_MAX_PATH] = { 0 };
	DWORD dwSize = sizeof(szPath);
	CString strValue;
	if (RegOpenKey(HKEY_CLASSES_ROOT, strExt, &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, _T("zmsbak"), NULL, NULL, (LPBYTE)szPath, &dwSize) == ERROR_SUCCESS)
		{
			strValue = szPath;
			if (RegSetValue(hKey, NULL, REG_SZ, strValue, strValue.GetLength()) == ERROR_SUCCESS)
			{
				RegDeleteValue(hKey, _T("zmsbak"));
				nRtn = TRUE;
			}
		}
		RegCloseKey(hKey);
	}
	return nRtn;
}