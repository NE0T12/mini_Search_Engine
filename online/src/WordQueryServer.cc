 ///
 /// @file    WordQueryServer.cc
 /// @author  ne0t12
 /// @date    2018-05-25 12:05:32
 ///
 
#include "../include/WordQueryServer.h"
#include "../include/Task.h"
#include "../include/mylog.h"
#include "../include/RedisCache.h"
#include <stdio.h>
#include <functional>
#include <map>
#include <string>
using std::map;
using std::string;
using std::cout;
using std::endl;

using namespace MSE;
using namespace wd;

WordQueryServer::WordQueryServer(const string & cfgFileName)
: _conf(cfgFileName)
, _tcpserver(_conf.getConfigMap()["ip"], stoi(_conf.getConfigMap()["port"]))
, _threadpool(stoi(_conf.getConfigMap()["thread_num"]), stoi(_conf.getConfigMap()["queue_size"]))
, _wordQuery(_conf)
, _redisCache(_conf.getConfigMap()["redis_connect_ip"], stoi(_conf.getConfigMap()["redis_connect_port"]))
{

}

void WordQueryServer::start()
{
	_threadpool.start();
	using namespace std::placeholders;
	_tcpserver.setConnectionCallback(std::bind(&WordQueryServer::onConnection, this, _1));
	_tcpserver.setMessageCallback(std::bind(&WordQueryServer::onMessage, this, _1));
	_tcpserver.setCloseCallback(std::bind(&WordQueryServer::onClose, this, _1));
	_tcpserver.start();
}

void WordQueryServer::onConnection(const TcpConnectionPtr & conn)
{
	logInfo(conn->toString().c_str());
	//conn->send("hello, welcome to WordQueryServer!\n");
}

//运行在IO线程
void WordQueryServer::onMessage(const TcpConnectionPtr & conn)
{
    std::string query(conn->receive());
	size_t pos = 0; 
	pos	= query.find("\n");
	if(pos)	
		query = query.substr(0, pos);
	//cout << "query word = " << query << endl;
	//cout << "query word size = " << query.size() << endl;

	Task task(query, conn, _wordQuery, _redisCache);
	_threadpool.addTask(std::bind(&Task::process, task));
	logInfo("%s, add task to threadpool\n", conn->toString().c_str());
}

void WordQueryServer::onClose(const TcpConnectionPtr & conn)
{
	logInfo("%s close.\n", conn->toString().c_str());
}
