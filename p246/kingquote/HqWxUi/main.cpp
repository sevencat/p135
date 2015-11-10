#include "wxui.h"


IMPLEMENT_APP(HqUiApp)

#ifdef _DEBUG
#pragma comment(lib,"wxmsw30ud")
#else
#pragma comment(lib,"wxmsw30u")
#endif

#pragma comment(lib,"ws2_32")
#pragma comment(lib,"kinglib")
#pragma comment(lib,"log4cxx")
#pragma comment(lib,"shlwapi")