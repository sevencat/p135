#pragma once
#include "../base/hqdef.h"
#include "int64map.h"
#include "MemMapFile.h"
#include "../base/BoostFile.h"
#include "../base/Stockdrv.h"
#include <asio/io_service.hpp>
#include "HqCfg.h"
#include "../base/dtz.h"
#include "HqLifeCycle.h"
#include "../base/basic_repeating_timer_asio.h"

namespace pugi
{
	class xml_node;
}

class HqFile
{
public:
	HqFile();
	~HqFile();
	void shoupan_market();
	void init_market(_datetime_t &dt);

	static void set_default_dir(const std::string &rtdir, const std::string &datadir)
	{
		default_panhoudir = datadir;
		default_rtdir = rtdir;
	}

	KingInt64ToIntMap stk_map;//股票快查表!!!当然是hash了
	MemMapFile stk_file;//主文件
	HqFileHdr *stk_hdr;//文件头部
	BoostFile tick_file;//逐笔文件初始化用的
	bool is_valid()
	{
		return stk_hdr != NULL;
	}



	std::string _name;
	uint32_t _hsid;
	int32_t _market_time_offset;
	_datetime_t _curtm;
	asio::repeating_timer* _loop_timer;

	std::string _rt_dir;
	std::string _panhou_dir;
	
	static std::string default_panhoudir;
	static std::string default_rtdir;

	std::string get_fullfilename()
	{
		char filename[256];
		sprintf(filename, "%s\\%u.dat",(uint32_t)_hsid);
		return filename;
	}

	void handle_rpt(RCV_REPORT_STRUCTEx *rpt);

	bool handle_stk_report_other_thread(std::vector<std::string> *newreports)
	{
		run_in_own_thread(boost::bind(&HqFile::handle_stk_report_in_thread, this, newreports));
		return true;
	}

	bool handle_stk_report_in_thread(std::vector<std::string> *newreports);

	asio::io_service ioqueue;//io队列
	HqCfg param;

	//收盘作业和初始化!!!
	HqLifeCycle _hq_infi;

	template <typename Handler>
	void run_in_own_thread(Handler handler)
	{
		ioqueue.post(handler);
	}

	//关闭当前文件
	void close_current_file();
	void build_stk_map();
	void load_exist_files();
	bool load_cfg(pugi::xml_node &nodecfg);

	int find_or_add(const char *symbol, const char *name, int preclose);

	void log(const char* a_format, ...)
	{
		va_list va;
		va_start(va, a_format);
		vlog(a_format, va);
		va_end(va);
	}
	void vlog(const char* a_format, va_list a_args);

public:
	void on_day_changed();
	void handle_timer_init_and_shoupan();
	void handle_timer_refresh_boursetime();
	void handle_timer(const asio::error_code& e);
};

