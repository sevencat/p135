#include "stdafx.h"
#include "HqDataModel.h"


HqDataModel::HqDataModel()
{
	m_attrForOddRows.SetBackgroundColour(*wxLIGHT_GREY);
	//m_attrForOddRows->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	//m_attrForOddRows->SetReadOnly(true);

	//m_defaultCellAttr.SetBackgroundColour(*wxLIGHT_GREY);
	//m_defaultCellAttr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	//m_defaultCellAttr->SetReadOnly(true);

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


HqDataModel::~HqDataModel()
{
}

void HqDataModel::InitLc(MyVListCtrl* mlc)
{
	this->set_lc(mlc);
	mlc->set_dp(this);
	lc->DeleteAllColumns();
	for (int i = 0; i < m_colLabels.size();i++)
	{
		wxListItem itemCol;
		itemCol.SetText(m_colLabels[i]);
		itemCol.SetImage(-1);
		itemCol.SetAlign(wxLIST_FORMAT_RIGHT);
		mlc->InsertColumn(i, itemCol);
	}
}

wxString from_time_t(uint32_t value);
wxString from_value(int value, int divvalue = 1000);
wxString from_complex_value(double value);
wxString HqDataModel::OnGetItemText(long row, long col) const
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
			return from_value(currec.newpx - currec.preclosepx, 1000);
		else
			return "";
	}
	case Fld_Zdf:
	{
		if (currec.preclosepx > 0.000001)
		{
			double zdz = (double)(currec.newpx - currec.preclosepx);
			double zdf = 100 * zdz / (double)currec.preclosepx;
			return from_value(zdf) + "%";
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

int HqDataModel::OnGetItemCount()
{
	if (m_hq == NULL)
		return 0;
	HqFileHdr *hdr = m_hq->stk_hdr;
	if (hdr == NULL)
		return 0;
	return hdr->curstkcount;
}