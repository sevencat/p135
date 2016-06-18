#pragma once
#include "inifile.h"
#include "mysqldb.h"
class MysqlWriter
{
public:
	bool init(CIniFile &cfgfile);
	MysqlDb::ConnOpt _db_opt;

	void writeonce();
};

extern MysqlWriter gMysqlWriter;