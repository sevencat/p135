#pragma once
#include <string>
class UTF8toChar
{
public :
	UTF8toChar(const char *utf8_string);

	operator const char*()
	{
		return t_string;
	}

	~UTF8toChar()
	{
		if (needFree)
			delete[] t_string;
	}

	std::string to_string()
	{
		return std::string(t_string);
	}

private :
	char *t_string;
	bool needFree;

	//
	// helper utility to test if a string contains only ASCII characters
	//
	bool isPureAscii(const char *s)
	{
		while (*s != 0) { if (*(s++) & 0x80) return false; }
		return true;
	}

	//disable assignment
	UTF8toChar(const UTF8toChar &rhs);
	UTF8toChar &operator=(const UTF8toChar &rhs);
};

class ChartoUTF8
{
public :
	ChartoUTF8(const char *t_string);

	operator const char*()
	{
		return utf8_string;
	}

	std::string to_string()
	{
		return std::string(utf8_string);
	}

	~ChartoUTF8()
	{
		if (needFree)
			delete[] utf8_string;
	}

private :
	char *utf8_string;
	bool needFree;

	//
	// helper utility to test if a string contains only ASCII characters
	//
	bool isPureAscii(const char *s)
	{
		while (*s != 0) { if (*(s++) & 0x80) return false; }
		return true;
	}

	//disable assignment
	ChartoUTF8(const ChartoUTF8 &rhs);
	ChartoUTF8 &operator=(const ChartoUTF8 &rhs);
};

inline std::string gbk_2_utf(const char *t_string)
{
	return ChartoUTF8(t_string).to_string();
}
inline std::string utf8_2_gbk(const char *t_string)
{
	return UTF8toChar(t_string).to_string();
}


inline std::string gbk_2_utf(const std::string &t_string)
{
	return ChartoUTF8(t_string.c_str()).to_string();
}
inline std::string utf8_2_gbk(const std::string &t_string)
{
	return UTF8toChar(t_string.c_str()).to_string();
}