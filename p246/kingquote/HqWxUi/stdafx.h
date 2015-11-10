#pragma once
#define WINVER		0x0501
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0500

#define WIN32_LEAN_AND_MEAN


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>
#include <winsock2.h>

#include <string>
#include <vector>
#include <map>

#include <wx/grid.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/wxprec.h>
#include <wx/app.h>
#include <wx/frame.h>

#include "../base/hqdef.h"

#undef min
#undef max