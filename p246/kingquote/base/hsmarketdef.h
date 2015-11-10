#pragma once

/*
市场类别定义：
各位含义表示如下：
15		   12		8					0
|			|	  	  |					|
| 金融分类	|市场分类 |	交易品种分类	|
*/
typedef unsigned short HSMarketDataType;			  // 市场分类数据类型
/*金融大类*/
#define STOCK_MARKET			 0X1000   // 股票
#	define SH_BOURSE			 0x0100   // 上海
#	define SZ_BOURSE			 0x0200   // 深圳
#	define SYSBK_BOURSE			 0x0400   // 系统板块
#	define USERDEF_BOURSE		 0x0800   // 自定义（自选股或者自定义板块）
#			define KIND_INDEX		 0x0000   // 指数 
#			define KIND_STOCKA		 0x0001   // A股 
#			define KIND_STOCKB		 0x0002   // B股 
#			define KIND_BOND		 0x0003   // 债券
#			define KIND_FUND		 0x0004   // 基金
#			define KIND_THREEBOAD	 0x0005   // 三板
#			define KIND_SMALLSTOCK	 0x0006   // 中小盘股
#			define KIND_PLACE		 0x0007	  // 配售
#			define KIND_LOF			 0x0008	  // LOF
#			define KIND_ETF			 0x0009   // ETF
#			define KIND_QuanZhen	 0x000A   // 权证

#			define KIND_OtherIndex	 0x000E   // 第三方行情分类，如:中信指数

#			define SC_Others		 0x000F   // 其他 0x09
#			define KIND_USERDEFINE	 0x0010   // 自定义指数


// 港股市场
#define HK_MARKET				 0x2000 // 港股分类
#	define HK_BOURSE			 0x0100 // 港股
#	define	HK_OPTIONS			 0x0200 // 牛熊证


#	define SYSBK_BOURSE			 0x0400 // 港股板块(H股指数成份股，讯捷软件指数成份股）。
#	define USERDEF_BOURSE		 0x0800 // 自定义（自选股或者自定义板块）
#			define HK_KIND_BOND		 0x0000   // 债券
#			define HK_KIND_MulFund	 0x0001   // 一揽子认股证
#			define HK_KIND_FUND		 0x0002   // 基金
#			define KIND_WARRANTS	 0x0003   // 认股证
#			define KIND_JR			 0x0004   // 金融
#			define KIND_ZH			 0x0005   // 综合
#			define KIND_DC			 0x0006   // 地产
#			define KIND_LY			 0x0007   // 旅游
#			define KIND_GY			 0x0008   // 工业
#			define KIND_GG			 0x0009   // 公用
#			define KIND_QT			 0x000A   // 其它

/*期货大类*/
#define FUTURES_MARKET			 0x4000 // 期货
#		define DALIAN_BOURSE		 0x0100	// 大连
#		define SHANGHAI_BOURSE	  0x0200	// 上海
#		define ZHENGZHOU_BOURSE	  0x0300	// 郑州
#		define HUANGJIN_BOURSE	  0x0400		// 黄金交易所
#		define GUZHI_BOURSE		  0x0500		// 股指期货

#		define SELF_BOURSE		  0x0600	// 自定义数据
#		define DZGT_BOURSE		  0x0610	// 大宗钢铁数据
#		define DLTL_BOURSE		  0x0620	// 大连套利
#		define YSJS_BOURSE		  0x0630	// 有色金属

#		define DynamicSelf_BOURSE 0x0700	// 动态数据类型自定义数据

/*外盘大类*/
#define WP_MARKET				 (0x5000) // 外盘
#		define WP_INDEX				0x0100	// 国际指数 // 不用了
#		define WP_LME				0x0200	// LME		// 不用了
#			define WP_LME_CLT			0x0210 //"场内铜";
#			define WP_LME_CLL			0x0220 //"场内铝";
#			define WP_LME_CLM			0x0230 //"场内镍";
#			define WP_LME_CLQ			0x0240 //"场内铅";
#			define WP_LME_CLX			0x0250 //"场内锌";
#			define WP_LME_CWT			0x0260 //"场外铝";
#			define WP_LME_CW			0x0270 //"场外";
#			define WP_LME_SUB			0x0000

#			define WP_CBOT				0x0300	// CBOT			
#			define WP_NYMEX	 			0x0400	// NYMEX	 	
#			define WP_NYMEX_YY			0x0000	//"原油";
#			define WP_NYMEX_RY			0x0001	//"燃油";
#			define WP_NYMEX_QY			0x0002	//"汽油";

#			define WP_COMEX	 			0x0500	// COMEX	 	
#			define WP_TOCOM	 			0x0600	// TOCOM	 	
#			define WP_IPE				0x0700	// IPE			
#			define WP_NYBOT				0x0800	// NYBOT		
#			define WP_NOBLE_METAL		0x0900	// 贵金属	
#			  define WP_NOBLE_METAL_XH	0x0000  //"现货";
#			  define WP_NOBLE_METAL_HJ	0x0001  //"黄金";
#			  define WP_NOBLE_METAL_BY	0x0002  //"白银";

#			define WP_MY		0x0a00	// 马来西亚大马交易所
#			define WP_SICOM				0x0b00	// SICOM		
#			define WP_LIBOR				0x0c00	// LIBOR		
#			define WP_NYSE				0x0d00	// NYSE
#			define WP_CEC				0x0e00	// CEC


#			define WP_Other_TZTHuanjin	0x0F10	// 黄金期货数据生成,第三方数据
#			define WP_Other_JinKaiXun	0x0F20	// 金凯讯的数据
#			define WP_JKX               WP_Other_JinKaiXun
#			define WP_XJP               0x0F30	// 新加坡数据
#			define WP_LYSEE 			0x0F40  // 纽约交易所


#			define WP_INDEX_MZ1	 		0x0110 //"美州一";
#			define WP_INDEX_MZ2	 		0x0120 //"美洲二";
#			define WP_INDEX_OZ	 		0x0130 //"欧洲";
#			define WP_INDEX_YZ1	 		0x0140 //"亚洲一";
#			define WP_INDEX_YZ2	 		0x0150 //"亚洲二";
#			define WP_INDEX_YZ3	 		0x0160 //"亚洲三";



/*外汇大类*/
#define FOREIGN_MARKET			(0x8000) // 外汇
#	define WH_BASE_RATE			0x0100	// 基本汇率
#	define WH_ACROSS_RATE		0x0200	// 交叉汇率
#	define WH_FUTURES_RATE		0x0300  // 期汇