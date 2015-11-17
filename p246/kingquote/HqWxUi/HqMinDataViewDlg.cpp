#include "stdafx.h"
#include "HqMinDataViewDlg.h"

wxString from_value(double value);
wxString from_value(int value, int divvalue = 1000);
wxString from_complex_value(double value);
wxString from_time_t(uint32_t value);

HqMinDataViewDlg::HqMinDataViewDlg(wxWindow* parent, HqMinRecord *mindata, int minoffset)
	:wxDialog(parent,wxID_ANY,"分钟线查看")
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	grid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxSize(750,500), 0);

	//grid->SetUseNativeColLabels(true);
	grid->UseNativeColHeader(true);
	// Grid
	grid->CreateGrid(minoffset, 8);
	
	grid->EnableEditing(true);
	grid->EnableGridLines(true);
	grid->EnableDragGridSize(false);
	grid->SetMargins(0, 0);

	// Columns
	grid->EnableDragColMove(false);
	grid->EnableDragColSize(true);
	grid->SetColLabelSize(24);
	grid->SetColLabelValue(0, wxT("时间"));
	grid->SetColLabelValue(1, wxT("开盘价"));
	grid->SetColLabelValue(2, wxT("最高价"));
	grid->SetColLabelValue(3, wxT("最低价"));
	grid->SetColLabelValue(4, wxT("最新价"));
	grid->SetColLabelValue(5, wxT("量"));
	grid->SetColLabelValue(6, wxT("金额"));
	grid->SetColLabelValue(7, wxT("持仓"));
	grid->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);

	// Rows
	grid->EnableDragRowSize(false);
	grid->SetRowLabelSize(0);
	
	for (int i = 0; i < minoffset;i++)
	{
		HqMinRecord &curmin = mindata[i];
		if (curmin.lasthqtime == 0)
			continue;
		grid->SetCellValue(i, 0, from_time_t(curmin.lasthqtime));
		grid->SetCellValue(i, 1, from_value(curmin.openpx));
		grid->SetCellValue(i, 2, from_value(curmin.highpx));
		grid->SetCellValue(i, 3, from_value(curmin.lowpx));
		grid->SetCellValue(i, 4, from_value(curmin.newpx));

		grid->SetCellValue(i, 5, from_complex_value(curmin.vol));
		grid->SetCellValue(i, 6, from_complex_value(curmin.money));
		grid->SetCellValue(i, 7, from_complex_value(curmin.position));


	}

	// Label Appearance

	// Cell Defaults
	grid->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	bSizer1->Add(grid, 0, wxALL, 5);


	this->SetSizer(bSizer1);
	this->Layout();
	bSizer1->Fit(this);

	this->Centre(wxBOTH);
}
