 ///
 /// @file    WordQueryServer.h
 /// @author  ne0t12
 /// @date    2018-06-20 12:07:00
 ///
 
#ifndef __WORDQUERYSERVER_H__
#define __WORDQUERYSERVER_H__

#include "Configuration.h"
#include "net/TcpServer.h"
#include "threadpool/Threadpool.h"
#include "RedisCache.h"
#include "WordQuery.h"

using namespace wd;
namespace MSE
{

class WordQueryServer
{
public:
	WordQueryServer(const string & cfgFileName);
	void start();
	void onConnection(const TcpConnectionPtr & conn);
	void onMessage(const TcpConnectionPtr & conn);
	void onClose(const TcpConnectionPtr & conn);

private:
	Configuration _conf;
	TcpServer _tcpserver;
	Threadpool _threadpool;
	WordQuery _wordQuery;
	RedisCache _redisCache;
};

} //end of namespace MSE

#endif
