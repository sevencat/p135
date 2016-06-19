#include "stdafx.h"
#include "mysqlwriter.h"
#include <asio/thread.hpp>
#include "format.h"

void logsqlstr(const char *sql, int ret, MysqlDb *db)
{
	addlog("%d;%s", ret, sql);
}

MysqlWriter gMysqlWriter;
MysqlDb gDb;
bool MysqlWriter::init(CIniFile &inicfg)
{
	_db_opt._db = inicfg.GetKeyValue("db", "db");
	_db_opt._server = inicfg.GetKeyValue("db", "host");
	_db_opt._port = atoi(inicfg.GetKeyValue("db", "port").c_str());
	_db_opt._clientflags = CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS;
	_db_opt._user = inicfg.GetKeyValue("db", "user");
	_db_opt._password = inicfg.GetKeyValue("db", "pwd");

	my_bool autoreconnect = true;
	gDb.options(MYSQL_OPT_RECONNECT, &autoreconnect);
	gDb.options(MYSQL_SET_CHARSET_NAME, "gbk");
	if (!gDb.connect(_db_opt))
	{
		addlog("写数据库连接失败");
		return false;
	}
	addlog("数据库连接成功");
	new asio::thread([this]
	{
		addlog("数据库写线程启动");
		while (true)
		{
			SleepEx(100,TRUE);
			writeonce();
		}
	});
	return true;
}

void MysqlWriter::writeonce()
{
	//先处理日线
	{
		std::list<KLineData> curdaydata;
		gDataWriteQueue.get_daydata(curdaydata);
		if (curdaydata.size() > 0)
		{
			write_day(curdaydata);
		}
	}
}

void MysqlWriter::write_day(std::list<KLineData> &kd)
{
	gDb.ping();
	MysqlTransaction trans(gDb);
	for (auto it = kd.begin(); it != kd.end(); it++)
	{
		KLineData &curitem = *it;
		std::string cursql = cvtdaysql(curitem);
		gDb.exec(cursql.c_str(), cursql.size());
	}
	trans.commit();
}

std::string MysqlWriter::cvtdaysql(KLineData &item)
{
	return fmt::format("replace INTO day (dt, mkt, code, openpx, highpx, lowpx, closepx, vol, money, jspx, pos) VALUES({0}, '{1}', '{2}', {3}, {4}, {5},{6},{7},{8},{9},{10})",
		item.rq,item.mkt,item.code,item.opoenpx,item.highpx,item.lowpx,item.closepx,item.vol,item.money,item.jspx,item.pos);
}
