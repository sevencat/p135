#include "stdafx.h"
#include "codemap.h"


//SG5120    伦敦金现              1649.687   1650.007   1650.350       7898             0   1650.100   1650.600      0.000        0 
//SG5121    伦敦银现                31.705     31.685     31.700       3335             0     31.690     31.710      0.000        0 
//SG5122    伦敦铂                1583.750   1582.375   1588.250       1794             0   1585.500   1591.000      0.000        0 
//SG5123    伦敦钯                 662.500    661.900    664.000        670             0    661.000    667.000      0.000        0


//NQ020501  棕榈1301              6992.000   7044.000   7048.000     124650        297980   7046.000   7048.000   7054.000        0 
//NQ020502  棕榈1302              7140.000   7164.000   7190.000         10            12   7134.000   7216.000   7212.000        0 
//NQ020503  棕榈1303              7170.000      0.000      0.000          0            16   7184.000   7272.000      0.000        0 
//NQ020504  棕榈1304              7228.000      0.000      0.000          0             2   7168.000   7296.000      0.000        0 
//NQ020505  棕榈1305              7286.000   7316.000   7304.000      57130        239614   7302.000   7304.000   7316.000        0 
//NQ020506  棕榈1306              7358.000      0.000      0.000          0            32   7312.000   7438.000      0.000        0 
//NQ020507  棕榈1307              7300.000      0.000      0.000          0             6   7324.000   7416.000      0.000        0 
//NQ020508  棕榈1308              7328.000      0.000      0.000          0             2   7300.000   7444.000      0.000        0 
//NQ020509  棕榈1309              7374.000   7412.000   7410.000         32          1178   7390.000   7430.000   7404.000        0 
//NQ020510  棕榈1210                 0.000      0.000      0.000          0             0      0.000      0.000      0.000        0 
//NQ020511  棕榈1211                 0.000      0.000      0.000          0             2      0.000      0.000      0.000        0 
//NQ020512  棕榈1212              6942.000   6976.000   6976.000          2            54   6912.000   6990.000   6976.000        0 
//NQ020520  棕榈连续                 0.000      0.000      0.000          0             0      0.000      0.000      0.000        0 
//NQ020523  棕榈连三              6992.000   7044.000   7048.000     124650        297980   7046.000   7048.000   7054.000        0 
//NQ020524  棕榈连四      
void CodeMap::init()
{
	//国内期货
	nq_code_map['0010']=std::make_pair(FUTURES_MARKET|SHANGHAI_BOURSE,"CU");
	nq_code_map['1010']=std::make_pair(FUTURES_MARKET|SHANGHAI_BOURSE,"AL");
	nq_code_map['2010']=std::make_pair(FUTURES_MARKET|SHANGHAI_BOURSE,"FU");
	nq_code_map['3010']=std::make_pair(FUTURES_MARKET|SHANGHAI_BOURSE,"ZN");
	nq_code_map['5010']=std::make_pair(FUTURES_MARKET|SHANGHAI_BOURSE,"AU");
	nq_code_map['6010']=std::make_pair(FUTURES_MARKET|SHANGHAI_BOURSE,"RB");
	nq_code_map['7010']=std::make_pair(FUTURES_MARKET|SHANGHAI_BOURSE,"RU");
	nq_code_map['8010']=std::make_pair(FUTURES_MARKET|SHANGHAI_BOURSE,"WR");
	nq_code_map['9010']=std::make_pair(FUTURES_MARKET|SHANGHAI_BOURSE,"PB");
	nq_code_map['0110']=std::make_pair(FUTURES_MARKET|SHANGHAI_BOURSE,"AG");

	nq_code_map['1020']=std::make_pair(FUTURES_MARKET|DALIAN_BOURSE,"M");
	nq_code_map['2020']=std::make_pair(FUTURES_MARKET|DALIAN_BOURSE,"Y");
	nq_code_map['3020']=std::make_pair(FUTURES_MARKET|DALIAN_BOURSE,"C");
	nq_code_map['4020']=std::make_pair(FUTURES_MARKET|DALIAN_BOURSE,"L");
	nq_code_map['5020']=std::make_pair(FUTURES_MARKET|DALIAN_BOURSE,"P");
	nq_code_map['6020']=std::make_pair(FUTURES_MARKET|DALIAN_BOURSE,"V");
	nq_code_map['7020']=std::make_pair(FUTURES_MARKET|DALIAN_BOURSE,"JT");//焦炭
	nq_code_map['8020']=std::make_pair(FUTURES_MARKET|DALIAN_BOURSE,"JM");//焦煤
	nq_code_map['0120']=std::make_pair(FUTURES_MARKET|DALIAN_BOURSE,"A");//豆一
	nq_code_map['0220']=std::make_pair(FUTURES_MARKET|DALIAN_BOURSE,"A");
	nq_code_map['0320']=std::make_pair(FUTURES_MARKET|DALIAN_BOURSE,"B");//豆二

	nq_code_map['5030']=std::make_pair(FUTURES_MARKET|ZHENGZHOU_BOURSE,"CF");//棉花
	nq_code_map['7030']=std::make_pair(FUTURES_MARKET|ZHENGZHOU_BOURSE,"TA");//PTA
	nq_code_map['8030']=std::make_pair(FUTURES_MARKET|ZHENGZHOU_BOURSE,"RO");//菜油
	nq_code_map['9030']=std::make_pair(FUTURES_MARKET|ZHENGZHOU_BOURSE,"ER");//籼稻
	nq_code_map['0130']=std::make_pair(FUTURES_MARKET|ZHENGZHOU_BOURSE,"WT");//
	nq_code_map['1130']=std::make_pair(FUTURES_MARKET|ZHENGZHOU_BOURSE,"WS");
	nq_code_map['3130']=std::make_pair(FUTURES_MARKET|ZHENGZHOU_BOURSE,"SR");
	nq_code_map['0230']=std::make_pair(FUTURES_MARKET|ZHENGZHOU_BOURSE,"WT");
	nq_code_map['1230']=std::make_pair(FUTURES_MARKET|ZHENGZHOU_BOURSE,"WS");
	nq_code_map['3230']=std::make_pair(FUTURES_MARKET|ZHENGZHOU_BOURSE,"SR");
	nq_code_map['3030']=std::make_pair(FUTURES_MARKET|ZHENGZHOU_BOURSE,"PM");//普麦
	nq_code_map['4030']=std::make_pair(FUTURES_MARKET|ZHENGZHOU_BOURSE,"ME");//甲醇

	nq_code_map['1040']=std::make_pair(FUTURES_MARKET|GUZHI_BOURSE,"IF");
	nq_code_map['2019']=std::make_pair(FUTURES_MARKET|GUZHI_BOURSE,"TF");


	//外盘期货
	//COMEX
	wq_code_map['HC']=WP_MARKET|WP_COMEX;//精铜
	wq_code_map['LG']=WP_MARKET|WP_COMEX;//黄金
	wq_code_map['LS']=WP_MARKET|WP_COMEX;//美白银
	//NYMEX
	wq_code_map['OC']=WP_MARKET|WP_NYMEX;//原油
	wq_code_map['OH']=WP_MARKET|WP_NYMEX;//燃油
	//CBOT
	wq_code_map['RC']=WP_MARKET|WP_CBOT;//玉米
	wq_code_map['BS']=WP_MARKET|WP_CBOT;//美黄豆
	wq_code_map['MS']=WP_MARKET|WP_CBOT;//豆粉，就是豆巴
	wq_code_map['OS']=WP_MARKET|WP_CBOT;//豆油
	wq_code_map['HW']=WP_MARKET|WP_CBOT;//美小麦

	//马来西亚,WP_MY,WQYP
	wq_code_map['PY']=WP_MARKET|WP_MY;//棕榈油
	
	//NYBOT
	wq_code_map['TC']=WP_MARKET|WP_NYBOT;//棉花
	wq_code_map['GS']=WP_MARKET|WP_NYBOT;//美期糖
	//LME
	wq_code_map['ML']=WP_MARKET|WP_LME;//LME金属
	//IPE
	wq_code_map['IO']=WP_MARKET|WP_IPE;//布原油
	wq_code_map['AG']=WP_MARKET|WP_IPE;//布柴油???
	//TOCOM
	wq_code_map['BR']=WP_MARKET|WP_TOCOM;//日胶

	//国外指数
	id_code_map[7129]=std::make_pair(WP_MARKET|WP_INDEX_YZ2,"NZX50");
	id_code_map[7120]=std::make_pair(WP_MARKET|WP_INDEX_YZ1,"N225");
	id_code_map[7122]=std::make_pair(WP_MARKET|WP_INDEX_YZ1,"KS11");
	id_code_map[7128]=std::make_pair(WP_MARKET|WP_INDEX_YZ1,"AORD");
	id_code_map[7185]=std::make_pair(WP_MARKET|WP_INDEX_YZ1,"KOSPI");
	id_code_map[7123]=std::make_pair(WP_MARKET|WP_INDEX_YZ1,"TWI");
	id_code_map[7124]=std::make_pair(WP_MARKET|WP_INDEX_YZ1,"STI");
	id_code_map[7125]=std::make_pair(WP_MARKET|WP_INDEX_YZ1,"KLSE");
	id_code_map[7130]=std::make_pair(WP_MARKET|WP_INDEX_YZ1,"SGXMT");
	id_code_map[7121]=std::make_pair(WP_MARKET|WP_INDEX_YZ1,"HSI0");
	id_code_map[7127]=std::make_pair(WP_MARKET|WP_INDEX_YZ1,"JKSE");
	id_code_map[7319]=std::make_pair(WP_MARKET|WP_INDEX_YZ1,"CSI");
	id_code_map[7126]=std::make_pair(WP_MARKET|WP_INDEX_YZ3,"SETI");
	id_code_map[7164]=std::make_pair(WP_MARKET|WP_INDEX_YZ3,"SENSEX");
	id_code_map[7309]=std::make_pair(WP_MARKET|WP_INDEX_OZ,"FTSE");
	id_code_map[7310]=std::make_pair(WP_MARKET|WP_INDEX_OZ,"CAC40");
	id_code_map[7312]=std::make_pair(WP_MARKET|WP_INDEX_OZ,"DAX");
	id_code_map[7313]=std::make_pair(WP_MARKET|WP_INDEX_OZ,"AEX");
	id_code_map[7314]=std::make_pair(WP_MARKET|WP_INDEX_OZ,"KFX");
	id_code_map[7315]=std::make_pair(WP_MARKET|WP_INDEX_OZ,"BEL20");
	id_code_map[7316]=std::make_pair(WP_MARKET|WP_INDEX_OZ,"SSMI");
	id_code_map[7323]=std::make_pair(WP_MARKET|WP_INDEX_OZ,"RTS");
	id_code_map[7324]=std::make_pair(WP_MARKET|WP_INDEX_OZ,"FTMIB");
	id_code_map[7301]=std::make_pair(WP_MARKET|WP_INDEX_MZ1,"DJI");
	id_code_map[7302]=std::make_pair(WP_MARKET|WP_INDEX_MZ1,"DJTA");
	id_code_map[7303]=std::make_pair(WP_MARKET|WP_INDEX_MZ1,"DJUA");
	id_code_map[7304]=std::make_pair(WP_MARKET|WP_INDEX_MZ1,"IXIC");
	id_code_map[7305]=std::make_pair(WP_MARKET|WP_INDEX_MZ1,"SP500");
	id_code_map[7306]=std::make_pair(WP_MARKET|WP_INDEX_MZ1,"CCI");
	id_code_map[7307]=std::make_pair(WP_MARKET|WP_INDEX_MZ1,"CRBRJ");
	id_code_map[7317]=std::make_pair(WP_MARKET|WP_INDEX_MZ1,"BVSP");
	id_code_map[7321]=std::make_pair(WP_MARKET|WP_INDEX_MZ1,"BDI");
	id_code_map[7322]=std::make_pair(WP_MARKET|WP_INDEX_MZ1,"SP100");
	id_code_map[7308]=std::make_pair(WP_MARKET|WP_INDEX_MZ2,"SPTSX");
}



//000001=1A0001
//000002=1A0002
//000003=1A0003
//000004=1B0001
//000005=1B0002
//000006=1B0004
//000007=1B0005
//000008=1B0006
//000010=1B0007
//000011=1B0008
//000012=1B0009
//000013=1B0010
//000016=1B0011
void CodeMap::transfer_sh_code(char *label)
{
	int prefix=*(int *)label;
	if(prefix!='0000')
		return;
	if(label[4]!='0')
		return;
	int index=(label[5]-'1');
	if(index<0)
		return;
	if(index>=8)
		return;
	const char *transfed_sh_codes[]=
	{
		"1A0001","1A0002","1A0003","1B0001","1B0002","1B0004","1B0005","1B0006"
	};
	memcpy(label,transfed_sh_codes[index],6);
}

//399001=2A01
//399002=2A02
//399003=2A03
//399106=2C01
//399107=2C02
//399108=2C03
void CodeMap::transfer_sz_code(char *label)
{
	if(label[4]!='0')
		return;
	int prefix=*(int *)label;
	if(prefix=='0993')
	{
		if(label[5]=='1')
		{
			memcpy(label,"2A01",4);
			label[4]=label[5]=0;
			return;
		}
		if(label[5]=='2')
		{
			memcpy(label,"2A02",4);
			label[4]=label[5]=0;
			return;
		}
		if(label[5]=='3')
		{
			memcpy(label,"2A03",4);
			label[4]=label[5]=0;
			return;
		}
		return;
	}

	if(prefix=='1993')
	{
		if(label[5]=='6')
		{
			memcpy(label,"2C01",4);
			label[4]=label[5]=0;
			return;
		}
		if(label[5]=='7')
		{
			memcpy(label,"2C02",4);
			label[4]=label[5]=0;
			return;
		}
		if(label[5]=='8')
		{
			memcpy(label,"2C03",4);
			label[4]=label[5]=0;
			return;
		}
		return;
	}
}

//这里是否要把000001转成1A01?
int CodeMap::transfer_code(uint16_t &market,char *label)
{
	for(int i=0;i<6;i++)
		label[i]=toupper(label[i]);

	if(market=='HS')
	{
		//transfer_sh_code(label);
		market=STOCK_MARKET|SH_BOURSE;
		return 1;
	}
	if(market=='ZS')
	{
		//transfer_sz_code(label);
		market=STOCK_MARKET|SZ_BOURSE;
		return 1;
	}
	if(market=='KH')
	{
		market=HK_MARKET|HK_BOURSE;
		return 2;
	}
	if(market=='QH')
	{
		market=HK_MARKET|HK_OPTIONS;
		return 2;
	}
	if(market=='ZH')
	{
		market=HK_MARKET|0x0300;
		return 2;
	}
	//如果是外汇，就啥也不做
	if(market=='HW')
	{
		if((label[0]=='U')&&(label[1]=='S'))
			market=FOREIGN_MARKET|WH_BASE_RATE;
		else
			market=FOREIGN_MARKET|WH_ACROSS_RATE;

		return 5;
	}
	if((market=='QN')||(market=='FS')) 
	{
		std::unordered_map<uint32_t,std::pair<uint16_t,const char *> >::iterator it=nq_code_map.find(*(uint32_t *)label);
		if(it==nq_code_map.end())
			return 0;
		std::pair<uint16_t,const char *> &curpair=it->second;
		market=curpair.first;
		const char *transedcode=curpair.second;
		if(transedcode[0]!=NULL)
		{
			label[0]=transedcode[0];
			if(transedcode[1]!=NULL)
			{
				label[1]=transedcode[1];
			}
		}
		return 3;
	}
	//这个是外盘
	if(market=='QW')
	{
		std::unordered_map<uint16_t,uint16_t >::iterator it=wq_code_map.find(*(uint16_t *)label);
		if(it==wq_code_map.end())
			return 0;
		market=it->second;
		return 4;
	}
	//国外指数
	if(market=='DI')
	{
		int wpid=atoi(label);
		if(wpid==7312)
		{
			int xxx=1;
		}
		std::unordered_map<uint32_t,std::pair<uint16_t,std::string> >::iterator it=id_code_map.find(wpid);
		if(it==id_code_map.end())
			return 0;
		std::pair<uint16_t,std::string> &curdi=it->second;
		market=curdi.first;
		{
			uint32_t *ptemp=(uint32_t *)label;
			ptemp[0]=ptemp[1]=0;
			memcpy(label,curdi.second.c_str(),curdi.second.size());
		}
		return 4;
	}

	if(market=='GS')
	{
		//这里是贵金属了
		market=WP_MARKET|WP_NOBLE_METAL;
		label[0]='G';//金属
		return 4;
	}
	return 0;
}