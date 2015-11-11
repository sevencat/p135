#include "stdafx.h"
#include "HqFile.h"


std::string HqFile::default_panhoudir;
std::string HqFile::default_rtdir;
_tzinfo_t globaltz = _tzinfo_t::china();

HqFile::HqFile()
	:_hq_infi(*this)
{
}


HqFile::~HqFile()
{
}

void HqFile::shoupan_market()
{
}

void HqFile::init_market(_datetime_t& dt)
{
	//清除快查接口
	stk_map.DeleteMap();
	//先检查收盘
	shoupan_market();
	//再关闭文件
	close_current_file();
	
	int rawdt = dt.raw_date();


	std::string curfilename = get_fullfilename();
	int32_t filesize = HqFileHdr::get_init_size(param._max_stockcount, param._max_mincount);
	if (!stk_file.map(curfilename.c_str(), filesize))
	{
		return;
	}
	HqFileHdr *hdr = (HqFileHdr *)stk_file.addr();
	hdr->hdrflag = HDRFLAG;
	hdr->dt = rawdt;
	hdr->stkchgtime = 0;
	hdr->maxstkcount = param._max_stockcount;
	hdr->mincount = param._max_mincount;
	stk_hdr = hdr;
}

void HqFile::handle_rpt(RCV_REPORT_STRUCTEx* rpt)
{
	int preclose = multifloat(rpt->m_fLastClose);
	int curpos = find_or_add(rpt->m_szLabel, rpt->m_szName, preclose);
	if (curpos < 0)
		return;
	HqRecord &curstk = stk_hdr->rtrec[curpos];
	curstk.preclosepx = preclose;
	curstk.openpx = multifloat(rpt->m_fOpen);
	curstk.highpx = multifloat(rpt->m_fHigh);
	curstk.lowpx = multifloat(rpt->m_fLow);
	curstk.newpx = multifloat(rpt->m_fNewPrice);
	curstk.vol = rpt->m_fVolume;
	curstk.money = rpt->m_fAmount;

	//下面来更新分钟线
	_datetime_t utcdt;
	utcdt.from_gmt_timer((int64_t)rpt->m_time - _market_time_offset);
	_datetime_t localdt = globaltz.to_local(utcdt);
	int curoffset = get_min_offset(localdt.hour(), localdt.min());
	//取得分钟线

	HqMinRecord *minrec = stk_hdr->get_min_rec_by_idx(curpos);
	HqMinRecord &curminrec = minrec[curoffset];
	if (curminrec.openpx == 0)
		curminrec.openpx = curstk.newpx;
	if (curminrec.newpx == 0)
		curminrec.newpx = curstk.newpx;
	if (curminrec.highpx == 0)
		curminrec.highpx = curstk.newpx;
	else
		curminrec.highpx = std::max(curminrec.highpx, curstk.newpx);
	if (curminrec.lowpx == 0)
		curminrec.lowpx = curstk.newpx;
	else
		curminrec.lowpx = std::min(curminrec.lowpx, curstk.newpx);
	if (rpt->m_time>curminrec.lasthqtime)
	{
		curminrec.vol = rpt->m_fVolume;
		curminrec.money = rpt->m_fAmount;
		curminrec.lasthqtime = rpt->m_time;
	}
	if (rpt->m_time>curstk.lasttime)
	{
		curstk.lasttime = rpt->m_time;
	}
	if (curstk.curminpos < curoffset)
		curstk.curminpos = curoffset;
}


//用local时间,这个地方注意，可能会有偏移量！！！
int HqFile::get_min_offset(int hour, int minute)
{
	int nCurTimePos = (hour * 60 + minute);
	int nCount = 0;
	int nPreCount = 0;

	for (int i = 0; i < param._mts.size(); i++)
	{
		SubMarketTime curmt = param._mts[i];
		curmt.suboffset(this->_market_time_offset);
		if (curmt._open == -1 || curmt._close == -1)
		{
			//如果在这个之外的话，就返回总的时间
			nCount += nPreCount;
			return nCount;
		}
		if (nCurTimePos < curmt._open)
		{
			nCount += nPreCount;
			return nCount;
		}
		nCount += nPreCount;
		if (nCurTimePos >= curmt._open && nCurTimePos <= curmt._close)
		{
			return (nCount + nCurTimePos - curmt._open);
		}
		nPreCount = curmt.dist();
	}
	nCount += nPreCount;
	return nCount;
}

bool HqFile::handle_stk_report_in_thread(std::vector<std::string> *newreports)
{
	if (stk_hdr != NULL)
	{
		for (int i = 0; i < newreports->size(); i++)
		{
			std::string &curstr = (*newreports)[i];
			RCV_REPORT_STRUCTEx &report = *(RCV_REPORT_STRUCTEx *)curstr.c_str();
			handle_rpt(&report);
		}
	}
	delete newreports;
	return true;
}

void HqFile::close_current_file()
{
	stk_hdr = NULL;
	stk_file.sync();
	stk_file.close();
}

void HqFile::build_stk_map()
{
	stk_map.DeleteMap();
	for (int i = 0; i < stk_hdr->curstkcount;i++)
	{
		HqRecord &curstk = stk_hdr->rtrec[i];
		stk_map.AddToMap(curstk.symbol, i);
	}
}

void HqFile::load_exist_files()
{
	close_current_file();
	//这里进行map文件
	std::string curfilename = get_fullfilename();
	if (_access(curfilename.c_str(), 0) == -1)
	{
		//没有找到，就退了
		return;
	}
	//int ndate = _curtm.raw_date();
	int32_t filesize = HqFileHdr::get_init_size(param._max_stockcount, param._max_mincount);
	if(!stk_file.map(curfilename.c_str(),filesize))
	{
		return;
	}
	HqFileHdr *hdr = (HqFileHdr *)stk_file.addr();
	if (hdr->hdrflag != HDRFLAG)
		return;
	stk_hdr = hdr;
	build_stk_map();
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
	load_exist_files();

	return true;
}

int HqFile::find_or_add(const char* symbol, const char* name, int preclose)
{
	uint64_t key = KingInt64ToIntMap::KeyToInt64(symbol);
	int curpos = stk_map.GetStockByMap(key);
	if (curpos >= 0)
		return curpos;

	curpos = stk_hdr->curstkcount;
	HqRecord &currecord = stk_hdr->rtrec[curpos];
	strncpy(currecord.symbol, symbol, 6);
	strncpy(currecord.name, name, 31);
	currecord.preclosepx = preclose;
	uint16_t stkid = param.get_type_by_code(std::string(symbol));
	currecord.mktid = stkid;
	currecord.idx = curpos;

	stk_hdr->curstkcount++;
	stk_map.AddToMap(key, curpos);
	return curpos;
}

void HqFile::vlog(const char* a_format, va_list a_args)
{
}

void HqFile::on_day_changed()
{
}

void HqFile::handle_timer_init_and_shoupan()
{
	_datetime_t newlocaltm;
	newlocaltm.from_local_timer(time(NULL) - _market_time_offset);
	//减去偏移量，外盘要用的
	if (newlocaltm.raw_date() != _curtm.raw_date())
	{
		//检查是否要收盘作业！！！
		on_day_changed();
	}
	_curtm = newlocaltm;
	_hq_infi.on_localdt_changed(newlocaltm);
}

void HqFile::handle_timer_refresh_boursetime()
{
	//if (!_status_init)
	//	return;

	//_datetime_t newlocaltm;
	//newlocaltm.from_local_timer(time(NULL) - _market_time_offset);

	////_datetime_t newlocaltm=_datetime_t::now()-_time_t((long)(_market_time_offset));
	////减去偏移量，外盘要用的,如果不是今天的，就不管了。
	//if (newlocaltm.raw_date() != _biInfo.m_lDate)
	//{
	//	return;
	//}

	//int timeoffset = get_min_offset(_last_hq_min / 60, _last_hq_min % 60);
}

void HqFile::handle_timer(const asio::error_code& e)
{
	handle_timer_init_and_shoupan();
	handle_timer_refresh_boursetime();
	stk_file.sync();//写下硬盘，保存起来。
}