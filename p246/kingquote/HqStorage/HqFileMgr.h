#pragma once
#include "HqFile.h"

typedef std::map<uint32_t, HqFile*> HqFileMap;
//这个就是管理所有的
class HqFileMgr
{
public:
	HqFileMgr();
	~HqFileMgr();
	//用数组来存放？

	

	HqFile* get_by_hsid(uint32_t mktid)
	{
		if (mktid == 0)
			return NULL;
		HqFileMap::iterator it = stk_file_map.find(mktid);
		if (it == stk_file_map.end())
			return NULL;
		return it->second;
	}
	void load_cfg(const char *defdir, const std::string &rtdir, const std::string &datadir);
protected:
	void load_hqstorage_group(pugi::xml_node &nodecfg);
	HqFileMap stk_file_map;
};

