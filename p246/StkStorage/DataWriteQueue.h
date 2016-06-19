#pragma once
#include "decimal.h"

class KLineData
{
public:
	std::string mkt;
	std::string code;
	int32_t rq;
	dec::decimal4 opoenpx;
	dec::decimal4 highpx;
	dec::decimal4 lowpx;
	dec::decimal4 closepx;
	int32_t vol;
	dec::decimal4 money;
	dec::decimal4 jspx;
	int32_t pos;
};


class DataWriteQueue
{
public:
	void merge_daydata(std::list<KLineData> &kd)
	{
		asio::detail::win_mutex::scoped_lock lock(mtx);
		daydatalst.splice(daydatalst.end(),kd);
	}
	std::list<KLineData> daydatalst;
	std::list<KLineData> min1datalst;
	std::list<KLineData> min5datalst;
	asio::detail::win_mutex mtx;

	void get_daydata(std::list<KLineData> &kd)
	{
		asio::detail::win_mutex::scoped_lock lock(mtx);
		kd.swap(daydatalst);
	}
};

extern DataWriteQueue gDataWriteQueue;