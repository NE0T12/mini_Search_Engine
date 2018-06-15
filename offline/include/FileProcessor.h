 ///
 /// @file    FileProcessor.h
 /// @author  ne0t12
 /// @date    2018-06-14 16:55:35
 ///

#ifndef __FILEPROCESSOR_H__
#define __FILEPROCESSOR_H__

#include "tinyxml2.h"
#include <string>
#include <fstream>
#include <vector>
using std::string;
using std::ifstream;
using std::vector;

namespace MSE
{

typedef struct RssItem
{
	string title;
	string link;
	string content;
}RssItem;

class FileProcessor
{

public:
	FileProcessor();
	void process(const string & filepath);
	vector<RssItem> & get_doc();

private:
	vector<RssItem> _rssItem_vec;
};

} //end of namespace MSE

#endif
