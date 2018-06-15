 ///
 /// @file    Configuration.h
 /// @author  ne0t12
 /// @date    2018-05-22 21:06:22
 ///

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include <map>
#include <fstream>
#include <sstream>
using std::string;
using std::map;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::ostringstream;
using std::pair;

namespace SPC
{

class Configuration
{
public:
	Configuration(const string & filepath);
	map<string, string> & getConfigMap();

private:
	string _filepath;
	map<string, string> _configMap;
};

} // end of namespace

#endif
