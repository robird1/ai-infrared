//
// Created by uriah on 17-11-17.
//

#ifndef UNTITLED2_SERVERMGR_H
#define UNTITLED2_SERVERMGR_H


#include <iostream>
#include "LicenseMgr.h"
namespace uls {
    typedef struct {
        std::string type;
        std::string alg;
        std::string version;
    } ULS_HTTP_HEADER;

    typedef struct {
        int user_id;
        int expiration;
        int count;
        std::string auth_group;
        int platform;
        std::string bind_content;
    } ULS_HTTP_BODY;

    class ServerMgr {
    public:
        /**
         * 和服务器交互
         * @param key
         * @param keySecret
         * @param ret 授权结果
         * @return first:网络连接; second:key验证
         */
        std::pair<bool, bool>
        connectServer(std::string &key, std::string &keySecret, std::string & data, ULS_HTTP_HEADER &header, ULS_HTTP_BODY &body);

    private:

        /**
         * http请求
         * @param key [input]
         * @param keySecret [input]
         * @param strResponse [out] http response body
         * @return <0:error;
         */
        int httpConnect(std::string &key, std::string &keySecret, std::string &data,std::string &strResponse);

        int checkHttpRespValid(std::string &&headAndBody, std::string secret);

        int getRespHeader(std::string header, ULS_HTTP_HEADER &http_header);

        int getRespBody(std::string body, ULS_HTTP_BODY &http_body);

        int decryptHttp(char header[], char body[], std::string &clearHeader, std::string &clearBody);


    };
}


#endif //UNTITLED2_SERVERMGR_H
