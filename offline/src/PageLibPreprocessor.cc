 ///
 /// @file    PageLibPreprocessor.cc
 /// @author  ne0t12
 /// @date    2018-06-15 23:39:34
 ///
 
#include "../include/PageLibPreprocessor.h"
#include "../include/mylog.h"
#include <sstream>
#include <fstream>
#include <utility>
#include <cmath>

using namespace MSE;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::pair;
using std::endl;
using std::cout;

PageLibPreprocessor::PageLibPreprocessor(Configuration & conf)
: _conf(conf)
{
	_splitToolCppjieba = (SplitTool *) new SplitToolCppJieba(conf);
}

PageLibPreprocessor::~PageLibPreprocessor()
{
	delete _splitToolCppjieba;
}

void PageLibPreprocessor::doProcess()
{
	readInfoFromFile();
	cutRedundantPages();
	buildInvertIndexTable();
	store();
}

void PageLibPreprocessor::readInfoFromFile()
{
	ifstream ifs_pageLib(_conf.getConfigMap()["ripepage_lib_dir"] + "/" + _conf.getConfigMap()["ripepage_lib_name"]);
	ifstream ifs_offsetLib(_conf.getConfigMap()["offset_lib_dir"] + "/" + _conf.getConfigMap()["offset_lib_name"]);
	if(!ifs_pageLib || !ifs_offsetLib)
	{
		logInfo("ifstream open error");
		return;
	}
	
	string line;
	int page_id;
	long offset, length;
	while(getline(ifs_offsetLib, line)) 
	{
		//读入位置偏移库
		istringstream iss(line);
		iss >> page_id >> offset >> length;
		//可能没必要保存
		_offsetLib.insert(std::make_pair(page_id, std::make_pair(offset, length)));
		
		//读入网页库
		string doc;
		doc.resize(length, ' ');
		ifs_pageLib.seekg(offset, ifs_pageLib.beg);
		ifs_pageLib.read(&*doc.begin(), length);
		
		WebPage webPage(_conf, _splitToolCppjieba, doc);
		_pageLib.push_back(webPage);
	}

	ifs_pageLib.close();
	ifs_offsetLib.close();
}

void PageLibPreprocessor::cutRedundantPages()
{
	for(size_t i = 0; i != _pageLib.size(); ++i)
	{
		for(size_t j = i + 1; j != _pageLib.size();)
		{
			if(_pageLib[i] == _pageLib[j])
			{	
				_pageLib[j] = _pageLib[_pageLib.size() - 1];
				_pageLib.pop_back();
			}
			else
				++j;
		}
	}
}

void PageLibPreprocessor::buildInvertIndexTable()
{
	//计算DF。 DF:Document Frequency, 某个词在所有文章中出现的次数，即包含该词语的文档数量
	unordered_map<string, int> mapDF;
	for(auto & webpage : _pageLib)
	{
		auto & wordsMap = webpage.getWordsMap();
		//auto docId = webpage.getDocId();
		for(auto & wordItem : wordsMap)
		{
			auto search = mapDF.find(wordItem.first);
			if(search == mapDF.end())
				mapDF.insert(std::make_pair(wordItem.first, 1));
			else
				search->second++;
		}
	}
	
	int N = _pageLib.size(); //表示文档的总数或网页库的文档数
	for(auto & webpage : _pageLib)
	{
		auto & wordsMap = webpage.getWordsMap();
		auto docId = webpage.getDocId();
		unordered_map<string, double> mapWeight;
		
		for(auto & wordItem : wordsMap)
		{
			int TF = wordItem.second; //Term Frequency, 某个词在文章中出现的次数
			int DF = mapDF[wordItem.first]; //Document Frequency, 某个词在所有文章中出现的次数，即包含该词语的文档数量
			double IDF = log2(N/(DF + 1)); //逆文档频率，表示该词对于该篇文章的重要性的一个系数		
			double w = TF * IDF; //词语的权重
			mapWeight.insert(std::make_pair(wordItem.first, w));
		}

		double w_below = 0;
		for(auto & item : mapWeight)
		{
			w_below += pow(item.second, 2);
		}
		w_below = sqrt(w_below);

		for(auto & wordItem : wordsMap)
		{
			auto & w = mapWeight[wordItem.first];
			double w1 = w / w_below;

			auto & map_value = _invertIndexTable[wordItem.first];
			map_value.push_back(std::make_pair(docId, w1));
		}
	}
}

void PageLibPreprocessor::store()
{
	ofstream ofs_newPageLib(_conf.getConfigMap()["newripepage_lib_dir"] + "/" + _conf.getConfigMap()["newripepage_lib_name"]);
	ofstream ofs_newOffsetLib(_conf.getConfigMap()["newoffset_lib_dir"] + "/" + _conf.getConfigMap()["newoffset_lib_name"]);
	ofstream ofs_invertIndexLib(_conf.getConfigMap()["invertIndex_lib_dir"] + "/" + _conf.getConfigMap()["invertIndex_lib_name"]);
	if(!ofs_newPageLib || !ofs_newOffsetLib || !ofs_invertIndexLib)
	{
		logInfo("ofstream open error");
		return;
	}
	
	long offset = 0;
	long length = 0;
	for(auto & page : _pageLib)
	{
		
		//存储偏移库
		offset = ofs_newPageLib.tellp();
		length = page.getDoc().size();
	
		ofs_newOffsetLib << page.getDocId() << "\t" 
						 << offset << " "
						 << length << endl;
		
		//存储网页库
		ofs_newPageLib << page.getDoc() << endl;
	}
	
	//存储倒排索引表
	for(auto & invertIndex : _invertIndexTable)
	{
		ofs_invertIndexLib << invertIndex.first << "\t";
		for(auto & pair : invertIndex.second)
		{
			ofs_invertIndexLib << pair.first << " " 
							   << pair.second << "  ";
		}
		ofs_invertIndexLib << endl;
	}

	ofs_newPageLib.close();
	ofs_newOffsetLib.close();
	ofs_invertIndexLib.close();
}
