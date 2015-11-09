#pragma once
#include "../base/hqdef.h"
#include "int64map.h"
#include "MemMapFile.h"
#include "BoostFile.h"
#include "Stockdrv.h"

class HqFile
{
public:
	HqFile();
	~HqFile();
	KingInt64ToIntMap stk_map;//股票快查表!!!当然是hash了
	MemMapFile stk_file;//主文件
	HqFileHdr *stk_hdr;//文件头部
	BoostFile tick_file;//逐笔文件初始化用的
	void handle_rpt(RCV_REPORT_STRUCTEx *rpt);
};

