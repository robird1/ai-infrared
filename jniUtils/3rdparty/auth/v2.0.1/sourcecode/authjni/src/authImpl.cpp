//
// Created by uriah on 18-2-8.
//

#include <uls_json/uls_json/json.hpp>
#include <infra-utils/uls_infra_utils.h>
#include <iostream>
#include <infra-utils/uls_infra_log.h>
#include "authImpl.h"
#include "httpclient.h"
#include "utils/timeUtil.h"
#include "utils/encryptUtil.h"
#include "define.h"
#include <vector>
#include "Log.h"
#include "../include/authorization/authDefine.h"
#include "../include/infra-utils/uls_infra_utils.h"

#define TAG "AuthImpl"

void uls::AuthImpl::initLicenseFile(const char *fileName) {
  license_.initLicenseFile(fileName);
}

void uls::AuthImpl::initDevice(const char *deviceId, const char *macAddress, const char *appId,
                               int platform) {
    device_.device_id_ = deviceId ? deviceId : "";
    device_.app_id_ = appId ? appId : "";
    device_.mac_address_ = macAddress ? macAddress : "";
    device_.platform_ = platform;
    // device_.key_num_ = 0;
}

void uls::AuthImpl::initDevice(const char *deviceId, const char *appId,
                               int platform) {
    initDevice(deviceId, "", appId, platform);
}

uls_license_key_status uls::AuthImpl::getAuthRet(const char *key, const char *keySecret, const char *authGroup) {

    if (nullptr == key || nullptr == keySecret || nullptr == authGroup) {
        return ULS_KEY_WRONG_PARAM;
    }

  if(device_.device_id_.empty() ) {
      LOGE(TAG,"please init device first");
    return ULS_KEY_NO_INIT_DEVICE;
  }

  uls_license_file_status status = ULS_FILE_SUCCESS;
  if(!license_.isReadLicenseFile()) {
    status = license_.readLicense();
  }

  switch (status){
    case ULS_NO_INIT_FILE_NAME:{
      //没有设置文件
      LOGI(TAG, "please init license file");
      return ULS_KEY_NO_INIT_LICENSE_FILE;
    }

    case ULS_NO_FILE:{
        LOGE(TAG, "ULS_NO_FILE");
      license_.updateDevice(device_);
      goto online;
    }

    case ULS_FILE_FORMAT_ERROR:
    case ULS_FILE_VERIFICATION:{
        LOGE(TAG, "ULS_FILE_VERIFICATION");
      //授权文件格式错误,请求网络验证
      //todo
      goto online;

    }

    case ULS_FILE_SUCCESS: {

      //判断设备信息是否正确
      if(!uls_device_equal(license_.device_, device_)) {
        //授权文件中的设备信息与传入的设备信息不同
        LOGE(TAG, "wrong device info");
        license_.updateDevice(device_);
        goto online;
      }

      //先走离线认证
      auto ret = license_.getAuthRet(key, keySecret,authGroup, device_);
      LOGE(TAG, "offline ret = %d", ret);
      if(ret == ULS_KEY_SUCCESS) {
       auto ret =  license_.updateLicense();
        if(ret < 0) {
          return ULS_KEY_WRONG_LICENSE_FILE;
        } else {
          return ULS_KEY_SUCCESS;
        }

      }else {
        goto online;
      }

    }
  }


  online:
  uls::Token token;
  auto onlineRet = onlineAuth(key, keySecret, authGroup, token);
  if(onlineRet != ULS_API_RET_SUCCESS) {
    //网络验证失败，请检查网络是否连接，或者参数是否有误
      if (onlineRet == ULS_API_RET_PARAM_ERR) //key验证失败
      {
          return ULS_NET_KEY_ERROR;
      }
      else
      {
          return ULS_KEY_OTHER_ERROR;
      }

  }

  //todo
  //判断系统返回时间与本地时间差距
  int64_t  diff = labs(uls_get_utc_time() - token.current_time_);

  //本地时间与系统时间差距过大
  if(diff > 24 * 60 * 60) {
    LOGD(TAG, "diff time = %ld", diff);
    return ULS_KEY_WRONG_LOCAL_TIME;
  }

  //在线认证成功并不是真的成功，呵呵
  auto code = token.valid(device_, authGroup);

  switch(code) {
    case ULS_TOKEN_EXPIRED: {
      return ULS_KEY_EXPIRED;
    }
    case ULS_TOKEN_WRONG_PLATFORM: {
      return ULS_KEY_WRONG_PLATFORM;
    }
    case ULS_TOKEN_WRONG_AUTH_GROUP: {
      return ULS_KEY_WRONG_AUTH_GROUP;
    }
    case ULS_TOKEN_WRONG_BIND_CONTENT:{
      return ULS_KEY_WRONG_BIND_CONTENT;
    }
    case ULS_TOKEN_SUCCESS:{
      //把token写进license文件中
      license_.insertToken(token);
      auto ret = license_.writeLicense();
      if(ret < 0) {
        //文件保存失败，授权文件地址是错误的
        return ULS_KEY_WRONG_LICENSE_FILE;
      } else {
        return ULS_KEY_SUCCESS;
      }

    }
  }

  return ULS_KEY_SUCCESS;

}


uls_license_key_status uls::AuthImpl::getAuthRet(const char *key, const char *keySecret,
                                                 std::vector<std::string> authGroups) {

    if (nullptr == key || nullptr == keySecret || authGroups.size() == 0) {
        LOGE(TAG, "wrong param");
        return ULS_KEY_WRONG_PARAM;
    }


    if (device_.device_id_.empty()) {
        LOGE(TAG, "please init device first");
        return ULS_KEY_NO_INIT_DEVICE;
    }

    uls_license_file_status status = ULS_FILE_SUCCESS;
    if (!license_.isReadLicenseFile()) {
        status = license_.readLicense();
    }

    switch (status) {
        case ULS_NO_INIT_FILE_NAME: {
            //没有设置文件
            LOGI(TAG, "please init license file");
            return ULS_KEY_NO_INIT_LICENSE_FILE;
        }

        case ULS_NO_FILE: {
            LOGE(TAG, "ULS_NO_FILE");
            license_.updateDevice(device_);
            goto online;
        }

        case ULS_FILE_FORMAT_ERROR:
        case ULS_FILE_VERIFICATION: {
            LOGE(TAG, "ULS_FILE_VERIFICATION");
            //授权文件格式错误,请求网络验证
            //todo
            goto online;

        }

        case ULS_FILE_SUCCESS: {

            //判断设备信息是否正确
            if (!uls_device_equal(license_.device_, device_)) {
                //授权文件中的设备信息与传入的设备信息不同
                LOGE(TAG, "wrong device info");
                license_.updateDevice(device_);
                goto online;
            }

            //先走离线认证
            auto ret = license_.getAuthRet(key, keySecret, authGroups, device_);
            LOGE(TAG, "offline ret = %d", ret);
            if (ret == ULS_KEY_SUCCESS) {
                auto ret = license_.updateLicense();
                if (ret < 0) {
                    return ULS_KEY_WRONG_LICENSE_FILE;
                } else {
                    return ULS_KEY_SUCCESS;
                }

            } else {
                goto online;
            }

        }
    }


    online:
    uls::Token token;
    auto onlineRet = onlineAuth(key, keySecret, "", token);
    if (onlineRet != ULS_API_RET_SUCCESS) {
        //网络验证失败，请检查网络是否连接，或者参数是否有误
        if (onlineRet == ULS_API_RET_PARAM_ERR) //key验证失败
        {
            return ULS_NET_KEY_ERROR;
        }
        else
        {
            return ULS_KEY_OTHER_ERROR;
        }

    }

    //todo
    //判断系统返回时间与本地时间差距
    int64_t diff = labs(uls_get_utc_time() - token.current_time_);

    //本地时间与系统时间差距过大
    if (diff > 24 * 60 * 60) {
        LOGD(TAG, "diff time = %ld", diff);
        return ULS_KEY_WRONG_LOCAL_TIME;
    }

    //在线认证成功并不是真的成功，呵呵
    auto code = token.valid(device_, authGroups);

    switch (code) {
        case ULS_TOKEN_EXPIRED: {
            return ULS_KEY_EXPIRED;
        }
        case ULS_TOKEN_WRONG_PLATFORM: {
            return ULS_KEY_WRONG_PLATFORM;
        }
        case ULS_TOKEN_WRONG_AUTH_GROUP: {
            return ULS_KEY_WRONG_AUTH_GROUP;
        }
        case ULS_TOKEN_WRONG_BIND_CONTENT: {
            return ULS_KEY_WRONG_BIND_CONTENT;
        }
        case ULS_TOKEN_SUCCESS: {
            //把token写进license文件中
            license_.insertToken(token);
            auto ret = license_.writeLicense();
            if (ret < 0) {
                //文件保存失败，授权文件地址是错误的
                return ULS_KEY_WRONG_LICENSE_FILE;
            } else {
                return ULS_KEY_SUCCESS;
            }

        }
    }

    return ULS_KEY_SUCCESS;
}

int uls::AuthImpl::onlineAuth(const char *key, const char *keySecret, const char *authGroup, Token & token) {

  uls::json j_data;
  //j_data["auth_group"] = authGroup;
  j_data["bind_content"] = device_.app_id_;
  j_data["machine_id"] = device_.device_id_;
  j_data["platform"] = device_.platform_;
  std::string data = j_data.dump();

  std::string response;
  uls::HttpClient httpClient;

  std::string s_key = key;
  std::string s_keySecret = keySecret;

  auto httpStatus = httpClient.connectServer(s_key, s_keySecret, data, device_.mac_address_,response);


  if(httpStatus == ULS_HTTP_BAD_REQUEST) {
    LOGI(TAG, "ULS_HTTP_BAD_REQUEST http ret = 400");
    LOGI(TAG,"response = %s",response.c_str());
    return ULS_API_RET_PARAM_ERR;
  }else if(httpStatus == ULS_HTTP_OTHER_ERROR) {
    LOGI(TAG, "ULS_HTTP_OTHER_ERRORs");
    LOGD(TAG, "response = %s", response.c_str());
    return ULS_API_BADRESOURCE;
  }

  std::string s_token;
  std::string s_translate_key;
  auto getRet = httpClient.getResToken(response, s_token, s_translate_key);
  if(!getRet) {
    //格式错误
      LOGI(TAG, "ULS_API_RET_INTERNAL_ERR");
    return ULS_API_RET_INTERNAL_ERR;
  }

//

  token.enc_key_ = uls_calc_encrypt_key(s_key, s_keySecret);

  //token.key_ = s_key;
  //token.key_secret_ = s_keySecret;
  auto exRet = httpClient.extractToken(s_token, token);

  auto vl = httpClient.valid(s_token, s_translate_key);
  if(!vl) {
    //http校验失败
      LOGI(TAG, "ULS_API_RET_INTERNAL_ERRVl");
    return ULS_API_RET_INTERNAL_ERR;
  }

  if(exRet != ULS_API_RET_SUCCESS) {
      LOGI(TAG, "ULS_API_RET_INTERNAL_ERR exRet");
    return  ULS_API_RET_INTERNAL_ERR;
  } else {
    return ULS_API_RET_SUCCESS;
  }

}

void uls::AuthImpl::setHttpCallback(ConnectServer func) {
  HttpClient::setCallback(func);
}