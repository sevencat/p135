#include "StdAfx.h"
#include "NeZipDrv.h"
#include "DataWriteQueue.h"
#include "dtz.h"

BOOL NeZipDrv::m_initialized;

NeZipDrv::NeZipDrv(void)
{
}

NeZipDrv::~NeZipDrv(void)
{
}

bool NeZipDrv::load(const char *dllfile)
{
	addlog("开始加载nezip 驱动%s",dllfile);
	m_stockdrvDll = ::LoadLibrary(dllfile);		//导入动态库（该动态库可以放在其他目录，推荐放置默认位置，以便以后自动升级）
	if (!m_stockdrvDll)
	{
		addlog("加载失败");
		return false;
	}
	InitStockDrv_ = (_InitStockDrv)GetProcAddress(m_stockdrvDll, "InitStockDrv");
	AskData_ = (_AskData)GetProcAddress(m_stockdrvDll, "AskData");
	if (!InitStockDrv_ || !AskData_)
	{
		addlog("未能成功加载函数");
		return false;
	}
	m_traceCount = 0;
	m_dataVersion = OEM_MAIN_CLIENT;
	strcpy(m_clientPackInfo, "Client Data .");
	InitStockDrv_(NULL, 0, m_dataVersion, "localhost", NULL, [](const void *pdata)
	{
		return OnCallBack((TCP_DATA_HEAD *)pdata);
	});
	addlog("加载成功");
	return true;
}

bool NeZipDrv::querydata(LPCSTR stkLabel, KLINETYPE klineType, bool sameDay /*= false*/, bool isSplit /*= false*/, char splitLeft /*= 1*/, bool askServer /*= false*/)
{
	TCP_ASK_DATA askData;
	TCP_DATA_HEAD* pTcpHead = &askData.dataHead;
	TCP_ASK_HEAD* pAskHead = &askData.askHead;
	pAskHead->Init(stkLabel, klineType, 0, 0, sameDay, isSplit, splitLeft, askServer);		//从本地缓冲区读出数据
	pTcpHead->Init(m_dataVersion, klineType, m_clientPackInfo, 0, sizeof TCP_ASK_HEAD, sizeof TCP_ASK_HEAD, 1);
	return AskData_(pTcpHead);
}

BOOL NeZipDrv::OnCallBack(TCP_DATA_HEAD *pTcpHead)
{
	UINT count = pTcpHead->count;
	PSTR pData = pTcpHead->pData;
	switch (pTcpHead->klineType)
	{
	case OEM_DATA_0526:			//申请10档行情（如果您不是10档提供者，忽略）
	case OEM_DATA_0551:			//申请逐笔数据（如果您不是数据源提供者，忽略）
	case OEM_DATA_0552:			//申请当前逐笔数据
	{
		return TRUE;
	}
	case OEM_ASK_NO:				//服务器无该数据
	case OEM_ASK_TRUE:				//服务器已收到该次请求
	{
		m_initialized = TRUE;	//收到网际风发来的信息，表示连接正常
		return TRUE;
	}
	case OEM_MARKETLABEL:			//市场代码表，主动通知，也可以通过OEM_INIT_CLIENT申请
	{
		OEM_MARKET_REPLY* pMarketInfoReply = (OEM_MARKET_REPLY *)pData;
		OEM_HEAD* pOemHead = &pMarketInfoReply->head;
		OEM_MARKETINFO* pOemMarketInfo = &pMarketInfoReply->marketInfo;
		OEM_STOCKINFO* pOemStockInfo = pMarketInfoReply->stockInfo;
		return TRUE;
	}
	case OEM_REPORTDATA:	//实时数据处理模块
	{
		handle_reportdata(pTcpHead);
		return TRUE;
	}
	case TICK_KLINE:		//补分时
	case TRACE_KLINE:		//补分笔处理程序
	{
		handle_tick(pTcpHead);
		return TRUE;
	}
	case MIN1_KLINE:			//1分钟线
	{
		handle_mindata(pTcpHead, true);
		return TRUE;
	}
	case MIN5_KLINE:			//5分钟线	
	{
		handle_mindata(pTcpHead, false);
		return TRUE;
	}
	case MIN15_KLINE:			//15分钟线
	case MIN30_KLINE:			//30分钟线
	case MIN60_KLINE:			//60分钟线
	{
		return TRUE;
	}
	case DAY_KLINE:				//日线
	{
		handle_daydata(pTcpHead);
		return TRUE;
	}
	case WEEK_KLINE:			//周线
	case MONTH_KLINE:			//月线
	case YEAR_KLINE:			//年
	case MANY_DAY_KLINE:		//多日线
	case MANY_MIN_KLINE:		//多分钟线
	case MANY_SEC_KLINE:		//多秒线
	{
		RCV_KLINE* pKline = (RCV_KLINE *)pData;
		return TRUE;
	}
	case SPLIT_FINANCE:	//处理除权数据，取出对自己有用的信息即可
	{
		BASEINFO* pBaseInfo = (BASEINFO *)pData;
		return TRUE;
	}
	case OEM_F10_FILE:		//F10基本资料文件，分成索引文件和实际文本文件
	{
		QIANLONG_F10HEAD* pF10Head = (QIANLONG_F10HEAD *)pData;
		QIANLONG_F10INDEX* pF10Index = pF10Head->f10Index;
		UINT  dataLen = pTcpHead->unZipDataLen;
		if (dataLen < 200)		//没有实际数据
			return FALSE;
		if (::memcmp(pF10Head->mark, "NEZIPF10", 8))	//非法数据
			return FALSE;
		UINT  f10head = pF10Head->GetHeadLen();
		if (dataLen < f10head + 200)		//文件似乎太小了
			return FALSE;
		dataLen -= f10head;
		LPSTR pF10Txt = pData + f10head;	//文本开始位置
		pF10Txt = pF10Txt;
	}
	return TRUE;
	default:
		return FALSE;
	}
}
_tzinfo_t globaltz = _tzinfo_t::china();
#define MINFLOATVALUE 0.00000000001

inline dec::decimal4 multifloat(float value, int multiint = 1000)
{
	double xvalue = value;
	xvalue *= multiint;
	int64_t xvalue2 = floor(xvalue + 0.5);
	dec::decimal4 xvalue3=dec::decimal4(10 * xvalue2)/dec::decimal4(multiint);
	return xvalue3;
}
#include "strutil.h"
void NeZipDrv::handle_daydata(TCP_DATA_HEAD *pTcpHead)
{
	int count = pTcpHead->count;
	RCV_KLINE* pKline = (RCV_KLINE *)pTcpHead->pData;
	std::list<KLineData> kd;

	std::string mkt;
	std::string code;
	std::string filtermkt = "DL, ZZ, ZJ, SQ, SH, SZ";
	for (int i = 0; i < count; i++)
	{
		RCV_KLINE &curline = pKline[i];
		if (curline.head.headTag == EKE_HEAD_TAG)
		{
			mkt = std::string(curline.head.marketEx,2);
			code = curline.head.stockId;
			StrUtil::toUpperCase(mkt);
			StrUtil::toUpperCase(code);
		}
		else
		{
			if (code.length() > 0)
			{
				if (filtermkt.find(mkt) == std::string::npos)
					continue;
				//DL, ZZ, ZJ, SQ, SH, SZ
				KLineData curkd;
				_datetime_t utcdt;
				utcdt.from_gmt_timer(curline.time);
				curkd.rq = utcdt.raw_date();
				curkd.mkt = mkt;
				curkd.code = code;
				curkd.opoenpx = multifloat(curline.open);
				curkd.highpx = multifloat(curline.high);
				curkd.lowpx = multifloat(curline.low);
				curkd.closepx = multifloat(curline.close);
				curkd.vol = curline.volume;
				curkd.money = multifloat(curline.amount);
				curkd.pos = 0;
				curkd.jspx = 0;
				kd.push_back(curkd);
			}
		}
	}
	gDataWriteQueue.merge_daydata(kd);
}

void NeZipDrv::handle_mindata(TCP_DATA_HEAD *pTcpHead, bool ismin1)
{
	int count = pTcpHead->count;
	RCV_KLINE* pKline = (RCV_KLINE *)pTcpHead->pData;
	std::list<KLineData> kd;

	std::string mkt;
	std::string code;
	std::string filtermkt = "DL, ZZ, ZJ, SQ, SH, SZ";
	for (int i = 0; i < count; i++)
	{
		RCV_KLINE &curline = pKline[i];
		if (curline.head.headTag == EKE_HEAD_TAG)
		{
			mkt = std::string(curline.head.marketEx, 2);
			code = curline.head.stockId;
			StrUtil::toUpperCase(mkt);
			StrUtil::toUpperCase(code);
		}
		else
		{
			if (code.length() > 0)
			{
				if (filtermkt.find(mkt) == std::string::npos)
					continue;
				//DL, ZZ, ZJ, SQ, SH, SZ
				KLineData curkd;
				_datetime_t utcdt;
				utcdt.from_local_timer(curline.time);

				curkd.rq = utcdt.raw_date();
				curkd.rq = curkd.rq * 10000 + utcdt.raw_time() / 10000;
				curkd.mkt = mkt;
				curkd.code = code;
				curkd.opoenpx = multifloat(curline.open);
				curkd.highpx = multifloat(curline.high);
				curkd.lowpx = multifloat(curline.low);
				curkd.closepx = multifloat(curline.close);
				curkd.vol = curline.volume;
				curkd.money = multifloat(curline.amount);
				curkd.pos = 0;
				curkd.jspx = 0;
				kd.push_back(curkd);
			}
		}
	}
	if (ismin1)
		gDataWriteQueue.merge_min1data(kd);
	else
		gDataWriteQueue.merge_min5data(kd);
}

void NeZipDrv::handle_reportdata(TCP_DATA_HEAD *pTcpHead)
{
	UINT count = pTcpHead->count;
	PSTR pData = pTcpHead->pData;
	for (UINT i = 0, size = sizeof OEM_REPORTV5; i < count; i++, pData += size)
	{
		OEM_REPORTV5 *pReport5 = reinterpret_cast<OEM_REPORTV5*>(pData);
	}
}

void NeZipDrv::handle_tick(TCP_DATA_HEAD *pTcpHead)
{
	UINT count = pTcpHead->count;
	//addlog("收到%d条tick数据",count);
	RCV_TRACEV50* pTrace = (RCV_TRACEV50 *)pTcpHead->pData;


	std::list<TickData> ticklst;
	std::string filtermkt = "DL, ZZ, ZJ, SQ, SH, SZ";
	TickData tickdata;

	for (UINT i = 0; i < count; i++)
	{
		RCV_TRACEV50 &curtick = pTrace[i];
		if (curtick.head.headTag == EKE_HEAD_TAG)
		{
			tickdata.mkt = std::string(curtick.head.marketEx, 2);
			tickdata.code = curtick.head.stockId;
			StrUtil::toUpperCase(tickdata.mkt);
			StrUtil::toUpperCase(tickdata.code);
			tickdata.lastclose = multifloat(curtick.lastClose);
			tickdata.openpx = multifloat(curtick.open);
		}
		else
		{
			if (filtermkt.find(tickdata.mkt) == std::string::npos)
				continue;
			_datetime_t utcdt;
			utcdt.from_local_timer(curtick.time);

			tickdata.rq = utcdt.raw_date();
			tickdata.rq = tickdata.rq * 1000000 + utcdt.raw_time() / 100;
			for (int idx = 0; idx < 3; idx++)
			{
				tickdata.buypx[idx] = multifloat(curtick.pricebuy[i]);
				tickdata.buyvol[idx] = multifloat(curtick.volbuy[i]);
				tickdata.sellpx[idx] = multifloat(curtick.pricesell[i]);
				tickdata.sellvol[idx] = multifloat(curtick.volsell[i]);
			}
			ticklst.push_back(tickdata);
		}
		gDataWriteQueue.merge_tickdata(ticklst);
		addlog("收到国内期货数据%d条", ticklst.size());
	}
	return;
}
