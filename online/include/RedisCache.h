 ///
 /// @file    RedisCache.h
 /// @author  ne0t12
 /// @date    2018-06-08 16:43:56
 ///

#ifndef __REDISCACHE_H__
#define __REDISCACHE_H__

#include <hiredis/hiredis.h>
#include <string>
using std::string;

namespace MSE
{

class RedisCache
{
public:
	RedisCache(string ip = "127.0.0.1", int port = 6379, int db_num = 2);
	~RedisCache();

	redisReply * get_redis_reply(string query);
	string get_cache_result(string query);
	bool set_redis_cache(string key, string & value);
	int get_redis_state();

private:
	redisContext * _redisContext;
	redisReply * _redisReply;
	int _redisState;
};

} // end of namespace SPC

#endif
