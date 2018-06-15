 ///
 /// @file    Dictionary.cc
 /// @author  ne0t12
 /// @date    2018-05-24 21:44:42
 ///
 
#include "../include/Dictionary.h"
#include "../include/mylog.h"
#include <pthread.h>

using namespace SPC;

Dictionary * Dictionary::_pInstance_dict = NULL;
pthread_once_t Dictionary::_once_control = PTHREAD_ONCE_INIT;

Dictionary * Dictionary::getInstance()
{
	::pthread_once(&_once_control, start);
	return _pInstance_dict;
}

void Dictionary::start()
{
	_pInstance_dict = new Dictionary();
	::atexit(destroy);
}

void Dictionary::destroy()
{
	if(_pInstance_dict)
		delete _pInstance_dict;
}

void Dictionary::init(const string & dictEnPath,  const string & dictZhPath, const string & indexPath)
{
	ifstream ifs_en_dict(dictEnPath);
	ifstream ifs_zh_dict(dictZhPath);
	ifstream ifs_index(indexPath);
	if(!ifs_en_dict || !ifs_zh_dict || !ifs_index)
	{
		logInfo("ifstream open error!");
		return;
	}
	
	string line;
	//读取词典, 必须先读英文词典再读中文词典
	while(getline(ifs_en_dict, line))
	{
		string word;
		int freq;
		istringstream iss(line);
		iss >> word;
		iss >> freq;
		_dictionary.push_back(make_pair(word, freq));
	}
	while(getline(ifs_zh_dict, line))
	{
		string word;
		int freq;
		istringstream iss(line);
		iss >> word;
		iss >> freq;
		_dictionary.push_back(make_pair(word, freq));
	}

	//读取索引
	while(getline(ifs_index, line)) //读入索引字符
	{
		string ch;
		set<int> tmpset;
		istringstream iss(line);
		iss >> ch;

		if(getline(ifs_index, line)) //读入索引标号
		{
			istringstream iss1(line);
			int num;
			while(iss1 >> num)
				tmpset.insert(num);
		}
		_index_table.insert(make_pair(ch, tmpset));
	}
}

vector<pair<string, int>> & Dictionary::getDict()
{
	return _dictionary;
}

unordered_map<string, set<int>> & Dictionary::getIndexTable()
{
	return _index_table;
}
