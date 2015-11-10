#include "stdafx.h"
#include "wxui.h"

#include "../HqStorage/HqFileMgr.h"
#include "../HqStorage/HqSrcMgr.h"

bool HqUiApp::OnInit()
{
	HqUiFrame *frame = new HqUiFrame();
	frame->Center();
	frame->Show(true);
	//这个地方开始初始化


	return true;
}

wxBEGIN_EVENT_TABLE(HqUiFrame, wxFrame)
	EVT_MENU(ID_MKT_SH, HqUiFrame::OnMktSh)
	EVT_MENU(ID_MKT_SZ, HqUiFrame::OnMktSz)
wxEND_EVENT_TABLE()

HqUiFrame::HqUiFrame()
	: wxFrame((wxFrame *)NULL, wxID_ANY, wxT("行情显示"),
	wxDefaultPosition,
	wxDefaultSize)
{
	wxMenu *mktmenu = new wxMenu;
	mktmenu->Append(ID_MKT_SH, wxT("上海股票"));
	mktmenu->Append(ID_MKT_SZ, wxT("深圳股票"));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(mktmenu, wxT("市场选择"));
	SetMenuBar(menuBar);

	CreateStatusBar(2);
	SetStatusText("Welcome to King's Stock");

	grid = new wxGrid(this,
		wxID_ANY,
		wxPoint(0, 0),
		wxSize(850, 400));
	grid->HideRowLabels();
	grid->SetColLabelSize(24);
	grid->SetDefaultRowSize(24);


	int gridW = 600, gridH = 300;
	int logW = gridW, logH = 100;

	logWin = new wxTextCtrl(this,
		wxID_ANY,
		wxEmptyString,
		wxPoint(0, gridH + 20),
		wxSize(logW, logH),
		wxTE_MULTILINE | wxTE_READONLY);

	logger = new wxLogTextCtrl(logWin);
	m_logOld = wxLog::SetActiveTarget(logger);
	wxLog::DisableTimestamp();

	wxLogMessage("程序已启动");

	hqtbl = new HqGridTable();
	grid->SetTable(hqtbl);

	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(grid,1,wxEXPAND);
	topSizer->Add(logWin,0,wxEXPAND);

	SetSizerAndFit(topSizer);
	Centre();


	//这里开始初始化
	gHqFileMgr.load_cfg("E:\\svn\\github\\P135\\p246\\kingquote\\hq\\cfg\\",
		"E:\\svn\\github\\P135\\p246\\kingquote\\hq\\","E:\\svn\\github\\P135\\p246\\kingquote\\hq\\data\\");
	gHqSrcMgr.start_stk_dll("E:\\projects\\wjfgp\\StockDrv.dll");
}

HqUiFrame::~HqUiFrame()
{
}

void HqUiFrame::switch_to_mkt(uint32_t mktid)
{
	HqFile *curfile = gHqFileMgr.get_by_hsid(mktid);
	hqtbl->sethq(curfile);
	grid->SetTable(hqtbl, false, wxGrid::wxGridSelectRows);
	grid->Refresh();
}
