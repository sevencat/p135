#include "stdafx.h"
#include "charconv.h"

UTF8toChar::UTF8toChar(const char *utf8_string) : t_string(0), needFree(false)
{
	if (0 == utf8_string)
		t_string = 0;
	else if (0 == *utf8_string)
		t_string = ("");
	else if ( isPureAscii(utf8_string))
        t_string = (char *)utf8_string;
	else
	{
		// Either TCHAR = Unicode (2 bytes), or utf8_string contains non-ASCII characters.
		// Needs conversion
		needFree = true;

		// Convert to Unicode (2 bytes)
		int string_len = (int)strlen(utf8_string);
		wchar_t *buffer = new wchar_t[string_len + 1];
		MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, buffer, string_len + 1);
		buffer[string_len] = 0;

		t_string = new char[string_len * 2 + 2];
		WideCharToMultiByte(CP_ACP, 0, buffer, -1, t_string, string_len * 2 + 1, 0, 0);
		t_string[string_len * 2 + 1] = 0;
		delete[] buffer;
	}
}

ChartoUTF8::ChartoUTF8(const char *t_string) : utf8_string(0), needFree(false)
{
	if (0 == t_string)
		utf8_string = 0;
	else if (0 == *t_string)
		utf8_string = "";
	else if (isPureAscii((char *)t_string))
		utf8_string = (char *)t_string;
	else
	{

		needFree = true;
		int string_len = (int)strlen(t_string);

		// Convert to Unicode if not already in unicode.
		wchar_t *w_string = new wchar_t[string_len + 1];
		MultiByteToWideChar(CP_ACP, 0, t_string, -1, w_string, string_len + 1);
		w_string[string_len] = 0;

		// Convert from Unicode (2 bytes) to UTF8
		utf8_string = new char[string_len * 3 + 1];
		WideCharToMultiByte(CP_UTF8, 0, w_string, -1, utf8_string, string_len * 3 + 1, 0, 0);
		utf8_string[string_len * 3] = 0;

		if (w_string != (wchar_t *)t_string)
			delete[] w_string;
	}

}
