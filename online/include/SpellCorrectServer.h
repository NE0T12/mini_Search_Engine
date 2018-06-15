 ///
 /// @file    SpellCorrectServer.h
 /// @author  ne0t12
 /// @date    2018-05-25 12:07:00
 ///
 
#ifndef __SPELLCORRECTSERVER_H__
#define __SPELLCORRECTSERVER_H__

#include "Configuration.h"
#include "net/TcpServer.h"
#include "threadpool/Threadpool.h"
#include "RedisCache.h"

using namespace wd;
namespace SPC
{

class SpellCorrectServer
{
public:
	SpellCorrectServer(const string & cfgFileName);
	void start();
	void onConnection(const TcpConnectionPtr & conn);
	void onMessage(const TcpConnectionPtr & conn);
	void onClose(const TcpConnectionPtr & conn);

private:
	Configuration _conf;
	TcpServer _tcpserver;
	Threadpool _threadpool;
	RedisCache _redisCache;
};


} //end of namespace SPC


#endif
