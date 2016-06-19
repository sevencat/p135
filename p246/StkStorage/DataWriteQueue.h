#pragma once
#include "decimal.h"

class KLineData
{
public:
	std::string mkt;
	std::string code;
	int64_t rq;
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
	void merge_min1data(std::list<KLineData> &kd)
	{
		asio::detail::win_mutex::scoped_lock lock(mtx);
		min1datalst.splice(min1datalst.end(), kd);
	}
	void merge_min5data(std::list<KLineData> &kd)
	{
		asio::detail::win_mutex::scoped_lock lock(mtx);
		min5datalst.splice(min5datalst.end(), kd);
	}

	int getdatasize(int &daysize,int &min1size,int &min5size)
	{
		daysize=daydatalst.size();
		min1size = min1datalst.size();
		min5size = min5datalst.size();
		return 0;
	}
	std::list<KLineData> daydatalst;
	std::list<KLineData> min1datalst;
	std::list<KLineData> min5datalst;
	asio::detail::win_mutex mtx;

	void get_daydata(std::list<KLineData> &kd)
	{
		get_daydata(kd, daydatalst);
	}

	void get_min1data(std::list<KLineData> &kd)
	{
		get_daydata(kd, min1datalst);
	}

	void get_min5data(std::list<KLineData> &kd)
	{
		get_daydata(kd, min5datalst);
	}

	void get_daydata(std::list<KLineData> &kd, std::list<KLineData> &kdlst)
	{
		asio::detail::win_mutex::scoped_lock lock(mtx);
		if (daydatalst.size() <= 8192)
		{
			kd.swap(kdlst);
		}
		else
		{
			std::list<KLineData>::iterator it = kdlst.begin();
			std::list<KLineData>::iterator it2 = it;
			for (int i = 0; i < 8192; i++)
				it2++;
			kd.splice(kd.end(), kdlst, it, it2);
		}
	}
};

extern DataWriteQueue gDataWriteQueue;