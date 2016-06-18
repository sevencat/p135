#pragma once

class KLineData
{
public:
	std::string mkt;
	std::string code;
	int32_t rq;
	int32_t opoenpx;
	int32_t highpx;
	int32_t lowpx;
	int32_t closepx;
	int32_t vol;
	int64_t money;
	int32_t jspx;
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
};

extern DataWriteQueue gDataWriteQueue;