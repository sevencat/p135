#include "stdafx.h"
#include "MyVListCtrl.h"


wxString MyVListCtrl::OnGetItemText(long item, long column) const
{
	return _dp->OnGetItemText(item, column);
}

int MyVListCtrl::OnGetItemImage(long item) const
{
	return _dp->OnGetItemImage(item);
}

int MyVListCtrl::OnGetItemColumnImage(long item, long column) const
{
	return _dp->OnGetItemColumnImage(item, column);
}

wxListItemAttr* MyVListCtrl::OnGetItemColumnAttr(long item, long column) const
{
	return _dp->OnGetItemColumnAttr(item, column);
}