//
// Created by uriah on 18-2-7.
//

#include <iostream>
#include <cstring>
#include <infra-utils/uls_infra_log.h>
#include <sstream>
#include "license.h"
#include "utils/timeUtil.h"
#include "utils/encryptUtil.h"
#include "define.h"
#include "Log.h"

#define TAG "License"

uls::License::License() {

}

uls::License::~License() {

}

void uls::License::initLicenseFile(const char *fileName) {

  license_file_ = fileName ? fileName : "";
}


uls::uls_license_file_status uls::License::readLicense() {

  if(license_file_.empty()) {
    LOGI(TAG,"please set license file name");
    return ULS_NO_INIT_FILE_NAME;
  }

  std::ifstream fin(license_file_.c_str(), std::ios::in | std::ios::binary);
  if (!fin) {
    fin.close();
    LOGI(TAG,"open license file error");
    return ULS_NO_FILE;
  }

  fin.read((char *)&last_access_time_, sizeof(last_access_time_));

  int64_t  current_time = uls_get_utc_time();

    LOGI(TAG,"current time = %ld", current_time);
    LOGD(TAG,"current_time = %lld",current_time);
    LOGI(TAG,"last access time = %ld", last_access_time_);
    LOGI(TAG,"last access time = %lld", last_access_time_);
  if(current_time < last_access_time_) {
      LOGI(TAG,"current time < last_access_time");
    fin.close();
    return ULS_FILE_VERIFICATION;
  } else{
      LOGI(TAG,"current time > last_access_time%ld", current_time - last_access_time_);
  }
  //读取设备信息
  auto ret = device_.read(fin);
  if(ret < 0) {
    LOGI(TAG,"read device error");
    fin.close();
    //读取失败，或者验证失败
    return ULS_FILE_FORMAT_ERROR;
  }

  tokens_.clear();
  //读取token信息
  for(int i= 0 ; i < device_.key_num_; i++) {
    uls::Token token;
    auto ret = token.read(fin);
    if(ret < 0) {
      LOGI(TAG,"read token error");
      fin.close();
      //读取失败，或者验证失败
      return ULS_FILE_FORMAT_ERROR;
    }
    tokens_.push_back(token);
  }

  fin.close();

  return ULS_FILE_SUCCESS;

}


int uls::License::writeLicense() {

  if(license_file_.empty()) {
    LOGI(TAG,"please set license file");
    return -1;
  }

  std::ofstream fout(license_file_.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if (!fout) {
    LOGI(TAG,"write license file error, open file error ");
    fout.close();
    return -1;
  }

  device_.key_num_ = tokens_.size();

  int64_t  current_time = uls_get_utc_time();
  fout.write((char *)&current_time, sizeof(current_time));

  LOGD(TAG,"current_time = %ld",current_time);

  device_.write(fout);

  for(auto & token : tokens_) {
    token.write(fout);
  }

  fout.close();
  return 0;

}

uls_license_key_status uls::License::getAuthRet(const char *key, const char *keySecret, const char * authGroup, uls_device_info & device) {

  std::string encryptKey = uls_calc_encrypt_key(key, keySecret);

  for(uls::Token & token : tokens_) {

    if(token.enc_key_ == encryptKey) {

      auto token_status = token.valid(device, authGroup );

      switch (token_status) {
        case ULS_TOKEN_WRONG_PLATFORM: return ULS_KEY_WRONG_PLATFORM;
        case ULS_TOKEN_WRONG_AUTH_GROUP: return ULS_KEY_WRONG_AUTH_GROUP;
        case ULS_TOKEN_WRONG_BIND_CONTENT: return ULS_KEY_WRONG_BIND_CONTENT;
        case ULS_TOKEN_EXPIRED:  return ULS_KEY_EXPIRED;
        case ULS_TOKEN_SUCCESS:{
          return ULS_KEY_SUCCESS;
        }
      }

    }
  }

  return  ULS_NO_KEY;

}

uls_license_key_status uls::License::getAuthRet(const char *key, const char *keySecret,
                                                std::vector<std::string> authGroups,
                                                uls_device_info &device) {

  std::string encryptKey = uls_calc_encrypt_key(key, keySecret);

  for (uls::Token &token : tokens_) {

    if (token.enc_key_ == encryptKey) {

      auto token_status = token.valid(device, authGroups);

      switch (token_status) {
        case ULS_TOKEN_WRONG_PLATFORM:
          return ULS_KEY_WRONG_PLATFORM;
        case ULS_TOKEN_WRONG_AUTH_GROUP:
          return ULS_KEY_WRONG_AUTH_GROUP;
        case ULS_TOKEN_WRONG_BIND_CONTENT:
          return ULS_KEY_WRONG_BIND_CONTENT;
        case ULS_TOKEN_EXPIRED:
          return ULS_KEY_EXPIRED;
        case ULS_TOKEN_SUCCESS: {
          return ULS_KEY_SUCCESS;
        }
      }

    }
  }

  return ULS_NO_KEY;
}

int uls::License::updateLicense() {

  if(license_file_.empty()) {
    LOGI(TAG,"please set license file");
    return -1;
  }

  std::ifstream fin(license_file_.c_str(), std::ofstream::binary );
  if (!fin) {

    LOGI(TAG,"open file error");
    fin.close();
    return -1;
  }

  // get size of file
  fin.seekg (0,fin.end);
  long size = fin.tellg();
  fin.seekg (0);

  // allocate memory for file content
  char* buffer = new char[size];

  // read content of infile
  fin.read (buffer,size);

  fin.close();

  std::ofstream fout(license_file_.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if (!fout) {
    LOGI(TAG,"open file error");
    fout.close();
    delete[] buffer;
    return -1;
  }


  int64_t  current_time = uls_get_utc_time();
  memcpy(buffer, &current_time, sizeof(current_time));
  fout.write(buffer,  size);

  delete[] buffer;

  fout.close();
  return 0;
}

int uls::License::insertToken(uls::Token &token) {
  int length = tokens_.size();


  for(int i = 0; i< length; i++) {
    //替换
    if(tokens_[i].enc_key_ == token.enc_key_) {
      tokens_[i] = token;
      return 0;
    }
  }

  tokens_.push_back(token);
  return 0;

}

void uls::License::updateDevice(uls_device_info &device) {

    std::ostringstream ostr;
    ostr<<device.device_id_;
    ostr<<device.app_id_;
    ostr<<device.platform_;
    std::string str = uls_calc_encrypt_key(ostr.str());

  device_.encrypt_device_ = str;
  //清空token
  tokens_.clear();

}