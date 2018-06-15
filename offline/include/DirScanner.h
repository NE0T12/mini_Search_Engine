 ///
 /// @file    DirScanner.h
 /// @author  ne0t12
 /// @date    2018-06-14 16:51:56
 ///
 
#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__

#include "Configuration.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace MSE
{

class DirScanner
{
public:
	DirScanner(Configuration & conf);
	vector<string> & get_files_path();
	void traverse(const char * dirname);

private:
	Configuration & _conf;
	vector<string> _vecFilePath;
};

} //end of namespace MSE



#endif
