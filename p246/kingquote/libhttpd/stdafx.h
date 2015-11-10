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
#include <map>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <asio/io_service.hpp>
#include <asio/basic_socket.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/streambuf.hpp>