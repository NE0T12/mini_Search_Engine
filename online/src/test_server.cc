#include "../include/SpellCorrectServer.h"
#include <iostream>
using std::cout;
using std::endl;

using namespace SPC;
using namespace wd;

#define CONFIG_FILE_PATH "/home/ne0t12/C++/Project/spellCorrect/online/conf/online.conf"

int main(int argc, char const *argv[])
{
	SpellCorrectServer spellCorrectServer(string(CONFIG_FILE_PATH));
	spellCorrectServer.start();

    return 0;
}
