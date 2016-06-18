#pragma once
#include <boost/version.hpp>
#include <boost/interprocess/detail/os_file_functions.hpp>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//struct OVERLAPPED;
//extern "C" __declspec(dllimport) int __stdcall ReadFile(void *hnd, void *buffer, unsigned long bytes_to_write,unsigned long *bytes_written, OVERLAPPED* overlapped);
class BoostFile
{
public:
	BoostFile()
	{
		_handle=boost::interprocess::ipcdetail::invalid_file(); 
	}
	~BoostFile()
	{
		close_file();
	}

	bool create_new_file(const char *name, boost::interprocess::mode_t mode = boost::interprocess::read_write, bool temporary = false)
	{
		//Del by lay 20100902
		//_handle=boost::interprocess::ipcdetail::create_new_file(name,mode,temporary);


		//<<<Add by lay 20100902
#if BOOST_VERSION>=104601
		_handle=boost::interprocess::ipcdetail::create_or_open_file(name,mode,boost::interprocess::permissions(),temporary);
#else
		_handle=boost::interprocess::ipcdetail::create_or_open_file(name,mode,temporary);
#endif
		if (valid())
			return truncate_file(0);
		return false;
		//>>>Add by lay 20100902
	}
	bool create_or_open_file(const char *name, boost::interprocess::mode_t mode = boost::interprocess::read_write, bool temporary = false)
	{
#if BOOST_VERSION>=104601
		_handle=boost::interprocess::ipcdetail::create_or_open_file(name,mode,boost::interprocess::permissions(),temporary);
#else
		_handle=boost::interprocess::ipcdetail::create_or_open_file(name,mode,temporary);
#endif
		return valid();
	}
	bool open_existing_file(const char *name, boost::interprocess::mode_t mode = boost::interprocess::read_write, bool temporary = false)
	{
		_handle=boost::interprocess::ipcdetail::open_existing_file(name,mode,temporary);
		return valid();
	}
	bool is_invalid_file()
	{  
		return _handle==boost::interprocess::ipcdetail::invalid_file();  
	}
	bool valid()
	{
		return _handle!=boost::interprocess::ipcdetail::invalid_file();
	}
	void close_file()
	{
		if(!is_invalid_file())
		{
			boost::interprocess::ipcdetail::close_file(_handle);
			_handle=boost::interprocess::ipcdetail::invalid_file();
		}
	}
	bool truncate_file (std::size_t size)
	{
		return boost::interprocess::ipcdetail::truncate_file(_handle,size);
	}
	bool get_file_size(boost::interprocess::offset_t &size)
	{
		return boost::interprocess::ipcdetail::get_file_size(_handle,size);
	}
	unsigned long long get_file_size()
	{
		boost::interprocess::offset_t size=0;
		if(!get_file_size(size))
			size=0;
		return size;
	}
	bool set_file_pointer(boost::interprocess::offset_t off, boost::interprocess::file_pos_t pos)
	{
		return boost::interprocess::ipcdetail::set_file_pointer(_handle,off,pos);
	}
	bool seek_to_begin(int offsize=0)
	{
		return set_file_pointer(offsize,boost::interprocess::file_begin);
	}
	bool seek_current(int offsize=0)
	{
		return set_file_pointer(offsize,boost::interprocess::file_current);
	}
	bool seek_to_end(int offsize=0)
	{
		return set_file_pointer(offsize,boost::interprocess::file_end);
	}
	bool get_file_pointer(boost::interprocess::offset_t &off)
	{
		return boost::interprocess::ipcdetail::get_file_pointer(_handle,off);
	}
	unsigned long long get_file_pointer()
	{
		boost::interprocess::offset_t off=0;
		if(!get_file_pointer(off))
			return 0;
		return off;
	}
	bool write_file(const void *data, std::size_t numdata)
	{
		return boost::interprocess::ipcdetail::write_file(_handle,data,numdata);
	}
	bool read_file(void *data, std::size_t numdata)
	{
		unsigned long readbytes=0;
		int ret=ReadFile(_handle,data,numdata,&readbytes,NULL);
		return numdata==readbytes;
	}
	int read_file_length(void *data, std::size_t numdata)
	{
		unsigned long readbytes=0;
		int ret=ReadFile(_handle,data,numdata,&readbytes,NULL);
		return readbytes;
	}
private:
	boost::interprocess::file_handle_t _handle;

public:
	static bool delete_file(const char *name)
	{
		return boost::interprocess::ipcdetail::delete_file(name);
	}
	static bool read_file_contents(const char *filename,std::string &buffer)
	{
		BoostFile bf;
		if(!bf.open_existing_file(filename,boost::interprocess::read_only))
			return false;
		unsigned int filesize=bf.get_file_size();
		if(filesize==0)
			return false;
		buffer.resize(filesize);
		return bf.read_file((void *)buffer.c_str(),filesize);
	}
	static bool write_file_contents(const char *filename,const void *pdata,int datalen)
	{
		BoostFile bf;
		if(!bf.create_or_open_file(filename))
			return false;
		bf.set_file_pointer(0,boost::interprocess::file_begin);
		return bf.write_file(pdata,datalen);
	}
};