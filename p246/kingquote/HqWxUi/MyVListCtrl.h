#pragma once
#include <wx/listctrl.h>

class MyListCtrlDataProvider;

class MyVListCtrl : public wxListCtrl
{
public:
	MyVListCtrl() {}

	MyVListCtrl(wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxLC_ICON,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxListCtrlNameStr)
		:wxListCtrl(parent,id,pos,size,style,validator,name)
	{
	}

	~MyVListCtrl(){}

	void set_dp(MyListCtrlDataProvider *dp)
	{
		_dp = dp;
	}

	MyListCtrlDataProvider *_dp;

public:
	// return the text for the given column of the given item
	virtual wxString OnGetItemText(long item, long column) const;

	// return the icon for the given item. In report view, OnGetItemImage will
	// only be called for the first column. See OnGetItemColumnImage for
	// details.
	virtual int OnGetItemImage(long item) const;

	// return the icon for the given item and column.
	virtual int OnGetItemColumnImage(long item, long column) const;

	// return the attribute for the given item and column (may return NULL if none)
	virtual wxListItemAttr *OnGetItemColumnAttr(long item, long WXUNUSED(column)) const;
};

class MyListCtrlDataProvider
{
public:
	MyListCtrlDataProvider()
	{
		lc = NULL;
	}
	void set_lc(MyVListCtrl *hlc)
	{
		lc = hlc;
	}
	MyVListCtrl *lc;

	void RefreshData()
	{
		lc->SetItemCount(OnGetItemCount());
		lc->Refresh();
	}

	//主要是要重载这个！！！
	virtual wxString OnGetItemText(long item, long column) const
	{
		return "";
	}

	virtual int OnGetItemCount()
	{
		return 0;
	}

	// return the icon for the given item. In report view, OnGetItemImage will
	// only be called for the first column. See OnGetItemColumnImage for
	// details.
	virtual int OnGetItemImage(long item) const
	{
		return -1;
	}

	// return the icon for the given item and column.
	virtual int OnGetItemColumnImage(long item, long column) const
	{
		return -1;
	}

	// return the attribute for the given item and column (may return NULL if none)
	virtual wxListItemAttr *OnGetItemColumnAttr(long item, long WXUNUSED(column))
	{
		return lc->OnGetItemAttr(item);
	}
};