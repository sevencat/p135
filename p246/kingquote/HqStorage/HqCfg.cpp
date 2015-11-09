#include "stdafx.h"
#include "HqCfg.h"


HqCfg::HqCfg()
{
	_use_stk_open = true;
	_use_stk_high = true;
	_use_stk_low = true;

	_has_vol = true;
	_has_money = true;
	_has_avgprice = true;
	_check_dup = false;
	_userrpttime = true;
}


HqCfg::~HqCfg()
{
}
