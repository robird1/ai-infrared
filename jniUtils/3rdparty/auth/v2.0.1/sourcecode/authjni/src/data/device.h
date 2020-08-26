//
// Created by uriah on 18-2-7.
//

#ifndef ULSAUZMGR_DEVICE_H_H
#define ULSAUZMGR_DEVICE_H_H

#include <cstdio>
#include <string>
#include "../define.h"

namespace uls
{
class Device{
public:
    Device();
    ~Device();

    bool empty();

    int read(std::ifstream & fin);

    int write(std::ofstream & fout);

public:

    std::string encrypt_device_;  //sh256(device_id_ + app_id_ + platform_).base64

//    std::string device_id_;  //设备ID
//    std::string app_id_;     //等同于bind_content
//    int  platform_;         //平台
    int key_num_;           //token数目

};

bool uls_device_equal(Device & device1, uls_device_info & device2);

}
#endif //ULSAUZMGR_DEVICE_H_H
