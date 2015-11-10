#pragma once
#include "../base/Stockdrv.h"
#include "codemap.h"

//行情源管理
class HqSrcMgr
{
public:

	void start_stk_dll(const char *dllpath);

public:
	void on_recv_report(RCV_DATA *pHeader);
	void on_recv_file(RCV_DATA* tag_rcv_data);
	CodeMap m_cm;
};

extern HqSrcMgr gHqSrcMgr;