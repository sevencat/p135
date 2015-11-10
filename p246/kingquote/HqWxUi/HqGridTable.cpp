#include "HqGridTable.h"

HqGridTable::HqGridTable()
{
	m_colLabels.push_back("代码");
	m_colLabels.push_back("名称");
	m_colLabels.push_back("昨收价");
	m_colLabels.push_back("开盘价");
	m_colLabels.push_back("最高价");
	m_colLabels.push_back("最低价");
	m_colLabels.push_back("最新价");
	m_colLabels.push_back("成交量");
	m_colLabels.push_back("成交金额");
	m_colLabels.push_back("持仓量");
}


HqGridTable::~HqGridTable()
{
}

int HqGridTable::GetNumberRows()
{
	return 2;
}

int HqGridTable::GetNumberCols()
{
	return m_colLabels.size();
}

wxString HqGridTable::GetValue(int row, int col)
{
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