 ///
 /// @file    WebPage.h
 /// @author  ne0t12
 /// @date    2018-06-14 19:43:55
 ///
 
#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__

#include "Configuration.h"
#include "SplitTool.h"
#include "tinyxml2.h"
#include "mylog.h"
#include <map>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <iterator>

using std::vector;
using std::map;
using std::set;
using std::string;

namespace MSE
{

class WebPage
{
	friend bool operator == (WebPage & lhs, WebPage & rhs);	//判断两篇文档是否相等
	friend bool operator < (WebPage & lhs, WebPage & rhs);	//对文档按Docid进行排序
public:
	const static int TOPK_NUMBER = 20;

public:
	WebPage(Configuration & conf, SplitTool * splitTool, string & doc);
	int getDocId();
	string getDoc();
	map<string, int> & getWordsMap();
	string getDocTitle();
	string getSummary();
	string getDocUrl();

private:
	void processDoc();
	void statistic(); //统计title和content中词频
	void push_dict(const string & word);
	void calcTopK();
	void make_summary(); //生成摘要

private:
	//Configuration & _conf;
	//SplitTool & _splitTool;
	//string & _doc;
	Configuration  _conf;
	SplitTool * _splitTool;
	string _doc;
	int _docId;
	string _docTitle;
	string _docUrl;
	string _docContent;
	string _docSummary;
	vector<string> _topWords; //词频最高前20词语
	map<string, int> _wordsMap; //保存每篇文档的所有词语和词频，不包括停用词
};

} // end of namespace MSE

#endif
