#pragma once


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
	int _shoupan_time;
};

