//
// Created by uriah on 18-2-8.
//

#ifndef ULSAUZMGR_AUTHIMPL_H
#define ULSAUZMGR_AUTHIMPL_H

#include <authorization/authDefine.h>
#include "license.h"


namespace uls
{

class AuthImpl{
public:
    void initLicenseFile(const char * fileName);

    void initDevice(const char * deviceId, const char *macAddress, const char * appId, int platform);
    //为了兼容旧版本, 保留之前的initDevice接口
    void initDevice(const char * deviceId, const char * appId, int platform);

    uls_license_key_status getAuthRet(const char * key, const char * keySecret, const char * authGroup);

    uls_license_key_status
    getAuthRet(const char *key, const char *keySecret, std::vector<std::string> authGroups);

    int onlineAuth(const char * key, const char * keySecret, const char * authGroup, Token & token);

    void setHttpCallback(ConnectServer func);
public:
    uls::License license_;

    //uls::Device device_;
    uls_device_info device_;

};

}//end namespace uls
#endif //ULSAUZMGR_AUTHIMPL_H
