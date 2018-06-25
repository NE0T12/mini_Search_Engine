///
/// @file    Task.h
/// @author  ne0t12
/// @date    2018-05-25 14:09:32
///

#ifndef __TASK_H__
#define __TASK_H__

#include "net/TcpConnection.h"
#include "RedisCache.h"
#include "WordQuery.h"
#include <string.h>
#include <string>
#include <queue>
#include <vector>
#include <cctype>
using std::string;
using std::vector;
using std::priority_queue;

using namespace wd; 

namespace MSE
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
	Task(const string & query, const TcpConnectionPtr & conn, WordQuery & wordQuery, RedisCache & cache);

	void process(); //处理业务
	void query_in_lib(); //从网页库中查找
	bool query_in_cache(); //从缓存查找
	bool check_input();

private:
	string _query;
	TcpConnectionPtr _conn;
	WordQuery & _wordQuery;
	RedisCache & _redisCache;
	priority_queue<MyResult, vector<MyResult>, MyCompare> _result_priority_que;
};

} // end of namespace SPC

#endif
