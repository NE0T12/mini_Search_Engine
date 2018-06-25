#include "../include/WordQueryServer.h"
#include <iostream>
using std::cout;
using std::endl;

using namespace MSE;
using namespace wd;

#define CONFIG_FILE_PATH "/home/ne0t12/GitHub/mini_Search_Engine/online/conf/online.conf"

int main(int argc, char const *argv[])
{
	WordQueryServer wordQueryServer(string(CONFIG_FILE_PATH));
	wordQueryServer.start();

    return 0;
}
