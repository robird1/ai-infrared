//
// Created by uriah on 18-2-7.
//

#ifndef ULSAUZMGR_LICENSE_H
#define ULSAUZMGR_LICENSE_H

#include <vector>
#include <fstream>
#include "data/token.h"
#include "data/device.h"
#include "authorization/authDefine.h"
#include "define.h"


namespace uls
{

//保存的均为从文件中读取的信息或者即将要写入文件的信息
class License{
public:
    License();
    ~License();

    void initLicenseFile(const char * fileName);

    bool isReadLicenseFile(){
      return !device_.empty();
    }

    uls_license_key_status getAuthRet(const char * key, const char * keySecret, const char * authGroup, uls_device_info & device);

    uls_license_key_status
    getAuthRet(const char *key, const char *keySecret, std::vector<std::string> authGroups,
               uls_device_info &device);

    uls_license_file_status readLicense();

    int writeLicense();

    int insertToken(uls::Token & token);

    void updateDevice(uls_device_info & device);

    //更新访问时间
    int updateLicense();

public:
    uls::Device device_;    //设备信息需要比对
private:
    int64_t  last_access_time_;  //记录上次访问时间
    std::vector<uls::Token> tokens_;
    std::string license_file_;
};
}  //end of namespace uls
#endif //ULSAUZMGR_LICENSE_H
