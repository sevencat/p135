#pragma once
#include <wx/generic/grid.h>

class HqFile;

class HqGridTable : public wxGridTableBase
{
public:
	HqGridTable();
	~HqGridTable();

	// these are pure virtual in wxGridTableBase
	//
	virtual int GetNumberRows();
	virtual int GetNumberCols();
	virtual wxString GetValue(int row, int col);

	//这个不设置，这是个只读的！！！
	virtual void SetValue(int row, int col, const wxString& s){}

	void Clear(){}
	bool InsertRows(size_t pos = 0, size_t numRows = 1){ return false; }
	bool AppendRows(size_t numRows = 1){ return false; }
	bool DeleteRows(size_t pos = 0, size_t numRows = 1){ return false; }
	bool InsertCols(size_t pos = 0, size_t numCols = 1){ return false; }
	bool AppendCols(size_t numCols = 1){ return false; }
	bool DeleteCols(size_t pos = 0, size_t numCols = 1){ return false; }

	virtual void SetRowLabelValue(int row, const wxString&) override;
	virtual void SetColLabelValue(int col, const wxString&) override;
	virtual wxString GetRowLabelValue(int row) override;
	virtual wxString GetColLabelValue(int col) override;

private:
	wxGridStringArray m_data;

	// notice that while we don't need to store the number of our rows as it's
	// always equal to the size of m_data array, we do need to store the number
	// of our columns as we can't retrieve it from m_data when the number of
	// rows is 0 (see #10818)
	int m_numCols;

	// These only get used if you set your own labels, otherwise the
	// GetRow/ColLabelValue functions return wxGridTableBase defaults
	//
	wxArrayString     m_rowLabels;
	wxArrayString     m_colLabels;

	HqFile *m_hq;
};