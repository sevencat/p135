// StkStorage.cpp : main source file for StkStorage.exe
//

#include "stdafx.h"

#include "resource.h"

#include "MainDlg.h"

CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	HRESULT hRes = ::CoInitialize(NULL);
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


#pragma comment(lib,"libmysql")