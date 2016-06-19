#pragma once
#include "inifile.h"
#include "mysqldb.h"
#include "DataWriteQueue.h"

class MysqlWriter
{
public:
	bool init(CIniFile &cfgfile);
	MysqlDb::ConnOpt _db_opt;

	void writeonce();

protected:
	void write_day(std::list<KLineData> &kd);

	std::string cvtdaysql(KLineData &item);
};

extern MysqlWriter gMysqlWriter;