#pragma once
#define WINVER		0x0501
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0500

#define WIN32_LEAN_AND_MEAN


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

#include <io.h>
#include <asio/placeholders.hpp>
#include <asio/io_service.hpp>
#include "../base/basic_repeating_timer_asio.h"
#include "../base/pugixml.hpp"
#include "../base/dtz.h"
#include "../base/strutil.h"