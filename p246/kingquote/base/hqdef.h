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
#define HQTICKCOUNT 1024
class HqTickBlock
{
public:
	int16_t ncount;//当前块数量
	int16_t nextblock;//下一个块偏移
	int32_t nouse;//没有用到
	HqTick tick[HQTICKCOUNT];
	HqTickBlock(int ntotal=0)
	{
		memset(this, 0, sizeof(this));
		ncount = ntotal;
	}
};

//每条记录信息，这是一个头部信息
class HqRecord
{
public:
	int16_t idx;//序号
	uint16_t mktid;//市场序号,可能会有子市场号
	int32_t lasttime;//最后时间，这里是减掉1990的时间
	int16_t curminpos;//当前最新分钟线
	int16_t nouse;//没有用
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

	int32_t buypx[5];
	int32_t buyvol[5];
	int32_t sellpx[5];
	int32_t sellvol[5];
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

#define HDRFLAG 0x00ffffff
//头部信息，可以设定为
//头部  reccount条HqRecord  reccount*mincount条HqMinRecord
class HqFileHdr
{
public:
	int32_t hdrflag;//头部=0x00ffffff
	int32_t dt;//当前日期
	int32_t stkchgtime;//股票数量变化时间
	int32_t maxstkcount;//总数量
	int32_t mincount;//分钟线数量
	int32_t ticklastpos;//最后一个tick块数据
	int32_t curstkcount;//当前股票数量
	HqRecord rtrec[0];
	//HqMinRecord minrec[0];

	//获取头部大小，不包含tick数据
	static int32_t get_hdr_size(int recount,int hqmincount)
	{
		int32_t strsize = sizeof(HqFileHdr) + recount*sizeof(HqRecord) + hqmincount*recount*sizeof(HqMinRecord);
		return strsize;
	}

	//获取初始化的文件大小，这个比头部大小多逐笔的数量
	static int32_t get_init_size(int recount, int hqmincount)
	{
		int32_t totalsize = get_hdr_size(recount, hqmincount);
		return totalsize + recount*sizeof(HqTickBlock);
	}

	int32_t get_size()
	{
		return get_hdr_size(maxstkcount, mincount);
	}

	HqMinRecord *get_min_rec_by_idx(int stkidx)
	{
		char *pdata = (char *)this;
		pdata =pdata+ sizeof(HqFileHdr) + maxstkcount*sizeof(HqRecord);
		HqMinRecord *mindata = (HqMinRecord *)pdata;
		return mindata + stkidx*mincount;
	}
	int64_t get_tickblock(int idx)
	{
		//取得第N块的逐笔偏移量
		int64_t hdrsize = get_size();
		return hdrsize + idx*sizeof(HqTickBlock);
	}

	
};

#pragma pack(pop)
//10240+4096


#define MINFLOATVALUE 0.00000000001

inline long multifloat(float value, int multiint = 1000)
{
	double xvalue = value;
	xvalue *= multiint;
	return floor(xvalue + 0.5);
}