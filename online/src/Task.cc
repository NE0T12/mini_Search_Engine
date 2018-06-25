 ///
 /// @file    Task.cc
 /// @author  ne0t12
 /// @date    2018-05-25 14:09:01
 ///
 
#include "../include/Task.h"
#include "../include/mylog.h"
#include <hiredis/hiredis.h>
#include <algorithm>
#include <cctype>
#include <iterator>
#include <set>
using std::set;
using std::set_intersection;
using std::cout;
using std::endl;

using namespace MSE;

Task::Task(const string & query, const TcpConnectionPtr & conn, WordQuery & wordQuery, RedisCache & cache)
: _query(query)
, _conn(conn)
, _wordQuery(wordQuery)
, _redisCache(cache)
{

}

void Task::process()
{
	logInfo("%s, task is processing\n", _conn->toString().c_str());
	//if(!check_input())
	//{
	//	_conn->sendInLoop(string("Error:Wrong Input!"));
	//	return;
	//}
	
//	//1. 先到缓存中查找
	bool flag = false;
//	if(_redisCache.get_redis_state())
//	{
//		flag = query_in_cache();
//	}

	//2. 若缓存中没找到，再到词典中查找
	if(!flag)
	{
		query_in_lib();
	}
}

void Task::query_in_lib()
{
	string s = _wordQuery.doQuery(_query);
	cout << s << endl;
	_conn->sendInLoop(s);
	//_conn->sendInLoop(_wordQuery.doQuery(_query));


//	//1. 得到词典/索引   (方法一：通过单例模式)
//
//	//2...再做处理
//
//	//3. 将候选词放到优先级队列
//
//
//	//4. 取优先级队列中的前K个词作为返回给客户端的结果
//	//cout << "_result_priority_que.size = " << _result_priority_que.size() << endl;
//
//
//	//5. 将结果保存到cache中
//	if(_redisCache.get_redis_state())
//		_redisCache.set_redis_cache(_query, candidate_res);
//
//	//6. 返回结果给客户端
//	_conn->sendInLoop(candidate_res);
//
//	delete res; //释放内存
//	res = NULL;
}

//bool Task::query_in_cache()
//{
//	string res = _redisCache.get_cache_result(_query);
//	if(res == string())
//	{
//		return false;
//	}
//	else
//	{
//		_conn->sendInLoop(res);
//		return true;
//	}
//}


//bool Task::check_input()
//{
//	if(0 == _query.length())
//		return false;
//	vector<string> sp_punct = {
//		"，", "。", "！", "？", "……", "（", "）", 
//		"——", "【", "】", "、", "；", "：", "”", 
//		"《", "＊",	"－", "．",	"／", "０",	"１",	
//		"２", "３",	"４", "５",	"６", "７",	"８",	
//		"９", "＜",	"＞", "＠",	"Ｇ", "Ｊ", "Ｌ",	
//		"Ｍ", "Ｓ",	"Ｔ", "［",	"］", "ｃ",	"ｄ",	
//		"ｆ", "ｇ",	"ｈ", "ｉ",	"ｊ", "ｎ",	"ｏ",	
//		"ｔ", "ｕ", "ｗ", "ｘ",	"ｙ", "ｚ",	"～",	
//        "—",  "‘",  "’",  "“",  "…",  "″",	"Ⅰ",
//		"Ⅳ",  "Ⅵ",  "①",  "②",  "③",  "④",  "⑤",
//		"⑥",  "⑦",  "⑧",  "⑨",  "⑩",  "⑴",  "⑵",
//		"⑶",  "⑿",  "○",  "〈",	"〉", "〔",	"〕",		
//		"》"
//		//, ",",  ".",  "?",  "<",  ">",  "/",
//		//";",  ":",  "'",  "\"", "[",  "]",  "{",
//		//"}",  "\\", "|",  "~",  "`",  "!"  
//	};
//
//	string ch_str, query = _query;
//	for(size_t idx = 0; idx != query.length();)
//	{
//		int byte_size = get_bytes_size(query[idx]);
//		ch_str = query.substr(idx, byte_size);
//		if(3 == byte_size)
//		{
//			for(auto & it : sp_punct)
//			{
//				if(it == ch_str)
//					return false;
//			}
//		}
//		else if(1 == byte_size)
//		{
//			if(!isalpha(ch_str.c_str()[0]))
//				return false;
//		}
//		idx += byte_size;
//	}
//	return true;
//}
