//
// Created by uriah on 18-2-7.
//

#include <fstream>
#include <cstring>
#include <iostream>
#include <cryptlite/sha256.h>
#include <cryptlite/hmac.h>
#include <infra-utils/uls_infra_log.h>
#include "token.h"
#include "device.h"
#include "../utils/timeUtil.h"

#define STRING_LEN  32
#define LONG_STRING_LEN 128

int uls::Token::read(std::ifstream &fin) {

    char encryptKey[LONG_STRING_LEN + 1] = "";
    fin.read(encryptKey, LONG_STRING_LEN);
    //std::cout<<"read encryptKey = "<<encryptKey<<std::endl;

    if(fin.eof()) {
        return -1;
    }

    fin.read((char *)&create_time_, sizeof(create_time_));

    if(fin.eof()) {
        return -1;
    }

    fin.read((char *)&current_time_, sizeof(current_time_));

    if(fin.eof()) {
        return -1;
    }

    fin.read((char *)&expiration_, sizeof(expiration_));

    if(fin.eof()) {
        return -1;
    }

    fin.read((char *)&limit_days_, sizeof(limit_days_));

    if(fin.eof()) {
        return -1;
    }

    fin.read((char *)&days_, sizeof(days_));

    if(fin.eof()) {
        return -1;
    }

    fin.read((char *)&limit_auth_group_, sizeof(limit_auth_group_));

    if(fin.eof()) {
        return -1;
    }

    //读取授权组
    int64_t authNum = 0;
    auth_groups_.clear();
    fin.read((char *)&authNum, sizeof(authNum));
    for(int64_t i = 0; i < authNum; i++ ) {
        char authGroup[STRING_LEN + 1] = "";
        fin.read(authGroup, STRING_LEN);
        if(fin.eof()) {
            return -1;
        }

        std::string auth = authGroup;
        auth_groups_.push_back(auth);
    }


    fin.read((char *)&limit_platform_, sizeof(limit_platform_));

    if(fin.eof()) {
        return -1;
    }

    fin.read((char *)&platform_, sizeof(platform_));

    if(fin.eof()) {
        return -1;
    }

    fin.read((char *) &limit_bind_content_, sizeof(limit_bind_content_));

    if(fin.eof()) {
        return -1;
    }

    char bindContent[STRING_LEN + 1] = "";
    fin.read(bindContent, STRING_LEN);

    if(fin.eof()) {
        return -1;
    }

    uint8_t read_hash[32] = "";
    fin.read((char *)read_hash, 32);

    //计算３２位hash值
    std::string source = std::string(encryptKey)  + std::string(bindContent);
    const char *key_hmac = "ULSJwtSigningKeyHeiHei";
    uint8_t digest[32];
    uls::cryptlite::hmac<uls::cryptlite::sha256>::calc(source.c_str(), source.length(), key_hmac, strlen(key_hmac), digest);

    //判断hash值是否相等
    for(int i = 0;i < 32 ;i++) {
        if(read_hash[i] != digest[i]) {
            //std::cout<<"11"<<std::endl;
            return -1;
        }
    }

    enc_key_ = encryptKey; //加密后的字符串
    bind_content_ = bindContent;

    return 0;
}

int uls::Token::write(std::ofstream &fout) {

    char encryptKey[LONG_STRING_LEN + 1] = "";
   //不会发生越界（32 / 3 * 4 < 128）
    memcpy(encryptKey, enc_key_.data(), enc_key_.length());

    fout.write(encryptKey, LONG_STRING_LEN);
    std::cout<<"write encryptKey = "<<encryptKey<<std::endl;

    fout.write((char *)&create_time_, sizeof(create_time_));

    fout.write((char *)&current_time_, sizeof(current_time_));

    fout.write((char *)&expiration_, sizeof(expiration_));

    fout.write((char *)&limit_days_, sizeof(limit_days_));

    fout.write((char *)&days_, sizeof(days_));

    fout.write((char *)&limit_auth_group_, sizeof(limit_auth_group_));

    int64_t authNum = auth_groups_.size();
    fout.write((char *) &authNum, sizeof(authNum));

    for(auto ele : auth_groups_) {
        char authGroup[STRING_LEN + 1] = "";
        int authGroupLen = ele.length() > STRING_LEN ? STRING_LEN : ele.length();
        memcpy(authGroup, ele.data(), authGroupLen);

        fout.write(authGroup, STRING_LEN);
    }

    fout.write((char *)&limit_platform_, sizeof(limit_platform_));

    fout.write((char *)&platform_, sizeof(platform_));

    fout.write((char *) &limit_bind_content_, sizeof(limit_bind_content_));

    char bindContent[STRING_LEN + 1] = "";
    int bindContentLen = bind_content_.length() > STRING_LEN ? STRING_LEN : bind_content_.length();
    memcpy(bindContent, bind_content_.data(), bindContentLen);

    fout.write(bindContent, STRING_LEN);

    //计算３２位hash值
    std::string source = std::string(encryptKey)   + std::string(bindContent);
    const char *key_hmac = "ULSJwtSigningKeyHeiHei";
    uint8_t digest[32];
    uls::cryptlite::hmac<uls::cryptlite::sha256>::calc(source.c_str(), source.length(), key_hmac, strlen(key_hmac), digest);
    fout.write((char *)digest, 32);

    return 0;
}


uls::uls_token_error_code uls::Token::valid(uls_device_info & device, std::string authGroup) {

    int64_t  locate = uls_get_utc_time();

    //1.时间是否过期
    if(limit_days_) {
       int64_t  deadline = create_time_ + days_;  //截至时间
       LOGE("deadline = %ld(s), locate = %ld(s), create_time = %ld(s), days_ = %ld(s)", deadline, locate, create_time_, days_);
        if(locate > deadline) {
            //key过期
            return ULS_TOKEN_EXPIRED;
        }
    }

    if(limit_platform_ && device.platform_ != platform_) {

        return ULS_TOKEN_WRONG_PLATFORM;

    }


    if(limit_auth_group_) {

        //此auth_group是否在容器中
        bool isExist = false;
        for(auto ele : auth_groups_) {
            if(ele == authGroup) {
                isExist = true;
                break;
            }
        }

        if(!isExist) {
            LOGD("wrong auth group");
            return ULS_TOKEN_WRONG_AUTH_GROUP;
        }
    }


    if(limit_bind_content_ && device.app_id_.compare(bind_content_) != 0) {

        return ULS_TOKEN_WRONG_BIND_CONTENT;
    }


    return ULS_TOKEN_SUCCESS;
}


uls::uls_token_error_code
uls::Token::valid(uls_device_info &device, std::vector<std::string> authGroups) {

    int64_t locate = uls_get_utc_time();

    //1.时间是否过期
    if (limit_days_) {
        int64_t deadline = create_time_ + days_;  //截至时间
        LOGE("deadline = %ld(s), locate = %ld(s), create_time = %ld(s), days_ = %ld(s)", deadline,
             locate, create_time_, days_);
        if (locate > deadline) {
            //key过期
            return ULS_TOKEN_EXPIRED;
        }
    }

    if (limit_platform_ && device.platform_ != platform_) {

        return ULS_TOKEN_WRONG_PLATFORM;

    }


    if (limit_auth_group_) {

        //所有组均在集合中，才算验证成功
        for (auto auth : authGroups) {

            //此auth_group是否在容器中
            bool isExist = false;
            for (auto ele : auth_groups_) {
                if (ele == auth) {
                    isExist = true;
                    break;
                }
            }

            if (!isExist) {
                LOGD("wrong auth group");
                return ULS_TOKEN_WRONG_AUTH_GROUP;
            }

        }

    }


    if (limit_bind_content_ && device.app_id_.compare(bind_content_) != 0) {

        return ULS_TOKEN_WRONG_BIND_CONTENT;
    }


    return ULS_TOKEN_SUCCESS;
}