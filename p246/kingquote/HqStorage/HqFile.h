#pragma once
#include "../base/hqdef.h"
#include "int64map.h"
#include "MemMapFile.h"
#include "../base/BoostFile.h"
#include "../base/Stockdrv.h"
#include "../base/pugixml.hpp"
#include <asio/io_service.hpp>
#include "HqCfg.h"
#include "../base/dtz.h"
#include "HqLifeCycle.h"

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
	void handle_rpt(RCV_REPORT_STRUCTEx *rpt);
	asio::io_service ioqueue;//io队列
	HqCfg param;
	HqLifeCycle _hq_infi;
	bool load_cfg(pugi::xml_node &nodecfg)
};

