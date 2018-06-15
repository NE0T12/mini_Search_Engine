 ///
 /// @file    test.cc
 /// @author  ne0t12
 /// @date    2018-05-23 21:18:39
 ///
 
#define OFFLINE_CONF_FILE_PATH "/home/ne0t12/GitHub/mini_Search_Engine/offline/conf/offline.conf"

#include "../include/Configuration.h"
#include "../include/mylog.h"
#include "../include/PageLib.h"
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
	DirScanner dirScanner(conf);
	FileProcessor fileProcessor;
	PageLib pagelib(conf, dirScanner, fileProcessor);
		
	pagelib.create();
	pagelib.store();
}

int main()
{
	test0();
	return 0;
}
