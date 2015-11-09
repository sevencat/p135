#pragma once

//我们假设key的长度最多是6！！！，正好没有一个uint64长！
class KingInt64ToIntMap
{
public:
	boost::unordered_map<unsigned long long,int> m_mapPosition;

	bool  InitMap(int nHashTableSize=0)
	{
		DeleteMap();
		return true;
	}

	bool  InitMap_Raw(int nHashTableSize=0)
	{
		return InitMap(nHashTableSize);
	}

	bool  AddToMap(unsigned long long key,int position)
	{
		m_mapPosition[key]=position;
		return true;
	}

	bool  AddToMap(LPCTSTR strKey,int position)
	{
		if( strKey == NULL )
			return false;
		return AddToMap(KeyToInt64(strKey),position);
	}

	bool  AddToMap(CodeInfo* pStock,int position)
	{
		if(pStock == NULL )
			return false;

		return AddToMap(pStock->m_cCode,position);
	}

	bool  AddToMap(StockUserInfo* pStock,int position)
	{
		if( pStock == NULL )
			return false;
		return AddToMap( &pStock->m_ciStockCode,position );
	}


	bool  AddToMap(const std::string &strKey,int position)
	{
		if(strKey.empty())
			return false;
		return AddToMap(strKey.c_str(),position);	
		return true;
	}

	bool  DeleteFromMap(unsigned long long lKey)
	{
		boost::unordered_map<unsigned long long,int>::iterator it=m_mapPosition.find(lKey);
		if(it==m_mapPosition.end())
			return false;
		m_mapPosition.erase(it);
		return true;
	}

	bool  DeleteFromMap(const char* lKey)
	{
		if(lKey==NULL)
			return false;
		if(lKey[0]==0)
			return false;
		return DeleteFromMap(KeyToInt64(lKey));
	}

	void  DeleteMap()
	{
		m_mapPosition.clear();
	}

	int GetStockByMap(StockUserInfo* pKey)
	{
		if(pKey==NULL)
			return -1;
		return GetStockByMap(&pKey->m_ciStockCode);
	}

	int GetStockByMap(CodeInfo* pKey)
	{
		if(pKey==NULL)
			return -1;
		return GetStockByMap(pKey->m_cCode);
	}

	int GetStockByMap(unsigned long long key)
	{
		boost::unordered_map<unsigned long long,int>::iterator it=m_mapPosition.find(key);
		if(it==m_mapPosition.end())
			return -1;
		return it->second;
	}

	int GetStockByMap(LPCTSTR key)
	{
		if(key==NULL)
			return -1;
		return GetStockByMap(KeyToInt64(key));
	}

	int GetStockByMap(const std::string &key) 
	{
		return GetStockByMap(key.c_str());
	}

public:
	static void YlsGetKey(unsigned long long  &strKey,CodeInfo* pCode) // strKey > 9
	{
		strKey=KeyToInt64(pCode->m_cCode);
	}

	static unsigned long long KeyToInt64(const char *key)
	{
		unsigned long long result=0;
		char *presult=(char *)&result;
		strncpy(presult,key,6);
		//现在有没有四位的？应该没有了，除了2a01，判断一下也没事。
		if(presult[5]==' ')
		{
			presult[5]=0;
			if(presult[4]==' ')
				presult[4]=0;
		}
		return result;
	}
};