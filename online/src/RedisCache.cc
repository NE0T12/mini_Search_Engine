 ///
 /// @file    RedisCache.cc
 /// @author  ne0t12
 /// @date    2018-06-08 16:50:24
 ///
 
#include "../include/RedisCache.h"
#include "../include/mylog.h"

using namespace MSE;
using std::cout;
using std::endl;

RedisCache::RedisCache(string ip, int port)
: _redisContext(redisConnect(ip.c_str(), port))
, _redisReply(NULL)
, _redisState(1)
{
	if (_redisContext == NULL || _redisContext->err) 
	{
		if (_redisContext) 
		{
			logError("Error: %s\n", _redisContext->errstr);
			// handle error
		} else 
		{
			logError("Can't allocate redis context\n");
		}
		_redisState = 0;
	}
}

RedisCache::~RedisCache()
{
	freeReplyObject(_redisReply);
	redisFree(_redisContext);
}


redisReply * RedisCache::get_redis_reply(string query)
{
	_redisReply = (redisReply *)redisCommand(_redisContext, "GET %s", query.c_str());
	return _redisReply;
}

string RedisCache::get_cache_result(string query)
{
	get_redis_reply(query);
	if(_redisReply != NULL)
	{
		if(_redisReply->type == REDIS_REPLY_STRING)
		{
			//cout << "string RedisCache::get_cache_result(string query)" << endl;
			return _redisReply->str;
		}
	}
	return string();
}

bool RedisCache::set_redis_cache(string key, string & value)
{
	if(_redisReply)
		delete _redisReply;
	//	freeReplyObject(_redisReply);
	_redisReply = (redisReply *)redisCommand(_redisContext, "SET %s %s", key.c_str(), value.c_str());
	if(_redisReply != NULL && _redisReply->type == REDIS_REPLY_STATUS)
		return true;
	else
		return false;
}

int RedisCache::get_redis_state()
{
	return _redisState;
}
