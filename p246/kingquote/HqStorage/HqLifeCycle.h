#pragma once
#include "../base/dtz.h"


namespace pugi
{
	class xml_node;
}

class HqLifeCycle
{
public:
	HqLifeCycle();
	~HqLifeCycle();
	void load_cfg(pugi::xml_node xml_nodes);

	void on_localdt_changed(_datetime_t datetime_t)
	{
	}

	int _shoupan_time;
};

