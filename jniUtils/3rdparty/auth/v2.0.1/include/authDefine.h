//
// Created by uriah on 18-2-9.
//

#ifndef ULSAUZMGR_AUTHDEFINE_H_H
#define ULSAUZMGR_AUTHDEFINE_H_H

#include <string>
typedef enum{
    ULS_KEY_SUCCESS = 0,   //key有效
    ULS_NO_KEY = 1,       //本地文件中不存在
    ULS_KEY_EXPIRED = 2,  //key失效
    ULS_KEY_WRONG_PLATFORM = 3,
    ULS_KEY_WRONG_AUTH_GROUP = 4,
    ULS_KEY_WRONG_BIND_CONTENT = 5,
    ULS_KEY_NO_INIT_LICENSE_FILE = 6,  //未设置授权文件
    ULS_KEY_WRONG_LICENSE_FILE = 7,     //错误的授权文件，该文件无法写
    ULS_KEY_NO_LICENSE_FILE = 8,        //不存在授权文件
    ULS_KEY_WRONG_LOCAL_TIME = 9,       //本地时间与服务时间差距较大
    ULS_KEY_NO_INIT_DEVICE = 10,          //未初始化设备信息
    ULS_KEY_WRONG_PARAM = 11,            //错误参数（如传nullptr等信息）
    ULS_KEY_OTHER_ERROR = 12,            //网络错误（没网,可能是客户端的网络问题,也可能是服务端的key服务器的问题,还是
                                         //要通过log,以及客户端访问key服务端地址等方式来判断是哪边的问题）
            ULS_NET_KEY_ERROR  =17            //网络校验错误（网络正常,但key校验失败,可能是key不正确或者过期了）


} uls_license_key_status;

typedef enum {
    ULS_HTTP_OK,
    ULS_HTTP_BAD_REQUEST,
    ULS_HTTP_OTHER_ERROR
} uls_http_status;

//自定义http实现
typedef uls_http_status (*ConnectServer)(std::string key, std::string keySecret, std::string random, std::string data, std::string macAddress, std::string &httpResp);
#endif //ULSAUZMGR_AUTHDEFINE_H_H
