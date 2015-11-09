#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"

CAppModule _Module;

#include "../HqStorage/HqFile.h"
#include "../base/hqdef.h"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	int64_t actsize = HqFileHdr::get_hdr_size(241)/(1024*1024);
	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	::DefWindowProc(NULL, 0, 0, 0L);
	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls
	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));
	int nRet = 0;
	{
		CMainDlg dlgMain;
		nRet = dlgMain.DoModal();
	}
	_Module.Term();
	::CoUninitialize();
	return nRet;
}
