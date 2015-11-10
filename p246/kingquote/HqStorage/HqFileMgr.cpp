#include "stdafx.h"
#include "HqFileMgr.h"
#include "../base/pugixml.hpp"


HqFileMgr::HqFileMgr()
{
}


HqFileMgr::~HqFileMgr()
{
}

void HqFileMgr::load_cfg(const char* defdir, const std::string& rtdir, const std::string& datadir)
{
	const char *deffiles[] =
	{
		"market_1000.xml",
		"market_2000.xml",
		"market_4000.xml",
		"market_5000.xml",
		"market_8000.xml"
	};
	for (int i = 0; i < sizeof(deffiles) / sizeof(const char *); i++)
	{
		std::string deffile = defdir;
		deffile.append("\\");
		deffile.append(deffiles[i]);

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(deffile.c_str());
		if (!result)
			continue;

		pugi::xml_node root_node = doc.document_element();
		bool bactive = root_node.attribute("active").as_bool();
		if (bactive)
		{
			//std::string rtdir=root_node.attribute("rtdir").value();
			//std::string panhoudir=root_node.attribute("daydir").value();

			HqFile::set_default_dir(rtdir, datadir);
			load_hqstorage_group(root_node);
		}
	}
	return;
}

void HqFileMgr::load_hqstorage_group(pugi::xml_node& node_group)
{
	for (pugi::xml_node node_market = node_group.first_child(); node_market; node_market = node_market.next_sibling())
	{
		HqFile *curstorage = new HqFile();
		bool ret = curstorage->load_cfg(node_market);
		if (!ret)
		{
			delete curstorage;
			continue;
		}
		stk_file_map[curstorage->_hsid] = curstorage;
		new asio::detail::thread(boost::bind(&asio::io_service::run, &curstorage->ioqueue));
	}
}