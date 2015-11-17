#include "stdafx.h"
#include "wxui.h"

#include "../HqStorage/HqFileMgr.h"
#include "../HqStorage/HqSrcMgr.h"
#include "resource.h"

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
	EVT_LIST_ITEM_ACTIVATED(wxID_ANY, HqUiFrame::OnLcDc)
wxEND_EVENT_TABLE()

HqUiFrame::HqUiFrame()
	: wxFrame((wxFrame *)NULL, wxID_ANY, wxT("行情显示"),
	wxDefaultPosition,
	wxDefaultSize)
{
	wxIcon icon("main");
	SetIcon(icon);
	wxMenu *mktmenu = new wxMenu;
	mktmenu->Append(ID_MKT_SH, wxT("上海股票"));
	mktmenu->Append(ID_MKT_SZ, wxT("深圳股票"));

	wxMenu *opmenu = new wxMenu();
	opmenu->Append(ID_OP_VIEWMINDATA, wxT("查看分钟数据"));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(mktmenu, wxT("市场选择"));
	menuBar->Append(opmenu, wxT("数据查看"));
	SetMenuBar(menuBar);

	CreateStatusBar(2);
	SetStatusText("Welcome to King's Stock");

	grid = new MyVListCtrl(this, wxID_ANY, wxPoint(0, 0), wxSize(850, 400), wxLC_HRULES | wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_VIRTUAL | wxLC_VRULES);
	
	/**
	grid = new wxGrid(this,
		wxID_ANY,
		wxPoint(0, 0),
		wxSize(850, 400));
	grid->HideRowLabels();
	grid->SetColLabelSize(24);
	grid->SetDefaultRowSize(24);
	*/


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

	hqtbl.InitLc(grid);

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
	hqtbl.sethq(curfile);
	hqtbl.RefreshData();
	//grid->SetTable(hqtbl, false, wxGrid::wxGridSelectRows);
	grid->Refresh();
}

#include "hqmindataviewdlg.h"
void HqUiFrame::OnLcDc(wxListEvent& event)
{
	int currowno = event.GetIndex();
	HqFile *curfile = hqtbl.gethq();
	if (curfile == NULL)
		return;
	HqFileHdr *stkhdr = curfile->stk_hdr;
	if (stkhdr == nullptr)
		return;

	HqRecord &curhq = stkhdr->rtrec[currowno];
	int minoffset = curhq.curminpos;
	HqMinRecord *mindata = stkhdr->get_min_rec_by_idx(currowno);

	HqMinDataViewDlg dlg(this, mindata, minoffset);
	dlg.ShowModal();
	grid->Refresh();
}


