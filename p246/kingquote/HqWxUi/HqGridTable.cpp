#include "stdafx.h"
#include "HqGridTable.h"
#include "../HqStorage/HqFile.h"

HqGridTable::HqGridTable()
{
	m_attrForOddRows = new wxGridCellAttr;
	m_attrForOddRows->SetBackgroundColour(*wxLIGHT_GREY);
	m_attrForOddRows->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);

	m_defaultCellAttr = new wxGridCellAttr();
	m_defaultCellAttr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);

	m_hq = NULL;

	m_colLabels.push_back("代码");
	m_colLabels.push_back("名称");
	m_colLabels.push_back("昨收价");
	m_colLabels.push_back("开盘价");
	m_colLabels.push_back("最高价");
	m_colLabels.push_back("最低价");
	m_colLabels.push_back("最新价");
	m_colLabels.push_back("成交量");
	m_colLabels.push_back("金额(元)");
	m_colLabels.push_back("持仓量");
}


HqGridTable::~HqGridTable()
{
}

wxGridCellAttr* HqGridTable::GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind)
{
	//if (kind != wxGridCellAttr::Cell)
	//{
	//	return wxGridTableBase::GetAttr(row, col, kind);
	//}
	wxGridCellAttr *attr = NULL;
	if (row % 2)
	{
		attr = m_attrForOddRows;
	}
	else
	{
		attr = m_defaultCellAttr;
	}
	attr->IncRef();
	return attr;
}

int HqGridTable::GetNumberRows()
{
	if (m_hq == NULL)
		return 0;
	HqFileHdr *hdr = m_hq->stk_hdr;
	if (hdr == NULL)
		return 0;
	return hdr->curstkcount;
}

int HqGridTable::GetNumberCols()
{
	return m_colLabels.size();
}

static wxString from_value(double value)
{
	return wxString::Format("%.2f", value);
}

static wxString from_value(int value, int divvalue = 1000)
{
	double curvalue = value / (double)divvalue;
	return from_value(curvalue);
}

static wxString from_complex_value(double value)
{
	if (value<=10000)
	{
		return from_value(value);
	}
	value = value / 10000.0f;
	if (value <= 10000)
	{
		return from_value(value)+"万";
	}
	return from_value(value/10000.0f) + "亿";
}

wxString HqGridTable::GetValue(int row, int col)
{
	if (m_hq == NULL)
		return "";
	HqFileHdr *hdr = m_hq->stk_hdr;
	if (hdr == NULL)
		return "";
	if (row >= hdr->curstkcount)
		return "";
	HqRecord &currec = hdr->rtrec[row];
	switch (col)
	{
	case 0:
		return currec.symbol;
	case 1:
		return currec.name;
	case 2:
		return from_value(currec.preclosepx);
	case 3:
		return from_value(currec.openpx);
	case 4:
		return from_value(currec.highpx);
	case 5:
		return from_value(currec.lowpx);
	case 6:
		return from_value(currec.newpx);
	case 7:
		return from_complex_value(currec.vol);
	case 8:
		return from_complex_value(currec.money);
	}
	return "";
}

void HqGridTable::SetRowLabelValue(int row, const wxString& value)
{
	if (row > (int)(m_rowLabels.GetCount()) - 1)
	{
		int n = m_rowLabels.GetCount();
		int i;

		for (i = n; i <= row; i++)
		{
			m_rowLabels.Add(wxGridTableBase::GetRowLabelValue(i));
		}
	}

	m_rowLabels[row] = value;
}
void HqGridTable::SetColLabelValue(int col, const wxString& value)
{
	if (col > (int)(m_colLabels.GetCount()) - 1)
	{
		int n = m_colLabels.GetCount();
		int i;

		for (i = n; i <= col; i++)
		{
			m_colLabels.Add(wxGridTableBase::GetColLabelValue(i));
		}
	}

	m_colLabels[col] = value;
	
}
wxString HqGridTable::GetRowLabelValue(int row)
{
	if (row > (int)(m_rowLabels.GetCount()) - 1)
	{
		// using default label
		//
		return wxGridTableBase::GetRowLabelValue(row);
	}
	else
	{
		return m_rowLabels[row];
	}
}
wxString HqGridTable::GetColLabelValue(int col)
{
	if (col > (int)(m_colLabels.GetCount()) - 1)
	{
		// using default label
		//
		return wxGridTableBase::GetColLabelValue(col);
	}
	else
	{
		return m_colLabels[col];
	}
}