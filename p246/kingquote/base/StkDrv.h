#pragma once
#include "Stockdrv.h"

//需要换成ace中的函数，NO，这个不可能跨平台，换了干啥呢。
class StkDrv
{
public:
	StkDrv(void)
	{
		m_hSTKDrv=NULL;
		m_pfnStock_Init = NULL;
	}
	~StkDrv(void){}

	//加载
	bool LoadDrv(const char *dllname);

public:
	DWORD GetStockDrvInfo(int nInfo, void* pBuf);
	int SetupReceiver(BOOL bSetup);
	int GetTotalNumber();
	int GetStockByNoEx(int nNo, RCV_REPORT_STRUCTEx* pBuf);
	int GetStockByCodeEx(char* pszStockCode, int nMarket, RCV_REPORT_STRUCTEx* pBuf);
	int Stock_Quit(HWND hWnd);
	int Stock_Init(HWND hWnd, UINT uMsg, int nWorkMode);
	void SetNewsPath(LPCTSTR lpszNewsPath);
	int SetExtMsg(UINT msg);
	int GetKData(LPCSTR scode, time_t mt);
	int QueryKData(LPCSTR scode, int nMarket, time_t mt);
	int GetMinData(LPCSTR scode, time_t mt);
	int QueryMinData(LPCSTR scode, int nMarket, time_t mt);
private:
	int (WINAPI *m_pfnStock_Init)(HWND hWnd, UINT Msg, int nWorkMode);
	int (WINAPI *m_pfnStock_Quit)(HWND hWnd);
	int (WINAPI *m_pfnGetTotalNumber)();	
	int (WINAPI *m_pfnGetStockByNoEx)(int nNo, RCV_REPORT_STRUCTEx* pBuf);
	int (WINAPI *m_pfnGetStockByCodeEx)(char* pszStockCode, int nMarket, RCV_REPORT_STRUCTEx* pBuf);
	int	(WINAPI *m_pfnSetupReceiver)(BOOL bSetup);
	DWORD (WINAPI *m_pfnGetStockDrvInfo)(int nInfo, void* pBuf);
	void (WINAPI *m_pfnSetNewsPath)(LPCTSTR lpszNewsPath);
	int (WINAPI *m_pfnSetExtMsg)(UINT msg);
	int (WINAPI *m_pfnGetKData)(LPCSTR scode, time_t mt);
	int (WINAPI *m_pfnQueryKData)(LPCSTR scode, int nMarket, time_t mt);
	int (WINAPI *m_pfnGetMinData)(LPCSTR scode, time_t mt);
	int (WINAPI *m_pfnQueryMinData)(LPCSTR scode, int nMarket, time_t mt);
	HINSTANCE	m_hSTKDrv;
};
