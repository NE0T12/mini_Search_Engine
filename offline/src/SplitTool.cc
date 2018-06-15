 ///
 /// @file    SplitTool.cc
 /// @author  ne0t12
 /// @date    2018-05-23 16:25:37
 ///
 
#include "../include/SplitTool.h"

using namespace MSE;

SplitToolCppJieba::SplitToolCppJieba(Configuration & conf)
: _conf(conf)
, _jieba(
		_conf.getConfigMap()["cppjieba_dict_path"],
		_conf.getConfigMap()["cppjieba_hmm_path"],
		_conf.getConfigMap()["cppjieba_user_dict_path"],
		_conf.getConfigMap()["cppjieba_idf_path"],
		_conf.getConfigMap()["cppjieba_stop_word_path"]
		)
{

}

vector<string> * SplitToolCppJieba::cut(const string & s)
{
//	auto configMap = _conf.getConfigMap();
//	
//	auto cppjieba_dict_path = configMap.find("cppjieba_dict_path");
//	auto cppjieba_hmm_path = configMap.find("cppjieba_hmm_path");
//	auto cppjieba_user_dict_path = configMap.find("cppjieba_user_dict_path");
//	auto cppjieba_idf_path = configMap.find("cppjieba_idf_path");
//	auto cppjieba_stop_word_path = configMap.find("cppjieba_stop_word_path");
	vector<string> * result_vector = new vector<string>;
	
	_jieba.Cut(s, *result_vector, true);

	return result_vector;
}
