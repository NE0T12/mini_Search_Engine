 ///
 /// @file    test.cc
 /// @author  ne0t12
 /// @date    2018-05-23 21:18:39
 ///
 
#define OFFLINE_CONF_FILE_PATH "/home/ne0t12/GitHub/mini_Search_Engine/offline/conf/offline.conf"

#include "../include/Configuration.h"
#include "../include/mylog.h"
#include "../include/PageLib.h"
#include "../include/PageLibPreprocessor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <memory>

using std::cout;
using std::endl;
using std::cin;
using std::unique_ptr;

using namespace MSE;

void test0()
{
	//配置类
	Configuration conf(string(OFFLINE_CONF_FILE_PATH));
	//auto configMap = configuration.getConfigMap(); //配置项与内容的map
	
	//1. 建立网页库和网页偏移库
	DirScanner dirScanner(conf);
	FileProcessor fileProcessor;
	PageLib pagelib(conf, dirScanner, fileProcessor);
		
	pagelib.create();
	pagelib.store();
	
	//2. 网页去重；建立倒排索引，并生成网页去重之后的网页库和网页偏移库
	PageLibPreprocessor pageLibPreprocessor(conf);
	pageLibPreprocessor.doProcess();
}

int main()
{
	test0();
	return 0;
}
