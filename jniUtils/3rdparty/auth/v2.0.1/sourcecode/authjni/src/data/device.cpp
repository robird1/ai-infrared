//
// Created by uriah on 18-2-7.
//
#include "device.h"
#include "../define.h"
#include "../utils/encryptUtil.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <cryptlite/sha256.h>
#include <cryptlite/hmac.h>
#include <infra-utils/uls_infra_log.h>

#define STRING_LEN 32
#define LONG_STRING_LEN 128

namespace uls {
bool uls_device_equal(uls::Device &device1, uls_device_info &device2) {
  LOGD("device2 device_id = %s, mac_address_ = %s, appId = %s, platform = %d", device2.device_id_.c_str(), device2.mac_address_.c_str(), device2.app_id_.c_str(), device2.platform_);


  std::ostringstream ostr;
  ostr<<device2.device_id_;
  ostr<<device2.app_id_;
  ostr<<device2.platform_;
  std::string str = uls_calc_encrypt_key(ostr.str());
  return device1.encrypt_device_ == str;
}

}

uls::Device::Device() {
  //platform_ = -1;
  key_num_ = 0;
}

uls::Device::~Device() {

}

int uls::Device::read(std::ifstream &fin) {
  char encrypt_device[LONG_STRING_LEN + 1] = "";
  //char app[STRING_LEN + 1] = "";

  fin.read(encrypt_device, LONG_STRING_LEN);
  if(fin.eof()) {
    return -1;
  }

//  fin.read(app, STRING_LEN);
//  if(fin.eof()) {
//    return -1;
//  }
//
//  fin.read((char *)&platform_, sizeof(platform_));
//
//  if(fin.eof()) {
//    return -1;
//  }

  fin.read((char *)&key_num_, sizeof(key_num_));

  //读取hash值
//  uint8_t read_hash[32] = "";
//  fin.read((char *)read_hash, 32);
//
//  //计算３２位hash值
//  std::string source = std::string(device) + std::string(app);
//  const char *key = "ULSJwtSigningKeyHeiHei";
//  uint8_t digest[32];
//  uls::cryptlite::hmac<uls::cryptlite::sha256>::calc(source.c_str(), source.length(), key, strlen(key), digest);
//
//  //判断hash值是否相等
//  for(int i=0;i< 32;i++) {
//    if(read_hash[i] != digest[i]) {
//      return -1;
//    }
//  }

  //device_id_ = device;
  //app_id_ = app;

  encrypt_device_ = encrypt_device;
  return 0;
}

int uls::Device::write(std::ofstream &fout) {

//  char device[STRING_LEN + 1] = "";
//  char app[STRING_LEN + 1] = "";
//
//  int device_len = device_id_.length() > STRING_LEN ?
//                   STRING_LEN : device_id_.length();
//
//  int app_len = app_id_.length() > STRING_LEN ?
//                STRING_LEN : app_id_.length();

  char encrypt_device[LONG_STRING_LEN + 1] = "";

  memcpy(encrypt_device, encrypt_device_.data(), encrypt_device_.length());
//  memcpy(app, app_id_.data(), app_len);
//
//  fout.write(device, STRING_LEN);
//
//  fout.write(app, STRING_LEN);
//
//  fout.write((char *)&platform_, sizeof(platform_));

  fout.write(encrypt_device, LONG_STRING_LEN);

  fout.write((char *)&key_num_, sizeof(key_num_));

  //计算３２位hash值
//  std::string source = std::string(device) + std::string(app);
//  const char *key = "ULSJwtSigningKeyHeiHei";
//  uint8_t digest[32];
//  uls::cryptlite::hmac<uls::cryptlite::sha256>::calc(source.c_str(), source.length(), key, strlen(key), digest);
//  fout.write((char *)digest, 32);

  return 0;
}


bool uls::Device::empty() {

//  if(device_id_.empty() || app_id_.empty()) {
//    return true;
//  }
//  return false;
  return encrypt_device_.empty();
}