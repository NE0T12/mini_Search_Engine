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
	//获取配置项
	ifstream ifs(_filepath);
	if(!ifs.is_open())
	{
		logInfo("ifstream open error");
		exit(-1);
	}
	string line;
	while(getline(ifs, line))
	{
		istringstream iss(line);
		pair<string, string> item;
		iss >> item.first;
		iss >> item.second;
		_configMap.insert(item);
	}
	ifs.close();

	//获取停用词集
	ifstream ifs_stopword(_configMap["cppjieba_stop_word_path"]);
	if(!ifs_stopword.is_open())
	{
		logInfo("ifstream open error");
		exit(-1);
	}
	while(getline(ifs_stopword, line))
	{
		string word;
		_stopWordList.insert(line);
	}
	ifs_stopword.close();
}

map<string, string> & Configuration::getConfigMap()
{
	return _configMap;
}

set<string> & Configuration::getStopWordList()
{
	return _stopWordList;
}
