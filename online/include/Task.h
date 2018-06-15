///
/// @file    Task.h
/// @author  ne0t12
/// @date    2018-05-25 14:09:32
///

#ifndef __TASK_H__
#define __TASK_H__

#include "net/TcpConnection.h"
#include "Dictionary.h"
#include "RedisCache.h"
#include <string.h>
#include <string>
#include <queue>
#include <vector>
#include <cctype>
using std::string;
using std::vector;
using std::priority_queue;

using namespace wd; 

namespace SPC
{

struct MyResult
{
	string candidate_word;
	int frequency;
	int distance;
};

struct MyCompare
{
	bool operator()(const MyResult & lhs, const MyResult & rhs)
	{
		if(lhs.distance > rhs.distance)
		{
			return true;
		}
		else if(lhs.distance == rhs.distance && lhs.frequency < rhs.frequency)
		{
			return true;
		}
		else if(lhs.distance == rhs.distance 
				&& lhs.frequency == rhs.frequency
				&& lhs.candidate_word > rhs.candidate_word)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

class Task
{
public:
	Task(const string & query, const TcpConnectionPtr & conn);
	Task(const string & query, const TcpConnectionPtr & conn, RedisCache & cache);

	void process(); //处理业务
	void query_in_dict(); //从词典查找
	bool query_in_cache(); //从缓存查找
	int calcDistance(string & lhs, string & rhs);//求最小编辑距离算法:
	int get_min(int a, int b, int c);
	vector<MyResult> * statistic(Dictionary * p);
	bool check_input();
	int get_bytes_size(char ch);
	int get_str_length(string & s);

private:
	string _query;
	TcpConnectionPtr _conn;
	RedisCache & _redisCache;
	priority_queue<MyResult, vector<MyResult>, MyCompare> _result_priority_que;
};

} // end of namespace SPC

#endif
