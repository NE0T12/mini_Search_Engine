 ///
 /// @file    mylog.cc
 /// @author  ne0t12
 /// @date    2018-02-25 19:54:56
 ///

#include "../include/mylog.h"
#include <iostream>
using std::cout;

Mylog * Mylog::_pInstance = NULL;
Mylog::AutoRelease Mylog::_autoRelease;

Mylog * Mylog::getInstance() //单例模式
{
	if(NULL == _pInstance)
	{
		_pInstance = new Mylog;
	}
	return _pInstance;
}

Mylog::Mylog() //构造函数
: _category(log4cpp::Category::getRoot())
{
	log4cpp::Appender * ostreamAppender = new log4cpp::OstreamAppender("ostreamAppender", &cout);
	log4cpp::Appender * fileAppender = new log4cpp::FileAppender("fileAppender", "../log/mylog.log");
	
	log4cpp::PatternLayout * ptnLyt1 = new log4cpp::PatternLayout();
	ptnLyt1->setConversionPattern("%d [%p] %c: %m%n");
	ostreamAppender->setLayout(ptnLyt1);

	log4cpp::PatternLayout * ptnLyt2 = new log4cpp::PatternLayout();
	ptnLyt2->setConversionPattern("%d [%p] %c: %m%n");
	fileAppender->setLayout(ptnLyt2);

	_category.setPriority(log4cpp::Priority::DEBUG); //设置日志记录的默认优先级

	_category.addAppender(ostreamAppender);
	_category.addAppender(fileAppender);

	//_category.info("mylog create");
}

//设置日志优先级
void Mylog::setPriority(const char *priority)
{
	if(std::string(priority) == std::string("ERROR"))
	{
		_category.setPriority(log4cpp::Priority::ERROR);
	}
	else if(std::string(priority) == std::string("WARN"))
	{
		_category.setPriority(log4cpp::Priority::WARN);
	}
	else if(std::string(priority) == std::string("INFO"))
	{
		_category.setPriority(log4cpp::Priority::INFO);
	}
	else if(std::string(priority) == std::string("DEBUG"))
	{		
		_category.setPriority(log4cpp::Priority::DEBUG);
	}
	else
	{
		_category.setPriority(log4cpp::Priority::DEBUG);
	}
}

void Mylog::Info(const char * message)
{
	_category.info(message);
}

void Mylog::Error(const char * message)
{
	_category.error(message);
}

void Mylog::Warn(const char * message)
{
	_category.warn(message);
}

void Mylog::Debug(const char * message)
{
	_category.debug(message);
}
