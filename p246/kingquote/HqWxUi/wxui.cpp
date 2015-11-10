#include "wxui.h"
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>

bool HqUiApp::OnInit()
{
	HqUiFrame *frame = new HqUiFrame();
	frame->Center();
	frame->Show(true);
	return true;
}

wxBEGIN_EVENT_TABLE(HqUiFrame, wxFrame)
	EVT_MENU(ID_MKT_SH, HqUiFrame::OnMktSh)
	EVT_MENU(ID_MKT_SH, HqUiFrame::OnMktSz)
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

	grid = new wxGrid(this,
		wxID_ANY,
		wxPoint(0, 0),
		wxSize(400, 300));
	int gridW = 600, gridH = 300;
	int logW = gridW, logH = 100;

	logWin = new wxTextCtrl(this,
		wxID_ANY,
		wxEmptyString,
		wxPoint(0, gridH + 20),
		wxSize(logW, logH),
		wxTE_MULTILINE);

	logger = new wxLogTextCtrl(logWin);
	m_logOld = wxLog::SetActiveTarget(logger);
	wxLog::DisableTimestamp();

	hqtbl = new HqGridTable();
	grid->SetTable(hqtbl);

	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(grid,1,wxEXPAND);
	topSizer->Add(logWin,0,wxEXPAND);

	SetSizerAndFit(topSizer);
	Centre();
}

HqUiFrame::~HqUiFrame()
{
}