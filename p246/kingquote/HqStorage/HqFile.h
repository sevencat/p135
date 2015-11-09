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
	KingInt64ToIntMap stk_map;//股票快查表!!!当然是hash了
	MemMapFile stk_file;//主文件
	HqFileHdr *stk_hdr;//文件头部
	BoostFile tick_file;//逐笔文件初始化用的
	std::string _name;
	uint32_t _hsid;
	int32_t _market_time_offset;
	_datetime_t _curtm;
	asio::repeating_timer* _loop_timer;

	std::string _rt_dir;
	std::string _panhou_dir;
	
	static std::string default_panhoudir;
	static std::string default_rtdir;

	void handle_rpt(RCV_REPORT_STRUCTEx *rpt);
	asio::io_service ioqueue;//io队列
	HqCfg param;

	//其实只是收盘作业 
	HqLifeCycle _hq_infi;

	//关闭当前文件
	void close_current_file();
	void load_exist_files();
	bool load_cfg(pugi::xml_node &nodecfg);

public:
	void handle_timer(const asio::error_code& e);
};

