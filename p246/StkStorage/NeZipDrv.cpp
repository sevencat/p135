#include "StdAfx.h"
#include "NeZipDrv.h"

BOOL NeZipDrv::m_initialized;

NeZipDrv::NeZipDrv(void)
{
}

NeZipDrv::~NeZipDrv(void)
{
}

bool NeZipDrv::load(const char *dllfile)
{
	m_stockdrvDll = ::LoadLibrary(dllfile);		//导入动态库（该动态库可以放在其他目录，推荐放置默认位置，以便以后自动升级）
	if (!m_stockdrvDll)
	{
		return false;
	}
	InitStockDrv_ = (_InitStockDrv)GetProcAddress(m_stockdrvDll, "InitStockDrv");
	AskData_ = (_AskData)GetProcAddress(m_stockdrvDll, "AskData");
	if (!InitStockDrv_ || !AskData_)
	{
		return false;
	}
	m_traceCount = 0;
	m_dataVersion = OEM_MAIN_CLIENT;
	strcpy(m_clientPackInfo, "Client Data .");
	InitStockDrv_(NULL, 0, m_dataVersion, "localhost", NULL, [](const void *pdata)
	{
		return OnCallBack((TCP_DATA_HEAD *)pdata);
	});
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
		OEM_REPORTV5* pReport5;
		for (UINT i = 0, size = sizeof OEM_REPORTV5; i < count; i++, pData += size)
		{
			OEM_REPORTV5 *pReport5 = reinterpret_cast<OEM_REPORTV5*>(pData);		//指针转换
		}
		return TRUE;
	}
	case TICK_KLINE:		//补分时
	case TRACE_KLINE:		//补分笔处理程序
	{
		RCV_TRACEV50* pTrace = (RCV_TRACEV50 *)pData;
		return TRUE;
	}
	case MIN1_KLINE:			//1分钟线
	case MIN5_KLINE:			//5分钟线	
	case MIN15_KLINE:			//15分钟线
	case MIN30_KLINE:			//30分钟线
	case MIN60_KLINE:			//60分钟线
	case DAY_KLINE:				//日线
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
