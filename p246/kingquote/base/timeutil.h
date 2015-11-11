#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>
#include <string>

class LocalTimeUtil
{
public:
	LocalTimeUtil()
	{
		Update();
	}

	void Update()
	{
		GetLocalTime(&st);
	}

	SYSTEMTIME st;

	time_t GetTimeT(int nDST=0)
	{
		struct tm atm;
		atm.tm_sec = st.wSecond;
		atm.tm_min = st.wMinute;
		atm.tm_hour = st.wHour;
		atm.tm_mday = st.wDay;
		atm.tm_mon = st.wMonth - 1;        // tm_mon is 0 based
		atm.tm_year = st.wYear - 1900;     // tm_year is 1900 based
		atm.tm_isdst = nDST;
		return _mktime64(&atm);
	}

	std::string GetSimpleTimeStr()
	{
		char buf[64];
		sprintf(buf,"%d:%d",GetDayNum(),GetMinNum());
		return buf;
	}

	int GetYear() const
	{
		return st.wYear;
	}
	int GetMonth() const
	{
		return st.wMonth;
	}
	int GetDay() const
	{
		return st.wDay;
	}
	int GetHour() const 
	{
		return st.wHour;
	}
	int GetMinute() const
	{
		return st.wMinute;
	}
	int GetSecond() const
	{
		return st.wSecond;
	}
	int GetDayOfWeek() const
	{
		return st.wDayOfWeek;
	}

	int GetDayNum()const
	{
		return st.wYear*10000+st.wMonth*100+st.wDay;
	}
	int GetMinNum()const
	{
		return st.wHour*10000+st.wMinute*100+st.wSecond;
	}
};

class SysTimeUtil
{
public:
	SysTimeUtil()
	{
		GetSystemTime(&st);
	}
	SYSTEMTIME st;
};