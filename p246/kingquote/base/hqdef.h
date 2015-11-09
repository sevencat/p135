#pragma once
#include <stdint.h>
//行情的相关定义
#pragma warning(disable:4200)

//整数对齐，否则有可能会有问题
#pragma pack(push,4)

//一个股票可能会有4800笔!!!
class HqTick
{
public:
	//tick数据
	int32_t upttime;//时间，时间格式为当天的秒数*10+微秒（比如一秒钟有多笔推过来)，但基本上不可能有一秒超过10笔的
	int32_t px;//价格
	int32_t vol;//成交量
	float money;//成交金额
	int32_t position;//持仓量
	int32_t cjbs;//成交笔数
};
//分配是在文件里一个个的分配的!!!先分配后写!!!，最多的是4800,我们一次性分配1024
class HqTickBlock
{
public:
	int16_t curpos;//当前偏移量
	int16_t nextblock;//下一个块偏移
	int32_t nouse;//没有用到
};

//每条记录信息，这是一个头部信息
class HqRecord
{
public:
	int16_t idx;//序号
	int16_t mktid;//市场序号,可能会有子市场号
	int32_t lasttime;//最后时间，这里是减掉1990的时间
	char symbol[8];//实际上只用到6
	char name[32];//实际上只想用到24
	int32_t preclosepx;//前收盘价,初期可以统一用这个
	int32_t prejspx;//前结算价
	int32_t openpx;
	int32_t highpx;
	int32_t lowpx;
	int32_t newpx;//收盘价，其实还有个最新价，最新价！=收盘价
	int32_t todayjspx;//今结算价
	int32_t vol;//成交量
	float money;//成交金额
	int32_t position;//持仓量
	int32_t cjbs;//成交笔数
};


class HqMinRecord
{
public:
	//行情的分钟线
	uint32_t lasthqtime;//最后行情时间，去掉1990的时间
	int32_t openpx;
	int32_t highpx;
	int32_t lowpx;
	int32_t newpx;//收盘价，其实还有个最新价，最新价！=收盘价
	int32_t vol;//成交量
	float money;//成交金额
	int32_t position;//持仓量
	int32_t cjbs;//成交笔数
	int32_t nouse;//未使用
};


#define HQFILEITEMCOUNT 14336
//头部信息，可以设定为
class HqFileHdr
{
public:
	int32_t dt;//当前日期
	int32_t count;//总数量
	int32_t mincount;//分钟线数量
	int32_t ticklastpos;//最后一个tick块数据
	HqRecord rtrec[HQFILEITEMCOUNT];
	HqMinRecord minrec[0];
	static int32_t get_hdr_size(int hqmincount)
	{
		int32_t strsize = sizeof(HqFileHdr);
		strsize += hqmincount*HQFILEITEMCOUNT*sizeof(HqMinRecord);
		return strsize;
	}
	HqMinRecord *get_min_rec_by_idx(int stkidx)
	{
		return &minrec[stkidx + mincount];
	}
	int64_t get_tickblock(int idx)
	{
		//取得第N块的逐笔偏移量
		int64_t hdrsize = get_hdr_size(mincount);
		return hdrsize + idx*sizeof(HqTickBlock);
	}
};

#pragma pack(pop)
//10240+4096