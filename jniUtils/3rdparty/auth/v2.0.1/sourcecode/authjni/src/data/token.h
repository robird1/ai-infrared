//
// Created by uriah on 18-2-7.
//

#ifndef ULSAUZMGR_TOKEN_H
#define ULSAUZMGR_TOKEN_H

#include <vector>
#include "device.h"
#include "../define.h"

namespace uls{

class Token{
public:
    uls_token_error_code valid(uls_device_info & device, std::string authGroup);

    uls_token_error_code valid(uls_device_info &device, std::vector<std::string> authGroups);
    int read(std::ifstream & fin);

    int write(std::ofstream & fout);

public:
    //std::string key_;
  //  std::string key_secret_;

    std::string enc_key_;   //sh256(key_ + key_secret_).base64

    int64_t create_time_;
    int64_t current_time_;
    int64_t expiration_;
    bool limit_days_;
    int64_t days_;
    bool limit_auth_group_;
    //std::string auth_group_;
    bool limit_platform_;
    int64_t platform_;
    bool limit_bind_content_;
    std::string bind_content_;
//    bool limit_count_;
//    uint32_t count_;

    std::vector<std::string> auth_groups_;  //授权组可能有多个


};
}
#endif //ULSAUZMGR_TOKEN_H
