 ///
 /// @file    mylog.h
 /// @author  ne0t12
 /// @date    2018-02-25 19:56:04
 ///
#ifndef __MYLOG_H__
#define __MYLOG_H__

#include <log4cpp/Category.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/OstreamAppender.hh>                                                                                                                      
#include <log4cpp/FileAppender.hh>

class Mylog
{
	class AutoRelease
	{
	public:
		~AutoRelease()
		{
			if(_pInstance)
			{
				//cout << "~AutoRelease()" << endl;
				//_pInstance->_category.info("mylog destroy");
				//_pInstance->_category.shutdown();
				delete _pInstance;
			}
		}
	};

public:
	static Mylog * getInstance();
	void setPriority(const char * priority);
	
	void Info(const char * message);
	template <typename... Args>
	void Info(const char * message, Args... args)
	{
		_category.info(message, args...);
	}

	void Error(const char * message);
	template <typename... Args>
	void Error(const char * message, Args... args)
	{
		_category.error(message, args...);
	}
	
	void Warn(const char * message);
	template <typename... Args>
	void Warn(const char * message, Args... args)
	{
		_category.warn(message, args...);
	}
	
	void Debug(const char * message);
	template <typename... Args>
	void Debug(const char * message, Args... args)
	{
		_category.debug(message, args...);
	}

private:
	static Mylog * _pInstance;
	static  AutoRelease _autoRelease;
	log4cpp::Category & _category;
	Mylog(); //单例模式：将构造函数私有化
	~Mylog(){};
};

//将整形行号转化为string类型
inline std::string int2string(int line)
{
	std::ostringstream oss;
	oss << line;
	return oss.str();
}

//定义一个在日志后添加 文件名、函数名、行号 的宏定义
//宏定义：文件名__FILE__，函数名__func__，行号__LINE__是编译器实现的
//其中__FILE__和__func__是string类型，__LINE__是整形
#define prefix(msg) std::string("").append(" #")\
	.append(__FILE__).append(":").append(__func__)\
	.append(":").append(int2string(__LINE__))\
	.append("# ").append(": ").append(msg).c_str()


//用宏给 调用类中函数成员取别名，可直接使用
#define logPriority(priority) Mylog::getInstance()->setPriority(priority)
#define logInfo(msg, ...) Mylog::getInstance()->Info(prefix(msg), ##__VA_ARGS__)
#define logError(msg, ...) Mylog::getInstance()->Error(prefix(msg), ##__VA_ARGS__)
#define logWarn(msg, ...) Mylog::getInstance()->Warn(prefix(msg), ##__VA_ARGS__)
#define logDebug(msg, ...) Mylog::getInstance()->Debug(prefix(msg), ##__VA_ARGS__)

#endif
