#pragma once

namespace pugi
{
	class xml_node;
}

//市场的定义文件
struct SubMarketTime
{
	SubMarketTime()
	{
		_open = _close = -1;
	}
	int16_t	_open;	// 前开市时间
	int16_t	_close;	// 前闭市时间
	int16_t   dist() { return (_close - _open); }
	//减掉偏移量
	void suboffset(int offset)
	{
		//为什么要有这个？
		offset /= 60;
		if (_open >= offset)
		{
			_open -= offset;
			_close -= offset;
			if (_open >= 24 * 60)
			{
				_open = _open - 24 * 60;
				_close = _close - 24 * 60;
			}
			return;
		}
		_open = _open + 24 * 60 - offset;
		_close = _close + 24 * 60 - offset;
		if (_open >= 24 * 60)
		{
			_open = _open - 24 * 60;
			_close = _close - 24 * 60;
		}
	}
};

class SubMarketDef
{
public:
	SubMarketDef()
	{
		_codetype = 0;
		_client_price_unit = 0;
		_send_price_unit = 0;
		_lClientDecimal = 0;
	}
	uint32_t _codetype;//子市场
	std::string	_name;//股票名称	
	std::vector<std::string> _codescope;//包含的股票
	std::string	_str_notcode;  // 不包含的代码
	std::vector<SubMarketTime> _mts;

	long _client_price_unit;
	long _send_price_unit;
	long _lClientDecimal;
};

//现在是考虑是用json还是xml
//这里是读取比如子市场名，市场ID,时间配置之类的东西!!!
class HqCfg
{
public:
	HqCfg();
	~HqCfg();

	std::vector<SubMarketDef> _submarketdefs;
	SubMarketDef _submarketfasts[16];
	std::string _shortname;//短名称：如 上海证券

	uint16_t _hsid;

	uint32_t _max_mincount;//最大分钟数。
	uint32_t _max_stockcount;//股票最大数

	double _stk_volume_multi;//手数的单位
	double _stk_money_multi;
	double _stk_qhmoney_multi;

	float _stk_hand;//手数

	double _stk_minunit;

	bool _has_vol;
	bool _has_money;

	bool _has_avgprice;//是否有均价


	//第一笔认为是昨收？

	//是否使用stk里的开盘，最高，最低，昨收
	bool _use_stk_open;
	bool _use_stk_high;
	bool _use_stk_low;
	bool _use_stk_preclose;

	bool _check_dup;//检查是否重复。
	bool _userrpttime;//是否使用市场时间

	std::vector<SubMarketTime> _mts;

	bool load_cfg(pugi::xml_node &nodecfg);

	uint16_t get_type_by_code(const std::string &strCode);
};

