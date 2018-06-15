 ///
 /// @file    PageLibPreprocessor.h
 /// @author  ne0t12
 /// @date    2018-06-14 19:45:16
 ///
 
#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__

#include "Configuration.h"
#include "SplitTool.h"
#include "WebPage.h"
#include <vector>
#include <unordered_map>
#include <string>

using std::unordered_map;
using std::vector;
using std::string;
using std::pair;

namespace MSE
{

class PageLibPreprocessor
{
public:
	PageLibPreprocessor(Configuration & conf);
	void doProcess();

private:
	void readInfoFromFile();
	void cutRedundantPages(); //对冗余网页进行去重
	void buildInvertIndexTable();
	void store();

private:
	Configuration & _conf;
	SplitToolCppjieba _splitToolCppjieba; //分词对象
	vector<WebPage> _pageLib;
	unordered_map<int, pair<int, int>> _offsetLib;
	unordered_map<string, vector<pair<int, double>>> _invertIndexTable; //倒排索引表
};

} // end of namespace MSE

#endif
