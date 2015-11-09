#pragma once
#include <wtypes.h>
#include <time.h>

#pragma pack(push,4)

#if _MSC_VER <= 1310
typedef time_t __time32_t;
#endif

// 工作方式类型定义
#define RCV_WORK_DEMAND				0	// 查询接口,方式一
#define RCV_WORK_MEMSHARE			2					// 内存共享直接引用
#define RCV_WORK_SENDMSG			4					// 版本 2 建议使用的方式	
// 其他工作方式保留

#define	TS_StockDrv_IdTxt	"TongShi_StockDrv_1.00"		// 通视信息卡特征串


// 证券市场
// 证券市场
#define		SH_MARKET_EX			'HS'		// 上海
#define		SZ_MARKET_EX			'ZS'		// 深圳
#define		BK_MARKET_EX			'KB'		// 板块
#define		HK_MARKET_EX			'KH'		// 香港
#define		SQ_MARKET_EX			'QS'		// 上期
#define		DL_MARKET_EX			'LD'		// 大连
#define		ZZ_MARKET_EX			'ZZ'		// 郑州
#define		JZ_MARKET_EX			'JZ'		// 中金所
#define		WP_MARKET_EX			'PW'		// 外盘
#define		ID_MARKET_EX			'DI'		// 国际指数
#define		WH_MARKET_EX			'WH'		// 外汇


#define		FILE_IMTITLE_EX			0x3001		// 信息地雷标题
#define		FILE_IMCONTENT_EX		0x3002		// 信息地雷内容
#define		FILE_TRACE_EX			0x4001		// 补分明细数据



#define		STKLABEL_LEN			10			// 股号数据长度,国内市场股号编码兼容钱龙
#define		STKNAME_LEN				32			// 股名长度

#define		TITLE_LEN				68			// 信息地雷标题

#define		IM_REALTIME				0x11		// 某股票的实时信息地雷
#define		IM_HISTORY				0x12		// 某股票的历史信息地雷
#define		IM_JYSINFO				0x13		// 交易所信息
#define		IM_F10					0x21        //  3		// F10

//信息地雷内容格式定义
#define		IM_FMT_HTTP				1
#define		IM_FMT_HTML				2
#define		IM_FMT_TEXT				3
// 文件数据类型
// 结构数组形式的文件数据
#define		FILE_HISTORY_EX			2			// 补日线数据
#define		FILE_MINUTE_EX			4			// 补分钟线数据
#define     FILE_POWER_EX           6           //除权信息
#define     FILE_HISTORY_MINUTE_EX        8          //补历史五分钟数据
#define     FILE_BASEINFO_EX             10          //补充全部股票财务数据
#define     FILE_DISPARTBARGAINING_EX    12          //补分笔交易明细数据
#define     FILE_STOCKLABELLIST_EX       14          //接收全部股票代码表
#define     FILE_SYSTEMRUN_EX            16          //接收系统运行参数
#define     FILE_INDEXATTRIBUTE_EX       18          //大盘红绿军 
#define     FILE_LOGINAUTH_EX            20          //客户登入授权
#define     FILE_TECHNICINDEX            22          //客户端技术指标

//网际风扩展！！！
#define FILE_5MINUTE_EX 0x51        // 补5分钟K线  数据格式与日线完全相同 仅仅参数不同而已
#define FILE_1MINUTE_EX 0x52        // 补1分钟K线  数据格式与日线完全相同 仅仅参数不同而已

#define		FILE_BASE_EX			0x1000		// 钱龙兼容基本资料文件,m_szFileName仅包含文件名
#define		FILE_NEWS_EX			0x1002		// 新闻类,其类型由m_szFileName中子目录名来定
#define     FILE_HTML_EX            0x1004      //HTML 文件，m_szFileName为URL
#define     FILE_ALLNEWSINFO_EX     0x1006      //市场当天全部公告信息 
#define     FILE_NOTIFY_EX          0x1008      //通知信息
#define     FILE_TECHNIC_EX         0x1010      //服务端技术指标文件    
#define     FILE_SOFTWARE_EX        0x2000      //升级软件      

#define		FILE_TYPE_RES			-1			// 保留

// 消息子类型
#define		News_Sha_Ex				 2			// 上证消息
#define		News_Szn_Ex				 4			// 深证消息
#define		News_Fin_Ex				 6			// 财经报道
#define		News_TVSta_Ex			 8			// 电视台通知
#define     News_SzEb_Ex            10          // 深圳二板
#define		News_Unknown_Ex			 -1			// 未知提供者

//Definition For nInfo of Function GetStockDrvInfo(int nInfo,void * pBuf);
#define		RI_IDSTRING				1			// 厂商名称,返回(LPCSTR)厂商名
#define		RI_IDCODE				2			// 卡号
#define		RI_VERSION				3			// 驱动程序版本
#define		RI_ERRRATE				4			// 取信道误码
#define		RI_STKNUM				5			// 取上市股票总家数
#define		RI_SUPPORTEXTHQ			 6

#define		RI_PanKouSUPPORT   0x100     // 支持盘口数据　　　　　　　　　　网际风新增　驱动是否支持盘口数据
#define		RI_DATAExtSUPPORT1 0x101     // 支持扩展数据传输 内嵌通视协议   V3.62新增　扩充数据以内嵌方式提供（存放在品种名称的最后8字节）行情大小仍为158字节 代码名称空间从16汉字变成12汉字
#define		RI_DATAExtSUPPORT2 0x102

#define     STKPINGYING_LEN          6

//////////////////////////////////////////////////////////////////////////////////
//宽带版
//========================数据结构==================================
//----------------------------------------系统运行参数结果-----------------------------------------------
typedef  struct  tagRCV_SYSTEMRUN_STRUCTEx
{
	BYTE  m_btBargainingTime[3];      //开盘分钟数 
	BYTE  m_btTodayInitialize;        //大盘是否初始化 
	BYTE  m_btCloseWork;              //大盘是否收盘  
	long  m_BargainingDate;           //交易日期  
	float   m_fNewIndex[3];           //最新指数 
	float   m_fVolume[3];             //最新成交量
	float   m_fAmount[3];             //最新成交金额
	float   m_fRedGreenArmy[3];       //大盘红绿军
}RCV_SYSTEMRUN_STRUCTEx;
//----------------------------------------客户登入授权返回确认包-------------------------------------
typedef struct tagRCV_LOGINAUTH_STRUCTEx
{
	BOOL  bAuthorizationState;              //授权状态 
	DWORD dwAuthorizationPurview;         //授权后程序运行范围;如没有将为0；          
}RCV_LOGINAUTH_STRUCTEx;

//----------------------------------------股票代码表-------------------------------------------------------
typedef  struct  tagRCV_STKLABEL_STRUCTEx
{
	WORD m_wMarket;                   //股票市场类型
	WORD m_wPos;                      //在这一市场类型中股票排列位置 
	char m_szLabel[STKLABEL_LEN];      //股票代码
	char m_szName[STKNAME_LEN];       //股票名称
	char m_szPingYing[STKPINGYING_LEN]; //股票名称拼音
	float m_fLastClose;                    //昨收
	float m_fAverageVolumn;               //5天均量 
}RCV_STKLABEL_STRUCTEx;
//-----------------------------------大盘红绿军与领先指数数据结构------------------------
typedef struct tagRCV_INDEXATTRIBUTE_STRUCTEx
{
	__time32_t m_time;         //          分钟时间    
    short sec5;             //          加权指数值    
    short rp;               //          多空指标中RP值
    short dp;               //          多空指标中DP值
	short rsn;              //          红绿军 
	short dnn;
}RCV_INDEXATTRIBUTE_STRUCTEx;
//----------------------------------股票财务数据-----------------------------------------------------------
typedef  struct tagRCV_BASEINFO_STRUCTEx
{
	char m_szLabel[STKLABEL_LEN];  //股票代码
	float m_fZgb;                    //总股本(万股)
	float m_fGjg;                    //国家股(万股)
	float m_fFqrfrg;                  //发起人法人股(万股)
	float m_fFrg;                    //法人股(万股)
	float m_fZgg;                   //职工股(万股)
	float m_fGzAg;                  //公众A股(万股)
	float m_fLtAg;                  //流通A股(万股)
	float m_fHg;                    //Ｈ股(万股)
	float m_fBg;                    //B股(万股)
	float m_fZpg;                   //转配股(万股)
	float m_fZzc;                   //总资产(万元)
	float m_fLdzc;                  //流动资产(万元)
	float m_fCqtz;                  //长期投资(万元)
	float m_fGdzc;                  //固定资产(万元)
	float m_fWxzc;                 //无形资产(万元)
	float m_fLdfz;                  //流动负债(万元)
	float m_fCqfz;                 //长期负债(万元)
	float m_fGgqy;                 //股东权益(万元)
	float m_fZbgjj;                 //资本公积金(万元)
	float m_fYygjj;                 //盈余公积金(万元)
	float m_fMgjz;                 //每股净值(元)
	float m_fGdqybl;               //股东权益比率(%)
	float m_fMggjj;                //每股公积金(元)
	float m_fZyywsr;               //主营业务收入(万元)
	float m_fZyywlr;               //主营业务利润(万元)
	float m_fQtywlr;               //其它业务利润(万元)
	float m_fLrze;                 //利润总额(万元)
	float m_fJlr;                  //净利润(万元)
	float m_fWfplr;               //未分配利润(万元)
	float m_fMgsy;                //每股收益(元)
	float m_fJzcsyl;                //净资产收益率(%)
	float m_fMgwfplr;              //每股未分配利润(元)
	float m_fMgjzc;                //每股净资产(元)
}RCV_BASEINFO_STRUCTEx;
//////////////////////////////////////////////////////////////////////////////////
//补充数据头
//数据头 m_dwHeadTag == EKE_HEAD_TAG 
#define EKE_HEAD_TAG	0xffffffff

typedef struct	tagRCV_EKE_HEADEx
{
	DWORD	m_dwHeadTag;								// = EKE_HEAD_TAG
	WORD	m_wMarket;									// 市场类型
	char	m_szLabel[STKLABEL_LEN];					// 股票代码
}RCV_EKE_HEADEx;

//-------------------------------------补充历史五分钟K线数据----------------------------------------
typedef union tagRCV_HISMINUTE_STRUCTEx
{
	struct
	{
		__time32_t  m_time;                  //UCT
		float  m_fOpen;			          //开盘
		float	m_fHigh;			          //最高
		float  m_fLow;                    //最低
		float  m_fClose;                   //收盘
		float  m_fVolume;                 //量
		float  m_fAmount;                 //额
		float   m_fActiveBuyVol;          //主动买量如没有计算m_fActiveBuyVol=0
	};
	RCV_EKE_HEADEx  m_head;
}RCV_HISMINUTE_STRUCTEx;
//------------------------------------------------补明细数据-----------------------------------------------
typedef  union  tagRCV_DISPBARGAINING_STRUCTEx
{
	struct
	{
		__time32_t m_time;                      //成交时间 
		float  m_fNewPrice;                 // 最新
		float  m_fVolume;                  // 成交量,以股为单位,股的含义依股票类型定义
		float  m_fAmount;                  //成交额,以元为单位
		float  m_fBuyPrice[3];              // 申买价1,2,3
		float  m_fBuyVolume[3];            // 申买量1,2,3
		float  m_fSellPrice[3];              // 申卖价1,2,3
		float m_fSellVolume[3];             // 申卖量1,2,3
	};
	RCV_EKE_HEADEx  m_head;
}RCV_DISPBARGAINING_STRUCTEx;
//-----------------------------补充除权数据结构---------------------------------------------------------
typedef union tagRCV_POWER_STRUCTEx
{
	struct
	{
		__time32_t m_time;             //UCT
		float m_fGive;             //每股送  
		float m_fPei;              //每股配   
		float m_fPeiPrice;          //配股价   
		float m_fProfit;            //每股红利 
	};
	RCV_EKE_HEADEx  m_head;
}RCV_POWER_STRUCTEx;

#pragma pack(push)
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//行情数据
//注:
//		某些字段可能为0,用上次的数据代替

typedef struct tagRCV_REPORT_STRUCTEx
{
	WORD	m_cbSize;									// 结构大小
	__time32_t	m_time;										// 交易时间
	WORD	m_wMarket;									// 股票市场类型
	char	m_szLabel[STKLABEL_LEN];					// 股票代码,以'\0'结尾
	char	m_szName[STKNAME_LEN-8];						// 股票名称,以'\0'结尾
	float   m_jsj;//期货结算价
	DWORD	m_fTickAll;//总笔数。

	float	m_fLastClose;								// 昨收
	float	m_fOpen;									// 今开
	float	m_fHigh;									// 最高
	float	m_fLow;										// 最低
	float	m_fNewPrice;								// 最新
	float	m_fVolume;									// 成交量,以股为单位,股的含义依股票类型定义
	float	m_fAmount;									// 成交额,以元为单位

	float	m_fBuyPrice[3];								// 申买价1,2,3
	float	m_fBuyVolume[3];							// 申买量1,2,3
	float	m_fSellPrice[3];							// 申卖价1,2,3
	float	m_fSellVolume[3];							// 申卖量1,2,3


	float	m_fBuyPrice4;								// 申买价4
	float	m_fBuyVolume4;								// 申买量4
	float	m_fSellPrice4;								// 申卖价4
	float	m_fSellVolume4;								// 申卖量4

	float	m_fBuyPrice5;								// 申买价
	float	m_fBuyVolume5;								// 申买量
	float	m_fSellPrice5;								// 申卖价
	float	m_fSellVolume5;								// 申卖量
	float   m_jsj2;//期货结算价
	DWORD	m_fTickAll2;//总笔数。
} RCV_REPORT_STRUCTEx;


typedef struct tagRCV_REPORT_STRUCTEx2
{
	WORD	m_cbSize;									// 结构大小
	__time32_t	m_time;										// 交易时间
	WORD	m_wMarket;									// 股票市场类型
	char	m_szLabel[STKLABEL_LEN];					// 股票代码,以'\0'结尾
	char	m_szName[STKNAME_LEN-8];						// 股票名称,以'\0'结尾
	float   m_jsj;//期货结算价
	DWORD	m_fTickAll;//总笔数。

	float	m_fLastClose;								// 昨收
	float	m_fOpen;									// 今开
	float	m_fHigh;									// 最高
	float	m_fLow;										// 最低
	float	m_fNewPrice;								// 最新
	float	m_fVolume;									// 成交量,以股为单位,股的含义依股票类型定义
	float	m_fAmount;									// 成交额,以元为单位

	float	m_fBuyPrice[3];								// 申买价1,2,3
	float	m_fBuyVolume[3];							// 申买量1,2,3
	float	m_fSellPrice[3];							// 申卖价1,2,3
	float	m_fSellVolume[3];							// 申卖量1,2,3


	float	m_fBuyPrice4;								// 申买价4
	float	m_fBuyVolume4;								// 申买量4
	float	m_fSellPrice4;								// 申卖价4
	float	m_fSellVolume4;								// 申卖量4

	float	m_fBuyPrice5;								// 申买价
	float	m_fBuyVolume5;								// 申买量
	float	m_fSellPrice5;								// 申卖价
	float	m_fSellVolume5;								// 申卖量
	float   m_jsj2;//期货结算价
	DWORD	m_fTickAll2;//总笔数。
	double  m_dQhMoney;//期货总成交金额
	float   m_ztj;
	float   m_dtj;
} RCV_REPORT_STRUCTEx2;


typedef struct tagRCV_REPORT_STRUCTEx3
{
	WORD	m_cbSize;									// 结构大小
	__time32_t	m_time;										// 交易时间
	WORD	m_wMarket;									// 股票市场类型
	char	m_szLabel[STKLABEL_LEN];					// 股票代码,以'\0'结尾
	char	m_szName[STKNAME_LEN-8];						// 股票名称,以'\0'结尾
	float   m_qh_pre_spj;//期货结算价，昨收
	float	m_qh_pre_cc;//昨持仓

	float	m_qh_pre_jsj;								// 昨收，期货是昨结算
	float	m_fOpen;									// 今开
	float	m_fHigh;									// 最高
	float	m_fLow;										// 最低
	float	m_fNewPrice;								// 最新
	float	m_fVolume;									// 成交量,以股为单位,股的含义依股票类型定义
	float	m_fZcc;									// 成交额,以元为单位，总持仓

	float	m_fBuyPrice[3];								// 申买价1,2,3
	float	m_fBuyVolume[3];							// 申买量1,2,3
	float	m_fSellPrice[3];							// 申卖价1,2,3
	float	m_fSellVolume[3];							// 申卖量1,2,3


	float	m_fBuyPrice4;								// 申买价4
	float	m_fBuyVolume4;								// 申买量4
	float	m_fSellPrice4;								// 申卖价4
	float	m_fSellVolume4;								// 申卖量4

	float	m_fBuyPrice5;								// 申买价
	float	m_fBuyVolume5;								// 申买量
	float	m_fSellPrice5;								// 申卖价
	float	m_fSellVolume5;								// 申卖量
	float   m_qh_jsj;//期货今天结算价
	DWORD	m_fTickAll2;//总笔数。，昨持仓
	double  m_dQhMoney;//期货总成交金额
	float   m_ztj;
	float   m_dtj;
	float   m_fClosePrice;//今天收盘价
} RCV_REPORT_STRUCTEx3;

//============================旧结构==============
typedef struct tagRCV_REPORT_STRUCTEx_OLD
{
	WORD	m_wMarket;									// 股票市场类型
	char	m_szLabel[STKLABEL_LEN];					// 股票代码,以'\0'结尾
	char	m_szName[STKNAME_LEN];						// 股票名称,以'\0'结尾
	
	float	m_fLastClose;								// 昨收
	float	m_fOpen;									// 今开
	float	m_fHigh;									// 最高
	float	m_fLow;										// 最低
	float	m_fNewPrice;								// 最新
	float	m_fVolume;									// 成交量,以股为单位,股的含义依股票类型定义
	float	m_fAmount;									// 成交额,以元为单位

	float	m_fBuyPrice[3];								// 申买价1,2,3
	float	m_fBuyVolume[3];							// 申买量1,2,3
	float	m_fSellPrice[3];							// 申卖价1,2,3
	float	m_fSellVolume[3];							// 申卖量1,2,3
} RCV_REPORT_STRUCTEx_OLD;
//============================旧结构===================


#pragma pack(pop)

//--------------------------------宽带行情数据结构-------------------------------------------------------
typedef struct tagRCV_WIDOFEREPORT_STRUCTEx
{
	RCV_REPORT_STRUCTEx *pMarketReport;
	float m_fActiveBuyVolumn;   //主动买入量  
	float m_fConsignTotalBuyVolumn;  //委托总买量 
	float m_fConsignTotalSellVolumn; //委托总卖量
}RCV_WIDOFEREPORT_STRUCTEx;


//补充日线数据
//	注:
//		每一数据结构都应通过 m_time == EKE_HEAD_TAG,判断是否为 m_head,然后再作解释
typedef union tagRCV_HISTORY_STRUCTEx
{
	struct
	{
		__time32_t	m_time;				//UCT
		float	m_fOpen;			//开盘
		float	m_fHigh;			//最高
		float	m_fLow;				//最低
		float	m_fClose;			//收盘
		float	m_fVolume;			//量
		float	m_fAmount;			//额
		union
		{
			struct
			{
				WORD  m_wAdvance;    //涨数,仅大盘有效
				WORD  m_wDecline;     //跌数,仅大盘有效
			};
			float   m_fActiveBuyVol;      //主动买量
		};
	};
	RCV_EKE_HEADEx	m_head;
}RCV_HISTORY_STRUCTEx;

//补充分时线数据
//	注:
//		每一数据结构都应通过 m_time == EKE_HEAD_TAG,判断是否为 m_head,然后再作解释
typedef union tagRCV_MINUTE_STRUCTEx
{
	struct{
		__time32_t	m_time;				// UCT
		float	m_fPrice;
		float	m_fVolume;
		float	m_fAmount;
	};
	RCV_EKE_HEADEx	m_head; 
}RCV_MINUTE_STRUCTEx;

//////////////////////////////////////////////////////////////////////////////////
// 文件类型数据包头
//  注一:
//	m_wDataType == FILE_BASE_EX
//				m_dwAttrib = 股票证券市场,m_szFileName仅包含文件名
//				m_FileTime = 基本面资料文件创建日期
//  m_wDataType == FILE_NEWS_EX
//				m_dwAttrib = 消息来源,m_szFileName 包含目录的文件名,目录名为消息来源
//				m_dwSerialNo = 序列号
//				如:  "上交所消息\\0501Z012.TXT","新兰德\\XLD0001.TXT"
//  注二:
//		数据文件循环播出,每个文件有唯一的序列号,以避免重复接收
typedef struct tagRCV_FILE_HEADEx
{
	DWORD	m_dwAttrib;							// 文件子类型
	DWORD	m_dwLen;							// 文件长度
	union {
		DWORD	m_dwSerialNo;					// 序列号,对股评
		__time32_t	m_FileTime;						// 文件创建时间
	};
	char	m_szFileName[_MAX_PATH];			// 文件名 or URL
} RCV_FILE_HEADEx;

//////////////////////////////////////////////////////////////////////////////////
//  数据通知消息
//	wParam = RCV_WPARAM;
//  lParam 指向 RCV_DATA结构;
//	返回 1 已经处理, 0 未处理或不能处理,目前该返回值被忽略

#define RCV_REPORT			0x3f001234
#define RCV_FILEDATA		0x3f001235
#define RCV_PANKOUDATA		0x3f001258

//level2扩展
#define RCV_LVLDETAILTICK		0x3f002000//逐笔数据，扩展的
#define RCV_LVLORDERQUEUE		0x3f002001//上海委托队列


//这个包也通过RCV_REPORT来传输!!!
typedef struct tagRCV_LEVELTICK
{
	WORD	m_wMarket;									// 股票市场类型　　　　　　　　　　 同通视规范定义
	char	m_szLabel[STKLABEL_LEN];					// 股票代码,以'\0'结尾
	DWORD   m_trade_price;//成交价格
	DWORD m_tradetime;
	long long m_trade_qty;
	long long m_trade_money;
	DWORD   m_szext;//深圳扩展的消息。
}RCV_LEVELTICK;

typedef struct tagRCV_LEVELORDERQUEUE
{
	WORD	m_wMarket;									// 股票市场类型　　　　　　　　　　 同通视规范定义
	char	m_szLabel[STKLABEL_LEN];					// 股票代码,以'\0'结尾
	float buycount[50];
	float sellcount[50];
}RCV_LEVELORDERQUEUE;

// 注一:
//	  记录数表示行情数据和补充数据(包括 Header)的数据包数,对文件类型数据, = 1
// 注二:
//	  若 m_bDISK = FALSE, m_pData 为消息股评等数据缓冲区指针
//
//		 ******** 数据共享,不能修改数据 **********
//
//		 m_bDISK = TRUE,  m_pData 为该文件的存盘文件名,一般情况只有
//		 升级软件等大文件用存盘方式
typedef struct tagRCV_DATA
{
	int					m_wDataType;			// 文件类型
	int					m_nPacketNum;			// 记录数,参见注一
	RCV_FILE_HEADEx		m_File;					// 文件接口
	BOOL				m_bDISK;				// 文件是否已存盘的文件
	union
	{
		RCV_REPORT_STRUCTEx *m_pReport;                   //行情数据
		RCV_WIDOFEREPORT_STRUCTEx *m_pWideReport;         //宽带版行情数据 
		RCV_HISTORY_STRUCTEx *m_pDay;                     //日线数据 
		RCV_MINUTE_STRUCTEx *m_pMinute;                   //补当天分时数据   
		RCV_POWER_STRUCTEx *m_pPower;                     //除权数据
		RCV_DISPBARGAINING_STRUCTEx *m_pDispBargaining;  //明细数据
		RCV_BASEINFO_STRUCTEx *m_pBaseInfo;               //财务数据
		RCV_STKLABEL_STRUCTEx *m_pStkLabel;               //代码对照表
		RCV_HISMINUTE_STRUCTEx *m_pHisMinute;             //补历史五分钟线
		RCV_SYSTEMRUN_STRUCTEx *m_pSystemRun;            //系统运行参数
		RCV_INDEXATTRIBUTE_STRUCTEx *m_pIndexAttribute;   //大盘红绿军 
		RCV_LOGINAUTH_STRUCTEx *m_pLoginAuth;             //返回授权状态   
		RCV_LEVELTICK *m_pLevelTick;
		RCV_LEVELORDERQUEUE *m_pLevelOrderQueue;
		void *m_pData;                                    //文件内存指针   
	};
} RCV_DATA;

//==================同网络通讯层接口总数据到达结构=====================
typedef struct RCV_COMMINTERFACE_STRUCTEx
{
	RCV_DATA *m_pRcvData;                       //行情传送数据
	DWORD m_dwTransEnvironment;                 //行情传送环境数据 
} RCV_COMMINTERFACE_STRUCTEx;

////////////////////////////////////////////////////////
//客户端应用层数据请求部分
//
typedef enum {
	SHZS=0,                           //上证指数 
    SHAG=1,                             //上证A股 
	SHBG=2,                             //上证B股 
	SZZS=3,                              //深圳指数
	SZAG=4,                              //深圳A股
	SZBG=5,                              //深圳B股
	SHZQ=6,                              //上证债券
	SZZQ=7,                              //深圳债券
	SHJIJIN=8,                              //上证jijin 
	SZJIJIN=9,                              //深证jijin 
	EBZS=10,                              //二板指数   
	EBAG=11,                              //二板股票
	STKTYPE=12,                          //板块指数  
	CHOOSESTK=20,                           //自选股 
	QBGP=30                              //全部股票
};
//-----------------------------------客户端请求类型定义-------------------------------------------------
typedef enum ClientTransmitAttributes{
	LoginAuthorization=0,            //登入授权 
    SystemRunAttribute,              //系统运行属性  
	InitStockCodeList,               //取得初始化全部股票代码数据   
    IndexRealtimeData,               //取得某一大盘指数即时行情数据1A0001、2A01   
	StockRealtimeData,               //取得某一股票即时行情数据
	StockMinuteData,                 //取得某一股票当天分钟数据
    StockRealtimeDataKLine,          //取得某一股票在K线界面即时行情数据
	StockDisPartData,                //取得某一股票分笔明细数据
    StockTypesBaseRealTimeData,      //取得某一类股票基本行情数据 某一类股票全部最高、最低、开盘数据  
    StockTypesRealTimeMarketData,    //取得某一类股票行情数据   某一类股票全部最新、成交量、金额
    StockTypesRealTimeMarketWithMinute,      //取得某一类股票分时行情数据   某一分钟全部股票分时数据
	HistoryDayLineData,              //用于表示历史日线数据 
	HistoryMinuteLineData,           //用于表示历史分时数据 
	HistoryDisPartBargainingData,    //用于表示历史明细数据
	WideExRightData,                     //用于表示除权数据
	WideFinancingData,                   //用于表示财务数据
	WideStockBaseInfo,                   //用于表示个股资料
    WideShInfomation,                    //用于表示上证公告
	WideSzInfomation,                    //用于表示深证公告  
    WideEbInfomation,                    //用于表示二板公告  
	WideZjInfomation,                    //用于表示财经报道
	WideTechnicIndex,                    //用于表示技术指标 
	WideNotifyInfomation                 //用于表示通知信息
};
typedef struct tagTransmitStockAttribute
{
	WORD m_wMarket;                                  //股票市场类型
	char m_szTransmitStockLabel[8];                  //传送股票代码
	DWORD m_dwTransmitCounts;                        //表示记录数     
}TransmitStockAttribute, *PTransmitStockAttribute;   //传送股票属性指针
//=====================客户端请求数据结构============================
typedef struct tagTRANS_PACKAGE_STRUCTEx
{
	ClientTransmitAttributes  m_TransmitType;    //客户请求数据包类型  
	DWORD m_dwTransmitStockType;                 //用于表示板块类型  
	union
	{
		struct
		{
			TransmitStockAttribute *m_pTransmitStockAttribute;
			DWORD m_dwTransmitStockCount;                        //表示传送股票数
		};
		DWORD m_dwTransmitStockTypeCounts;                       //表示记录数,用于板块
	};
} TRANS_PACKAGE_STRUCTEx;

//////////////////////////////////////////////////////////////////////////////////
//APIs
#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////////////
// 注册函数

// 股票初始化
// 入口参数:
//		hWnd			处理消息的窗口句柄
//		Msg				用户自定义消息
//		nWorkMode		接口工作方式,目前只能为 RCV_WORK_MEMSHARE
// 返回参数:
//		 1				成功
//		-1				失败
// 注:
//		注册后,驱动程序会向处理窗口发送消息; 若不注册,通过查询方式亦可取得数据
//		若股票接收没启动,则启动股票接收程序
int WINAPI	Stock_Init(HWND hWnd,UINT Msg,int nWorkMode);

// 退出,停止发送消息
// 入口参数:
//		hWnd			处理消息的窗口句柄,同 Stock_Init 的调用入口参数
//	返回参数:
//		 1				成功	
//		-1				失败
int WINAPI 	Stock_Quit(HWND hWnd);

//////////////////////////////////////////////////////////////////////////////////
// 行情接口       
      
// 取已接收到的股票总数
int WINAPI 	GetTotalNumber();													

// 由序号取股票数据(扩展)
// 入口参数:	
//			nNo			序号
//			pBuf		存放股票数据的缓冲区
// 返回参数:
//		    NoStockData	无股票数据	
// 注:
//			该函数提供股票数据的主要数据;分析软件刚运行时,可以快速建立数据框架
int WINAPI 	GetStockByNoEx(int nNo,RCV_REPORT_STRUCTEx * pBuf);			

// 由股号取股票数据(扩展)
// 入口参数:	
//			pszStockCode股票代号	
//			nMarket		证券市场
//			pBuf		存放股票数据的缓冲区
// 返回参数:
//		    NoStockData	无股票数据	
// 注:
//			该函数提供股票数据的主要数据;分析软件刚运行时,可以快速建立数据框架
int WINAPI 	GetStockByCodeEx(char * pszStockCode,int nMarket,RCV_REPORT_STRUCTEx * pBuf);

// 激活接收程序,进行设置
// 入口参数:
//			bSetup		TRUE		显示窗口,进行设置
// 返回参数:
//			 1			成功
//			-1			失败
int	WINAPI  SetupReceiver(BOOL bSetup);

//	取得股票驱动信息
//	入口参数:
//			nInfo		索引
//			pBuf		缓冲区
//	出口参数:
//			nInfo == RI_IDSTRING,	返回特征字符串长度, pBuf 为特征字符串
//									如:	"TongShi_StockDrv_1.00"
//			nInfo == RI_IDCODE,		返回信息卡 ID 号, pBuf 为字符串形式的 ID 号
//									如:	0x78001234	  "78001234"
//			nInfo == RI_VERSION,	返回信息卡版本号, pBuf 为字符串版本
//									如:  1.00		  "1.00"		
//			nInfo == RI_ERRRATE,	取信道误码,
//			nInfo == RI_STKNUM,		取上市股票总家数
DWORD WINAPI  GetStockDrvInfo(int nInfo,void * pBuf);

#ifdef __cplusplus
}
#endif


/*******************************及时雨扩展通视卡接口部分*******************

    扩展原则：完全兼容通视卡的功能，保证原有功能正常工作，基于个股请求数据
    扩展项目：主动取分时数据，
              主动取分笔数据
              取股票代码
              取F10
    为实现扩展接口的功能，在处理时注意以下几点， 
    1>扩展部分的数据到位通知通过消息DA_SERVICE_MSG_EXT（或通过SetExtMsg自定义）通知到达。
    2>数据通过消息通知到达，数据结构为STRUCT_REXT_HEAD＋n个数据结构单元
    -----------------------------
    |    STRUCT_REXT_HEAD       |
    -----------------------------
    |                           |
    |       扩展数据结构        |
    |                           |
    -----------------------------
***************************************************************************/

#define DA_SERVICE_MSG_EXT      WM_APP + 255    //扩展接口
#define	DA_SERVICE_MSG			WM_APP + 256  

#define RCV_EXT_TRADEDATA   0X5A11      //带买卖3的分笔数据  STCP_QBD_STRUCT
#define RCV_EXT_KDATA       0X5A21      //个股日线数据  RITK_STRUCT 
#define RCV_EXT_TRADEDATAV2 0X5A15      //带买卖4的分笔数据, STCP_QBD6_STRUCT
#define RCV_EXT_STOCKTABLE  0x5A22      //股票代码表    RCV_STOCK_CELLEx


typedef struct _REXT_HEAD_
{
    TCHAR       m_Code[STKLABEL_LEN];
    TCHAR       m_Name[STKLABEL_LEN];
    WORD        m_wMarket;  // 市场类型
    WORD        m_Service;  //  类型如下
                            /*
                            #define RCV_EXT_TRADEDATA   0X5A11      //带买卖3的分笔数据  STCP_QBD_STRUCT
                            #define RCV_EXT_KDATA       0X5A21      //个股日线数据  RITK_STRUCT 
                            #define RCV_EXT_TRADEDATAV2 0X5A15      //带买卖4的分笔数据, STCP_QBD6_STRUCT
                            #define RCV_EXT_STOCKTABLE  0x5A22      //股票代码表    RCV_STOCK_CELLEx
                            */
    int         m_num;  //交易笔数
    __time32_t      m_mt;   //交易时间
    DWORD       m_unused[4];
}STRUCT_REXT_HEAD;

typedef struct _STCP_QBD_
{
    __time32_t          fTime ;             //时间(精确到秒)fTime       
    float           fPrice ;            //当前成交价         fPrice     
    float           fVolumn ;           //总成交量           fVolumn    
    float           fAmount;            //总成交额           fAmount    
    float           fBuyP[3];           //申买价             fBuyP[3]   
    float           fBuyV[3];           //申买量              
    float           fSellP[3];          //申卖价             
    float           fSellV[3];          //申卖量
} STCP_QBD_STRUCT ;       // 

typedef struct _STCP_QBD6_
{
    __time32_t          fTime ;                   //时间(精确到秒)fTime       
    float           fPrice ;            //当前成交价         fPrice     
    float           fVolumn ;           //总成交量           fVolumn    
    float           fAmount;            //总成交额           fAmount    
    float           fBuyP[4];           //申买价             fBuyP[3]   
    float           fBuyV[4];           //申买量              
    float           fSellP[4];          //申卖价             
    float           fSellV[4];          //申卖量
} STCP_QBD6_STRUCT ;       //


//K线数据结构
typedef struct _RITK_
{
    __time32_t  m_time;             //UCT   
    float   m_fOpen;            //开盘
    float   m_fHigh;            //最高
    float   m_fLow;             //最低
    float   m_fClose;           //收盘
    float   m_fVolume;          //量
    float   m_fAmount;          //额
}RITK_STRUCT ;  



//股票代码表单元结构：
typedef  struct tagRCV_STOCK_CELLEx
{
    WORD    m_wMarket;              // 股票市场类型
    char    m_szLabel[STKLABEL_LEN];// 股票代码,以'\0'结尾
    char    m_szName[STKNAME_LEN];  // 股票名称,以'\0'结尾
}RCV_STOCK_CELLEx ;

//////////////////////////////////////////////////////////////////////////////////
//APIs

#ifdef __cplusplus
extern "C"{
#endif

/************************设置扩展消息标志********************
******************************************************/
int WINAPI SetExtMsg(UINT msg);


/************************取个股K线数据********************
    scode: 股票代码， 以'\0'结尾
    mt   : 日线数据起始交易日， 时间精度到天，小时、分不计， mt ＝0表示取所有日线
    返回 : 1    成功
           -1   失败
    日线数据以通视卡的格式返回
******************************************************/
int WINAPI GetKData(LPCSTR scode, __time32_t mt);

int WINAPI QueryKData(LPCSTR scode, int nMarket, __time32_t mt);
/************************取分时数据********************
    scode: 股票代码， 以'\0'结尾
    mt   : 分时数据所属底交易日， 时间精度到天，小时、分不计
    返回 : 1    成功
           -1   失败
    分时数据以通视卡的格式返回
******************************************************/
int  WINAPI GetMinData(LPCSTR scode, __time32_t mt);


int  WINAPI QueryMinData(LPCSTR scode, int nMarket, __time32_t mt);
/************************取交易分笔数据********************
    scode: 股票代码， 以'\0'结尾
    mt   : 分时数据所属底交易日， 时间精度到天，小时、分不计
    返回 : 1    成功
           -1   失败
    分笔数据格式见下面, 当数据准备好后，通过消息DA_SERVICE_MSG_EXT通知数据已准备好。

******************************************************/
int  WINAPI GetTradeData(LPCSTR scode, __time32_t mt);

int  WINAPI QueryTradeData(LPCSTR scode, int nMarket, __time32_t mt);

/************************取新闻公告资料********************
    channel: 新闻频道， 以'\0'结尾
    fmt: 保留，目前无效
    返回 : 1    成功
           -1   失败
        
**********************************************************/  
int  WINAPI GetNews(LPCSTR  channel = NULL,  __time32_t fmt =0 );

/************************取个股基本面资料********************
    scode: 股票代码， 以'\0'结尾
    fmt  : 保留
    返回 : 1    成功
           -1   失败
        
**********************************************************/  
int  WINAPI GetBaseInfo(LPCSTR  scode,  __time32_t fmt);

int  WINAPI QueryBaseInfo(LPCSTR scode, int nMarket, __time32_t mt);

/***********************取所有股票代码***********************
     通过消息返回代码表
     STRUCT_REXT_HEAD   *pHead  pHead->m_Service == RCV_EXT_STOCKTABLE ;
     RCV_STOCK_CELLEx   * pBuf ;
     
*************************************************************/
int  WINAPI GetStockTable();

/***********************取服务器时间***********************
     用于客户端与服务器端时间同步
     返回：TRUE 成功
           FALSE 失败
*************************************************************/
BOOL  WINAPI GetServerTime(__time32_t *ptime);

#ifdef __cplusplus
}
#endif

#pragma pack(pop)


//网际风扩展协议
#pragma pack(push,1)
typedef struct tagRCV_PANKOU_STRUCTEx
{
	__time32_t  m_time;                              // 与动态行情一致
    float m_fHigh;                       //      { 最高 }
    float m_fLow;                        //     { 最低 }
    float m_fNewPrice;                   //      { 最新 }
    float m_fVolume;                     //      { 成交量 }
    float m_fAmount;                     //      { 成交额 }
    DWORD m_lStroke;                     //     { 保留   }
    float m_fBuyPrice[5];          //{ 申买价1,2,3,4,5 }
    float m_fBuyVolume[5];         //{ 申买量1,2,3,4,5 }
    float m_fSellPrice[5];        // { 申卖价1,2,3,4,5 }
    float m_fSellVolume[5];       // { 申卖量1,2,3,4,5 }
    float m_fAvgPrice;        //                  // 期货结算价 对国内期货市场有效
	DWORD m_fTickAll;                            // 成交总笔数(交易所发布) 最字节

} RCV_PANKOU_STRUCTEx;


#pragma warning(disable:4200)
typedef struct tagRCV_WJFPANKOU
{
	WORD	m_wMarket;									// 股票市场类型　　　　　　　　　　 同通视规范定义
	WORD	m_wStkIdx;									// 股票在该市场的码表索引2字节      V3.62 版本修改字段
	char	m_szLabel[STKLABEL_LEN];					// 股票代码,以'\0'结尾
	DWORD	m_lDate;								// FORMAT: 20010305,0表示当天或最近交易日的明细数据
	float m_fLastClose;//昨收
	float m_fOpen ;//今开
	DWORD m_nAllCount; // 总盘口行情数
	DWORD m_nCount;// 当前块笔数        V3.62 版本从双字节扩充到4字节
	WORD RO;//保留字段
	RCV_PANKOU_STRUCTEx m_Data[0];
} RCV_WJFPANKOU; 



#pragma pack(pop)




