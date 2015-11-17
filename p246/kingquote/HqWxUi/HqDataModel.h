#pragma once
#include "MyVListCtrl.h"
#include "../HqStorage/HqFile.h"

enum HqField
{
	Fld_Code = 0,
	Fld_Name,
	FLd_LastMin,//最后分钟线
	Fld_LastTime,//最后时间
	Fld_Zdz,//涨跌值
	Fld_Zdf,//涨跌幅
	Fld_Preclose,
	Fld_Openpx,
	Fld_Highpx,
	Fld_Lowpx,
	Fld_Newpx,
	Fld_Vol,
	Fld_Money,
	Fld_Position,
	Fld_Max,
};

class HqDataModel : public MyListCtrlDataProvider
{
public:
	HqDataModel();
	~HqDataModel();

public:
	void sethq(HqFile *hf)
	{
		m_hq = hf;
	}



	HqFile *gethq()
	{
		return m_hq;
	}

	void InitLc(MyVListCtrl *mlc);

	//主要是要重载这个！！！
	virtual wxString OnGetItemText(long item, long column) const;

	virtual int OnGetItemCount();


	virtual wxListItemAttr *OnGetItemColumnAttr(long item, long WXUNUSED(column))
	{
		if (item % 2)
		{
			return &m_attrForOddRows;
		}
		else
		{
			return &m_defaultCellAttr;
		}
	}

protected:
	HqFile* m_hq;
	wxArrayString     m_colLabels;
	wxListItemAttr m_attrForOddRows;
	wxListItemAttr m_defaultCellAttr;
};

