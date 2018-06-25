 ///
 /// @file    WordQuery.cc
 /// @author  ne0t12
 /// @date    2018-06-20 22:43:41
 ///
 
#include "../include/WordQuery.h"
#include "../include/mylog.h"
#include "json/json.h"
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <functional>
#include <algorithm>
#include <iterator>

using namespace MSE;
using std::ifstream;
using std::istringstream;
using std::string;
using std::cout;
using std::endl;

WordQuery::WordQuery(Configuration & conf)
: _conf(conf)
{
	_splitToolCppjieba = (SplitTool *)new SplitToolCppJieba(conf);
	loadLibrary(); //读取偏移库、网页库和倒排索引表
}

void WordQuery::loadLibrary()
{
	ifstream ifs_pagelib(_conf.getConfigMap()["newripepage_lib_dir"] + "/" + _conf.getConfigMap()["newripepage_lib_name"]);
	ifstream ifs_offsetlib(_conf.getConfigMap()["newoffset_lib_dir"] + "/" + _conf.getConfigMap()["newoffset_lib_name"]);
	ifstream ifs_invertIndex(_conf.getConfigMap()["invertIndex_lib_dir"] + "/" + _conf.getConfigMap()["invertIndex_lib_name"]);
	if(!ifs_pagelib || !ifs_offsetlib || !ifs_invertIndex)
	{
		logInfo("ifstream open error");
		return;
	}

	int page_id = 0;
	long offset = 0;
	long length = 0;
	string line;
	while(getline(ifs_offsetlib, line))
	{
		//读入偏移库
		istringstream iss(line);
		iss >> page_id >> offset >> length;
		_offsetLib.insert(std::make_pair(page_id, std::make_pair(offset, length)));
		
		//读入网页库
		string doc;
		doc.resize(length, ' ');
		ifs_pagelib.seekg(offset, ifs_pagelib.beg);
		ifs_pagelib.read(&*doc.begin(), length);
		WebPage webPage(_conf, _splitToolCppjieba, doc);
		_pageLib.insert(std::make_pair(page_id, webPage));
	}

	//读入倒排索引表
	while(getline(ifs_invertIndex, line))
	{
		istringstream iss(line);
		string word;
		int docId;
		double weight;
		
		iss >> word;
		while(iss >> docId >> weight)
		{
			auto search = _invertIndexTable.find(word);
			if(search == _invertIndexTable.end())
			{
				set<pair<int, double>> tmpSet;
				tmpSet.insert(std::make_pair(docId, weight));
				_invertIndexTable.insert(std::make_pair(word, tmpSet));
			}
			else
			{
				search->second.insert(std::make_pair(docId, weight));
			}
		}
	}

	ifs_pagelib.close();
	ifs_offsetlib.close();
	ifs_invertIndex.close();
}

string WordQuery::doQuery(const string & str)
{
	auto query_word_vec = _splitToolCppjieba->cut(str); //将查询词句分词
	vector<double> query_weight_vec;
	bool weight_flag = false;
	weight_flag	= get_query_words_weight_vec(*query_word_vec, query_weight_vec);
	if(!weight_flag)
	{
		return returnNoAnswer();
	}
	else
	{
		vector<pair<int, vector<double>>> page_weight_vec;
		vector<int> result_docId_vec;

		executeQuery(*query_word_vec, page_weight_vec);
		get_search_result_page(query_weight_vec, page_weight_vec, result_docId_vec);
		
		vector<string> query_word = *query_word_vec;
		delete query_word_vec;

		return createJson(result_docId_vec, query_word);
	}
	return returnNoAnswer();
}

//把查询词当作一篇文档X来处理
bool WordQuery::get_query_words_weight_vec(vector<string> & queryWords, vector<double> & result_vec) //计算查询词的权重值
//bool WordQuery::get_query_words_weight_vec(vector<string> & queryWords, vector<double> & result_vec)
{
	//计算DF。 DF:Document Frequency, 某个词在所有文章中出现的次数，即包含该词语的文档数量
	map<string, pair<int, int>> mapDF; //pair中前一个int是DF，后一个int是TF（词频）
	for(auto & word : queryWords)
	{
		auto search = mapDF.find(word);
		if(search == mapDF.end())
		{
			auto search1 = _invertIndexTable.find(word);
			if(search1 == _invertIndexTable.end())
			{
				logInfo("not find, word = ", word);
				return false; //库中没找到这词语
			}
			mapDF.insert(std::make_pair(word, std::make_pair(_invertIndexTable[word].size(), 1)));
		}
		else
			++(search->second.second); //词频+1
	}

	unordered_map<string, double> mapWeight;
	int N = _pageLib.size(); //表示文档的总数或网页库的文档数
	for(auto & wordItem : mapDF)
	{
		int DF = wordItem.second.first; //Document Frequency, 某个词在所有文章中出现的次数，即包含该词语的文档数量
		int TF = wordItem.second.second; //Term Frequency, 某个词在文章中出现的次数
		double IDF = log2(N/(DF + 1)); //逆文档频率，表示该词对于该篇文章的重要性的一个系数		
		double w = TF * IDF; //词语的权重
		mapWeight.insert(std::make_pair(wordItem.first, w));
	}

	//分母
	double w_below = 0;
	for(auto & item : mapWeight)
	{
		w_below += pow(item.second, 2);
	}
	w_below = sqrt(w_below);

	//计算最终权重
	for(auto & word : queryWords)
	{
		auto & w = mapWeight[word];
		double w1 = w / w_below;

		result_vec.push_back(w1);
	}
	return true;
}


bool WordQuery::executeQuery(const vector<string> & queryWords, vector<pair<int, vector<double>>> & resultVec)
{
	//获取包含所有查询词的文档ID
	set<int> set_tmp1, set_tmp2, set_res; 
	for(auto & item : _invertIndexTable[queryWords[0]])
		set_res.insert(item.first);

	for(size_t idx = 1; idx != queryWords.size(); ++idx)
	{
		set_tmp1 = set_res;
		set_res.clear();
		for(auto & item : _invertIndexTable[queryWords[idx]])
			set_tmp2.insert(item.first);
		
		set_intersection(set_tmp1.begin(), set_tmp1.end(),
						 set_tmp2.begin(), set_tmp2.end(),
						 std::inserter(set_res, set_res.begin())
						);
	}
	
	//计算以上获得文档的对应查询词的权重向量
	for(auto & docId : set_res)
	{
		vector<double> weight_vec;
		for(auto & query_word : queryWords)
		{
			auto & set_invertIndex = _invertIndexTable[query_word];	
			for(auto & item : set_invertIndex)
			{
				if(docId == item.first) //对应ID
				{
					weight_vec.push_back(item.second);
					break;
				}
			}
		}
		resultVec.push_back(std::make_pair(docId, weight_vec));
	}
	return true;
}

string WordQuery::createJson(vector<int> & docIdVec, const vector<string> & queryWords)
{
	Json::Value json_root;
	Json::Value json_page;
	Json::Value json_arr;
	Json::StyledWriter json_writer;

	for(size_t idx = 0; idx != docIdVec.size() && idx < 10; ++idx) //获取前10篇文章
	{
		auto it = _pageLib.find(docIdVec[idx]);
		auto & webpage = it->second;
		json_page["title"] = webpage.getDocTitle();
		json_page["url"] = webpage.getDocUrl();
		json_page["summary"] = webpage.getSummary();
		//json_page["doc"] = webpage.getDoc();
		
		json_arr.append(json_page);
	}
	json_root["files"] = json_arr;
	
	return json_writer.write(json_root);	
	//return returnNoAnswer();
}

string WordQuery::returnNoAnswer()
{
	Json::Value json_root;
	Json::Value json_page;
	Json::Value json_arr;
	Json::StyledWriter json_writer;
	
	json_page["title"] = "no answer!";
	json_page["summary"] = "Sorry, we can't find it!";
	json_arr.append(json_page);
	json_root["files"] = json_arr;

	return json_writer.write(json_root);
}

bool myCompare(pair<int, double> & lhs, pair<int, double> & rhs)
{
	return lhs.second > rhs.second;
}
void WordQuery::get_search_result_page(vector<double> & query_weight_vec, vector<pair<int, vector<double>>> & page_vec, vector<int> & result_page_docId) //执行余弦相似度算法，获取搜索结果
{
	
	vector<pair<int, double>> page_rank_vec;
	double XY, abs_X, abs_Y, cos;
	XY = abs_X = abs_Y = cos = 0;

	for(size_t idx = 0; idx != query_weight_vec.size(); ++idx)
	{
		abs_X += pow(query_weight_vec[idx], 2);
	}
	abs_X = sqrt(abs_X);

	for(auto & page : page_vec)
	{
		for(size_t idx = 0; idx != page.second.size(); ++idx)
		{
			abs_Y += pow(page.second[idx], 2);
		}
		abs_Y= sqrt(abs_Y);
		
		for(size_t idx = 0; idx != query_weight_vec.size(); ++idx)
		{
			XY += query_weight_vec[idx] * page.second[idx];
		}

		cos = XY / (abs_X * abs_Y);

		page_rank_vec.push_back(std::make_pair(page.first, cos));
	}

	sort(page_rank_vec.begin(), page_rank_vec.end(), myCompare);
	for(auto & item : page_rank_vec)
		result_page_docId.push_back(item.first);
}
