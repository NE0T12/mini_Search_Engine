 ///
 /// @file    PageLib.h
 /// @author  ne0t12
 /// @date    2018-06-14 19:42:39
 ///
 
#ifndef __PAGELIB_H__
#define __PAGELIB_H__

#include "Configuration.h"
#include "DirScanner.h"
#include "FileProcessor.h"
#include <vector>
#include <string>
#include <map>

using std::string;
using std::map;
using std::vector;
using std::pair;

namespace MSE
{

class PageLib
{
public:
	PageLib(Configuration & conf, DirScanner & dirScanner, FileProcessor & fileProcessor);
	void create();
	void store();

private:
	Configuration & _conf;
	DirScanner & _dirScanner;
	FileProcessor & _fileProcessor;
	vector<string> _PageLib; 
	map<int, pair<int, int>> _offsetLib;
};

}	//end of namespace MSE


#endif
