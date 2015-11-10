#include "stdafx.h"
#include "HqLifeCycle.h"
#include "HqFile.h"
#include "../base/pugixml.hpp"

HqLifeCycle::HqLifeCycle(HqFile &hf)
	:_storage(hf)
{
}


HqLifeCycle::~HqLifeCycle()
{
}

void HqLifeCycle::load_cfg(pugi::xml_node xml_nodes)
{
}

bool HqLifeCycle::need_init(_datetime_t &td)
{
	int curweek = td.day_of_week();
	if (curweek >= 1)
		curweek = curweek - 1;
	else
		curweek = 6;
	if (_init_week[curweek] == false)
		return false;
	if (_init_not_days.find(td.raw_date()) != _init_not_days.end())
		return false;
	return true;
}

void HqLifeCycle::on_localdt_changed(_datetime_t &ldt)
{
	_localdt = ldt;
	//这里要判断,如果是自动初始化和收盘
	if (_auto_infi)
	{
		//如果是自动收盘和初始化
		//如果还没有初始化，或者当前市场的时间不是当前的时间，就先收盘！！！
		//这个主要是用在黄金，外汇等合约代码固定的市场，只要过日期就执行开盘作业 。
		if ((!_storage.is_valid()) || (_localdt.raw_date() != _storage.stk_hdr->dt))
		{
			//这里要开始收盘作业，并且做初始化工作！！！
			_storage.shoupan_market();//先收盘做业
			//再开始初始化！！！
			if (need_init(_localdt))
			{
				_storage.init_market(_localdt);
			}
			else
			{
				_storage.log("%s 不需要初始化 %d (auto infi)", _storage._name.c_str(), _localdt.raw_date());
			}
		}
	}
	else
	{
		int hour = _localdt.hour();
		int minute = _localdt.min();
		int temphm = hour * 100 + minute;
		//如果行情还没有初始化，或者日期不是当前日期
		if ((!_storage.is_valid()) || (_localdt.raw_date() != _storage.stk_hdr->dt))
		{

			if (temphm >= _init_time)
			{
				if (need_init(_localdt))
					_storage.init_market(_localdt);
				else
				{
					_storage.log("%s 不需要初始化 %d (non auto infi)", _storage._name.c_str(), _localdt.raw_date());
				}
			}
		}
		else if (temphm >= _shoupan_time)
		{
			_storage.shoupan_market();
		}
	}
}