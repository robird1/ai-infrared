//
// Created by uriah on 18-2-10.
//
#include <infra-utils/uls_infra_log.h>
#include "authorization.h"
#include "authImpl.h"

#define  VERSION_STRING    "uls authorization v2.0.1, " __DATE__ ", " __TIME__

uls::Authorization::Authorization() {

  LOGI("welcome use %s", VERSION_STRING);
  uls::AuthImpl * impl = new uls::AuthImpl;

  impl_ = impl;
}

uls::Authorization::~Authorization() {
  if(impl_ != nullptr) {
    delete static_cast<uls::AuthImpl *>(impl_);
  }
}

void uls::Authorization::initLicenseFile(const char *fileName) {
  if(impl_ != nullptr) {
    static_cast<uls::AuthImpl *>(impl_)->initLicenseFile(fileName);
  }
}

void uls::Authorization::initDevice(const char *deviceId, const char *macAddress, const char *appId, int platform) {
    if(impl_ != nullptr) {
        static_cast<uls::AuthImpl *>(impl_)->initDevice(deviceId, macAddress, appId, platform);
    }
}

void uls::Authorization::initDevice(const char *deviceId, const char *appId, int platform) {
    initDevice(deviceId,NULL,appId,platform);
}


uls_license_key_status uls::Authorization::getAuthRet(const char *key, const char *keySecret, const char *authGroup) {
  if(impl_ != nullptr) {
    return static_cast<uls::AuthImpl *>(impl_)->getAuthRet(key, keySecret, authGroup);
  }

  return ULS_KEY_OTHER_ERROR;
}

uls_license_key_status uls::Authorization::getAuthRet(const char *key, const char *keySecret,
                                                      std::vector<std::string> authGroups) {
    if (impl_ != nullptr) {
        return static_cast<uls::AuthImpl *>(impl_)->getAuthRet(key, keySecret, authGroups);
    }

    return ULS_KEY_OTHER_ERROR;
}

uls_license_key_status
uls::Authorization::getAuthRet(const char *key, const char *keySecret, uls_string authGroups[],
                               int len) {

    std::vector<std::string> vec;
    for (int i = 0; i < len; i++) {
        std::string str = authGroups[i].buf;
        LOGD("str = %s", str.c_str());
        vec.push_back(str);
    }

    return getAuthRet(key, keySecret, vec);

}

void uls::Authorization::setHttpCallback(ConnectServer func) {
  if(impl_ != nullptr) {
    static_cast<uls::AuthImpl *>(impl_)->setHttpCallback(func);
  }
}