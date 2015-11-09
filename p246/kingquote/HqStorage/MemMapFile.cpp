#include "stdafx.h"
#include <io.h>
#include <fstream>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "MemMapFile.h"

void MemMapFile::close()
{
	if (_map_region != NULL)
		delete _map_region;
	if (_file_map != NULL)
		delete _file_map;
	_file_map = NULL;
	_map_region = NULL;
}

void MemMapFile::sync()
{
	if (_map_region)
		_map_region->flush();
}

void * MemMapFile::addr()
{
	if (_map_region)
		return _map_region->get_address();
	return NULL;
}

size_t MemMapFile::size()
{
	if (_map_region)
		return _map_region->get_size();
	return 0;
}


bool MemMapFile::map(const char *filename, long long size, int mode, int mapmode, bool zeroother)
{
	if (_access(filename, 0) == -1)
	{
		if (size <= 0)
			return false;

		std::filebuf fbuf;
		fbuf.open(filename, std::ios_base::in | std::ios_base::out
			| std::ios_base::trunc | std::ios_base::binary);
		//Set the size
		fbuf.pubseekoff(size - 1, std::ios_base::beg);
		fbuf.sputc(0);
		fbuf.close();
	}
	boost::interprocess::file_handle_t fh = boost::interprocess::ipcdetail::open_existing_file(filename, boost::interprocess::read_write);
	if (fh == boost::interprocess::ipcdetail::invalid_file())
	{
		return false;
	}
	long long filesize = 0;
	if (!boost::interprocess::ipcdetail::get_file_size(fh, filesize))
	{
		boost::interprocess::ipcdetail::close_file(fh);
		return false;
	}

	int64_t orgsize = size;
	if (size <= 0)
		size = filesize;
	else if (size != filesize)
	{
		//这里扩大可能是没问题的，缩小不知道有没有问题。
		if (size > filesize)
		{
			//如果需要映射的大小比原先的要大！
			if (!boost::interprocess::ipcdetail::truncate_file(fh, size))
				return false;
		}
		else
		{
			//这里要填充零进去,而且是全部映射进来！！！
			if (!boost::interprocess::ipcdetail::truncate_file(fh, size))
				size = filesize;
		}
	}
	boost::interprocess::ipcdetail::close_file(fh);

	//下面才会开始创建！！！
	_file_map = new boost::interprocess::file_mapping(filename, (boost::interprocess::mode_t)mode);
	if (_file_map == NULL)
		return false;
	_map_region = new boost::interprocess::mapped_region(*_file_map, (boost::interprocess::mode_t)mapmode, 0, size);
	if (_map_region == NULL)
	{
		delete _file_map;
		return false;
	}
	if (orgsize > 0)
	{
		if (zeroother)
		{
			char *pdata = (char *)_map_region->get_address();
			int64_t realsize = _map_region->get_size();
			if (realsize > orgsize)
			{
				pdata += orgsize;
				memset(pdata, 0, realsize - orgsize);
			}
		}
	}
	return true;
}
