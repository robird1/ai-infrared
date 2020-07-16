//
// Created by uriah on 18-2-7.
//

#ifndef ULSAUZMGR_AUTHORIZATION_H
#define ULSAUZMGR_AUTHORIZATION_H

#include <infra-utils/uls_infra_utils.h>
#include "authorization/authDefine.h"
#include <vector>

typedef struct {
    char buf[32];
} uls_string;
namespace uls
{

 class DLL_PUBLIC  Authorization{

public:
    Authorization();
    ~Authorization();

    void initLicenseFile(const char * fileName);

     /**
      * 初始化设备
      * @param deviceId　　
      * @param appId 　　　
      * @param platform
      */
    void initDevice(const char * deviceId,const char * appId, int platform);

    /**
      * 初始化设备
      * @param deviceId　　
      * @param macAddress
      * @param appId 　　　
      * @param platform
      */
    void initDevice(const char * deviceId,const char * macAddress,const char * appId, int platform);

     /**
      * 获取授权结果
      * @param key
      * @param keySecret
      * @param authGroup　　
      * @return
      */
    uls_license_key_status getAuthRet(const char * key, const char * keySecret, const char * authGroup);

/**
* 获取授权结果
* @param key
* @param keySecret
* @param authGroup　　
* @return
*/
uls_license_key_status getAuthRet(const char * key, const char * keySecret, std::vector<std::string> authGroups);

/**
* 获取授权结果
* @param key
* @param keySecret
* @param authGroup　　
* @return
*/
uls_license_key_status getAuthRet(const char * key, const char * keySecret, uls_string authGroups[], int len);




     /**
      * 自定义http实现，默认libcurl实现
      * @param func
      */
    void setHttpCallback(ConnectServer func);

private:
    void * impl_;

};
}
#endif //ULSAUZMGR_AUTHORIZATION_H
