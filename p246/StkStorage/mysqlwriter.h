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
	void write_min(std::list<KLineData> &kd, bool ismin1);
	std::string cvtdaysql(KLineData &item);
	std::string cvtminsql(KLineData &item, bool ismin1);
};

extern MysqlWriter gMysqlWriter;