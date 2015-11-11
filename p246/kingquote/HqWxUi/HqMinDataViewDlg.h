#pragma once

class HqMinDataViewDlg : public wxDialog
{
public:
	HqMinDataViewDlg(wxWindow* parent,HqMinRecord *mindata,int minoffset);
	~HqMinDataViewDlg(){}
	wxGrid *grid;
};

