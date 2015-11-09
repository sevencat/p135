#include "StkDrv.h"

bool StkDrv::LoadDrv(const char *dllname)
{
	memset(this,0,sizeof(*this));
	m_hSTKDrv = LoadLibrary(dllname);
	if (!m_hSTKDrv)
		return false;
	
	m_pfnStock_Init = \
		(int (WINAPI *)(HWND, UINT, int)) GetProcAddress(m_hSTKDrv, "Stock_Init");
	if(m_pfnStock_Init==NULL)
		return false;

	m_pfnStock_Quit = \
		(int (WINAPI *)(HWND)) GetProcAddress(m_hSTKDrv, "Stock_Quit");
	if(m_pfnStock_Quit==NULL)
		return false;

	m_pfnGetTotalNumber = \
		(int (WINAPI *)())GetProcAddress(m_hSTKDrv, "GetTotalNumber");

	m_pfnGetStockByNoEx = \
		(int (WINAPI *)(int, RCV_REPORT_STRUCTEx *))GetProcAddress(m_hSTKDrv, "GetStockByNoEx");
	m_pfnGetStockByCodeEx = \
		(int (WINAPI *)(char *, int, RCV_REPORT_STRUCTEx *))GetProcAddress(m_hSTKDrv, "GetStockByCodeEx");

	m_pfnSetupReceiver = \
		(int	(WINAPI *)(BOOL))GetProcAddress(m_hSTKDrv, "SetupReceiver");
	m_pfnGetStockDrvInfo = \
		(DWORD(WINAPI *)(int, void*))GetProcAddress(m_hSTKDrv, "GetStockDrvInfo");
	m_pfnSetNewsPath = \
		(void (WINAPI *)(LPCTSTR))GetProcAddress(m_hSTKDrv, "SetNewsPath");

	m_pfnSetExtMsg = \
		(int (WINAPI *)(UINT msg))GetProcAddress(m_hSTKDrv, "SetExtMsg");
	m_pfnGetKData = \
		(int (WINAPI *)(LPCSTR scode, time_t mt))GetProcAddress(m_hSTKDrv, "GetKData");
	m_pfnQueryKData = \
		(int (WINAPI *)(LPCSTR scode, int nMarket, time_t mt))GetProcAddress(m_hSTKDrv, "QueryKData");
	m_pfnGetMinData = \
		(int (WINAPI *)(LPCSTR scode, time_t mt))GetProcAddress(m_hSTKDrv, "GetMinData");
	m_pfnQueryMinData = \
		(int (WINAPI *)(LPCSTR scode, int nMarket, time_t mt))GetProcAddress(m_hSTKDrv, "QueryMinData");

	GetStockDrvInfo(RI_PanKouSUPPORT,NULL);
	return true;
}


int StkDrv::Stock_Init(HWND hWnd, UINT uMsg, int nWorkMode)
{
	if (!m_pfnStock_Init)
		return -1;
	GetStockDrvInfo(RI_DATAExtSUPPORT2,NULL);
	int n = ((*m_pfnStock_Init)(hWnd, uMsg, nWorkMode)); 
	return n;
}

int StkDrv::Stock_Quit(HWND hWnd)
{
	if (!m_pfnStock_Quit)
		return -1;
	return((*m_pfnStock_Quit)(hWnd));
}

int StkDrv::GetStockByNoEx(int nNo, RCV_REPORT_STRUCTEx* pBuf)
{
	if (!m_pfnGetStockByNoEx)
		return -1;
	return m_pfnGetStockByNoEx(nNo, pBuf);
}

int StkDrv::GetStockByCodeEx(char* pszStockCode, int nMarket, RCV_REPORT_STRUCTEx* pBuf)
{
	if (!m_pfnGetStockByCodeEx)
		return -1;
	return m_pfnGetStockByCodeEx(pszStockCode, nMarket, pBuf);
}

int StkDrv::GetTotalNumber()
{
	if (!m_pfnGetTotalNumber)
		return -1;
	return((*m_pfnGetTotalNumber)());
}

int StkDrv::SetupReceiver(BOOL bSetup)
{
	if (!m_pfnSetupReceiver)
		return -1;
	return((*m_pfnSetupReceiver)(bSetup));
}

DWORD StkDrv::GetStockDrvInfo(int nInfo, void* pBuf)
{
	if (!m_pfnGetStockDrvInfo)
		return -1;
	return((*m_pfnGetStockDrvInfo)(nInfo, pBuf));
}

void StkDrv::SetNewsPath(LPCTSTR lpszNewsPath)
{
	if (!m_pfnSetNewsPath)
		return ;

	try
	{
		(*m_pfnSetNewsPath)(lpszNewsPath);
	}
	catch (...)
	{
	}
}
int StkDrv::SetExtMsg(UINT msg)
{
	if (!m_pfnSetExtMsg)
		return 0;
	return((*m_pfnSetExtMsg)(msg));
}
int StkDrv::GetKData(LPCSTR scode, time_t mt)
{
	if (!m_pfnGetKData)
		return 0;
	return((*m_pfnGetKData)(scode, mt));
}
int StkDrv::QueryKData(LPCSTR scode, int nMarket, time_t mt)
{
	if (!m_pfnQueryKData)
		return 0;
	return((*m_pfnQueryKData)(scode, nMarket, mt));
}
int StkDrv::GetMinData(LPCSTR scode, time_t mt)
{
	if (!m_pfnGetMinData)
		return 0;
	return((*m_pfnGetMinData)(scode, mt));
}
int StkDrv::QueryMinData(LPCSTR scode, int nMarket, time_t mt)
{
	if (!m_pfnQueryMinData)
		return 0;
	return((*m_pfnQueryMinData)(scode, nMarket, mt));
}
