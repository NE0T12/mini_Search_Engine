 ///
 /// @file    WordQuery.h
 /// @author  ne0t12
 /// @date    2018-06-20 19:23:14
 ///
 
#ifndef __WORDQUERY_H__
#define __WORDQUERY_H__

#include "Configuration.h"
#include "SplitTool.h"
#include "WebPage.h"
#include <unordered_map>
#include <map>
#include <set>
#include <vector>
using std::unordered_map;
using std::map;
using std::set;
using std::pair;
using std::vector;

namespace MSE
{

class WordQuery
{
public:
	WordQuery(Configuration & conf);
	string doQuery(const string & str); //执行查询，返回结果

private:
	bool get_query_words_weight_vec( vector<string> & queryWords, vector<double> & result_vec ); //计算查询词的权重值
	bool executeQuery(const vector<string> & queryWords, vector<pair<int, vector<double>>> & resultVec); //执行查询
	string createJson(vector<int> & docIdVec, const vector<string> & queryWords);
	string returnNoAnswer();
	void get_search_result_page(vector<double> & query_weight_vec, vector<pair<int, vector<double>>> & page_vec, vector<int> & result_docId_vec); //执行余弦相似度算法，获取搜索结果
	void loadLibrary();
	void process_word(string & word);

private:
	Configuration & _conf;
	SplitTool * _splitToolCppjieba;
	unordered_map<int, WebPage> _pageLib;
	unordered_map<int, pair<long, long>> _offsetLib;
	unordered_map<string, set<pair<int, double>>> _invertIndexTable;
};

} // end of namespace MSE

#endif
