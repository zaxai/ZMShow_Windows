#include "stdafx.h"
#include "ZPlayList.h"

bool ZPlayList::m_bIsSrand = false;

ZPlayList::ZPlayList()
	: m_strPathDB(_T(""))
{
	m_strPathDB= ZUtil::GetExeCatalogPath() + _T("\\Data.db3");
	if (!m_bIsSrand)
	{
		::srand((unsigned int)time(NULL));
		m_bIsSrand = true;
	}
}


ZPlayList::~ZPlayList()
{
}


bool ZPlayList::IsExist()
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql = _T("CREATE TABLE PlayList (ID int PRIMARY KEY NOT NULL, ParentID int NOT NULL, Class int NOT NULL, Name nvarchar(300) NOT NULL,Path nvarchar(300) NOT NULL, LastPlayProgress int NOT NULL)");
	if (zsql.ExecSQL(strSql, &strError) == 0)
		return false;
	else
		return true;
}


bool ZPlayList::InsertItem(int nID, int nParentID, int nClass, CString  strName, CString strPath, int nLastPlayProgress)
{
	strName.Replace(_T("\'"), _T("\'\'"));
	strPath.Replace(_T("\'"), _T("\'\'"));
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql = _T("CREATE TABLE PlayList (ID int PRIMARY KEY NOT NULL, ParentID int NOT NULL, Class int NOT NULL, Name nvarchar(300) NOT NULL,Path nvarchar(300) NOT NULL, LastPlayProgress int NOT NULL)");
	zsql.ExecSQL(strSql, &strError);
	strSql.Format(_T("INSERT INTO PlayList VALUES (%d,%d,%d,'%s','%s',%d)"), nID, nParentID, nClass, strName, strPath, nLastPlayProgress);
	if (zsql.ExecSQL(strSql, &strError) == 0)
		return true;
	else
		return false;
}


void ZPlayList::GetAllItem(std::vector<std::vector <CString>> & vec2_strData)
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL = _T("SELECT * FROM PlayList ORDER BY Class,ParentID,ID");
	zsql.GetTable(strSQL, vec2_strData, &strError);
}

bool ZPlayList::DeleteItem(int nID)
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("DELETE FROM PlayList WHERE ID = %d OR ParentID = %d"), nID, nID);
	if (zsql.ExecSQL(strSql, &strError) == 0)
		return true;
	else
		return false;
}


bool ZPlayList::DeleteAllItem(void)
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql = _T("DELETE FROM PlayList ");
	if (zsql.ExecSQL(strSql, &strError) == 0)
		return true;
	else
		return false;
}


CString ZPlayList::GetPath(int nID)
{
	CString strPath;
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Path FROM PlayList WHERE ID = %d"), nID);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow>1)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			strPath = vec2_strData[1][0];
		}
	}
	return strPath;
}


int ZPlayList::GetParentID(int nID)
{
	int nParentID = -1;
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT ParentID FROM PlayList WHERE ID = %d"), nID);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow>1)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			nParentID = _ttoi(vec2_strData[1][0]);
		}
	}
	return nParentID;
}


int ZPlayList::GetClass(int nID)
{
	int nClass = -1;
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Class FROM PlayList WHERE ID = %d"), nID);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow>1)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			nClass = _ttoi(vec2_strData[1][0]);
		}
	}
	return nClass;
}


int ZPlayList::GetIDByPath(CString strPath)
{
	strPath.Replace(_T("\'"), _T("\'\'"));
	int nID = -1;
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT ID FROM PlayList WHERE Path = '%s'"), strPath);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow>1)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			nID = _ttoi(vec2_strData[1][0]);
		}
	}
	return nID;
}


void ZPlayList::GetChildPath(int nID, std::vector<std::vector <CString>> & vec2_strData)
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Path,ID FROM PlayList WHERE ParentID = %d"), nID);
	zsql.GetTable(strSQL, vec2_strData, &strError);
}


int ZPlayList::GetNextID(int nID, int nListMode, bool bIsPrev)
{
	int nNextID = -1;
	int nParentID = GetParentID(nID);
	int nClass = GetClass(nID);
	if (nParentID == -1 || nClass == -1)
		return nNextID;
	const int nList = 0, nCatalog = 1, nSingle = 2;
	const int nSeq = 0, nLoop = 1, nRand = 2;
	std::vector<std::vector <CString>>  vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	switch (nListMode / 3)
	{
	case nList:
		strSQL.Format(_T("SELECT ID FROM PlayList WHERE Class = %d ORDER BY ID ASC"), nClass);
		break;
	case nCatalog:
		strSQL.Format(_T("SELECT ID FROM PlayList WHERE ParentID = %d ORDER BY ID ASC"), nParentID);
		break;
	case nSingle:
		strSQL.Format(_T("SELECT ID FROM PlayList WHERE ID = %d ORDER BY ID ASC"), nID);
		break;
	}
	if (!strSQL.IsEmpty())
	{
		zsql.GetTable(strSQL, vec2_strData, &strError);
		int nRow = vec2_strData.size();
		if (nRow > 1)
		{
			int nColumn = vec2_strData[0].size();
			if (nColumn == 1)
			{
				switch (nListMode % 3)
				{
				case nSeq:
				{
					if (bIsPrev)
					{
						for (int i = nRow - 1; i > 0; --i)
						{
							if (_ttoi(vec2_strData[i][0]) < nID)
							{
								nNextID = _ttoi(vec2_strData[i][0]);
								break;
							}
						}
					}
					else
					{
						for (int i = 1; i <nRow; ++i)
						{
							if (_ttoi(vec2_strData[i][0]) > nID)
							{
								nNextID = _ttoi(vec2_strData[i][0]);
								break;
							}
						}
					}
				}
				break;
				case nLoop:
				{
					if (bIsPrev)
					{
						for (int i = nRow - 1; i > 0; --i)
						{
							if (_ttoi(vec2_strData[i][0]) < nID)
							{
								nNextID = _ttoi(vec2_strData[i][0]);
								break;
							}
						}
						if (nNextID == -1)
							nNextID = _ttoi(vec2_strData[nRow - 1][0]);
					}
					else
					{
						for (int i = 1; i <nRow; ++i)
						{
							if (_ttoi(vec2_strData[i][0]) > nID)
							{
								nNextID = _ttoi(vec2_strData[i][0]);
								break;
							}
						}
						if (nNextID == -1)
							nNextID = _ttoi(vec2_strData[1][0]);
					}
				}
				break;
				case nRand:
				{
					int nIndex = (::rand()) % (nRow - 1) + 1;
					nNextID = _ttoi(vec2_strData[nIndex][0]);
				}
				break;
				}
			}
		}
	}
	return nNextID;
}


void ZPlayList::SearchKey(CString strKey, std::vector<std::vector <CString>> & vec2_strData)
{
	strKey.Replace(_T("\'"), _T("\'\'"));
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT Name,ID FROM PlayList WHERE Class = 2 AND Name LIKE '%%%s%%'"), strKey);
	zsql.GetTable(strSQL, vec2_strData, &strError);
}


int ZPlayList::GetLastPlayProgress(int nID)
{
	int nLastPlayProgress = 0;
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSQL, strError;
	strSQL.Format(_T("SELECT LastPlayProgress FROM PlayList WHERE ID = %d"), nID);
	zsql.GetTable(strSQL, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow>1)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 1)
		{
			nLastPlayProgress = _ttoi(vec2_strData[1][0]);
		}
	}
	return nLastPlayProgress;
}


bool ZPlayList::SetLastPlayProgress(int nID, int nLastPlayProgress)
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("UPDATE PlayList SET LastPlayProgress = %d WHERE ID = %d"), nLastPlayProgress, nID);
	if (zsql.ExecSQL(strSql, &strError) == 0)
		return true;
	else
		return false;
}