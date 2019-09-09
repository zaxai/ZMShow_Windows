/*
** Version  1.0.0.4
** Date     2019.03.15
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#pragma once
#include "sqlite3.h"
#include "ZUtil.h"
#pragma comment(lib,"sqlite3.lib")

class ZSqlite3
{
public:
	enum ERRORINFO
	{
		ERROR_OK=0,//�ɹ�
		ERROR_INVALIDSQLITE,//sqlite3���ݳ�Ա��Ч
		ERROR_EXEC,//Execִ�г���
		ERROR_GETTABLE,//GetTableִ�г���
		ERROR_OTHER//��������
	};
private:
	sqlite3 *m_p_sqlite3;
	ZSqlite3(const ZSqlite3 & zsql) = delete;
	ZSqlite3 & operator=(const ZSqlite3 & zsql) = delete;
public:
	ZSqlite3(void);
	ZSqlite3(ZSqlite3 && zsql);
	ZSqlite3 & operator=(ZSqlite3 && zsql);
	~ZSqlite3(void);
	bool OpenDB(const CString & in_strPath);
	bool CloseDB(void);
	int ExecSQL(const CString & in_strSQL, CString *  out_p_strErrMsg=nullptr);
	int GetTable(const CString & in_strSQL, std::vector<std::vector <CString>> & out_vec2_strData, CString *  out_p_strErrMsg=nullptr);
};

