#pragma once
#include <unordered_map>
#include <stdint.h>

//我们假设key的长度最多是6！！！，正好没有一个uint64长！
class KingInt64ToIntMap
{
public:
	std::unordered_map<uint64_t,int> m_mapPosition;

	bool  InitMap(int nHashTableSize=0)
	{
		DeleteMap();
		return true;
	}

	bool  InitMap_Raw(int nHashTableSize=0)
	{
		return InitMap(nHashTableSize);
	}

	bool  AddToMap(uint64_t key,int position)
	{
		m_mapPosition[key]=position;
		return true;
	}

	bool AddToMap(const char *symbol,int posotion)
	{
		uint64_t curkey = KeyToInt64(symbol);
		return AddToMap(curkey, posotion);
	}

	int GetPosition(const char *symbol)
	{
		uint64_t curkey = KeyToInt64(symbol);
		return GetStockByMap(curkey);
	}

	bool  DeleteFromMap(uint64_t lKey)
	{
		std::unordered_map<uint64_t, int>::iterator it = m_mapPosition.find(lKey);
		if(it==m_mapPosition.end())
			return false;
		m_mapPosition.erase(it);
		return true;
	}

	bool DeleteFromMap(const char *symbol)
	{
		uint64_t curkey = KeyToInt64(symbol);
		return DeleteFromMap(curkey);
	}

	void  DeleteMap()
	{
		m_mapPosition.clear();
	}


	int GetStockByMap(uint64_t key)
	{
		std::unordered_map<uint64_t,int>::iterator it=m_mapPosition.find(key);
		if(it==m_mapPosition.end())
			return -1;
		return it->second;
	}

public:

	static uint64_t KeyToInt64(const char *key)
	{
		uint64_t result=0;
		char *presult=(char *)&result;
		for (int i = 0; i < 6;i++)
		{
			//过滤空格
			if (key[i] == 0)
				break;
			if (key[i] == ' ')
				break;
		}
		return result;
	}
};