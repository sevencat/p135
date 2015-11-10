#pragma once
#include "../base/Stockdrv.h"

//行情源管理
class HqSrcMgr
{
public:

	void start_stk_dll(const char *dllpath);

public:
	void on_recv_report(RCV_DATA *pHeader);
	void on_recv_file(RCV_DATA* tag_rcv_data);
};

extern HqSrcMgr gHqSrcMgr;