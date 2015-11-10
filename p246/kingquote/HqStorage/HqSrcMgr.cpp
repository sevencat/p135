#include "stdafx.h"
#include "HqSrcMgr.h"
#include "../base/StkDrv.h"
#include "HqFileMgr.h"
#include <map>

HqSrcMgr gHqSrcMgr;

#define	Gp_Msg_StkData	WM_APP + 128
#define STKDRVWNDNAME "StkDrv_Helper_Window"
LRESULT CALLBACK StkWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void HqSrcMgr::start_stk_dll(const char* dllpath)
{
	StkDrv *stk_drv = new StkDrv();
	if (stk_drv->LoadDrv(dllpath))
	{
		WNDCLASSEX wndclass;
		wndclass.cbSize = sizeof wndclass;
		wndclass.style = 0;
		wndclass.lpfnWndProc = StkWindowProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = GetModuleHandle(0);
		wndclass.hIcon = 0;
		wndclass.hCursor = 0;
		wndclass.hbrBackground = 0;
		wndclass.lpszMenuName = 0;
		wndclass.lpszClassName = STKDRVWNDNAME;
		wndclass.hIconSm = 0;
		RegisterClassEx(&wndclass);
		HWND stk_wnd = CreateWindow(STKDRVWNDNAME, STKDRVWNDNAME, 0, 0, 0, 0, 0, 0, 0, 0, GetModuleHandle(0));
		::SetWindowLong(stk_wnd, GWL_USERDATA, (LONG)this);
		stk_drv->Stock_Init(stk_wnd, Gp_Msg_StkData, RCV_WORK_SENDMSG);
	}
	
}


void HqSrcMgr::on_recv_report(RCV_DATA *pHeader)
{
	int totallen = 0;
	char *reportheader = (char *)pHeader->m_pReport;
	std::map<uint32_t, std::vector<std::string> > reportmap;
	for (int i = 0; i < pHeader->m_nPacketNum; i++)
	{
		RCV_REPORT_STRUCTEx &curbuf = *(RCV_REPORT_STRUCTEx *)reportheader;
		int hqtype = m_cm.transfer_code(curbuf.m_wMarket, curbuf.m_szLabel);
		if ((hqtype > 0) && (hqtype <= 5))
		{
			uint32_t hsid = curbuf.m_wMarket;
			hsid = hsid - hsid % 16;
			reportmap[hsid].push_back(std::string((const char *)&curbuf, curbuf.m_cbSize));
		}
		else
		{
			int xxx = 1;
		}
		reportheader += curbuf.m_cbSize;
	}

	for (std::map<uint32_t, std::vector<std::string> >::iterator it = reportmap.begin(); it != reportmap.end(); it++)
	{
		uint32_t hsid = it->first;
		HqFile *pstorage = gHqFileMgr.get_by_hsid(hsid);
		if (pstorage == NULL)
			continue;
		std::vector<std::string> *newreports = new std::vector<std::string>();
		newreports->swap(it->second);
		pstorage->handle_stk_report_other_thread(newreports);
	}
}

void HqSrcMgr::on_recv_file(RCV_DATA* tag_rcv_data)
{
}

LRESULT CALLBACK StkWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == Gp_Msg_StkData)
	{
		HqSrcMgr *pThis = (HqSrcMgr *)::GetWindowLong(hWnd, GWL_USERDATA);
		UINT wFileType = wParam;
		RCV_DATA *pHeader = (RCV_DATA *)lParam;
		switch (wFileType)
		{
		case RCV_REPORT:
			//股票行情接收
			__try
			{
				pThis->on_recv_report(pHeader);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				MessageBox(NULL, "出错", "on_recv_report", MB_OK);
			}
			break;
		case RCV_FILEDATA:
			__try
			{
				pThis->on_recv_file(pHeader);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				char buf[64];
				sprintf(buf, "on_recv_file");
				MessageBox(NULL, "出错", buf, MB_OK);
			}
			break;
		}
		return TRUE;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
