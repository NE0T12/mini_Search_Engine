 ///
 /// @file    SplitTool.h
 /// @author  ne0t12
 /// @date    2018-05-22 21:35:35
 ///

#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

#include "Configuration.h"
#include "cppjieba/Jieba.hpp"
#include <string>
#include <vector>
using std::string;
using std::vector;

namespace MSE
{

class SplitTool
{
public:
	SplitTool(){}
	virtual ~SplitTool(){}
	virtual vector<string> * cut(const string & sentence) = 0;
};

class SplitToolCppJieba
: public SplitTool
{
public:
	//SplitToolCppJieba(){}
	SplitToolCppJieba(Configuration & conf);
	virtual vector<string> * cut(const string & sentence);
	virtual ~SplitToolCppJieba(){}
	
private:
	Configuration & _conf;
	cppjieba::Jieba _jieba;
};

} //end of namespace MSE

#endif
