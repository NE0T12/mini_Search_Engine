 ///
 /// @file    Dictionary.h
 /// @author  ne0t12
 /// @date    2018-05-22 22:12:30
 ///
 
#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
using std::string;
using std::map;
using std::unordered_map;
using std::vector;
using std::pair;
using std::set;
using std::ifstream;
using std::istringstream;

namespace SPC
{

//词典类， 单例模式
class Dictionary
{
public:
	static Dictionary * getInstance();
	void init(const string & dictEnPath, const string & dictCnPath, const string & indexPath); //必须先读英文词典再读中文词典，按此顺序
	vector<pair<string, int>> & getDict();
	unordered_map<string, set<int>> & getIndexTable();
	static void start();
	static void destroy();

private:
	Dictionary(){}
	~Dictionary(){}

private:
	static Dictionary * _pInstance_dict;
	static pthread_once_t  _once_control;
	vector<pair<string, int>> _dictionary;
	unordered_map<string, set<int>> _index_table;
};


} //end of namespace SPC


#endif
