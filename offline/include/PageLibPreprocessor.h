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
#include <memory>

using std::unordered_map;
using std::vector;
using std::string;
using std::pair;
using std::shared_ptr;

namespace MSE
{

class PageLibPreprocessor
{
public:
	PageLibPreprocessor(Configuration & conf);
	~PageLibPreprocessor();
	
	void doProcess();

private:
	void readInfoFromFile(); //根据配置信息读取网页库和位置偏移库的内容
	void cutRedundantPages(); //对冗余网页进行去重
	void buildInvertIndexTable(); //建立倒排索引表
	void store();

private:
	Configuration & _conf;
	SplitTool * _splitToolCppjieba; //分词对象
	vector<WebPage> _pageLib;
	unordered_map<int, pair<long, long>> _offsetLib; ///////////作废待定
	unordered_map<string, vector<pair<int, double>>> _invertIndexTable; //倒排索引表
};

} // end of namespace MSE

#endif
