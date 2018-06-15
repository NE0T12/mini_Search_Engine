 ///
 /// @file    Configuration.cc
 /// @author  ne0t12
 /// @date    2018-05-22 21:10:44
 ///

#include "../include/Configuration.h"
#include "../include/mylog.h"

using namespace MSE;
using std::cout;
using std::endl;

Configuration::Configuration(const string & filepath)
: _filepath(filepath)
{
	ifstream ifs(_filepath);
	if(!ifs.is_open())
	{
		logInfo("ifstream open error");
		exit(-1);
	}
	
	string line;
	while(getline(ifs, line))
	{
		string word;
		istringstream iss(line);
		pair<string, string> item;
		iss >> item.first;
		iss >> item.second;
		_configMap.insert(item);
	}
	ifs.close();
}

map<string, string> & Configuration::getConfigMap()
{
	return _configMap;
}
