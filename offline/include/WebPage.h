 ///
 /// @file    WebPage.h
 /// @author  ne0t12
 /// @date    2018-06-14 19:43:55
 ///
 
#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__

#include "Configuration.h"
#include "SplitTool.h"
#include <map>
#include <vector>
#include <string>
#include <set>

using std::vector;
using std::map;
using std::set;
using std::string;

namespace MSE
{
class WebPage
{
public:
	WebPage(string & doc, Configuration & conf, SplitTool & splitTool);
	int getDocId();
	string getDoc();
	map<string, int> & getWordsMap();

private:
	void processDoc(const string & doc);
	void calcTopK(vector<string> & wordsVec, int k, set<string> & stopWordList);

private:
	const int _TOPK_NUMBER = 20;
	Configuration & _conf;
	SplitTool & _splitTool;
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
