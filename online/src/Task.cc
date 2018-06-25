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
	size_t pos = 0; 
	pos	= _query.find("\n");
	if(pos)	
		_query = _query.substr(0, pos);
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
	if(_redisCache.get_redis_state())
	{
		flag = query_in_cache();
	}

	//2. 若缓存中没找到，再到库中查找
	if(!flag)
	{
		query_in_lib();
	}
}

void Task::query_in_lib()
{
	string res = _wordQuery.doQuery(_query);
	_conn->sendInLoop(res);
	//_conn->sendInLoop(_wordQuery.doQuery(_query));

	//将结果保存到cache中
	if(_redisCache.get_redis_state())
		_redisCache.set_redis_cache(_query, res);
}

bool Task::query_in_cache()
{
	string res = _redisCache.get_cache_result(_query);
	if(res == string())
	{
		return false;
	}
	else
	{
		//cout << "redis" << endl;
		_conn->sendInLoop(res);
		return true;
	}
}


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
