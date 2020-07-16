//
// Created by kyle on 18-8-20.
//

#include <algorithm>
#include <fstream>
#include <sys/stat.h>
#include <Log.h>
#include "CommonUtil.h"
#include "Log.h"

#define TAG "CommonUtil_native"

const std::string CommonUtil::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    std::string time = buf;
    std::replace( time.begin(), time.end(), ':', '_');//因为有些安卓系统不支持冒号的文件名,所以替换成_号
    return time;
}

bool CommonUtil::doesFileExist( const std::string& nameOfFile )
{
    std::ifstream infile(nameOfFile.c_str(  ));
    return infile.good(  );
}

bool CommonUtil::doesDirectoryExist( const std::string& nameOfDirectory )
{
    const char* pathName = nameOfDirectory.c_str(  );
    struct stat info;

    if( stat( pathName, &info ) != 0 )
        return false;
    else return ( info.st_mode & S_IFDIR ) != 0;
}

bool CommonUtil::createDirectory ( const std::string& pathOfDirectory )
{
    int errNo = 0;
    if ( (errNo = mkdir( pathOfDirectory.c_str(  ), 0777 ) ) != 0 )
    {
        LOGD(TAG,  "MKDIR FAILED %d", errNo );
        return false;
    }
    return true;
}

bool CommonUtil::deleteFile( const std::string& nameOfFile )
{
    if( remove( nameOfFile.c_str(  ) ) != 0 )
        return true;
    else
        return true;
}

