#include "stdafx.h"
#include "HqGridTable.h"
#include "../HqStorage/HqFile.h"
#include "../base/timeutil.h"
#include "HqDataModel.h"


HqGridTable::HqGridTable()
{
	m_attrForOddRows = new wxGridCellAttr;
	m_attrForOddRows->SetBackgroundColour(*wxLIGHT_GREY);
	m_attrForOddRows->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	m_attrForOddRows->SetReadOnly(true);

	m_defaultCellAttr = new wxGridCellAttr();
	m_defaultCellAttr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	m_defaultCellAttr->SetReadOnly(true);

	m_hq = NULL;

	m_colLabels.push_back("代码");
	m_colLabels.push_back("名称");
	m_colLabels.push_back("分钟");
	m_colLabels.push_back("时间");
	m_colLabels.push_back("涨跌值");
	m_colLabels.push_back("涨跌幅");
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

wxString from_value(double value)
{
	return wxString::Format("%.2f", value);
}

wxString from_value(int value, int divvalue = 1000)
{
	double curvalue = value / (double)divvalue;
	return from_value(curvalue);
}

wxString from_complex_value(double value)
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

extern _tzinfo_t globaltz;
wxString from_time_t(uint32_t value)
{
	_datetime_t utcdt;
	utcdt.from_gmt_timer(value);
	_datetime_t localdt = globaltz.to_local(utcdt);
//	int curoffset = get_min_offset(localdt.hour(), localdt.min());
	int value2 = localdt.hour() * 10000 + localdt.min() * 100 + localdt.sec();
	return wxString::Format("%d", value2);
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
	case Fld_Code:
		return currec.symbol;
	case Fld_Name:
		return currec.name;

	case FLd_LastMin:
		return wxString::Format("%d", (int)currec.curminpos);

	case Fld_LastTime:
		return from_time_t(currec.lasttime);
	case Fld_Zdz:
	{
		if (currec.preclosepx > 0.000001)
			return from_value(currec.newpx - currec.preclosepx,1000);
		else
			return "";
	}
	case Fld_Zdf:
		{
			if (currec.preclosepx > 0.000001)
			{
				double zdz = (double)(currec.newpx - currec.preclosepx);
				double zdf = 100*zdz / (double)currec.preclosepx;
				return from_value(zdf)+"%";
			}
			else
				return "";
		}

	case Fld_Preclose:
		return from_value(currec.preclosepx);
	case Fld_Openpx:
		return from_value(currec.openpx);
	case Fld_Highpx:
		return from_value(currec.highpx);
	case Fld_Lowpx:
		return from_value(currec.lowpx);
	case Fld_Newpx:
		return from_value(currec.newpx);
	case Fld_Vol:
		return from_complex_value(currec.vol);
	case Fld_Money:
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

void HqGridTable::RefreshGridData()
{
	wxGrid *grid = GetView();
	if (grid == NULL)
		return;

	grid->BeginBatch();
	wxArrayInt oldselections = grid->GetSelectedRows();
	grid->ClearSelection();

	if (grid->IsCellEditControlEnabled())
	{
		grid->DisableCellEditControl();
	}
	int oldcount = grid->GetNumberRows();
	int newcount = GetNumberRows();

	if (newcount != oldcount)
	{
		{
			wxGridTableMessage pop(this,
				wxGRIDTABLE_NOTIFY_ROWS_DELETED,
				0, oldcount);
			grid->ProcessTableMessage(pop);
		}
		{
			wxGridTableMessage push(this,
				wxGRIDTABLE_NOTIFY_ROWS_APPENDED,
				newcount);
			grid->ProcessTableMessage(push);
		}
	}
	else
	{
		//这里全部要自画一下？
	}
	for (int i = 0; i < oldselections.size(); i++)
	{
		int cursel = oldselections[i];
		if (cursel < newcount)
			grid->SelectRow(cursel, true);
	}
	grid->EndBatch();

}
