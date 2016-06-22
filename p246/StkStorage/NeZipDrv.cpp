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
	addlog("��ʼ����nezip ����%s",dllfile);
	m_stockdrvDll = ::LoadLibrary(dllfile);		//���붯̬�⣨�ö�̬����Է�������Ŀ¼���Ƽ�����Ĭ��λ�ã��Ա��Ժ��Զ�������
	if (!m_stockdrvDll)
	{
		addlog("����ʧ��");
		return false;
	}
	InitStockDrv_ = (_InitStockDrv)GetProcAddress(m_stockdrvDll, "InitStockDrv");
	AskData_ = (_AskData)GetProcAddress(m_stockdrvDll, "AskData");
	if (!InitStockDrv_ || !AskData_)
	{
		addlog("δ�ܳɹ����غ���");
		return false;
	}
	m_traceCount = 0;
	m_dataVersion = OEM_MAIN_CLIENT;
	strcpy(m_clientPackInfo, "Client Data .");
	InitStockDrv_(NULL, 0, m_dataVersion, "localhost", NULL, [](const void *pdata)
	{
		return OnCallBack((TCP_DATA_HEAD *)pdata);
	});
	addlog("���سɹ�");
	return true;
}

bool NeZipDrv::querydata(LPCSTR stkLabel, KLINETYPE klineType, bool sameDay /*= false*/, bool isSplit /*= false*/, char splitLeft /*= 1*/, bool askServer /*= false*/)
{
	TCP_ASK_DATA askData;
	TCP_DATA_HEAD* pTcpHead = &askData.dataHead;
	TCP_ASK_HEAD* pAskHead = &askData.askHead;
	pAskHead->Init(stkLabel, klineType, 0, 0, sameDay, isSplit, splitLeft, askServer);		//�ӱ��ػ�������������
	pTcpHead->Init(m_dataVersion, klineType, m_clientPackInfo, 0, sizeof TCP_ASK_HEAD, sizeof TCP_ASK_HEAD, 1);
	return AskData_(pTcpHead);
}

BOOL NeZipDrv::OnCallBack(TCP_DATA_HEAD *pTcpHead)
{
	UINT count = pTcpHead->count;
	PSTR pData = pTcpHead->pData;
	switch (pTcpHead->klineType)
	{
	case OEM_DATA_0526:			//����10�����飨���������10���ṩ�ߣ����ԣ�
	case OEM_DATA_0551:			//����������ݣ��������������Դ�ṩ�ߣ����ԣ�
	case OEM_DATA_0552:			//���뵱ǰ�������
	{
		return TRUE;
	}
	case OEM_ASK_NO:				//�������޸�����
	case OEM_ASK_TRUE:				//���������յ��ô�����
	{
		m_initialized = TRUE;	//�յ����ʷ緢������Ϣ����ʾ��������
		return TRUE;
	}
	case OEM_MARKETLABEL:			//�г����������֪ͨ��Ҳ����ͨ��OEM_INIT_CLIENT����
	{
		OEM_MARKET_REPLY* pMarketInfoReply = (OEM_MARKET_REPLY *)pData;
		OEM_HEAD* pOemHead = &pMarketInfoReply->head;
		OEM_MARKETINFO* pOemMarketInfo = &pMarketInfoReply->marketInfo;
		OEM_STOCKINFO* pOemStockInfo = pMarketInfoReply->stockInfo;
		return TRUE;
	}
	case OEM_REPORTDATA:	//ʵʱ���ݴ���ģ��
	{
		handle_reportdata(pTcpHead);
		return TRUE;
	}
	case TICK_KLINE:		//����ʱ
	case TRACE_KLINE:		//���ֱʴ������
	{
		handle_tick(pTcpHead);
		return TRUE;
	}
	case MIN1_KLINE:			//1������
	{
		handle_mindata(pTcpHead, true);
		return TRUE;
	}
	case MIN5_KLINE:			//5������	
	{
		handle_mindata(pTcpHead, false);
		return TRUE;
	}
	case MIN15_KLINE:			//15������
	case MIN30_KLINE:			//30������
	case MIN60_KLINE:			//60������
	{
		return TRUE;
	}
	case DAY_KLINE:				//����
	{
		handle_daydata(pTcpHead);
		return TRUE;
	}
	case WEEK_KLINE:			//����
	case MONTH_KLINE:			//����
	case YEAR_KLINE:			//��
	case MANY_DAY_KLINE:		//������
	case MANY_MIN_KLINE:		//�������
	case MANY_SEC_KLINE:		//������
	{
		RCV_KLINE* pKline = (RCV_KLINE *)pData;
		return TRUE;
	}
	case SPLIT_FINANCE:	//�����Ȩ���ݣ�ȡ�����Լ����õ���Ϣ����
	{
		BASEINFO* pBaseInfo = (BASEINFO *)pData;
		return TRUE;
	}
	case OEM_F10_FILE:		//F10���������ļ����ֳ������ļ���ʵ���ı��ļ�
	{
		QIANLONG_F10HEAD* pF10Head = (QIANLONG_F10HEAD *)pData;
		QIANLONG_F10INDEX* pF10Index = pF10Head->f10Index;
		UINT  dataLen = pTcpHead->unZipDataLen;
		if (dataLen < 200)		//û��ʵ������
			return FALSE;
		if (::memcmp(pF10Head->mark, "NEZIPF10", 8))	//�Ƿ�����
			return FALSE;
		UINT  f10head = pF10Head->GetHeadLen();
		if (dataLen < f10head + 200)		//�ļ��ƺ�̫С��
			return FALSE;
		dataLen -= f10head;
		LPSTR pF10Txt = pData + f10head;	//�ı���ʼλ��
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
	addlog("�յ�%d��tick����");
	UINT count = pTcpHead->count;
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
			tickdata.rq = tickdata.rq * 10000 + utcdt.raw_time() / 100;
			for (int idx = 0; idx < 3; idx++)
			{
				tickdata.buypx[idx] = multifloat(curtick.pricebuy[i]);
				tickdata.buyvol[idx] = multifloat(curtick.volbuy[i]);
				tickdata.sellpx[idx] = multifloat(curtick.pricesell[i]);
				tickdata.sellvol[idx] = multifloat(curtick.volsell[i]);
			}
			ticklst.push_back(tickdata);
		}
	}
	return;
}
