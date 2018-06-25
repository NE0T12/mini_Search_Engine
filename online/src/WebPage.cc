 ///
 /// @file    WebPage.cc
 /// @author  ne0t12
 /// @date    2018-06-15 19:50:48
 ///
 
#include "../include/WebPage.h"
#include <algorithm>

using namespace MSE;
using namespace tinyxml2;
using std::cout;
using std::endl;

namespace MSE
{

WebPage::WebPage(Configuration & conf, SplitTool * splitTool, string & doc )
: _conf(conf)
, _splitTool(splitTool)
, _doc(doc)
{
	processDoc();
}

bool operator == (WebPage & lhs, WebPage & rhs)	//判断两篇文档是否相等
{
	vector<string> vec_intersection;
	std::set_intersection(lhs._topWords.begin(), lhs._topWords.end(),
						  rhs._topWords.begin(), rhs._topWords.end(),
						  std::back_inserter(vec_intersection)			
						 );
	return vec_intersection.size() > 12 ? true : false;
}

bool operator < (WebPage & lhs, WebPage & rhs)	//对文档按Docid进行排序
{
	return lhs._docId < rhs._docId;
}

int WebPage::getDocId()
{
	return _docId;
}

string WebPage::getDoc()
{
	return _doc;
}

map<string, int> & WebPage::getWordsMap()
{
	return _wordsMap;
}

void WebPage::processDoc()
{
	XMLDocument xmlDoc;
	XMLError xmlErr = xmlDoc.Parse(_doc.c_str());

	if(XML_SUCCESS == xmlErr)
	{
		XMLElement * pDoc = xmlDoc.RootElement();
		if(pDoc == NULL || strcmp(pDoc->Value(), "doc"))
		{
			logError("xml doc not found");
			return;
		}

		XMLElement * element = pDoc->FirstChildElement("docid");
		if(element == NULL)
		{
			logError("xml docid not found");
			return;
		}
		_docId = ::atoi(element->GetText()); //docId

		element = element->NextSiblingElement("title");
		if(element == NULL)
		{
			logError("xml doctitle not found");
			return;
		}
		_docTitle = element->GetText();

		element = element->NextSiblingElement("link");
		if(element == NULL)
		{
			logError("xml docUrl not found");
			return;
		}
		_docUrl = element->GetText();
		
		element = element->NextSiblingElement("content");
		if(element == NULL)
		{
			logError("xml docContent not found");
			return;
		}
		_docContent = element->GetText();

		statistic(); //统计词频
		calcTopK(); //获取词频最高TopK
		make_summary(); //生成摘要
	}
	else
	{
		logError("xmlDoc string parse error");
	}
}

void WebPage::statistic()
{
	auto result = _splitTool->cut(_docTitle + " " + _docContent);
	auto stopWordList = _conf.getStopWordList();
	for(auto & word : *result)
	{
		auto search = stopWordList.find(word);
		if(search == stopWordList.end())
			push_dict(word);
	}
	delete result;
}

void WebPage::push_dict(const string & word)
{
	pair<string, int> item(word, 1);
	pair<map<string, int>::iterator, bool> res;
	res = _wordsMap.insert(item);
	if(!res.second)
		++(res.first)->second;
}


bool myCompare(pair<string, int> & lhs, pair<string, int> & rhs)
{
	return lhs.second > rhs.second;
}
void WebPage::calcTopK()
{
	vector<pair<string, int>> wordsVec(_wordsMap.begin(), _wordsMap.end());
	sort(wordsVec.begin(), wordsVec.end(), myCompare);
	for(int idx = 0; idx != TOPK_NUMBER && idx < wordsVec.size(); ++idx)
	{
		auto word = wordsVec[idx];
		_topWords.push_back(word.first);
	}
}

void WebPage::make_summary()
{
	_docSummary = _docContent.substr(0, 300);
}

string WebPage::getDocTitle()
{
	return _docTitle;
}

string WebPage::getSummary()
{
	return _docSummary;
}

string WebPage::getDocUrl()
{
	return _docUrl;
}

} // end of namespace MSE
