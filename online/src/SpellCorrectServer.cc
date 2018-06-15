 ///
 /// @file    SpellCorrectServer.cc
 /// @author  ne0t12
 /// @date    2018-05-25 12:05:32
 ///
 
#include "../include/SpellCorrectServer.h"
#include "../include/Task.h"
#include "../include/Dictionary.h"
#include "../include/mylog.h"
#include <stdio.h>
#include <functional>
#include <map>
#include <string>
using std::map;
using std::string;

using namespace SPC;
using namespace wd;

SpellCorrectServer::SpellCorrectServer(const string & cfgFileName)
: _conf(cfgFileName)
, _tcpserver(_conf.getConfigMap()["ip"], stoi(_conf.getConfigMap()["port"]))
, _threadpool(stoi(_conf.getConfigMap()["thread_num"]), stoi(_conf.getConfigMap()["queue_size"]))
, _redisCache(_conf.getConfigMap()["redis_connect_ip"], stoi(_conf.getConfigMap()["redis_connect_port"]))
{
	Dictionary * dict = Dictionary::getInstance();
	dict->init(
			_conf.getConfigMap()["dictionary_en_name"],
			_conf.getConfigMap()["dictionary_zh_name"],
			_conf.getConfigMap()["index_table_name"]
			);
}

void SpellCorrectServer::start()
{	
	_threadpool.start();
	using namespace std::placeholders;
	_tcpserver.setConnectionCallback(std::bind(&SpellCorrectServer::onConnection, this, _1));
	_tcpserver.setMessageCallback(std::bind(&SpellCorrectServer::onMessage, this, _1));
	_tcpserver.setCloseCallback(std::bind(&SpellCorrectServer::onClose, this, _1));
	_tcpserver.start();
}

void SpellCorrectServer::onConnection(const TcpConnectionPtr & conn)
{
	logInfo(conn->toString().c_str());
	conn->send("hello, welcome to SpellCorrectServer!\n");
}

//运行在IO线程
void SpellCorrectServer::onMessage(const TcpConnectionPtr & conn)
{
    std::string query(conn->receive());
	
	Task task(query, conn, _redisCache);
	_threadpool.addTask(std::bind(&Task::process, task));
	logInfo("%s, add task to threadpool\n", conn->toString().c_str());
}

void SpellCorrectServer::onClose(const TcpConnectionPtr & conn)
{
	logInfo("%s close.\n", conn->toString().c_str());
}
