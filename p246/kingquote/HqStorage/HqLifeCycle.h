#pragma once
#include "../base/dtz.h"
#include <set>

class HqFile;

namespace pugi
{
	class xml_node;
}

class HqLifeCycle
{
public:
	HqLifeCycle(HqFile &hf);
	~HqLifeCycle();
	void load_cfg(pugi::xml_node &curnode);
	void on_localdt_changed(_datetime_t &ldt);
	bool need_init(_datetime_t &td);

	HqFile &_storage;

	bool _auto_infi;
	bool _init_use_conf_symbols;//是否使用固定代码,xml中的代码,缺省是false
	bool _init_week[7];//控制周几进行开盘,//0-星期日，1－星期一
	std::set<uint32_t> _init_not_days;//不需要初始化的日期
	std::vector<std::string> _init_symbols;//固定的行情代码

	_datetime_t _localdt;//本地最后时间，这个是已经算好偏移量的时间了！！！
	_datetime_t _hqdt;//行情源最后时间，这个也是算好偏移量后的行情时间了！！！

	//计算偏移量后的时间，以秒计算的！！！
	uint32_t _init_time;//比如说上海市场就是093000,9点钟进行初始化
	uint32_t _shoupan_time;//比如说上海市场就是153000,15点半进行收盘作业，如果是外盘，可能就是235959
	int32_t _isprev;
};

