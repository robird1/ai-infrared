//
// Created by uriah on 18-2-28.
//

#ifndef ULSAUZMGR_DEFINE_H
#define ULSAUZMGR_DEFINE_H

namespace uls{
    typedef enum {
        ULS_NO_INIT_FILE_NAME,  //未设置文件名
        ULS_NO_FILE,            //不存在授权文件
        ULS_FILE_FORMAT_ERROR,  //错误格式
        ULS_FILE_VERIFICATION,   //校验失败
        ULS_FILE_SUCCESS
    } uls_license_file_status;

    typedef enum {
        ULS_TOKEN_EXPIRED,           //key过期
        ULS_TOKEN_WRONG_PLATFORM,    //错误平台
        ULS_TOKEN_WRONG_AUTH_GROUP,  //错误授权组
        ULS_TOKEN_WRONG_BIND_CONTENT, //错误appid
        ULS_TOKEN_SUCCESS
    } uls_token_error_code;

    typedef struct {
        std::string device_id_;  //设备ID
        std::string mac_address_;  //mac地址
        std::string app_id_;     //等同于bind_content
        int  platform_;         //平台
    }uls_device_info;
}

#endif //ULSAUZMGR_DEFINE_H
