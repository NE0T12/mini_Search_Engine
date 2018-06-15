 ///
 /// @file    PageLib.cc
 /// @author  ne0t12
 /// @date    2018-06-15 16:31:29
 ///
 
#include "../include/PageLib.h"
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include "../include/mylog.h"


using namespace MSE;
using std::ofstream;
using std::endl;
using std::cout;

PageLib::PageLib(Configuration & conf, DirScanner & dirScanner, FileProcessor & fileProcessor)
: _conf(conf)
, _dirScanner(dirScanner)
, _fileProcessor(fileProcessor)
{

}

void PageLib::create()
{
	auto & filepaths = _dirScanner.get_files_path();
	
	for(auto & filepath : filepaths)
	{
		_fileProcessor.process(filepath);
	}
	
	auto & doc_vec = _fileProcessor.get_doc();
	
	boost::regex re("<.+?>|\n");
	//boost::regex re("<.*?>");
	//boost::regex re("</?[^>]+>");
	int page_id = 1;
	for(size_t idx = 0; idx != doc_vec.size(); ++idx)
	{
		string page = 
			"<doc>" 
			"<docid>" + std::to_string(page_id++) + "</docid>"
			"<title>" + doc_vec[idx].title + "</title>"
			"<link>" + doc_vec[idx].link + "</link>"
			"<content>" + boost::regex_replace(doc_vec[idx].content, re, string())+ " </content>"
			"</doc>";
		_PageLib.push_back(page);
	}
}

void PageLib::store()
{
	ofstream ofs_ripepage(_conf.getConfigMap()["ripepage_lib_path"] + "/" + _conf.getConfigMap()["ripepage_lib_name"]);
	ofstream ofs_offset(_conf.getConfigMap()["offset_lib_path"] + "/" + _conf.getConfigMap()["offset_lib_name"]);
	if(!ofs_ripepage || !ofs_offset)
	{
		logInfo("ofstream open error");
		return;
	}
	int page_id = 0;
	long long int offset = 0;
	long long int length = 0;
	for(auto & page : _PageLib)
	{
		offset = ofs_ripepage.tellp();
		length = page.size();
		ofs_offset << page_id++ << "	"
				   << offset << " "
				   << length << endl;

		ofs_ripepage << page << endl;
	}

	ofs_ripepage.close();
	ofs_offset.close();
}
