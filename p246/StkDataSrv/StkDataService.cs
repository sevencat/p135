using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ServiceStack;
using ServiceStack.OrmLite;

namespace StkDataSrv
{
	public enum BfBarPeriod
	{
		PERIOD_UNKNOWN = 0,         // 未知
		PERIOD_S01 = 1,             // 1秒
		PERIOD_S03 = 2,             // 3秒
		PERIOD_S05 = 3,             // 5秒
		PERIOD_S10 = 4,             // 10秒
		PERIOD_S15 = 5,             // 15秒
		PERIOD_S30 = 6,             // 30秒
		PERIOD_M01 = 7,             // 1分
		PERIOD_M03 = 8,             // 3分
		PERIOD_M05 = 9,             // 5分
		PERIOD_M10 = 10,            // 10分
		PERIOD_M15 = 11,            // 15分
		PERIOD_M30 = 12,            // 30分
		PERIOD_H01 = 13,            // 1小时
		PERIOD_H02 = 14,            // 2小时
		PERIOD_H03 = 15,            // 3小时
		PERIOD_H04 = 16,            // 4小时
		PERIOD_D01 = 17,            // 1天
		PERIOD_W01 = 18,            // 1周
		PERIOD_X01 = 19,            // 1月
	}

	public class KLineData
	{
		public long dt { get; set; }
		public decimal openpx { get; set; }
		public decimal highpx { get; set; }
		public decimal lowpx { get; set; }
		public decimal closepx { get; set; }
		public int vol { get; set; }
		public decimal money { get; set; }
	}

	[Route("/api/BfGetBarReq", "GET", Summary = "获取合约数据")]
	public class BfGetBarReq : IReturn<List<KLineData>>
	{
		[ApiMember(Description = "合约代码")]
		public string symbol { get; set; }              // 合约代码
		[ApiMember(Description = "交易所代码,ZZ郑州,ZJ中金,SQ上期,DL大连")]
		public string exchange { get; set; }            // 交易所代码
		[ApiMember(Description = "周期,17日线,7一分钟，9-五分钟")]
		public int period { get; set; }         // 周期
		[ApiMember(Description = "截止日期")]
		public long todatetime { get; set; }              // 截止日期
		[ApiMember(Description = "获取条数")]
		public int count { get; set; } // 获取条数
	}

	public class StkDataService : Service
	{
		public List<KLineData> Get(BfGetBarReq req)
		{
			string tblname = "";
			switch (req.period)
			{
				case (int)BfBarPeriod.PERIOD_M01:
					tblname = "min1";
					break;
				case (int)BfBarPeriod.PERIOD_M05:
					tblname = "min5";
					break;
				case (int)BfBarPeriod.PERIOD_D01:
					tblname = "day";
					break;
				default:
					return new List<KLineData>();
			}
			using (var conn = AppHost.Instance.GetDbConnection())
			{
				var datalst = conn.SelectFmt<KLineData>("select dt,openpx,highpx,lowpx,closepx,vol,money from " + tblname + " where mkt={0} and code={1} and dt<={2} order by dt desc limit {3};",
					req.exchange,req.symbol,req.todatetime,req.count);
				return datalst;
			}
		}
	}
}
