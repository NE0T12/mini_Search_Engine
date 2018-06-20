 ///
 /// @file    DirScanner.cc
 /// @author  ne0t12
 /// @date    2018-06-14 23:29:57
 ///
 
#include "../include/DirScanner.h"
#include "../include/mylog.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

using namespace MSE;

DirScanner::DirScanner(Configuration & conf)
: _conf(conf)
{
	traverse(_conf.getConfigMap()["xml_files_dir"].c_str());
}

vector<string> & DirScanner::get_files_path()
{
	return _vecFilePath;
}

void DirScanner::traverse(const char * dir_name)
{
	struct dirent * filename;     
	DIR * pDir;                    
	
	pDir = ::opendir( dir_name );  
	if( NULL == pDir )  
	{  
		return;  
	}

	while( ( filename = readdir(pDir) ) != NULL )  
	{  
		if( ::strcmp( filename->d_name , "." ) == 0 || ::strcmp( filename->d_name , "..") == 0 )  
			continue;
		
		struct stat s;  
		::lstat( filename->d_name , &s );  
		if( S_ISDIR( s.st_mode ) )  //如果是目录
		{
			char dir_buf[256] = {0};
			::sprintf(dir_buf, "%s/%s", dir_name, filename->d_name);
			traverse(dir_buf);
		}

		string file(string(dir_name).append("/").append(filename->d_name));
		_vecFilePath.push_back(file);
	}
	::closedir(pDir);
}
