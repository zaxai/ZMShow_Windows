#pragma once
#include "ZUtil.h"
#include "ZSqlite3.h"

class ZPlayList
{
public:
	ZPlayList();
	~ZPlayList();
private:
	CString m_strPathDB;
	static bool m_bIsSrand;
public:
	bool IsExist();
	bool InsertItem(int nID, int nParentID, int nClass, CString strName, CString strPath, int nLastPlayProgress);
	void GetAllItem(std::vector<std::vector <CString>> & vec2_strData);
	bool DeleteItem(int nID);
	bool DeleteAllItem(void);
	CString GetPath(int nID);
	int GetParentID(int nID);
	int GetClass(int nID);
	int GetIDByPath(CString strPath);
	void GetChildPath(int nID, std::vector<std::vector <CString>> & vec2_strData);
	int GetNextID(int nID, int nListMode, bool bIsPrev);
	void SearchKey(CString strKey, std::vector<std::vector <CString>> & vec2_strData);
	int GetLastPlayProgress(int nID);
	bool SetLastPlayProgress(int nID, int nLastPlayProgress);
};

