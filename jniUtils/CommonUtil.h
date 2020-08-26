//
// Created by kyle on 18-8-20.
//

#ifndef ULSEEAPPSTORE_COMMONUTIL_H
#define ULSEEAPPSTORE_COMMONUTIL_H

#include <string>
#include <sstream>

class CommonUtil {
public:


    static const std::string currentDateTime();

    template < typename T>
    static const std::string cast_to_string ( const T& item) {
        std::ostringstream sout;
        sout << item;
        if (!sout)
            return "error";
        return sout.str();
    }


    static bool doesFileExist( const std::string& nameOfFile );

    static bool doesDirectoryExist( const std::string& nameOfDirectory );

    static bool createDirectory ( const std::string& pathOfDirectory );

    bool deleteFile( const std::string& nameOfFile );
};




#endif //ULSEEAPPSTORE_COMMONUTIL_H
