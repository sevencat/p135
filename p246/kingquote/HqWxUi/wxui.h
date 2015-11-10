#pragma once
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include <stdint.h>
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
	ID_MKT_SH=100,
	ID_MKT_SZ,
};

class HqUiFrame : public wxFrame
{
public:
	HqUiFrame();
	~HqUiFrame();

	wxGrid  *grid;
	wxTextCtrl     *logWin;
	wxLogTextCtrl  *logger;
	wxLog *m_logOld;

	HqGridTable *hqtbl;

	wxDECLARE_EVENT_TABLE();


	void OnMktSh(wxCommandEvent&)
	{
		
	}

	void OnMktSz(wxCommandEvent&)
	{
		
	}

	void switch_to_mkt(uint32_t mktid)
	{
		
	}
};