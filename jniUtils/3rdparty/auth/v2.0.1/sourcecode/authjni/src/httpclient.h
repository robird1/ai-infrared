//
// Created by uriah on 18-2-8.
//

#ifndef ULSAUZMGR_HTTPCLIENT_H
#define ULSAUZMGR_HTTPCLIENT_H

#include <string>
#include "data/token.h"
#include "../include/authorization/authDefine.h"

namespace uls
{

class HttpClient{

public:
    //HttpClient();

//    ~HttpClient(){
//      func_ = nullptr;
//    }

    static void setCallback(ConnectServer func);

    uls_http_status connectServer(std::string & key, std::string &keySecret,
                                  std::string &data, std::string &macAddress, std::string & response);

    bool getResToken(std::string & response, std::string & token, std::string & transport_key);

    bool valid(std::string & resToken, std::string & transport_key);

    int extractToken(std::string & response, uls::Token & token);


public:

    std::string random_;  //这些字段用来校验服务器真伪
    std::string key_;
    std::string key_secret_;
    int version_;

public:
    static ConnectServer func_; //回调函数

};
}// end namespace uls
#endif //ULSAUZMGR_HTTPCLIENT_H
