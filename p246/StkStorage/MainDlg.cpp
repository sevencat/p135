// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "inifile.h"
#include "MainDlg.h"
#include "mysqlwriter.h"
std::string gexedir;

std::list<std::string> loglst;
bool logchanged = false;
asio::detail::win_mutex logmtx;

void addlog(const char* pszFormat, ...)
{
	char tempsqlbuf[1024] = { 0 };
	va_list argptr;
	va_start(argptr, pszFormat);
	int n = _vsnprintf(tempsqlbuf, sizeof(tempsqlbuf) - 1, pszFormat, argptr);
	va_end(argptr);
	
	asio::detail::win_mutex::scoped_lock lock(logmtx);
	while (loglst.size() > 128)
		loglst.pop_front();
	loglst.push_back(tempsqlbuf);
	logchanged = true;
}


#define TIMER_LOG 1000
LRESULT CMainDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (wParam != TIMER_LOG)
		return 0;
	std::string logstr;
	{
		asio::detail::win_mutex::scoped_lock lock(logmtx);
		if (!logchanged)
			return 0;
		for (auto it = loglst.begin(); it != loglst.end();it++)
		{
			logstr.append(*it);
			logstr.append("\r\n");
		}
		logchanged = false;
	}
	SetDlgItemText(IDC_EDIT_LOG, logstr.c_str());
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	this->SetTimer(TIMER_LOG, 3000, NULL);

	char cfgpath[256] = { 0 };
	GetModuleFileName(NULL, cfgpath, 256);
	PathRemoveFileSpec(cfgpath);
	gexedir = cfgpath;

	std::string cfgfilename = gexedir + "\\stksto.ini";
	CIniFile inifile;
	if (!inifile.Load(cfgfilename))
	{
		return FALSE;
	}
	std::string drvdllfile = inifile.GetKeyValue("stk", "drvdll");
	stkdrv.load(drvdllfile.c_str());

	gMysqlWriter.init(inifile);

	
	return TRUE;
}


LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnTest(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	stkdrv.querydata("ZZSRS09", DAY_KLINE, false, true, 1, true);
	return 0;
}
