 ///
 /// @file    Task.cc
 /// @author  ne0t12
 /// @date    2018-05-25 14:09:01
 ///
 
#include "../include/Task.h"
#include "../include/mylog.h"
#include "../include/Dictionary.h"
#include <hiredis/hiredis.h>
#include <algorithm>
#include <cctype>
#include <iterator>
#include <set>
using std::set;
using std::set_intersection;

	using std::cout;
	using std::endl;
	
using namespace SPC;

//Task::Task(const string & query, const TcpConnectionPtr & conn)
//: _query(query)
//, _conn(conn)
//{
//	
//}

Task::Task(const string & query, const TcpConnectionPtr & conn, RedisCache & cache)
: _query(query)
, _conn(conn)
, _redisCache(cache)
{

}

void Task::process()
{
	logInfo("%s, task is processing\n", _conn->toString().c_str());
	if(!check_input())
	{
		_conn->sendInLoop(string("Error:Wrong Input!"));
		return;
	}
	
	//1. 先到缓存中查找
	bool flag = false;
	if(_redisCache.get_redis_state())
	{
		flag = query_in_cache();
	}

	//2. 若缓存中没找到，再到词典中查找
	if(!flag)
	{
		query_in_dict();
	}
}

void Task::query_in_dict()
{
	//1. 得到词典/索引   (方法一：通过单例模式)
	Dictionary *pDictionary = Dictionary::getInstance();	

	//2...再做处理
	vector<MyResult> * res = statistic(pDictionary);

	//3. 将候选词放到优先级队列
	for(auto & item : *res) //
	{
		_result_priority_que.push(item);
		//cout << "word = " << item.candidate_word << " , distance = " << item.distance << " ,freq = " << item.frequency << endl;
	}

	//4. 取优先级队列中的前K个词作为返回给客户端的结果
	//cout << "_result_priority_que.size = " << _result_priority_que.size() << endl;
	size_t size = _result_priority_que.size(); //注意优先队列里面的size()在删除元素后会动态改变size大小
	if(0 == size)
	{
		_conn->sendInLoop(string("Sorry! No result for the word \"" + _query + "\""));
		return;
	}
	string candidate_res;
	for(size_t idx = 0; idx < 5 && idx < size; ++idx) //获取前5个候选词
	{
		MyResult my_res = _result_priority_que.top();
		//logDebug(my_res.candidate_word);
		candidate_res.append(my_res.candidate_word + "  ");

		_result_priority_que.pop();
	}

	//5. 将结果保存到cache中
	if(_redisCache.get_redis_state())
		_redisCache.set_redis_cache(_query, candidate_res);

	//6. 返回结果给客户端
	_conn->sendInLoop(candidate_res);

	delete res; //释放内存
	res = NULL;
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
		_conn->sendInLoop(res);
		return true;
	}
}


bool Task::check_input()
{
	if(0 == _query.length())
		return false;
	vector<string> sp_punct = {
		"，", "。", "！", "？", "……", "（", "）", 
		"——", "【", "】", "、", "；", "：", "”", 
		"《", "＊",	"－", "．",	"／", "０",	"１",	
		"２", "３",	"４", "５",	"６", "７",	"８",	
		"９", "＜",	"＞", "＠",	"Ｇ", "Ｊ", "Ｌ",	
		"Ｍ", "Ｓ",	"Ｔ", "［",	"］", "ｃ",	"ｄ",	
		"ｆ", "ｇ",	"ｈ", "ｉ",	"ｊ", "ｎ",	"ｏ",	
		"ｔ", "ｕ", "ｗ", "ｘ",	"ｙ", "ｚ",	"～",	
        "—",  "‘",  "’",  "“",  "…",  "″",	"Ⅰ",
		"Ⅳ",  "Ⅵ",  "①",  "②",  "③",  "④",  "⑤",
		"⑥",  "⑦",  "⑧",  "⑨",  "⑩",  "⑴",  "⑵",
		"⑶",  "⑿",  "○",  "〈",	"〉", "〔",	"〕",		
		"》"
		//, ",",  ".",  "?",  "<",  ">",  "/",
		//";",  ":",  "'",  "\"", "[",  "]",  "{",
		//"}",  "\\", "|",  "~",  "`",  "!"  
	};

	string ch_str, query = _query;
	for(size_t idx = 0; idx != query.length();)
	{
		int byte_size = get_bytes_size(query[idx]);
		ch_str = query.substr(idx, byte_size);
		if(3 == byte_size)
		{
			for(auto & it : sp_punct)
			{
				if(it == ch_str)
					return false;
			}
		}
		else if(1 == byte_size)
		{
			if(!isalpha(ch_str.c_str()[0]))
				return false;
		}
		idx += byte_size;
	}
	return true;
}

vector<MyResult> * Task::statistic(Dictionary * pDict)
{
	auto dictionary = pDict->getDict();
	auto index = pDict->getIndexTable();
	set<int> res_set, tmp_set, tmp2_set;
	vector<MyResult> * res_vector = new vector<MyResult>;

	size_t idx = 0;
	string query = _query;

	//查找索引
	int byte_size = get_bytes_size(query[idx]);
	res_set = index[query.substr(idx, byte_size)];
	idx += byte_size; 

	for(; idx != query.size();)
	{
		tmp_set = res_set;
		res_set.clear();
	    byte_size = get_bytes_size(query[idx]);
		tmp2_set = index[query.substr(idx, byte_size)];
		idx += byte_size;
		
		set_intersection(tmp_set.begin(), tmp_set.end(), //---> 用户输入助手
		//set_union(tmp_set.begin(), tmp_set.end(),		 //---> 与用户输入词相似词汇
						 tmp2_set.begin(), tmp2_set.end(),
						 std::inserter(res_set, res_set.begin())
						);
	}

	//获取候选词
	for(auto & it : res_set)
	{
		int dis = calcDistance(query, dictionary[it].first);
		if(dis >= 3)
			continue;
		MyResult tmp_MyResult;
		tmp_MyResult.candidate_word = dictionary[it].first;
		tmp_MyResult.frequency = dictionary[it].second;
		tmp_MyResult.distance = dis;
		res_vector->push_back(tmp_MyResult);
	}
	return res_vector;
}

int Task::calcDistance(string & lhs, string & rhs) //最小编辑距离算法，支持中文
{
	//int n = lhs.size();
	//int m = rhs.size();
	int n = get_str_length(lhs);
	int m = get_str_length(rhs);
	int distance[n + 1][m + 1]; //最小距离数组，动态规划

	//初始化
	for(int i = 0; i <= n; ++i)
	{
		distance[i][0] = i;
	}
	for(int j = 0; j <= m; ++j)
	{
		distance[0][j] = j;
	}

	string lhs_sub, rhs_sub;
	for(int i = 1, str_i = 0; i <= n; ++i)
	{
		int nBytes = get_bytes_size(lhs[str_i]);
		lhs_sub = lhs.substr(str_i, nBytes); 
		str_i += nBytes; 
		for(int j = 1, str_j = 0; j <= m; ++j)
		{
			nBytes = get_bytes_size(rhs[str_j]);
			rhs_sub = rhs.substr(str_j, nBytes);
			str_j += nBytes;
			if(lhs_sub == rhs_sub)
			{
				distance[i][j] = distance[i-1][j-1]; //替换成本
			}
			else
			{
				distance[i][j] =  get_min(distance[i-1][j] + 1, //插入成本
										  distance[i-1][j-1] + 2, //替换成本
										  distance[i][j-1] + 1 //删除成本
									 	  );
			}
		}
	}
	return distance[n][m];
}

inline
int Task::get_bytes_size(char ch)
{
	int size = 1;
	if(ch & (1 << 7))
	{
		for(int idx = 6; idx > 1; --idx)
		{
			if(ch & (1 << idx))
				++size;
			else
				break;
		}
	}
	return size;
}

inline
int Task::get_str_length(string & s)
{
	int len = 0;
	int nBytes = 0;
	for(size_t idx = 0; idx != s.size(); idx += nBytes)
	{
		nBytes = get_bytes_size(s.c_str()[idx]);
		if(nBytes)
			++len;
	}
	return len;
}


inline
int Task::get_min(int a, int b, int c)
{
	return a < (b < c ? b : c) ? a : (b < c ? b : c);
}
