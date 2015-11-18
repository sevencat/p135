#pragma once

//#include "Myvlistctrl.h"
//#include "HqDataModel.h"
#include "HqGridTable.h"

// Define a new application type, each program should derive a class from wxApp
class HqUiApp : public wxApp
{
public:
	// override base class virtuals
	// ----------------------------

	// this one is called on application startup and is a good place for the app
	// initialization (doing it here and not in the ctor allows to have an error
	// return: if OnInit() returns false, the application terminates)
	virtual bool OnInit();
};

enum
{
	ID_OP_VIEWMINDATA = 100,
	ID_REFRESH_GRID,
	ID_MKT_SH=1000,
	ID_MKT_SZ,
	
};

class HqUiFrame : public wxFrame
{
public:
	HqUiFrame();
	~HqUiFrame();

	//MyVListCtrl  *grid;
	wxGrid  *grid;
	wxTextCtrl     *logWin;
	wxLogTextCtrl  *logger;
	wxLog *m_logOld;

	//HqDataModel hqtbl;
	HqGridTable *hqtbl;

	wxTimer m_timer_grid_refresh;
	virtual void OnTimerRefreshGrid(wxTimerEvent& event);

	wxDECLARE_EVENT_TABLE();

	void OnMktSh(wxCommandEvent&)
	{
		switch_to_mkt(0x1100);
	}
	void OnMktSz(wxCommandEvent&)
	{
		switch_to_mkt(0x1200);
	}
	void switch_to_mkt(uint32_t mktid);

	//virtual void OnLcDc(wxListEvent& event);

	void OnViewFen(wxCommandEvent&);

	void OnRefreshGrid(wxCommandEvent&);
	
};