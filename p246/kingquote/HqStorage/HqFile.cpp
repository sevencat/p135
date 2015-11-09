#include "stdafx.h"
#include "HqFile.h"

#include "../base/pugixml.hpp"
#include "../base/dtz.h"
#include "../base/strutil.h"
#include <asio/placeholders.hpp>

HqFile::HqFile()
{
}


HqFile::~HqFile()
{
}

void HqFile::handle_rpt(RCV_REPORT_STRUCTEx* rpt)
{
	//1、实时，2、分钟，3、tick数据,可以先写实时数据
	return;
}

bool HqFile::load_cfg(pugi::xml_node& nodecfg)
{
	if (nodecfg.empty())
		return false;

	pugi::xml_node node_opt = nodecfg.child("opt");
	_name = node_opt.child("fullname").child_value();
	std::string strbactive = node_opt.child("active").child_value();
	if (strbactive == "false")
		return false;

	param._shortname = nodecfg.name();
	_hsid = strtoul(node_opt.child("id").child_value(), NULL, 16);

	param._max_mincount = atoi(node_opt.child("mincount").child_value());
	param._max_stockcount = atoi(node_opt.child("maxcount").child_value());

	param._stk_volume_multi = atof(node_opt.child("volumnmul").child_value());
	if (param._stk_volume_multi <= 0.000000001f)
		param._stk_volume_multi = 1.0f;

	{
		std::string tempstr = node_opt.child("hasvol").child_value();
		if (tempstr == "false")
			param._has_vol = false;
		tempstr = node_opt.child("hasmoney").child_value();
		if (tempstr == "false")
			param._has_money = false;

		tempstr = node_opt.child("hasavgpx").child_value();
		if (tempstr == "false")
			param._has_avgprice = false;

		tempstr = node_opt.child("check_dup").child_value();
		if (tempstr == "true")
			param._check_dup = true;

		tempstr = node_opt.child("userrpttime").child_value();
		if (tempstr == "false")
			param._userrpttime = false;
	}

	param._stk_hand = atof(node_opt.child("stkhand").child_value());

	param._stk_money_multi = atof(node_opt.child("moneymul").child_value());
	if (param._stk_money_multi <= 0.000000001f)
		param._stk_money_multi = 0.01f;//缺省的金额是元，这就是百元

	param._stk_qhmoney_multi = atof(node_opt.child("qhmoneymul").child_value());
	if (param._stk_qhmoney_multi <= 0.000000001f)
		param._stk_qhmoney_multi = 1.0f;//缺省的金额是元，这就是百元



	param._stk_minunit = atof(node_opt.child("minunit").child_value());
	if (param._stk_minunit <= 0.000000001f)
	{
		param._stk_minunit = 1.0f;
	}


	std::string usestk = node_opt.child("userstkfield").child_value();
	if (usestk.size() >= 4)
	{
		param._use_stk_open = (usestk[0] == '1');
		param._use_stk_high = (usestk[1] == '1');
		param._use_stk_low = (usestk[2] == '1');
		param._use_stk_preclose = (usestk[3] == '1');
	}

	_market_time_offset = atoi(node_opt.child("marketoffsettime").child_value());

	_hq_infi.load_cfg(node_opt);

	{
		param._mts.clear();
		static int timestrlen = strlen("09:30-11:30");
		for (pugi::xml_node node_time = node_opt.child("openclose"); node_time; node_time = node_time.next_sibling("openclose"))
		{
			std::string strtime = node_time.child_value();
			if (strtime.size() != timestrlen)
				continue;
			SubMarketTime cursubtime;
			cursubtime._open = 60 * atoi(strtime.c_str()) + atoi(&strtime[3]);
			cursubtime._close = 60 * atoi(&strtime[6]) + atoi(&strtime[9]);
			param._mts.push_back(cursubtime);
		}
	}

	_datetime_t newutctm = _datetime_t::gmt_now();
	//_datetime_t newlocaltm=_datetime_t::now()-_time_t((long)(_market_time_offset));
	_datetime_t newlocaltm;
	newlocaltm.from_local_timer(time(NULL) - _market_time_offset);
	_curtm = newlocaltm;

	for (pugi::xml_node node_submarket = nodecfg.first_child(); node_submarket; node_submarket = node_submarket.next_sibling())
	{
		SubMarketDef curmd;
		curmd._name = node_submarket.name();
		if (curmd._name == "opt")
			continue;

		const char *strid2 = node_submarket.attribute("id").value();
		curmd._codetype = strtoul(strid2, NULL, 16);
		curmd._client_price_unit = atoi(node_submarket.child("clientpriceunit").child_value());
		curmd._send_price_unit = atoi(node_submarket.child("sendpriceunit").child_value());
		curmd._lClientDecimal = atoi(node_submarket.child("clientradix").child_value());
		curmd._codescope = StrUtil::split(node_submarket.child("codeinclude").child_value(), ",", 64);
		curmd._str_notcode = node_submarket.child("codeexclude").child_value();

		static int timestrlen = strlen("09:30-11:30");

		int shoupantime = 60 * (_hq_infi._shoupan_time / 100) + (_hq_infi._shoupan_time % 100);
		for (pugi::xml_node node_time = node_submarket.child("openclose"); node_time; node_time = node_time.next_sibling("openclose"))
		{
			std::string strtime = node_time.child_value();
			if (strtime.size() != timestrlen)
				continue;
			SubMarketTime cursubtime;
			cursubtime._open = 60 * atoi(strtime.c_str()) + atoi(&strtime[3]);
			cursubtime._close = 60 * atoi(&strtime[6]) + atoi(&strtime[9]);
			if (cursubtime._open > shoupantime)
			{
				//减去3600
				cursubtime._open -= 24 * 60;
				cursubtime._close -= 24 * 60;
			}
			curmd._mts.push_back(cursubtime);
		}
		param._submarketdefs.push_back(curmd);
		param._submarketfasts[curmd._codetype % 16] = curmd;
	}

	//处理各个路径问题
	_rt_dir = nodecfg.attribute("rtdir").value();
	_panhou_dir = nodecfg.attribute("panhou").value();
	if (_rt_dir.empty())
		_rt_dir = default_rtdir;
	if (_panhou_dir.empty())
		_panhou_dir = default_panhoudir;

	////确认这两个目录存在


	//InitPath();

	////创建db

	//try
	//{
	//	_hq_infi.init_db();
	//}
	//catch (...)
	//{
	//	this->log("%s init_db失败:%s,%s", _name.c_str(), _rt_dir.c_str(), _panhou_dir.c_str());
	//}

	////初始的创建，只处理定时器和事件
	_loop_timer = new asio::repeating_timer(ioqueue);
	_loop_timer->start(boost::posix_time::seconds(15), boost::bind(&HqFile::handle_timer, this, asio::placeholders::error));


	//if (_hsid < 0x2000)
	//{
	//	_calc_timer = new asio::repeating_timer(_io_service);
	//	_calc_timer->start(boost::posix_time::seconds(6), boost::bind(&HqStorage::calc_gp_index, this, asio::placeholders::error));
	//}

	////加载当前的文件
	//load_exist_files();

	return true;
}

void HqFile::handle_timer(const asio::error_code& e)
{
}