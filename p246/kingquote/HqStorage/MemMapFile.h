#pragma once
#include <boost/interprocess/detail/os_file_functions.hpp>

namespace boost
{
	namespace interprocess
	{
		class file_mapping;
		class mapped_region;
	}
}



class MemMapFile
{
public:
	MemMapFile()
	{
		_file_map = NULL;
		_map_region = NULL;
	}
	~MemMapFile()
	{
		close();
	}
	void close();

	void sync();

	void *addr();

	size_t size();

	bool map(const char *filename, long long size = 0, int mode = boost::interprocess::read_write, int mapmode = boost::interprocess::read_write, bool zeroother = true);


	boost::interprocess::file_mapping *_file_map;
	boost::interprocess::mapped_region *_map_region;
};
